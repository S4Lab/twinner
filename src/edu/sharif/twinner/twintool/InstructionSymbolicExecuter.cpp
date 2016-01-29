//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2015  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "InstructionSymbolicExecuter.h"


#include "Instrumenter.h"
#include "RegisterResidentExpressionValueProxy.h"
#include "MemoryResidentExpressionValueProxy.h"
#include "ConstantExpressionValueProxy.h"

#include "edu/sharif/twinner/twintool/operationgroup/DummyOperationGroup.h"
#include "edu/sharif/twinner/twintool/operationgroup/SubtractOperationGroup.h"
#include "edu/sharif/twinner/twintool/operationgroup/AdditionOperationGroup.h"
#include "edu/sharif/twinner/twintool/operationgroup/AddWithCarryOperationGroup.h"
#include "edu/sharif/twinner/twintool/operationgroup/BitwiseAndOperationGroup.h"
#include "edu/sharif/twinner/twintool/operationgroup/ShiftArithmeticRightOperationGroup.h"
#include "edu/sharif/twinner/twintool/operationgroup/ShiftRightOperationGroup.h"

#include "edu/sharif/twinner/trace-twintool/TraceImp.h"
#include "edu/sharif/twinner/trace/ExpressionImp.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue128Bits.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/Syscall.h"
#include "edu/sharif/twinner/trace/StateSummary.h"

#include "edu/sharif/twinner/util/max.h"
#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/memory.h"
#include "edu/sharif/twinner/util/MemoryManager.h"

#include <stdexcept>

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

#ifdef TARGET_IA32E
static const int STACK_OPERATION_UNIT_SIZE = 8; // bytes
#else
static const int STACK_OPERATION_UNIT_SIZE = 4; // bytes
#endif

InstructionSymbolicExecuter::InstructionSymbolicExecuter (
    Instrumenter *_im,
    std::ifstream &symbolsFileInputStream, bool _disabled, bool _measureMode) :
    im (_im),
    bufferForTraceLazyLoad (std::string
    (std::istreambuf_iterator<char> (symbolsFileInputStream),
    std::istreambuf_iterator<char> ())), lazyTrace (0),
    memoryManager (edu::sharif::twinner::util::MemoryManager::getInstance ()),
    trackedReg (REG_INVALID_), operandSize (-1), hook (0),
    disabled (_disabled),
    measureMode (_measureMode), numberOfExecutedInstructions (0) {
}

InstructionSymbolicExecuter::InstructionSymbolicExecuter (Instrumenter *_im,
    bool _disabled) :
    im (_im),
    lazyTrace (new edu::sharif::twinner::trace::TraceImp ()),
    memoryManager (lazyTrace->getMemoryManager ()),
    trackedReg (REG_INVALID_), operandSize (-1), hook (0),
    disabled (_disabled),
    measureMode (false), numberOfExecutedInstructions (0) {
}

edu::sharif::twinner::trace::Trace *InstructionSymbolicExecuter::getTrace () {
  lazyLoad ();
  return lazyTrace;
}

const edu::sharif::twinner::trace::Trace *InstructionSymbolicExecuter::getTrace () const {
  InstructionSymbolicExecuter *me = const_cast<InstructionSymbolicExecuter *> (this);
  me->lazyLoad ();
  return me->lazyTrace;
}

void InstructionSymbolicExecuter::lazyLoad () {
  if (lazyTrace == 0) {
    lazyTrace = new edu::sharif::twinner::trace::TraceImp
        (bufferForTraceLazyLoad, memoryManager);
  }
}

void InstructionSymbolicExecuter::disable () {
  disabled = true;
}

void InstructionSymbolicExecuter::enable () {
  disabled = false;
}

void InstructionSymbolicExecuter::syscallInvoked (const CONTEXT *context,
    edu::sharif::twinner::trace::Syscall s) {
  runHooks (context);
  if (disabled) {
    return;
  }
  getTrace ()->syscallInvoked (s);
  if (measureMode) {
    numberOfExecutedInstructions++;
  }
}

void InstructionSymbolicExecuter::syscallReturned (CONTEXT *context) const {
  if (disabled) {
    return;
  }
  const edu::sharif::twinner::trace::Trace *trace = getTrace ();
  trace->syscallReturned (context);
}

edu::sharif::twinner::util::MemoryManager *
InstructionSymbolicExecuter::getTraceMemoryManager () const {
  return memoryManager;
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcReg (AnalysisRoutine routine,
    REG dstReg, const ConcreteValue &dstRegVal,
    REG srcReg, const ConcreteValue &srcRegVal,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstRegSrcReg(INS: "
      << insAssemblyStr << "): dst reg: " << REG_StringShort (dstReg)
      << ", src reg: " << REG_StringShort (srcReg) << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcMutableReg (
    MutableSourceAnalysisRoutine routine,
    REG dstReg, const ConcreteValue &dstRegVal,
    REG srcReg, const ConcreteValue &srcRegVal,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstRegSrcMutableReg(INS: "
      << insAssemblyStr << "): dst reg: " << REG_StringShort (dstReg)
      << ", src reg: " << REG_StringShort (srcReg) << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcRegAuxReg (
    AuxOperandHavingAnalysisRoutine routine,
    REG dstReg, const ConcreteValue &dstRegVal,
    REG srcReg, const ConcreteValue &srcRegVal,
    REG auxReg, const ConcreteValue &auxRegVal,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstRegSrcRegAuxReg(INS: "
      << insAssemblyStr << "): dst reg: " << REG_StringShort (dstReg)
      << ", src reg: " << REG_StringShort (srcReg)
      << ", aux reg: " << REG_StringShort (auxReg) << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    RegisterResidentExpressionValueProxy (srcReg, srcRegVal),
                    RegisterResidentExpressionValueProxy (auxReg, auxRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcRegAuxImd (
    DoubleSourcesAnalysisRoutine routine,
    REG dstReg, const ConcreteValue &dstRegVal,
    REG srcReg, const ConcreteValue &srcRegVal,
    const ConcreteValue &auxImmediateValue,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << std::hex << "analysisRoutineDstRegSrcRegAuxImd(INS: "
      << insAssemblyStr << "): dst reg: " << REG_StringShort (dstReg)
      << ", src reg: " << REG_StringShort (srcReg)
      << ", aux imd: 0x" << auxImmediateValue << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    RegisterResidentExpressionValueProxy (srcReg, srcRegVal),
                    ConstantExpressionValueProxy (auxImmediateValue, 8));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcMem (AnalysisRoutine routine,
    REG dstReg, const ConcreteValue &dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << std::hex << "analysisRoutineDstRegSrcMem(INS: "
      << insAssemblyStr << "): dst reg: " << REG_StringShort (dstReg)
      << ", src mem addr: 0x" << srcMemoryEa << ", mem read bytes: 0x" << memReadBytes
      << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    MemoryResidentExpressionValueProxy (srcMemoryEa, (int) memReadBytes));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcMemAuxReg (
    AuxOperandHavingAnalysisRoutine routine,
    REG dstReg, const ConcreteValue &dstRegVal,
    ADDRINT srcMemoryEa,
    REG auxReg, const ConcreteValue &auxRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << std::hex << "analysisRoutineDstRegSrcMemAuxReg(INS: "
      << insAssemblyStr << "): dst reg: " << REG_StringShort (dstReg)
      << ", src mem addr: 0x" << srcMemoryEa << ", mem read bytes: 0x" << memReadBytes
      << ", aux reg: " << REG_StringShort (auxReg) << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    MemoryResidentExpressionValueProxy (srcMemoryEa, (int) memReadBytes),
                    RegisterResidentExpressionValueProxy (auxReg, auxRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcMemAuxImd (
    DoubleSourcesAnalysisRoutine routine,
    REG dstReg, const ConcreteValue &dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    const ConcreteValue &auxImmediateValue,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << std::hex << "analysisRoutineDstRegSrcMemAuxImd(INS: "
      << insAssemblyStr << "): dst reg: " << REG_StringShort (dstReg)
      << ", src mem addr: 0x" << srcMemoryEa << ", mem read bytes: 0x" << memReadBytes
      << ", aux imd: 0x" << auxImmediateValue << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    MemoryResidentExpressionValueProxy (srcMemoryEa, memReadBytes),
                    ConstantExpressionValueProxy (auxImmediateValue, 8));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcImd (AnalysisRoutine routine,
    REG dstReg, const ConcreteValue &dstRegVal,
    const ConcreteValue &srcImmediateValue,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << std::hex << "analysisRoutineDstRegSrcImd(INS: "
      << insAssemblyStr << "): dst reg: " << REG_StringShort (dstReg)
      << ", src imd: 0x" << srcImmediateValue << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    ConstantExpressionValueProxy (srcImmediateValue, REG_Size (dstReg) * 8));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcReg (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    REG srcReg, const ConcreteValue &srcRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << std::hex << "analysisRoutineDstMemSrcReg(INS: "
      << insAssemblyStr << "): dst mem addr: 0x" << dstMemoryEa
      << ", src reg: " << REG_StringShort (srcReg) << '\n';
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
                    RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcMutableReg (
    MutableSourceAnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    REG srcReg, const ConcreteValue &srcRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << std::hex << "analysisRoutineDstMemSrcMutableReg(INS: "
      << insAssemblyStr << "): dst mem addr: 0x" << dstMemoryEa
      << ", src reg: " << REG_StringShort (srcReg) << '\n';
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
                    RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcRegAuxReg (
    AuxOperandHavingAnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    REG srcReg, const ConcreteValue &srcRegVal,
    REG auxReg, const ConcreteValue &auxRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << std::hex << "analysisRoutineDstMemSrcRegAuxReg(INS: "
      << insAssemblyStr << "): dst mem addr: 0x" << dstMemoryEa
      << ", src reg: " << REG_StringShort (srcReg)
      << ", aux reg: " << REG_StringShort (auxReg) << '\n';
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
                    RegisterResidentExpressionValueProxy (srcReg, srcRegVal),
                    RegisterResidentExpressionValueProxy (auxReg, auxRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcImd (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    const ConcreteValue &srcImmediateValue,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  if (insAssemblyStr) {
    logger << "analysisRoutineDstMemSrcImd(INS: "
        << insAssemblyStr << ")";
  } else {
    // FIXME: This else-part is unreachable code
    disassembledInstruction = 0;
    logger << "analysisRoutineDstMemSrcImd()";
  }
  const UINT32 maxReadSizeInBytes = srcImmediateValue.getSize () / 8;
  if (memReadBytes > maxReadSizeInBytes) {
    // FIXME: This then-part is unreachable code
    edu::sharif::twinner::util::Logger::warning () << std::hex
        << "memReadBytes was 0x" << memReadBytes << " which is larger than size of "
        "srcImmediateValue (which is just 0x" << maxReadSizeInBytes << " bytes)"
        "; lowering the memReadBytes to 0x" << maxReadSizeInBytes << " bytes.\n";
    memReadBytes = maxReadSizeInBytes;
  }
  logger << std::hex << ": dst mem addr: 0x" << dstMemoryEa
      << ", src imd: 0x" << srcImmediateValue << '\n';
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
                    ConstantExpressionValueProxy (srcImmediateValue, memReadBytes * 8));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcImdAuxReg (
    AuxOperandHavingAnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    const ConcreteValue &srcImmediateValue,
    REG auxReg, const ConcreteValue &auxRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  if (insAssemblyStr) {
    logger << "analysisRoutineDstMemSrcImdAuxReg(INS: "
        << insAssemblyStr << ")";
  } else {
    // FIXME: This else-part is unreachable code
    disassembledInstruction = 0;
    logger << "analysisRoutineDstMemSrcImdAuxReg()";
  }
  const UINT32 maxReadSizeInBytes = srcImmediateValue.getSize () / 8;
  if (memReadBytes > maxReadSizeInBytes) {
    // FIXME: This then-part is unreachable code
    edu::sharif::twinner::util::Logger::warning () << std::hex
        << "memReadBytes was 0x" << memReadBytes << " which is larger than size of "
        "srcImmediateValue (which is just 0x" << maxReadSizeInBytes << " bytes)"
        "; lowering the memReadBytes to 0x" << maxReadSizeInBytes << " bytes.\n";
    memReadBytes = maxReadSizeInBytes;
  }
  logger << std::hex << ": dst mem addr: 0x" << dstMemoryEa
      << ", src imd: 0x" << srcImmediateValue
      << ", aux reg: " << REG_StringShort (auxReg) << '\n';
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
                    ConstantExpressionValueProxy (srcImmediateValue, memReadBytes * 8),
                    RegisterResidentExpressionValueProxy (auxReg, auxRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcMem (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << std::hex << "analysisRoutineDstMemSrcMem(INS: "
      << insAssemblyStr << "): dst mem addr: 0x" << dstMemoryEa
      << ", src mem addr: 0x" << srcMemoryEa << ", mem read bytes: 0x" << memReadBytes
      << '\n';
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa, (int) memReadBytes),
                    MemoryResidentExpressionValueProxy (srcMemoryEa, (int) memReadBytes));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcMemAuxReg (
    AuxOperandHavingAnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa,
    REG auxReg, const ConcreteValue &auxRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << std::hex << "analysisRoutineDstMemSrcMemAuxReg(INS: "
      << insAssemblyStr << "): dst mem addr: 0x" << dstMemoryEa
      << ", src mem addr: 0x" << srcMemoryEa << ", mem read bytes: 0x" << memReadBytes
      << ", aux reg: " << REG_StringShort (auxReg) << '\n';
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa, (int) memReadBytes),
                    MemoryResidentExpressionValueProxy (srcMemoryEa, (int) memReadBytes),
                    RegisterResidentExpressionValueProxy (auxReg, auxRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineConditionalBranch (
    ConditionalBranchAnalysisRoutine routine,
    BOOL branchTaken,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineConditionalBranch(INS: "
      << insAssemblyStr << "): branch taken: " << branchTaken << '\n';
  (this->*routine) (branchTaken);
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcAdg (AnalysisRoutine routine,
    REG dstReg, const ConcreteValue &dstRegVal,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << std::hex << "analysisRoutineDstRegSrcAdg(INS: "
      << insAssemblyStr << ") [AFTER execution of instruction]: dst reg: "
      << REG_StringShort (dstReg) << ", dst reg value: 0x"
      << dstRegVal << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    ConstantExpressionValueProxy (dstRegVal, REG_Size (dstReg) * 8));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineBeforeChangeOfReg (
    SuddenlyChangedRegAnalysisRoutine routine,
    REG reg,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineBeforeChangeOfReg(INS: "
      << insAssemblyStr << ")\n"
      "\tregistering register to be tracked...";
  trackedReg = reg;
  hook = routine;
  logger << "done\n";
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineTwoDstRegOneSrcReg (
    DoubleDestinationsAnalysisRoutine routine,
    REG dstLeftReg, const ConcreteValue &dstLeftRegVal,
    REG dstRightReg, const ConcreteValue &dstRightRegVal,
    REG srcReg, const ConcreteValue &srcRegVal,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineTwoDstRegOneSrcReg(INS: "
      << insAssemblyStr << "): left dst reg: " << REG_StringShort (dstLeftReg)
      << ", right dst reg: " << REG_StringShort (dstRightReg)
      << ", src reg: " << REG_StringShort (srcReg) << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstLeftReg, dstLeftRegVal),
                    RegisterResidentExpressionValueProxy (dstRightReg, dstRightRegVal),
                    RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineTwoDstRegOneSrcMem (
    DoubleDestinationsAnalysisRoutine routine,
    REG dstLeftReg, const ConcreteValue &dstLeftRegVal,
    REG dstRightReg, const ConcreteValue &dstRightRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineTwoDstRegOneSrcMem(INS: "
      << insAssemblyStr << "): left dst reg: " << REG_StringShort (dstLeftReg)
      << ", right dst reg: " << REG_StringShort (dstRightReg)
      << ", src mem addr: 0x" << srcMemoryEa << ", mem read bytes: 0x" << memReadBytes
      << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstLeftReg, dstLeftRegVal),
                    RegisterResidentExpressionValueProxy (dstRightReg, dstRightRegVal),
                    MemoryResidentExpressionValueProxy (srcMemoryEa, memReadBytes));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineTwoRegOneMem (
    DoubleDestinationsAnalysisRoutine routine,
    REG dstLeftReg, const ConcreteValue &dstLeftRegVal,
    REG dstRightReg, const ConcreteValue &dstRightRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineTwoRegOneMem(INS: "
      << insAssemblyStr << "): left dst reg: " << REG_StringShort (dstLeftReg)
      << ", right dst reg: " << REG_StringShort (dstRightReg)
      << ", src mem addr: 0x" << srcMemoryEa << ", mem read bytes: 0x" << memReadBytes
      << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstLeftReg, dstLeftRegVal),
                    RegisterResidentExpressionValueProxy (dstRightReg, dstRightRegVal),
                    MemoryResidentExpressionValueProxy (srcMemoryEa, memReadBytes));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineOneMemTwoReg (
    DoubleDestinationsAnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    REG dstReg, const ConcreteValue &dstRegVal,
    REG srcReg, const ConcreteValue &srcRegVal,
    UINT32 memReadBytes, UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineOneMemTwoReg(INS: "
      << insAssemblyStr << "): dst mem: 0x" << dstMemoryEa
      << ", dst reg: " << REG_StringShort (dstReg)
      << ", src reg: " << REG_StringShort (srcReg)
      << ", mem read bytes: 0x" << memReadBytes << '\n';
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
                    RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineTwoRegTwoMem (
    OneToThreeOperandsAnalysisRoutine routine,
    REG dstLeftReg, const ConcreteValue &dstLeftRegVal,
    REG dstRightReg, const ConcreteValue &dstRightRegVal,
    ADDRINT dstMemoryEa, ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineTwoRegTwoMem(INS: "
      << insAssemblyStr << "): left dst reg: " << REG_StringShort (dstLeftReg)
      << ", right dst reg: " << REG_StringShort (dstRightReg)
      << ", dst mem addr: 0x" << dstMemoryEa << ", src mem addr: 0x" << srcMemoryEa
      << ", mem read bytes: 0x" << memReadBytes << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstLeftReg, dstLeftRegVal),
                    RegisterResidentExpressionValueProxy (dstRightReg, dstRightRegVal),
                    MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
                    MemoryResidentExpressionValueProxy (srcMemoryEa, memReadBytes));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineAfterOperandLessInstruction (
    OperandLessAnalysisRoutine routine,
    const CONTEXT *context,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineAfterOperandLessInstruction(INS: "
      << insAssemblyStr << "): [AFTER execution of instruction]: operand-less ins\n";
  (this->*routine) (context);
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcImplicit (
    SingleOperandAnalysisRoutine routine,
    REG dstReg, const ConcreteValue &dstRegVal,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstRegSrcImplicit(INS: "
      << insAssemblyStr << "): reg operand: " << REG_StringShort (dstReg) << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcImplicit (
    SingleOperandAnalysisRoutine routine,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << std::hex << "analysisRoutineDstMemSrcImplicit(INS: "
      << insAssemblyStr << "): src mem addr: 0x" << srcMemoryEa
      << ", mem read bytes: 0x" << memReadBytes << '\n';
  (this->*routine) (MemoryResidentExpressionValueProxy (srcMemoryEa, (int) memReadBytes));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineRunHooks (const CONTEXT *context) {
  runHooks (context);
}

void InstructionSymbolicExecuter::analysisRoutineInitializeRegisters (
    CONTEXT *context) const {
  edu::sharif::twinner::util::Logger::loquacious () << "analysisRoutineInitializeRegisters\n";
  syscallReturned (context);
  PIN_ExecuteAt (context); // never returns
}

void InstructionSymbolicExecuter::analysisRoutineRepEqualOrRepNotEqualPrefix (REG repReg,
    const ConcreteValue &repRegVal, BOOL executing, BOOL repEqual,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineRepEqualOrRepNotEqualPrefix(INS: "
      << insAssemblyStr << "): rep reg: " << REG_StringShort (repReg)
      << ", executing: " << executing
      << ", rep equal: " << repEqual << '\n';
  repAnalysisRoutine (RegisterResidentExpressionValueProxy (repReg, repRegVal),
                      executing, repEqual);
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineMemoryRegisterCorrespondence (
    REG baseReg, const ConcreteValue &baseRegVal, ADDRDELTA displacement,
    ADDRINT memoryEa, UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineMemoryRegisterCorrespondence(INS: "
      << insAssemblyStr << "): base reg: " << REG_StringShort (baseReg)
      << ", displacement: " << std::dec << displacement
      << ", mem addr: 0x" << std::hex << memoryEa << '\n';
  memoryRegisterCorrespondenceAnalysisRoutine
      (RegisterResidentExpressionValueProxy (baseReg, baseRegVal),
       displacement, memoryEa);
}

void InstructionSymbolicExecuter::analysisRoutineMemoryIndexedRegisterCorrespondence (
    REG baseReg, const ConcreteValue &baseRegVal, ADDRDELTA displacement,
    REG indexReg, const ConcreteValue &indexRegVal, UINT32 scale,
    ADDRINT memoryEa, UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineMemoryIndexedRegisterCorrespondence(INS: "
      << insAssemblyStr << "): base reg: " << REG_StringShort (baseReg)
      << ", displacement: " << std::dec << displacement
      << ", index reg: " << REG_StringShort (indexReg)
      << ", scale: " << std::dec << scale
      << ", mem addr: 0x" << std::hex << memoryEa << '\n';
  memoryIndexedRegisterCorrespondenceAnalysisRoutine
      (RegisterResidentExpressionValueProxy (baseReg, baseRegVal), displacement,
       RegisterResidentExpressionValueProxy (indexReg, indexRegVal), scale,
       memoryEa);
}

void InstructionSymbolicExecuter::analysisRoutinePrefetchMem (
    ADDRINT memoryEa, UINT32 memReadBytes) {
  if (disabled) {
    return;
  }
  if (measureMode) {
    numberOfExecutedInstructions++;
  }
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "analysisRoutinePrefetchMem(...): mem addr: 0x"
      << std::hex << memoryEa << ", mem read bytes: 0x" << memReadBytes
      << '\n';
  MemoryResidentExpressionValueProxy memory (memoryEa, memReadBytes);
  memory.checkForOverwritingMemory (trace);
}

edu::sharif::twinner::trace::Expression *
InstructionSymbolicExecuter::getExpression (const ExpressionValueProxy &proxy,
    edu::sharif::twinner::trace::Trace *trace) const {
  edu::sharif::twinner::trace::StateSummary state;
  edu::sharif::twinner::trace::Expression *exp =
      proxy.getExpression (trace, state);
  if (state.isWrongState ()) {
    edu::sharif::twinner::util::Logger::error () << state.getMessage () << '\n';
    abort ();
  }
  return exp;
}

void InstructionSymbolicExecuter::setExpression (
    const MutableExpressionValueProxy &dst,
    edu::sharif::twinner::trace::Trace *trace,
    const edu::sharif::twinner::trace::Expression *exp) const {
  edu::sharif::twinner::trace::StateSummary state;
  dst.setExpression (trace, exp, state);
  if (state.isWrongState ()) {
    edu::sharif::twinner::util::Logger::error () << state.getMessage () << '\n';
    abort ();
  }
}

void InstructionSymbolicExecuter::runHooks (const CONTEXT *context) {
  if (trackedReg != REG_INVALID_) {
    ConcreteValue *value =
        edu::sharif::twinner::util::readRegisterContent (context, trackedReg);
    Hook hfunc = hook;
    trackedReg = REG_INVALID_;
    hook = 0;
    (this->*hfunc) (context, *value);
    delete value;

  } else if (operandSize > 0) {
    edu::sharif::twinner::trace::cv::ConcreteValue64Bits os (operandSize);
    Hook hfunc = hook;
    operandSize = -1;
    hook = 0;
    (this->*hfunc) (context, os);
  }
}

void InstructionSymbolicExecuter::cmovbeAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "cmovbeAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool belowOrEqual;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForBelowOrEqualCase
      (belowOrEqual, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  if (belowOrEqual) {
    edu::sharif::twinner::util::Logger::loquacious () << "\texecuting the actual move...";
    movAnalysisRoutine (dst, src);
  } else {
    edu::sharif::twinner::util::Logger::loquacious () << "\tignoring the move...";
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::cmovnbeAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "cmovnbeAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool belowOrEqual;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForBelowOrEqualCase
      (belowOrEqual, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  if (!belowOrEqual) {
    edu::sharif::twinner::util::Logger::loquacious () << "\texecuting the actual move...";
    movAnalysisRoutine (dst, src);
  } else {
    edu::sharif::twinner::util::Logger::loquacious () << "\tignoring the move...";
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::cmpxchgAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src,
    const MutableExpressionValueProxy &aux) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "cmpxchgAnalysisRoutine(...)\n"
      << "\tcomparison part...";
  cmpAnalysisRoutine (aux, dst);
  edu::sharif::twinner::util::Logger::loquacious () << "\texchange part...";
  bool zero;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForZeroCase (zero, disassembledInstruction);
  if (zero) { // equal
    movAnalysisRoutine (dst, src);
  } else {
    movAnalysisRoutine (aux, dst);
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::pshufdAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src,
    const ExpressionValueProxy &order) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "pshufdAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting order byte...";
  const edu::sharif::twinner::trace::Expression *orderexp =
      getExpression (order, trace);

  struct OrderByte {
    unsigned int b0 : 2;
    unsigned int b1 : 2;
    unsigned int b2 : 2;
    unsigned int b3 : 2;
  } orderByte;
  *reinterpret_cast<UINT8 *> (&orderByte) =
      UINT8 (orderexp->getLastConcreteValue ().toUint64 ());
  delete orderexp;
  const unsigned int ob[] = {orderByte.b0, orderByte.b1, orderByte.b2, orderByte.b3};
  edu::sharif::twinner::trace::Expression *res =
      new edu::sharif::twinner::trace::ExpressionImp
      (new edu::sharif::twinner::trace::cv::ConcreteValue128Bits ());
  edu::sharif::twinner::util::Logger::loquacious () << "\tshuffling words...";
  for (int i = 0; i < 4; ++i) {
    edu::sharif::twinner::trace::Expression *exp = srcexp->clone ();
    exp->shiftToRight (ob[i] * 32);
    exp->truncate (32);
    exp->shiftToLeft (i * 32);
    res->bitwiseOr (exp);
    delete exp;
  }
  delete srcexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  setExpression (dst, trace, res);
  delete res;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::xchgAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const MutableExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "xchgAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp = getExpression (dst, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  setExpression (dst, trace, srcexp);
  delete srcexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting src exp...";
  setExpression (src, trace, dstexp);
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::movAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "movAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  setExpression (dst, trace, srcexp);
  delete srcexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::movsxAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "movsxAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp = getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsign-extending...";
  edu::sharif::twinner::trace::Expression *signExtendedExp =
      srcexp->signExtended (dst.getSize ());
  delete srcexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  setExpression (dst, trace, signExtendedExp);
  delete signExtendedExp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::movsAnalysisRoutine (
    const MutableExpressionValueProxy &rdi, const MutableExpressionValueProxy &rsi,
    const MutableExpressionValueProxy &dstMem, const ExpressionValueProxy &srcMem) {
  edu::sharif::twinner::util::Logger::loquacious () << "movsAnalysisRoutine(...)\n";
  movAnalysisRoutine (dstMem, srcMem);
  const int size = dstMem.getSize () / 8;
  adjustRsiRdiRegisters (size, rdi, rsi);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::cmpsAnalysisRoutine (
    const MutableExpressionValueProxy &rdi, const MutableExpressionValueProxy &rsi,
    const MutableExpressionValueProxy &dstMem, const ExpressionValueProxy &srcMem) {
  edu::sharif::twinner::util::Logger::loquacious () << "cmpsAnalysisRoutine(...)\n";
  cmpAnalysisRoutine (dstMem, srcMem);
  const int size = dstMem.getSize () / 8;
  adjustRsiRdiRegisters (size, rdi, rsi);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::adjustRsiRdiRegisters (int size,
    const MutableExpressionValueProxy &rdi, const MutableExpressionValueProxy &rsi) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tadjusting rsi/rdi values...";
  edu::sharif::twinner::trace::Expression *rdiexp = getExpression (rdi, trace);
  edu::sharif::twinner::trace::Expression *rsiexp = getExpression (rsi, trace);
  if (eflags.getDirectionFlag ()) { // DF == 1
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tdecrementing index register...";
    rdiexp->minus (size);
    rsiexp->minus (size);
  } else { // DF == 0
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tincrementing index register...";
    rdiexp->add (size);
    rsiexp->add (size);
  }
  setExpression (rdi, trace, rdiexp);
  setExpression (rsi, trace, rsiexp);
  delete rdiexp;
  delete rsiexp;
}

void InstructionSymbolicExecuter::pushAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src,
    const MutableExpressionValueProxy &rsp) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "pushAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  setExpression (dst, trace, srcexp);
  delete srcexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tadjusting rsp...";
  edu::sharif::twinner::trace::Expression *rspexp = getExpression (rsp, trace);
  rspexp->minus (8);
  setExpression (rsp, trace, rspexp);
  delete rspexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::popAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src,
    const MutableExpressionValueProxy &rsp) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "popAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  setExpression (dst, trace, srcexp);
  delete srcexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tadjusting rsp...";
  edu::sharif::twinner::trace::Expression *rspexp = getExpression (rsp, trace);
  rspexp->add (8);
  setExpression (rsp, trace, rspexp);
  delete rspexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::lodsdAnalysisRoutine (
    const MutableExpressionValueProxy &dstReg, const ExpressionValueProxy &srcMem,
    const MutableExpressionValueProxy &rsi) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "lodsdAnalysisRoutine(...)\n";
  movAnalysisRoutine (dstReg, srcMem);
  edu::sharif::twinner::trace::Expression *rsiexp = getExpression (rsi, trace);
  if (eflags.getDirectionFlag ()) { // DF == 1
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tdecrementing rsi/index register...";
    rsiexp->minus (dstReg.getSize () / 8);
  } else { // DF == 0
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tincrementing rsi/index register...";
    rsiexp->add (dstReg.getSize () / 8);
  }
  setExpression (rsi, trace, rsiexp);
  delete rsiexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::addAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "addAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexpOrig =
      getExpression (dst, trace);
  edu::sharif::twinner::trace::Expression *dstexp = dstexpOrig->clone ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tbinary operation...";
  dstexp->add (srcexp);
  setExpression (dst, trace, dstexp);
  delete dstexp;
  eflags.setFlags
      (new edu::sharif::twinner::twintool::operationgroup::AdditionOperationGroup
       (dstexpOrig, srcexp));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::adcAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "adcAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexp =
      getExpression (dst, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting carry exp...";
  const edu::sharif::twinner::trace::Expression *carryexp = eflags.getCarryFlag ();
  edu::sharif::twinner::trace::Expression *exp = carryexp->clone ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tbinary operation...";
  exp->add (srcexp);
  exp->add (dstexp);
  setExpression (dst, trace, exp);
  delete exp;
  eflags.setFlags
      (new edu::sharif::twinner::twintool::operationgroup::AddWithCarryOperationGroup
       (dstexp, srcexp, carryexp));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::subAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "subAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexpOrig =
      getExpression (dst, trace);
  edu::sharif::twinner::trace::Expression *dstexp = dstexpOrig->clone ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tbinary operation...";
  dstexp->minus (srcexp);
  setExpression (dst, trace, dstexp);
  delete dstexp;
  eflags.setFlags
      (new edu::sharif::twinner::twintool::operationgroup::SubtractOperationGroup
       (dstexpOrig, srcexp));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::sbbAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "sbbAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexp =
      getExpression (dst, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting carry exp...";
  const edu::sharif::twinner::trace::Expression *carryexp = eflags.getCarryFlag ();
  edu::sharif::twinner::trace::Expression *exp = dstexp->clone ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tbinary operation...";
  exp->minus (srcexp);
  exp->minus (carryexp);
  setExpression (dst, trace, exp);
  delete exp;
  delete dstexp;
  delete srcexp;
  delete carryexp;
  eflags.setFlags
      (new edu::sharif::twinner::twintool::operationgroup::DummyOperationGroup
       ("SubtractWithBorrowOperationGroup"));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::cmpAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "cmpAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexp =
      getExpression (dst, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting EFLAGS...";
  eflags.setFlags
      (new edu::sharif::twinner::twintool::operationgroup::SubtractOperationGroup
       (dstexp, srcexp));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::leaAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "leaAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  setExpression (dst, trace, srcexp);
  delete srcexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jnzAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "jnzAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool zero;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForZeroCase (zero, disassembledInstruction);
  if (zero == branchTaken) {
    edu::sharif::twinner::util::Logger::error ()
        << "InstructionSymbolicExecuter::jnzAnalysisRoutine"
        " (branchTaken=" << branchTaken << "):"
        " JNZ branching and last known EFLAGS state do not match\n";
    abort ();
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jzAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "jzAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool zero;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForZeroCase (zero, disassembledInstruction);
  if (zero != branchTaken) {
    edu::sharif::twinner::util::Logger::error ()
        << "InstructionSymbolicExecuter::jzAnalysisRoutine"
        " (branchTaken=" << branchTaken << "):"
        " JZ branching and last known EFLAGS state do not match\n";
    abort ();
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jleAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "jleAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool lessOrEqual;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForLessOrEqualCase
      (lessOrEqual, disassembledInstruction);
  if (lessOrEqual != branchTaken) {
    edu::sharif::twinner::util::Logger::error ()
        << "InstructionSymbolicExecuter::jleAnalysisRoutine"
        " (branchTaken=" << branchTaken << "):"
        " JLE branching and last known EFLAGS state do not match\n";
    abort ();
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jnleAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "jnleAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool lessOrEqual;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForLessOrEqualCase
      (lessOrEqual, disassembledInstruction);
  if (lessOrEqual == branchTaken) {
    edu::sharif::twinner::util::Logger::error ()
        << "InstructionSymbolicExecuter::jnleAnalysisRoutine"
        " (branchTaken=" << branchTaken << "):"
        " JNLE branching and last known EFLAGS state do not match\n";
    abort ();
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jlAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "jlAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool less;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForLessCase (less, disassembledInstruction);
  if (less != branchTaken) {
    edu::sharif::twinner::util::Logger::error ()
        << "InstructionSymbolicExecuter::jlAnalysisRoutine"
        " (branchTaken=" << branchTaken << "):"
        " JL branching and last known EFLAGS state do not match\n";
    abort ();
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jnlAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "jnlAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool less;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForLessCase (less, disassembledInstruction);
  if (less == branchTaken) {
    edu::sharif::twinner::util::Logger::error ()
        << "InstructionSymbolicExecuter::jnlAnalysisRoutine"
        " (branchTaken=" << branchTaken << "):"
        " JNL branching and last known EFLAGS state do not match\n";
    abort ();
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jbeAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "jbeAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool belowOrEqual;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForBelowOrEqualCase
      (belowOrEqual, disassembledInstruction);
  if (belowOrEqual != branchTaken) {
    edu::sharif::twinner::util::Logger::error ()
        << "InstructionSymbolicExecuter::jbeAnalysisRoutine"
        " (branchTaken=" << branchTaken << "):"
        " JBE branching and last known EFLAGS state do not match\n";
    abort ();
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jnbeAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "jnbeAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool belowOrEqual;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForBelowOrEqualCase
      (belowOrEqual, disassembledInstruction);
  if (belowOrEqual == branchTaken) {
    edu::sharif::twinner::util::Logger::error ()
        << "InstructionSymbolicExecuter::jnbeAnalysisRoutine"
        " (branchTaken=" << branchTaken << "):"
        " JNBE branching and last known EFLAGS state do not match\n";
    abort ();
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jnbAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "jnbAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool below;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForBelowCase (below, disassembledInstruction);
  if (below == branchTaken) {
    edu::sharif::twinner::util::Logger::error ()
        << "InstructionSymbolicExecuter::jnbAnalysisRoutine"
        " (branchTaken=" << branchTaken << "):"
        " JNB branching and last known EFLAGS state do not match\n";
    abort ();
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jbAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "jbAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool below;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForBelowCase (below, disassembledInstruction);
  if (below != branchTaken) {
    edu::sharif::twinner::util::Logger::error ()
        << "InstructionSymbolicExecuter::jbAnalysisRoutine"
        " (branchTaken=" << branchTaken << "):"
        " JB branching and last known EFLAGS state do not match\n";
    abort ();
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jsAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "jsAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool sign;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForSignCase (sign, disassembledInstruction);
  if (sign != branchTaken) {
    edu::sharif::twinner::util::Logger::error ()
        << "InstructionSymbolicExecuter::jsAnalysisRoutine"
        " (branchTaken=" << branchTaken << "):"
        "JS branching and last known EFLAGS state do not match\n";
    abort ();
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jnsAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "jnsAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool sign;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForSignCase (sign, disassembledInstruction);
  if (sign == branchTaken) {
    edu::sharif::twinner::util::Logger::error ()
        << "InstructionSymbolicExecuter::jnsAnalysisRoutine"
        " (branchTaken=" << branchTaken << "):"
        " JNS branching and last known EFLAGS state do not match\n";
    abort ();
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::callAnalysisRoutine (const CONTEXT *context,
    const ConcreteValue &rspRegVal) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "callAnalysisRoutine(...)\n"
      << "\tgetting rsp reg exp...";
  edu::sharif::twinner::trace::Expression *rsp =
#ifdef TARGET_IA32E
      trace->tryToGetSymbolicExpressionByRegister (64, REG_RSP);
#else
      trace->tryToGetSymbolicExpressionByRegister (32, REG_ESP);
#endif
  if (rsp) { // If we are not tracking RSP yet, it's not required to adjust its value
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tadjusting rsp...";
    const ConcreteValue &oldVal = rsp->getLastConcreteValue ();
    if (oldVal > rspRegVal) {
      // some items have been pushed into stack by CALL and so RSP is decremented
      ConcreteValue *cv = oldVal.clone ();
      (*cv) -= rspRegVal;
      if ((*cv) == STACK_OPERATION_UNIT_SIZE) {
        edu::sharif::twinner::util::Logger::loquacious ()
            << "\tupdating stack (pushing the ret address)...";
        MemoryResidentExpressionValueProxy stack (rspRegVal.toUint64 (),
                                                  STACK_OPERATION_UNIT_SIZE);
        edu::sharif::twinner::trace::StateSummary state;
        edu::sharif::twinner::trace::Expression *exp =
            stack.getExpression (trace, state);
        if (exp) {
          delete exp;
        }
        if (state.isWrongState ()) {
          const edu::sharif::twinner::trace::Expression *exp =
              new edu::sharif::twinner::trace::ExpressionImp
              (state.getExpectedStateValue ());
          setExpression (stack, trace, exp);
          delete exp;
        }
      } else {
        edu::sharif::twinner::util::Logger::warning ()
            << "CALL decremented RSP more/less than "
            << STACK_OPERATION_UNIT_SIZE << " bytes;"
            " check for CALL_FAR instruction!\n";
      }
      rsp->minus (cv);
      // TODO: call valueIsChanged from an expression proxy to address ESP, SP, and SPL

    } else {
      edu::sharif::twinner::util::Logger::warning ()
          << "RSP is not decremented at all after CALL instruction!\n";
    }
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::retAnalysisRoutine (const CONTEXT *context,
    const ConcreteValue &rspRegVal) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "retAnalysisRoutine(...)\n"
      << "\tgetting rsp reg exp...";
  edu::sharif::twinner::trace::Expression *rsp =
#ifdef TARGET_IA32E
      trace->tryToGetSymbolicExpressionByRegister (64, REG_RSP);
#else
      trace->tryToGetSymbolicExpressionByRegister (32, REG_ESP);
#endif
  if (rsp) { // If we are not tracking RSP yet, it's not required to adjust its value
    const ConcreteValue &oldVal = rsp->getLastConcreteValue ();
    if (oldVal < rspRegVal) {
      // some items have been popped out from the stack by RET and so RSP is incremented
      ConcreteValue *cv = rspRegVal.clone ();
      (*cv) -= oldVal;
      edu::sharif::twinner::util::Logger::loquacious ()
          << "\tadjusting rsp, amount = " << *cv;
      const bool normalRetInstruction = ((*cv) == STACK_OPERATION_UNIT_SIZE)
          || ((*cv) == 2 * STACK_OPERATION_UNIT_SIZE);
      if (!normalRetInstruction) {
        edu::sharif::twinner::util::Logger::error ()
            << "InstructionSymbolicExecuter::retAnalysisRoutine (...): "
            "ret instruction must pop either "
            << STACK_OPERATION_UNIT_SIZE << " or "
            << (2 * STACK_OPERATION_UNIT_SIZE) << " bytes\n";
        abort ();
      }
      rsp->add (cv);
      // TODO: call valueIsChanged from an expression proxy to address ESP, SP, and SPL

    } else {
      edu::sharif::twinner::util::Logger::warning ()
          << "RSP is not incremented at all after RET instruction!\n";
    }
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jmpAnalysisRoutine (const CONTEXT *context,
    const ConcreteValue &rspRegVal) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "jmpAnalysisRoutine(...)\n"
      << "\tgetting rsp reg exp...";
  edu::sharif::twinner::trace::Expression *rsp =
#ifdef TARGET_IA32E
      trace->tryToGetSymbolicExpressionByRegister (64, REG_RSP);
#else
      trace->tryToGetSymbolicExpressionByRegister (64, REG_ESP);
#endif
  if (rsp) { // If we are not tracking RSP yet, it's not required to adjust its value
    const ConcreteValue &oldVal = rsp->getLastConcreteValue ();
    if (oldVal != rspRegVal) { // This jump had side-effect on RSP
      if (oldVal < rspRegVal) {
        ConcreteValue *cv = rspRegVal.clone ();
        (*cv) -= oldVal;
        edu::sharif::twinner::util::Logger::warning ()
            << "JMP instruction popped items out of stack"
            ", amount = " << *cv << '\n';
      } else { // oldVal > rspRegVal
        ConcreteValue *cv = oldVal.clone ();
        (*cv) -= rspRegVal;
        edu::sharif::twinner::util::Logger::warning ()
            << "JMP instruction pushed items into stack"
            ", amount = " << *cv << '\n';
      }
      edu::sharif::twinner::util::Logger::error ()
          << "InstructionSymbolicExecuter::jmpAnalysisRoutine (...):"
          " jmp instruction must not have any side effect"
          " (it changed the RSP)\n";
      abort ();
    }
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::repAnalysisRoutine (
    const MutableExpressionValueProxy &dst, bool executing, bool repEqual) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "repAnalysisRoutine(...)\n"
      << "\tgetting dst (rep) reg exp...";
  edu::sharif::twinner::trace::Expression *dstexp = getExpression (dst, trace);
  bool zero;
  edu::sharif::twinner::trace::Constraint *cc =
      edu::sharif::twinner::trace::Constraint::instantiateEqualConstraint
      (zero, dstexp, disassembledInstruction);
  if (zero == executing) {
    edu::sharif::twinner::util::Logger::error ()
        << "InstructionSymbolicExecuter::repAnalysisRoutine (...): "
        "REP count and executing state do not match\n";
    abort ();
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  if (executing) {
    edu::sharif::twinner::util::Logger::loquacious () << "\tdecrementing count reg...";
    dstexp->minus (1);
    setExpression (dst, trace, dstexp);
  }
  delete dstexp;
  std::list <edu::sharif::twinner::trace::Constraint *> ccList;
  ccList.push_front (cc);
  trace->addPathConstraints (ccList);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::shlAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "shlAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexpOrig =
      getExpression (dst, trace);
  edu::sharif::twinner::trace::Expression *dstexp = dstexpOrig->clone ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tshifting operation...";
  if (dynamic_cast<const ConstantExpressionValueProxy *> (&src) != 0) {
    // src was an immediate value
    dstexp->shiftToLeft (srcexp->getLastConcreteValue ().clone ());
  } else {
    // src was CL register
    dstexp->shiftToLeft (srcexp);
  }
  // truncate bits which are shifted left, outside of dst boundaries
  dst.truncate (dstexp);
  setExpression (dst, trace, dstexp);
  delete dstexp;
  delete dstexpOrig;
  delete srcexp;
  eflags.setFlags (new edu::sharif::twinner::twintool::operationgroup::DummyOperationGroup
                   ("ShiftLeftOperationGroup"));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::shrAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "shrAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexpOrig =
      getExpression (dst, trace);
  edu::sharif::twinner::trace::Expression *dstexp = dstexpOrig->clone ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tshifting operation...";
  dstexp->shiftToRight (srcexp);
  setExpression (dst, trace, dstexp);
  delete dstexp;
  eflags.setFlags
      (new edu::sharif::twinner::twintool::operationgroup::ShiftRightOperationGroup
       (dstexpOrig, srcexp));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::sarAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "sarAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexpOrig =
      getExpression (dst, trace);
  edu::sharif::twinner::trace::Expression *dstexp = dstexpOrig->clone ();
  const int size = dst.getSize ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tshifting operation...";
  dstexp->arithmeticShiftToRight (srcexp);
  {//TODO: Remove following code when all sizes have their own concrete value imps.
    const ConcreteValue &srcval = srcexp->getLastConcreteValue ();
    const ConcreteValue &dstval = dstexp->getLastConcreteValue ();
    ConcreteValue *tmp = dstval.clone ();
    ConcreteValue *sizeval = srcval.twosComplement ();
    (*sizeval) += size - 1;
    (*tmp) >>= (*sizeval);
    if ((*tmp) == 1) {
      (*tmp) <<= srcval;
      (*tmp) -= 1;
      (*sizeval) += 1;
      (*tmp) <<= (*sizeval);
      delete sizeval;
      (*tmp) |= dstval;
      dstexp->setLastConcreteValue (tmp);
    } else {
      delete sizeval;
      delete tmp;
    }
  }
  setExpression (dst, trace, dstexp);
  delete dstexp;
  eflags.setFlags (new edu::sharif::twinner::twintool::operationgroup
                   ::ShiftArithmeticRightOperationGroup (dstexpOrig, srcexp));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::rorAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "rorAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      getExpression (dst, trace);
  edu::sharif::twinner::trace::Expression *lsb = dstexp->clone ();
  lsb->truncate (1);
  // TODO: set lsb as the carry flag (CF) value
  delete lsb;
  edu::sharif::twinner::util::Logger::loquacious () << "\trotating-right operation...";
  dstexp->rotateToRight (srcexp);
  delete srcexp;
  setExpression (dst, trace, dstexp);
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::rolAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "rolAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      getExpression (dst, trace);
  edu::sharif::twinner::trace::Expression *msb = dstexp->clone ();
  msb->shiftToRight (dstexp->getLastConcreteValue ().getSize () - 1);
  msb->truncate (1);
  // TODO: set msb as the carry flag (CF) value
  delete msb;
  edu::sharif::twinner::util::Logger::loquacious () << "\trotating-left operation...";
  dstexp->rotateToLeft (srcexp);
  delete srcexp;
  setExpression (dst, trace, dstexp);
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::andAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "andAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp = getExpression (dst, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tbinary operation...";
  dstexp->bitwiseAnd (srcexp);
  delete srcexp;
  setExpression (dst, trace, dstexp);
  eflags.setFlags
      (new edu::sharif::twinner::twintool::operationgroup::BitwiseAndOperationGroup
       (dstexp));
  eflags.setOverflowFlag (false);
  eflags.setCarryFlag (false);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::orAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "orAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp = getExpression (dst, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tbinary operation...";
  dstexp->bitwiseOr (srcexp);
  delete srcexp;
  setExpression (dst, trace, dstexp);
  eflags.setFlags
      (new edu::sharif::twinner::twintool::operationgroup::BitwiseAndOperationGroup
       (dstexp));
  eflags.setOverflowFlag (false);
  eflags.setCarryFlag (false);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::xorAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "xorAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp = getExpression (dst, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tbinary operation...";
  dstexp->bitwiseXor (srcexp);
  delete srcexp;
  setExpression (dst, trace, dstexp);
  eflags.setFlags
      (new edu::sharif::twinner::twintool::operationgroup::BitwiseAndOperationGroup
       (dstexp));
  eflags.setOverflowFlag (false);
  eflags.setCarryFlag (false);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::testAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "testAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp = getExpression (dst, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tbinary operation...";
  dstexp->bitwiseAnd (srcexp);
  delete srcexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting EFLAGS...";
  eflags.setFlags
      (new edu::sharif::twinner::twintool::operationgroup::BitwiseAndOperationGroup
       (dstexp));
  eflags.setOverflowFlag (false);
  eflags.setCarryFlag (false);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::btAnalysisRoutine (
    const MutableExpressionValueProxy &bitstring, const ExpressionValueProxy &offset) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "btAnalysisRoutine(...)\n"
      << "\tgetting offset exp...";
  edu::sharif::twinner::trace::Expression *offsetexp =
      getExpression (offset, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting bitstring exp...";
  edu::sharif::twinner::trace::Expression *bitstringexp =
      getExpression (bitstring, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tfinding requested bit...";
  offsetexp->bitwiseAnd (bitstring.getSize () - 1);
  bitstringexp->shiftToRight (offsetexp);
  bitstringexp->bitwiseAnd (0x1);
  delete offsetexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting EFLAGS...";
  eflags.setCarryFlag (bitstringexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::pmovmskbAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "pmovmskbAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tpreparing mask-byte(src)...";
  edu::sharif::twinner::trace::Expression *maskbyte =
      new edu::sharif::twinner::trace::ExpressionImp (); // zero-filled
  // src is a reg and is mutable
  const int size = static_cast<const MutableExpressionValueProxy &> (src).getSize ();
  for (int i = 7, loc = 0; i < size; i += 8) {
    edu::sharif::twinner::trace::Expression *ithBit = srcexp->clone ();
    ithBit->shiftToRight (i - loc); // it is (i+1)-th bit in 1-counting mode
    ithBit->bitwiseAnd (1 << loc); // i-th bit in 0-counting
    loc++;
    maskbyte->bitwiseOr (ithBit);
    delete ithBit;
  }
  delete srcexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  setExpression (dst, trace, maskbyte);
  delete maskbyte;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::pcmpeqbAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "pcmpeqbAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexp =
      getExpression (dst, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tcomparing...";
  const int size = dst.getSize ();
  ConcreteValue *result = dstexp->getLastConcreteValue ().clone ();
  *result = 0;
  std::list <edu::sharif::twinner::trace::Constraint *> cc;
  bool equal;
  for (int i = 0; i < size; i += 8) {
    edu::sharif::twinner::trace::Expression *ithByteSrc = srcexp->clone ();
    edu::sharif::twinner::trace::Expression *ithByteDst = dstexp->clone ();
    ithByteSrc->shiftToRight (i);
    ithByteSrc->bitwiseAnd (0xFF);
    ithByteDst->shiftToRight (i);
    ithByteDst->bitwiseAnd (0xFF);
    //    edu::sharif::twinner::util::Logger::warning () << "byte: " << i
    //        << " from src: " << ithByteSrc << " and from dst: " << ithByteDst << "\n";
    ithByteSrc->minus (ithByteDst);
    edu::sharif::twinner::trace::Constraint *ithBytesAreEqualConstraint =
        edu::sharif::twinner::trace::Constraint::instantiateEqualConstraint
        (equal, ithByteSrc, disassembledInstruction);
    cc.push_back (ithBytesAreEqualConstraint);
    ConcreteValue *c = dstexp->getLastConcreteValue ().clone ();
    *c = equal ? 0xFF : 0x00;
    delete ithByteSrc;
    delete ithByteDst;
    (*c) <<= i;
    (*result) |= (*c);
    delete c;
  }
  trace->addPathConstraints (cc);
  delete srcexp;
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  const edu::sharif::twinner::trace::Expression *resexp =
      new edu::sharif::twinner::trace::ExpressionImp (result);
  setExpression (dst, trace, resexp);
  delete resexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::pminubAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "pminubAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp = getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp = getExpression (dst, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tcalculating the minimum...";
  const int size = dst.getSize ();
  ConcreteValue *mask = dstexp->getLastConcreteValue ().clone ();
  *mask = 0;
  std::list <edu::sharif::twinner::trace::Constraint *> cc;
  bool below;
  for (int i = 0; i < size; i += 8) {
    edu::sharif::twinner::trace::Expression *ithByteSrc = srcexp->clone ();
    edu::sharif::twinner::trace::Expression *ithByteDst = dstexp->clone ();
    ithByteSrc->shiftToRight (i);
    ithByteSrc->bitwiseAnd (0xFF);
    ithByteDst->shiftToRight (i);
    ithByteDst->bitwiseAnd (0xFF);
    edu::sharif::twinner::trace::Constraint *srcIsBelowDstConstraint =
        edu::sharif::twinner::trace::Constraint::instantiateBelowConstraint
        (below, ithByteSrc, ithByteDst, disassembledInstruction);
    cc.push_back (srcIsBelowDstConstraint);
    if (below) {
      ConcreteValue *c = dstexp->getLastConcreteValue ().clone ();
      *c = 0xFF;
      (*c) <<= i;
      (*mask) |= (*c);
      delete c;
    }
    delete ithByteSrc;
    delete ithByteDst;
  }
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\ttransferring (mask & src) to (dst) for mask=" << (*mask);
  dstexp->bitwiseAnd (mask->bitwiseNegated ());
  srcexp->bitwiseAnd (mask);
  dstexp->bitwiseOr (srcexp);
  delete srcexp;
  setExpression (dst, trace, dstexp);
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::punpcklbwAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "punpcklbwAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexp =
      getExpression (dst, trace);
  const int size = dst.getSize ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tunpacking and interleaving low-data byte-to-word...";
  edu::sharif::twinner::trace::Expression *res = dstexp->clone ();
  res->truncate (8);
  // dst: d3 d2 d1 d0 | src: s3 s2 s1 s0
  // res: s3 d3 s2 d2 s1 d1 s0 d0
  UINT64 byteMask = 0xFF;
  const edu::sharif::twinner::trace::Expression *operand = srcexp;
  int bytesNumber = size / 16;
  for (int k = 0; k < 2; ++k) {
    for (int i = 0; i < bytesNumber; ++i) {
      const int shift = (i + 1) * 8;
      edu::sharif::twinner::trace::Expression *nextByte = operand->clone (size);
      nextByte->bitwiseAnd (byteMask);
      nextByte->shiftToLeft (shift);
      res->bitwiseOr (nextByte);
      delete nextByte;
      byteMask <<= 8;
    }
    byteMask = 0xFF00;
    operand = dstexp;
    --bytesNumber;
  }
  setExpression (dst, trace, res);
  delete res;
  delete srcexp;
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::punpcklwdAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "punpcklwdAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexp =
      getExpression (dst, trace);
  const int size = dst.getSize ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tunpacking and interleaving low-data word-to-double-word...";
  edu::sharif::twinner::trace::Expression *res = dstexp->clone ();
  res->truncate (16);
  // dst: d3 d2 d1 d0 | src: s3 s2 s1 s0 (di and si are 16-bits/word)
  // res: s3 d3 s2 d2 s1 d1 s0 d0
  UINT64 wordMask = 0xFFFF;
  const edu::sharif::twinner::trace::Expression *operand = srcexp;
  int wordsNumber = size / 32;
  for (int k = 0; k < 2; ++k) {
    for (int i = 0; i < wordsNumber; ++i) {
      const int shift = (i + 1) * 16;
      edu::sharif::twinner::trace::Expression *nextWord = operand->clone (size);
      nextWord->bitwiseAnd (wordMask);
      nextWord->shiftToLeft (shift);
      res->bitwiseOr (nextWord);
      delete nextWord;
      wordMask <<= 16;
    }
    wordMask = 0xFFFF0000;
    operand = dstexp;
    --wordsNumber;
  }
  setExpression (dst, trace, res);
  delete res;
  delete srcexp;
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::bsfAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "bsfAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  const edu::sharif::twinner::trace::cv::ConcreteValue &cv = srcexp->getLastConcreteValue ();
  UINT64 i = 0;
  for (unsigned int s = cv.getSize (); i < s; ++i) {
    edu::sharif::twinner::trace::cv::ConcreteValue *bit = cv.clone ();
    (*bit) >>= i;
    (*bit) &= 1;
    if ((*bit) == 1) {
      delete bit;
      break;
    }
    delete bit;
  }
  edu::sharif::twinner::trace::Expression *indexexp =
      new edu::sharif::twinner::trace::ExpressionImp (i);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  setExpression (dst, trace, indexexp);
  delete indexexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  edu::sharif::twinner::trace::Expression *conditionExp = srcexp;
  edu::sharif::twinner::trace::cv::ConcreteValue *bit = cv.clone ();
  (*bit) = 1;
  (*bit) <<= i;
  conditionExp->truncate (i + 1);
  conditionExp->minus (bit); // takes ownership of bit
  std::list <edu::sharif::twinner::trace::Constraint *> cc;
  edu::sharif::twinner::trace::Constraint *bsfConstraint =
      new edu::sharif::twinner::trace::Constraint
      (conditionExp, edu::sharif::twinner::trace::Constraint::ZERO,
       disassembledInstruction, false);
  cc.push_back (bsfConstraint);
  delete conditionExp;
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::divAnalysisRoutine (
    const MutableExpressionValueProxy &leftDst,
    const MutableExpressionValueProxy &rightDst,
    const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "divAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting left dst exp...";
  edu::sharif::twinner::trace::Expression *leftDstExp =
      getExpression (leftDst, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting right dst exp...";
  edu::sharif::twinner::trace::Expression *rightDstExp =
      getExpression (rightDst, trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tpreparing left-right in both dst regs...";
  operandSize = leftDst.getSize ();
  leftDstExp->shiftToLeft (operandSize);
  leftDstExp->bitwiseOr (rightDstExp);
  delete rightDstExp;
  rightDstExp = leftDstExp->clone ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tcalculating quotient (right) and remainder (left) of division...";
  leftDstExp->remainder (srcexp);
  rightDstExp->divide (srcexp);
  delete srcexp;
  leftDst.setExpressionWithoutChangeNotification (trace, leftDstExp);
  delete leftDstExp;
  rightDst.setExpressionWithoutChangeNotification (trace, rightDstExp);
  delete rightDstExp;
  // At this point, symbolic quotient and remainder are calculated correctly.
  // but concrete values are not! So we need to register a hook to synchronize concrete
  // values too (we can also calculate them in assembly, but it's not required).

  hook = &InstructionSymbolicExecuter::adjustDivisionMultiplicationOperands;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::idivAnalysisRoutine (
    const MutableExpressionValueProxy &leftDst,
    const MutableExpressionValueProxy &rightDst,
    const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "idivAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting left dst exp...";
  edu::sharif::twinner::trace::Expression *leftDstExp =
      getExpression (leftDst, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting right dst exp...";
  edu::sharif::twinner::trace::Expression *rightDstExp =
      getExpression (rightDst, trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tpreparing left-right in both dst regs...";
  operandSize = leftDst.getSize ();
  leftDstExp->shiftToLeft (operandSize);
  leftDstExp->bitwiseOr (rightDstExp);
  delete rightDstExp;
  rightDstExp = leftDstExp->clone ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tcalculating quotient (right)"
      " and remainder (left) of signed division...";
  leftDstExp->signedRemainder (srcexp);
  rightDstExp->signedDivide (srcexp);
  delete srcexp;
  leftDst.setExpressionWithoutChangeNotification (trace, leftDstExp);
  delete leftDstExp;
  rightDst.setExpressionWithoutChangeNotification (trace, rightDstExp);
  delete rightDstExp;
  // At this point, symbolic quotient and remainder are calculated correctly.
  // but concrete values are not! So we need to register a hook to synchronize concrete
  // values too (we can also calculate them in assembly, but it's not required).

  hook = &InstructionSymbolicExecuter::adjustDivisionMultiplicationOperands;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::adjustDivisionMultiplicationOperands (
    const CONTEXT *context, const ConcreteValue &operandSize) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "adjustDivisionMultiplicationOperands(...) hook...";
  const edu::sharif::twinner::trace::cv::ConcreteValue64Bits os = operandSize;
  const UINT64 osval = os.toUint64 ();
  REG leftReg, rightReg;
  switch (osval) {
  case 8:
    leftReg = REG_AH;
    rightReg = REG_AL;
    break;
  case 16:
    leftReg = REG_DX;
    rightReg = REG_AX;
    break;
  case 32:
    leftReg = REG_EDX;
    rightReg = REG_EAX;
    break;
#ifdef TARGET_IA32E
  case 64:
    leftReg = REG_RDX;
    rightReg = REG_RAX;
    break;
#endif
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "adjustDivisionMultiplicationOperands(...) hook: "
        "unsupported operand size: " << operandSize << '\n';
    abort ();
  }
  ConcreteValue *leftVal =
      edu::sharif::twinner::util::readRegisterContent (context, leftReg);
  ConcreteValue *rightVal =
      edu::sharif::twinner::util::readRegisterContent (context, rightReg);
  edu::sharif::twinner::trace::Expression *leftExp =
      trace->getSymbolicExpressionByRegister (osval, leftReg);
  edu::sharif::twinner::trace::Expression *rightExp =
      trace->getSymbolicExpressionByRegister (osval, rightReg);
  leftExp->setLastConcreteValue (leftVal);
  rightExp->setLastConcreteValue (rightVal);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tconcrete values are adjusted...";
  if (osval == 8) { // AX == AH:AL
    leftExp->shiftToLeft (8);
    leftExp->bitwiseOr (rightExp);
    const MutableExpressionValueProxy &ax = RegisterResidentExpressionValueProxy (REG_AX);
    setExpression (ax, trace, leftExp); // this deletes unused expressions by itself
  } else {
    const MutableExpressionValueProxy &left =
        RegisterResidentExpressionValueProxy (leftReg, *leftVal);
    const MutableExpressionValueProxy &right =
        RegisterResidentExpressionValueProxy (rightReg, *rightVal);
    edu::sharif::twinner::trace::StateSummary state;
    left.valueIsChanged (trace, *leftExp, state);
    if (state.isWrongState ()) {
      edu::sharif::twinner::util::Logger::error () << state.getMessage () << '\n';
      abort ();
    }
    right.valueIsChanged (trace, *rightExp, state);
    if (state.isWrongState ()) {
      edu::sharif::twinner::util::Logger::error () << state.getMessage () << '\n';
      abort ();
    }
  }
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\toverlapping registers are updated.\n";
}

void InstructionSymbolicExecuter::mulAnalysisRoutine (
    const MutableExpressionValueProxy &leftDst,
    const MutableExpressionValueProxy &rightDst,
    const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "mulAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::trace::Expression *leftDstExp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting right dst exp...";
  edu::sharif::twinner::trace::Expression *rightDstExp =
      getExpression (rightDst, trace);
  operandSize = leftDst.getSize ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tmultiplying (left-right = right * src; size=0x"
      << std::hex << operandSize << ")...";
  rightDstExp->multiply (srcexp);
  delete srcexp;
  leftDstExp = rightDstExp->clone ();
  leftDstExp->shiftToRight (operandSize);
  rightDstExp->truncate (operandSize);
  leftDst.setExpressionWithoutChangeNotification (trace, leftDstExp);
  delete leftDstExp;
  rightDst.setExpressionWithoutChangeNotification (trace, rightDstExp);
  delete rightDstExp;
  // At this point, symbolic multiplication result is calculated correctly.
  // but concrete values are not! So we need to register a hook to synchronize concrete
  // values too (we can also calculate them in assembly, but it's not required).

  hook = &InstructionSymbolicExecuter::adjustDivisionMultiplicationOperands;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::imulAnalysisRoutine (
    const MutableExpressionValueProxy &leftDst,
    const MutableExpressionValueProxy &rightDst,
    const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  operandSize = leftDst.getSize ();
  const int doubleSize = operandSize * 2;
  /*
   * Operation: (leftDst-rightDst) = rightDst <signed-multiply> src
   * Signed and unsigned multiplications are equivalent iff result is truncated
   * to size of operands. So we should first sign-extend operands to double-size
   * and then do unsigned multiplication.
   */
  edu::sharif::twinner::util::Logger::loquacious () << "imulAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::trace::Expression *signExtendedExp =
      srcexp->signExtended (doubleSize);
  delete srcexp;
  srcexp = signExtendedExp;
  edu::sharif::twinner::trace::Expression *leftDstExp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting right dst exp...";
  edu::sharif::twinner::trace::Expression *rightDstExp =
      getExpression (rightDst, trace);
  signExtendedExp = rightDstExp->signExtended (doubleSize);
  delete rightDstExp;
  rightDstExp = signExtendedExp;
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsigned multiplying (left-right = right * src; size=0x"
      << std::hex << operandSize << ")...";
  rightDstExp->multiply (srcexp);
  delete srcexp;
  leftDstExp = rightDstExp->clone ();
  leftDstExp->shiftToRight (operandSize);
  rightDstExp->truncate (operandSize);
  leftDst.setExpressionWithoutChangeNotification (trace, leftDstExp);
  delete leftDstExp;
  rightDst.setExpressionWithoutChangeNotification (trace, rightDstExp);
  delete rightDstExp;
  // At this point, symbolic multiplication result is calculated correctly.
  // but concrete values are not! So we need to register a hook to synchronize concrete
  // values too (we can also calculate them in assembly, but it's not required).

  hook = &InstructionSymbolicExecuter::adjustDivisionMultiplicationOperands;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::imulAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "imulAnalysisRoutine(...): "
      "two-operands-mode\n"
      "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp = getExpression (dst, trace);
  const int size = dst.getSize ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tmultiplying (dst = dst * src; size=0x" << std::hex << size << ")...";
  dstexp->multiply (srcexp);
  delete srcexp;
  dstexp->truncate (size);
  setExpression (dst, trace, dstexp);
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::imulAnalysisRoutine (
    const MutableExpressionValueProxy &dst,
    const ExpressionValueProxy &src, const ExpressionValueProxy &imd) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "imulAnalysisRoutine(...): "
      "three-operands-mode\n"
      "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp = getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting imd exp...";
  const edu::sharif::twinner::trace::Expression *imdexp =
      getExpression (imd, trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tmultiplying (dst = src * imd)...";
  srcexp->multiply (imdexp);
  delete imdexp;
  setExpression (dst, trace, srcexp); // sets and truncates expression
  delete srcexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::scasAnalysisRoutine (
    const MutableExpressionValueProxy &dstReg,
    const MutableExpressionValueProxy &srcReg,
    const ExpressionValueProxy &srcMem) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "scasAnalysisRoutine(...)\n";
  cmpAnalysisRoutine (dstReg, srcMem); // comparing AL/AX/EAX/RAX with memory
  edu::sharif::twinner::trace::Expression *exp = getExpression (srcReg, trace);
  if (eflags.getDirectionFlag ()) { // DF == 1
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tdecrementing index register...";
    exp->minus (dstReg.getSize () / 8);
  } else { // DF == 0
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tincrementing index register...";
    exp->add (dstReg.getSize () / 8);
  }
  setExpression (srcReg, trace, exp);
  delete exp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tchecking eflags...";
  bool zero;
  std::list <edu::sharif::twinner::trace::Constraint *> ccList =
      eflags.instantiateConstraintForZeroCase (zero, disassembledInstruction);
  trace->addPathConstraints (ccList);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::stosAnalysisRoutine (
    const MutableExpressionValueProxy &dstMem, const MutableExpressionValueProxy &rdireg,
    const ExpressionValueProxy &srcReg) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "stosAnalysisRoutine(...)\n";
  movAnalysisRoutine (dstMem, srcReg);
  edu::sharif::twinner::trace::Expression *rdiexp =
      getExpression (rdireg, trace);
  const int size = dstMem.getSize () / 8;
  if (eflags.getDirectionFlag ()) { // DF == 1
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tdecrementing index register...";
    rdiexp->minus (size);
  } else { // DF == 0
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tincrementing index register...";
    rdiexp->add (size);
  }
  setExpression (rdireg, trace, rdiexp);
  delete rdiexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::leaveAnalysisRoutine (
    const MutableExpressionValueProxy &fpReg,
    const MutableExpressionValueProxy &spReg,
    const ExpressionValueProxy &srcMem) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "leaveAnalysisRoutine(...)\n"
      << "\tgetting frame pointer (to be set in stack pointer)...";
  edu::sharif::twinner::trace::Expression *rsp = getExpression (fpReg, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tpopping frame pointer...";
  const edu::sharif::twinner::trace::Expression *memexp =
      getExpression (srcMem, trace);
  setExpression (fpReg, trace, memexp);
  delete memexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tadjusting rsp...";
  rsp->add (8);
  setExpression (spReg, trace, rsp);
  delete rsp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::rdtscAnalysisRoutine (const CONTEXT *context) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "rdtscAnalysisRoutine(...)\n";
  /**
   * Now, we are right after the RDTSC instruction and time-stamp is loaded in
   * the edx:eax registers. These registers should be loaded as immediate values
   * in symbolic expressions.
   */
  // FIXME: This code doesn't preserve time-stamp and is vulnerable to time bombs
  ConcreteValue *edxVal =
      edu::sharif::twinner::util::readRegisterContent (context, REG_EDX);
  ConcreteValue *eaxVal =
      edu::sharif::twinner::util::readRegisterContent (context, REG_EAX);
  edu::sharif::twinner::trace::Expression *edxNewExp =
      new edu::sharif::twinner::trace::ExpressionImp (edxVal);
  edu::sharif::twinner::trace::Expression *eaxNewExp =
      new edu::sharif::twinner::trace::ExpressionImp (eaxVal);

  const MutableExpressionValueProxy &edx =
      RegisterResidentExpressionValueProxy (REG_EDX, *edxVal);
  const MutableExpressionValueProxy &eax =
      RegisterResidentExpressionValueProxy (REG_EAX, *eaxVal);
  setExpression (edx, trace, edxNewExp);
  setExpression (eax, trace, eaxNewExp);
  delete edxNewExp; // expressions are cloned by above setter methods
  delete eaxNewExp;
}

void InstructionSymbolicExecuter::cldAnalysisRoutine (const CONTEXT *context) {
  edu::sharif::twinner::util::Logger::loquacious () << "cldAnalysisRoutine(...)\n";
  /**
   * Now, we are right after the CLD instruction. This is a decision to match with other
   * operand-less instructions. Anyway, our implementation for CLD works independent of
   * being executed before or after the CLD instruction itself.
   */
  eflags.setCarryFlag (false);
}

void InstructionSymbolicExecuter::cpuidAnalysisRoutine (const CONTEXT *context) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "cpuidAnalysisRoutine(...)\n";
  /*
   * CPUID instruction will read EAX and ECX and based on their values, sets 4 registers
   * deterministically. Now, we are after the CPUID and can set those registers.
   */
  // FIXME: Two constraints must be created to state previous values of EAX and ECX
  ConcreteValue *eaxVal =
      edu::sharif::twinner::util::readRegisterContent (context, REG_EAX);
  ConcreteValue *ebxVal =
      edu::sharif::twinner::util::readRegisterContent (context, REG_EBX);
  ConcreteValue *ecxVal =
      edu::sharif::twinner::util::readRegisterContent (context, REG_ECX);
  ConcreteValue *edxVal =
      edu::sharif::twinner::util::readRegisterContent (context, REG_EDX);
  edu::sharif::twinner::trace::Expression *eaxNewExp =
      new edu::sharif::twinner::trace::ExpressionImp (eaxVal);
  edu::sharif::twinner::trace::Expression *ebxNewExp =
      new edu::sharif::twinner::trace::ExpressionImp (ebxVal);
  edu::sharif::twinner::trace::Expression *ecxNewExp =
      new edu::sharif::twinner::trace::ExpressionImp (ecxVal);
  edu::sharif::twinner::trace::Expression *edxNewExp =
      new edu::sharif::twinner::trace::ExpressionImp (edxVal);

  const MutableExpressionValueProxy &eax =
      RegisterResidentExpressionValueProxy (REG_EAX, *eaxVal);
  const MutableExpressionValueProxy &ebx =
      RegisterResidentExpressionValueProxy (REG_EBX, *ebxVal);
  const MutableExpressionValueProxy &ecx =
      RegisterResidentExpressionValueProxy (REG_ECX, *ecxVal);
  const MutableExpressionValueProxy &edx =
      RegisterResidentExpressionValueProxy (REG_EDX, *edxVal);
  setExpression (eax, trace, eaxNewExp);
  setExpression (ebx, trace, ebxNewExp);
  setExpression (ecx, trace, ecxNewExp);
  setExpression (edx, trace, edxNewExp);
  delete eaxNewExp;
  delete ebxNewExp;
  delete ecxNewExp;
  delete edxNewExp;
}

void InstructionSymbolicExecuter::incAnalysisRoutine (
    const MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "incAnalysisRoutine(...)\n"
      << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexpOrig =
      getExpression (opr, trace);
  edu::sharif::twinner::trace::Expression *dstexp = dstexpOrig->clone ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tincrementing...";
  dstexp->add (1);
  setExpression (opr, trace, dstexp);
  delete dstexp;
  const edu::sharif::twinner::trace::Expression *one =
      new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  eflags.setFlags
      (new edu::sharif::twinner::twintool::operationgroup::AdditionOperationGroup
       (dstexpOrig, one));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::decAnalysisRoutine (
    const MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "decAnalysisRoutine(...)\n"
      << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexpOrig =
      getExpression (opr, trace);
  edu::sharif::twinner::trace::Expression *dstexp = dstexpOrig->clone ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tdecrementing...";
  dstexp->minus (1);
  setExpression (opr, trace, dstexp);
  delete dstexp;
  const edu::sharif::twinner::trace::Expression *one =
      new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  eflags.setFlags
      (new edu::sharif::twinner::twintool::operationgroup::SubtractOperationGroup
       (dstexpOrig, one));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::negAnalysisRoutine (
    const MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "negAnalysisRoutine(...)\n"
      << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexpOrig =
      getExpression (opr, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\ttwo's complementing...";
  edu::sharif::twinner::trace::Expression *dstexp = dstexpOrig->twosComplement ();
  setExpression (opr, trace, dstexp);
  delete dstexp;
  delete dstexpOrig;
  //TODO: set CF == (dstexpOrig is not zero) and other flags based on dstexp result
  eflags.setFlags
      (new edu::sharif::twinner::twintool::operationgroup::DummyOperationGroup
       ("NegationOperationGroup"));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setnzAnalysisRoutine (
    const MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "setnzAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool zero;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForZeroCase (zero, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp;
  if (!zero) {
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  } else { // shouldSetToZero
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
  }
  setExpression (opr, trace, dstexp);
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setzAnalysisRoutine (
    const MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "setzAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool zero;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForZeroCase (zero, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp;
  if (zero) {
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  } else { // shouldSetToZero
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
  }
  setExpression (opr, trace, dstexp);
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setleAnalysisRoutine (
    const MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "setleAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool lessOrEqual;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForLessOrEqualCase
      (lessOrEqual, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp;
  if (lessOrEqual) {
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  } else { // shouldSetToZero
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
  }
  setExpression (opr, trace, dstexp);
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setnleAnalysisRoutine (
    const MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "setnleAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool lessOrEqual;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForLessOrEqualCase
      (lessOrEqual, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp;
  if (!lessOrEqual) {
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  } else { // shouldSetToZero
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
  }
  setExpression (opr, trace, dstexp);
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setlAnalysisRoutine (
    const MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "setlAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool less;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForLessCase (less, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp;
  if (less) {
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  } else { // shouldSetToZero
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
  }
  setExpression (opr, trace, dstexp);
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setbeAnalysisRoutine (
    const MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "setbeAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool belowOrEqual;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForBelowOrEqualCase
      (belowOrEqual, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp;
  if (belowOrEqual) {
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  } else { // shouldSetToZero
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
  }
  setExpression (opr, trace, dstexp);
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setnbeAnalysisRoutine (
    const MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "setnbeAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool belowOrEqual;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForBelowOrEqualCase
      (belowOrEqual, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp;
  if (!belowOrEqual) {
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  } else { // shouldSetToZero
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
  }
  setExpression (opr, trace, dstexp);
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setbAnalysisRoutine (
    const MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "setbAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool below;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForBelowCase (below, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp;
  if (below) {
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  } else { // shouldSetToZero
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
  }
  setExpression (opr, trace, dstexp);
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setnlAnalysisRoutine (
    const MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "setnlAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool less;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForLessCase (less, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp;
  if (less) { // shouldSetToZero
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
  } else { // NL; set to one
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  }
  setExpression (opr, trace, dstexp);
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setnbAnalysisRoutine (
    const MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "setnbAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool below;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForBelowCase
      (below, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp;
  if (below) { // shouldSetToZero
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
  } else { // NB; set to one
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  }
  setExpression (opr, trace, dstexp);
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::notAnalysisRoutine (
    const MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "notAnalysisRoutine(...)\n"
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp = getExpression (opr, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tnegating...";
  dstexp->bitwiseNegate ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  setExpression (opr, trace, dstexp);
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::memoryRegisterCorrespondenceAnalysisRoutine (
    const ExpressionValueProxy &baseReg, ADDRDELTA displacement,
    ADDRINT memoryEa) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "memoryRegisterCorrespondenceAnalysisRoutine(...)\n"
      << "\tgetting base reg exp...";
  edu::sharif::twinner::trace::Expression *baseexp =
      getExpression (baseReg, trace);
  const int size = edu::sharif::twinner::util::max
      (baseexp->getLastConcreteValue ().getSize (), 64);
  if (displacement > 0) {
    baseexp->add (displacement);
  } else {
    baseexp->minus (-displacement);
  }
  baseexp->minus (memoryEa);
  baseexp->truncate (size);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  std::list <edu::sharif::twinner::trace::Constraint *> cc;
  edu::sharif::twinner::trace::Constraint *regMemConstraint =
      new edu::sharif::twinner::trace::Constraint
      (baseexp, edu::sharif::twinner::trace::Constraint::ZERO,
       disassembledInstruction, false);
  cc.push_back (regMemConstraint);
  delete baseexp;
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::memoryIndexedRegisterCorrespondenceAnalysisRoutine (
    const ExpressionValueProxy &baseReg, ADDRDELTA displacement,
    const ExpressionValueProxy &indexReg, UINT32 scale,
    ADDRINT memoryEa) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "memoryIndexedRegisterCorrespondenceAnalysisRoutine(...)\n"
      << "\tgetting base reg exp...";
  edu::sharif::twinner::trace::Expression *baseexp =
      getExpression (baseReg, trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting index reg exp...";
  edu::sharif::twinner::trace::Expression *indexexp =
      getExpression (indexReg, trace);
  const int size = edu::sharif::twinner::util::max
      (baseexp->getLastConcreteValue ().getSize (),
       64, indexexp->getLastConcreteValue ().getSize ());
  if (displacement > 0) {
    baseexp->add (displacement);
  } else {
    baseexp->minus (-displacement);
  }
  indexexp->multiply (scale);
  baseexp->add (indexexp);
  delete indexexp;
  baseexp->minus (memoryEa);
  baseexp->truncate (size);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  std::list <edu::sharif::twinner::trace::Constraint *> cc;
  edu::sharif::twinner::trace::Constraint *regMemConstraint =
      new edu::sharif::twinner::trace::Constraint
      (baseexp, edu::sharif::twinner::trace::Constraint::ZERO,
       disassembledInstruction, false);
  cc.push_back (regMemConstraint);
  delete baseexp;
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

InstructionSymbolicExecuter::AnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToAnalysisRoutine (OPCODE op) const {
  switch (op) {
  case XED_ICLASS_MOV:
  case XED_ICLASS_MOVZX:
  case XED_ICLASS_MOVD:
  case XED_ICLASS_MOVAPS:
  case XED_ICLASS_MOVDQU:
  case XED_ICLASS_MOVDQA:
  case XED_ICLASS_MOVSD_XMM:
    return &InstructionSymbolicExecuter::movAnalysisRoutine;
  case XED_ICLASS_IMUL:
    return &InstructionSymbolicExecuter::imulAnalysisRoutine;
  case XED_ICLASS_CDQE:
  case XED_ICLASS_MOVSX:
  case XED_ICLASS_MOVSXD:
    return &InstructionSymbolicExecuter::movsxAnalysisRoutine;
  case XED_ICLASS_CMOVBE:
    return &InstructionSymbolicExecuter::cmovbeAnalysisRoutine;
  case XED_ICLASS_CMOVNBE:
    return &InstructionSymbolicExecuter::cmovnbeAnalysisRoutine;
  case XED_ICLASS_ADD:
    return &InstructionSymbolicExecuter::addAnalysisRoutine;
  case XED_ICLASS_ADC:
    return &InstructionSymbolicExecuter::adcAnalysisRoutine;
  case XED_ICLASS_SUB:
    return &InstructionSymbolicExecuter::subAnalysisRoutine;
  case XED_ICLASS_SBB:
    return &InstructionSymbolicExecuter::sbbAnalysisRoutine;
  case XED_ICLASS_CMP:
    return &InstructionSymbolicExecuter::cmpAnalysisRoutine;
  case XED_ICLASS_LEA:
    return &InstructionSymbolicExecuter::leaAnalysisRoutine;
  case XED_ICLASS_SHL:
    return &InstructionSymbolicExecuter::shlAnalysisRoutine;
  case XED_ICLASS_SHR:
    return &InstructionSymbolicExecuter::shrAnalysisRoutine;
  case XED_ICLASS_SAR:
    return &InstructionSymbolicExecuter::sarAnalysisRoutine;
  case XED_ICLASS_ROR:
    return &InstructionSymbolicExecuter::rorAnalysisRoutine;
  case XED_ICLASS_ROL:
    return &InstructionSymbolicExecuter::rolAnalysisRoutine;
  case XED_ICLASS_AND:
    return &InstructionSymbolicExecuter::andAnalysisRoutine;
  case XED_ICLASS_OR:
  case XED_ICLASS_POR:
    return &InstructionSymbolicExecuter::orAnalysisRoutine;
  case XED_ICLASS_PXOR:
  case XED_ICLASS_XOR:
    return &InstructionSymbolicExecuter::xorAnalysisRoutine;
  case XED_ICLASS_TEST:
    return &InstructionSymbolicExecuter::testAnalysisRoutine;
  case XED_ICLASS_BT:
    return &InstructionSymbolicExecuter::btAnalysisRoutine;
  case XED_ICLASS_PMOVMSKB:
    return &InstructionSymbolicExecuter::pmovmskbAnalysisRoutine;
  case XED_ICLASS_PCMPEQB:
    return &InstructionSymbolicExecuter::pcmpeqbAnalysisRoutine;
  case XED_ICLASS_PMINUB:
    return &InstructionSymbolicExecuter::pminubAnalysisRoutine;
  case XED_ICLASS_PUNPCKLBW:
    return &InstructionSymbolicExecuter::punpcklbwAnalysisRoutine;
  case XED_ICLASS_PUNPCKLWD:
    return &InstructionSymbolicExecuter::punpcklwdAnalysisRoutine;
  case XED_ICLASS_BSF:
    return &InstructionSymbolicExecuter::bsfAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: Unknown opcode: "
        << OPCODE_StringShort (op) << '\n';
    abort ();
  }
}

InstructionSymbolicExecuter::MutableSourceAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToMutableSourceAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_XCHG:
    return &InstructionSymbolicExecuter::xchgAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Having Mutable Source operand: Unknown opcode: "
        << OPCODE_StringShort (op) << '\n';
    abort ();
  }
}

InstructionSymbolicExecuter::AuxOperandHavingAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToAuxOperandHavingAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_CMPXCHG:
    return &InstructionSymbolicExecuter::cmpxchgAnalysisRoutine;
  case XED_ICLASS_PUSH:
    return &InstructionSymbolicExecuter::pushAnalysisRoutine;
  case XED_ICLASS_POP:
    return &InstructionSymbolicExecuter::popAnalysisRoutine;
  case XED_ICLASS_LODSD:
    return &InstructionSymbolicExecuter::lodsdAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Having an auxiliary third (dst) operand: Unknown opcode: "
        << OPCODE_StringShort (op) << '\n';
    abort ();
  }
}

InstructionSymbolicExecuter::DoubleDestinationsAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToDoubleDestinationsAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_DIV:
    return &InstructionSymbolicExecuter::divAnalysisRoutine;
  case XED_ICLASS_IDIV:
    return &InstructionSymbolicExecuter::idivAnalysisRoutine;
  case XED_ICLASS_MUL:
    return &InstructionSymbolicExecuter::mulAnalysisRoutine;
  case XED_ICLASS_IMUL:
    return &InstructionSymbolicExecuter::imulAnalysisRoutine;
  case XED_ICLASS_SCASB:
    return &InstructionSymbolicExecuter::scasAnalysisRoutine;
  case XED_ICLASS_STOSB:
  case XED_ICLASS_STOSW:
  case XED_ICLASS_STOSD:
  case XED_ICLASS_STOSQ:
    return &InstructionSymbolicExecuter::stosAnalysisRoutine;
  case XED_ICLASS_LEAVE:
    return &InstructionSymbolicExecuter::leaveAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Double Destinations: Unknown opcode: " << OPCODE_StringShort (op) << '\n';
    abort ();
  }
}

InstructionSymbolicExecuter::DoubleSourcesAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToDoubleSourcesAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_IMUL:
    return &InstructionSymbolicExecuter::imulAnalysisRoutine;
  case XED_ICLASS_PSHUFD:
    return &InstructionSymbolicExecuter::pshufdAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Double Sources: Unknown opcode: " << OPCODE_StringShort (op) << '\n';
    abort ();
  }
}

InstructionSymbolicExecuter::OneToThreeOperandsAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToOneToThreeOperandsAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_MOVSQ:
    return &InstructionSymbolicExecuter::movsAnalysisRoutine;
  case XED_ICLASS_CMPSB:
  case XED_ICLASS_CMPSW:
  case XED_ICLASS_CMPSD:
  case XED_ICLASS_CMPSQ:
    return &InstructionSymbolicExecuter::cmpsAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "1-to-3 operands: Unknown opcode: "
        << OPCODE_StringShort (op) << '\n';
    abort ();
  }
}

InstructionSymbolicExecuter::ConditionalBranchAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToConditionalBranchAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_JNZ:
    return &InstructionSymbolicExecuter::jnzAnalysisRoutine;
  case XED_ICLASS_JZ:
    return &InstructionSymbolicExecuter::jzAnalysisRoutine;
  case XED_ICLASS_JLE:
    return &InstructionSymbolicExecuter::jleAnalysisRoutine;
  case XED_ICLASS_JNLE:
    return &InstructionSymbolicExecuter::jnleAnalysisRoutine;
  case XED_ICLASS_JL:
    return &InstructionSymbolicExecuter::jlAnalysisRoutine;
  case XED_ICLASS_JNL:
    return &InstructionSymbolicExecuter::jnlAnalysisRoutine;
  case XED_ICLASS_JBE:
    return &InstructionSymbolicExecuter::jbeAnalysisRoutine;
  case XED_ICLASS_JNBE:
    return &InstructionSymbolicExecuter::jnbeAnalysisRoutine;
  case XED_ICLASS_JNB:
    return &InstructionSymbolicExecuter::jnbAnalysisRoutine;
  case XED_ICLASS_JB:
    return &InstructionSymbolicExecuter::jbAnalysisRoutine;
  case XED_ICLASS_JS:
    return &InstructionSymbolicExecuter::jsAnalysisRoutine;
  case XED_ICLASS_JNS:
    return &InstructionSymbolicExecuter::jnsAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Conditional Branch: Unknown opcode: " << OPCODE_StringShort (op) << '\n';
    abort ();
  }
}

InstructionSymbolicExecuter::SuddenlyChangedRegAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToSuddenlyChangedRegAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_CALL_FAR:
  case XED_ICLASS_CALL_NEAR:
    return &InstructionSymbolicExecuter::callAnalysisRoutine;
  case XED_ICLASS_RET_FAR:
  case XED_ICLASS_RET_NEAR:
    return &InstructionSymbolicExecuter::retAnalysisRoutine;
  case XED_ICLASS_JMP_FAR:
  case XED_ICLASS_JMP:
    return &InstructionSymbolicExecuter::jmpAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Suddenly Changed Register: Unknown opcode: " << OPCODE_StringShort (op) << '\n';
    abort ();
  }
}

InstructionSymbolicExecuter::OperandLessAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToOperandLessAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_RDTSC:
    return &InstructionSymbolicExecuter::rdtscAnalysisRoutine;
  case XED_ICLASS_CLD:
    return &InstructionSymbolicExecuter::cldAnalysisRoutine;
  case XED_ICLASS_CPUID:
    return &InstructionSymbolicExecuter::cpuidAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Operand Less: Unknown opcode: " << OPCODE_StringShort (op) << '\n';
    abort ();
  }
}

InstructionSymbolicExecuter::SingleOperandAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToSingleOperandAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_INC:
    return &InstructionSymbolicExecuter::incAnalysisRoutine;
  case XED_ICLASS_DEC:
    return &InstructionSymbolicExecuter::decAnalysisRoutine;
  case XED_ICLASS_NEG:
    return &InstructionSymbolicExecuter::negAnalysisRoutine;
  case XED_ICLASS_SETNZ:
    return &InstructionSymbolicExecuter::setnzAnalysisRoutine;
  case XED_ICLASS_SETZ:
    return &InstructionSymbolicExecuter::setzAnalysisRoutine;
  case XED_ICLASS_SETLE:
    return &InstructionSymbolicExecuter::setleAnalysisRoutine;
  case XED_ICLASS_SETNLE:
    return &InstructionSymbolicExecuter::setnleAnalysisRoutine;
  case XED_ICLASS_SETL:
    return &InstructionSymbolicExecuter::setlAnalysisRoutine;
  case XED_ICLASS_SETNL:
    return &InstructionSymbolicExecuter::setnlAnalysisRoutine;
  case XED_ICLASS_SETB:
    return &InstructionSymbolicExecuter::setbAnalysisRoutine;
  case XED_ICLASS_SETBE:
    return &InstructionSymbolicExecuter::setbeAnalysisRoutine;
  case XED_ICLASS_SETNBE:
    return &InstructionSymbolicExecuter::setnbeAnalysisRoutine;
  case XED_ICLASS_SETNB:
    return &InstructionSymbolicExecuter::setnbAnalysisRoutine;
  case XED_ICLASS_NOT:
    return &InstructionSymbolicExecuter::notAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Single Operand: Unknown opcode: " << OPCODE_StringShort (op) << '\n';
    abort ();
  }
}

VOID analysisRoutineDstRegSrcReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcReg
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (dstRegVal),
       (REG) srcReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (srcRegVal),
       insAssembly);
}

VOID analysisRoutineDstRegSrcMutableReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcMutableReg
      (ise->convertOpcodeToMutableSourceAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (dstRegVal),
       (REG) srcReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (srcRegVal),
       insAssembly);
}

VOID analysisRoutineDstRegSrcRegAuxReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 auxReg, ADDRINT auxRegVal,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcRegAuxReg
      (ise->convertOpcodeToAuxOperandHavingAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (dstRegVal),
       (REG) srcReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (srcRegVal),
       (REG) auxReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (auxRegVal),
       insAssembly);
}

VOID analysisRoutineDstRegSrcLargeReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, const PIN_REGISTER *srcRegVal,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcReg
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (dstRegVal),
       (REG) srcReg, edu::sharif::twinner::trace::cv::ConcreteValue128Bits (*srcRegVal),
       insAssembly);
}

VOID analysisRoutineDstLargeRegSrcReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, const PIN_REGISTER *dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcReg
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::cv::ConcreteValue128Bits (*dstRegVal),
       (REG) srcReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (srcRegVal),
       insAssembly);
}

VOID analysisRoutineDstLargeRegSrcLargeReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, const PIN_REGISTER *dstRegVal,
    UINT32 srcReg, const PIN_REGISTER *srcRegVal,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcReg
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::cv::ConcreteValue128Bits (*dstRegVal),
       (REG) srcReg, edu::sharif::twinner::trace::cv::ConcreteValue128Bits (*srcRegVal),
       insAssembly);
}

VOID analysisRoutineDstLargeRegSrcLargeRegAuxImd (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, const PIN_REGISTER *dstRegVal,
    UINT32 srcReg, const PIN_REGISTER *srcRegVal,
    ADDRINT auxImmediateValue,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcRegAuxImd
      (ise->convertOpcodeToDoubleSourcesAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::cv::ConcreteValue128Bits (*dstRegVal),
       (REG) srcReg, edu::sharif::twinner::trace::cv::ConcreteValue128Bits (*srcRegVal),
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (auxImmediateValue),
       insAssembly);
}

VOID analysisRoutineDstRegSrcRegAuxImd (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    ADDRINT auxImmediateValue,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcRegAuxImd
      (ise->convertOpcodeToDoubleSourcesAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (dstRegVal),
       (REG) srcReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (srcRegVal),
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (auxImmediateValue),
       insAssembly);
}

VOID analysisRoutineDstLargeRegSrcMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, const PIN_REGISTER *dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcMem
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::cv::ConcreteValue128Bits (*dstRegVal),
       srcMemoryEa, memReadBytes,
       insAssembly);
}

VOID analysisRoutineDstLargeRegSrcMemAuxImd (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, const PIN_REGISTER *dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    ADDRINT auxImmediateValue,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcMemAuxImd
      (ise->convertOpcodeToDoubleSourcesAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::cv::ConcreteValue128Bits (*dstRegVal),
       srcMemoryEa, memReadBytes,
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (auxImmediateValue),
       insAssembly);
}

VOID analysisRoutineDstRegSrcMemAuxImd (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    ADDRINT auxImmediateValue,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcMemAuxImd
      (ise->convertOpcodeToDoubleSourcesAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (dstRegVal),
       srcMemoryEa, memReadBytes,
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (auxImmediateValue),
       insAssembly);
}

VOID analysisRoutineDstRegSrcMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcMem
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (dstRegVal),
       srcMemoryEa, memReadBytes,
       insAssembly);
}

VOID analysisRoutineDstRegSrcMemAuxReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcMemoryEa,
    UINT32 auxReg, ADDRINT auxRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcMemAuxReg
      (ise->convertOpcodeToAuxOperandHavingAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (dstRegVal),
       srcMemoryEa,
       (REG) auxReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (auxRegVal),
       memReadBytes,
       insAssembly);
}

VOID analysisRoutineDstRegSrcImd (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcImmediateValue,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcImd
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (dstRegVal),
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (srcImmediateValue),
       insAssembly);
}

VOID analysisRoutineDstMemSrcReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstMemSrcReg
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       dstMemoryEa,
       (REG) srcReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (srcRegVal),
       memReadBytes,
       insAssembly);
}

VOID analysisRoutineDstMemSrcMutableReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstMemSrcMutableReg
      (ise->convertOpcodeToMutableSourceAnalysisRoutine ((OPCODE) opcode),
       dstMemoryEa,
       (REG) srcReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (srcRegVal),
       memReadBytes,
       insAssembly);
}

VOID analysisRoutineDstMemSrcRegAuxReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 auxReg, ADDRINT auxRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstMemSrcRegAuxReg
      (ise->convertOpcodeToAuxOperandHavingAnalysisRoutine ((OPCODE) opcode),
       dstMemoryEa,
       (REG) srcReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (srcRegVal),
       (REG) auxReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (auxRegVal),
       memReadBytes,
       insAssembly);
}

VOID analysisRoutineDstMemSrcLargeReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    UINT32 srcReg, const PIN_REGISTER *srcRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstMemSrcReg
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       dstMemoryEa,
       (REG) srcReg, edu::sharif::twinner::trace::cv::ConcreteValue128Bits (*srcRegVal),
       memReadBytes,
       insAssembly);
}

VOID analysisRoutineDstMemSrcImd (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcImmediateValue,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstMemSrcImd
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       dstMemoryEa,
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (srcImmediateValue),
       memReadBytes,
       insAssembly);
}

VOID analysisRoutineDstMemSrcImdAuxReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcImmediateValue,
    UINT32 auxReg, ADDRINT auxRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstMemSrcImdAuxReg
      (ise->convertOpcodeToAuxOperandHavingAnalysisRoutine ((OPCODE) opcode),
       dstMemoryEa,
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (srcImmediateValue),
       (REG) auxReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (auxRegVal),
       memReadBytes,
       insAssembly);
}

VOID analysisRoutineDstMemSrcMem (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstMemSrcMem
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       dstMemoryEa,
       srcMemoryEa, memReadBytes,
       insAssembly);
}

VOID analysisRoutineDstMemSrcMemAuxReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa,
    UINT32 auxReg, ADDRINT auxRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstMemSrcMemAuxReg
      (ise->convertOpcodeToAuxOperandHavingAnalysisRoutine ((OPCODE) opcode),
       dstMemoryEa,
       srcMemoryEa,
       (REG) auxReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (auxRegVal),
       memReadBytes,
       insAssembly);
}

VOID analysisRoutineConditionalBranch (VOID *iseptr, UINT32 opcode,
    BOOL branchTaken,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineConditionalBranch
      (ise->convertOpcodeToConditionalBranchAnalysisRoutine ((OPCODE) opcode),
       branchTaken,
       insAssembly);
}

VOID analysisRoutineDstRegSrcAdg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcAdg
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (dstRegVal),
       insAssembly);
}

VOID analysisRoutineBeforeChangeOfReg (VOID *iseptr, UINT32 opcode,
    UINT32 reg,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineBeforeChangeOfReg
      (ise->convertOpcodeToSuddenlyChangedRegAnalysisRoutine ((OPCODE) opcode),
       (REG) reg,
       insAssembly);
}

VOID analysisRoutineTwoDstRegOneSrcReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstLeftReg, ADDRINT dstLeftRegVal,
    UINT32 dstRightReg, ADDRINT dstRightRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineTwoDstRegOneSrcReg
      (ise->convertOpcodeToDoubleDestinationsAnalysisRoutine ((OPCODE) opcode),
       (REG) dstLeftReg,
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (dstLeftRegVal),
       (REG) dstRightReg,
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (dstRightRegVal),
       (REG) srcReg,
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (srcRegVal),
       insAssembly);
}

VOID analysisRoutineTwoDstRegOneSrcMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstLeftReg, ADDRINT dstLeftRegVal,
    UINT32 dstRightReg, ADDRINT dstRightRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineTwoDstRegOneSrcMem
      (ise->convertOpcodeToDoubleDestinationsAnalysisRoutine ((OPCODE) opcode),
       (REG) dstLeftReg,
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (dstLeftRegVal),
       (REG) dstRightReg,
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (dstRightRegVal),
       srcMemoryEa, memReadBytes,
       insAssembly);
}

VOID analysisRoutineAfterOperandLess (VOID *iseptr, UINT32 opcode,
    const CONTEXT *context,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineAfterOperandLessInstruction
      (ise->convertOpcodeToOperandLessAnalysisRoutine ((OPCODE) opcode),
       context,
       insAssembly);
}

VOID analysisRoutineRunHooks (VOID *iseptr, const CONTEXT *context) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineRunHooks (context);
}

VOID analysisRoutineDstRegSrcImplicit (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcImplicit
      (ise->convertOpcodeToSingleOperandAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (dstRegVal),
       insAssembly);
}

VOID analysisRoutineDstMemSrcImplicit (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstMemSrcImplicit
      (ise->convertOpcodeToSingleOperandAnalysisRoutine ((OPCODE) opcode),
       dstMemoryEa, memReadBytes,
       insAssembly);
}

VOID analysisRoutineInitializeRegisters (VOID *iseptr, CONTEXT *context) {
  static bool executed = false;
  if (executed) {
    return;
  }
  executed = true;
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineInitializeRegisters (context);
}

VOID analysisRoutineStrOpRegMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineTwoRegOneMem
      (ise->convertOpcodeToDoubleDestinationsAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg,
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (dstRegVal),
       (REG) srcReg,
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (srcRegVal),
       srcMemoryEa, memReadBytes,
       insAssembly);
}

VOID analysisRoutineStrOpMemReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineOneMemTwoReg
      (ise->convertOpcodeToDoubleDestinationsAnalysisRoutine ((OPCODE) opcode),
       dstMemoryEa,
       (REG) dstReg,
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (dstRegVal),
       (REG) srcReg,
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (srcRegVal),
       memReadBytes, insAssembly);
}

VOID analysisRoutineStrOpMemMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT dstMemoryEa,
    UINT32 srcReg, ADDRINT srcRegVal,
    ADDRINT srcMemoryEa,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineTwoRegTwoMem
      (ise->convertOpcodeToOneToThreeOperandsAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg,
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (dstRegVal),
       (REG) srcReg,
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (srcRegVal),
       dstMemoryEa, srcMemoryEa, memReadBytes,
       insAssembly);
}

VOID analysisRoutineRepPrefix (VOID *iseptr, UINT32 opcode,
    UINT32 repReg, ADDRINT repRegVal,
    UINT32 executing, UINT32 repEqual,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineRepEqualOrRepNotEqualPrefix
      ((REG) repReg,
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (repRegVal),
       executing, repEqual,
       insAssembly);
}

VOID analysisRoutineMemoryRegisterCorrespondence (VOID *iseptr,
    UINT32 baseReg, ADDRINT baseRegVal,
    ADDRINT displacement,
    ADDRINT memoryEa,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineMemoryRegisterCorrespondence
      ((REG) baseReg,
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (baseRegVal),
       ADDRDELTA (displacement),
       memoryEa, insAssembly);
}

VOID analysisRoutineMemoryIndexedRegisterCorrespondence (VOID *iseptr,
    UINT32 baseReg, ADDRINT baseRegVal,
    ADDRINT displacement,
    UINT32 indexReg, ADDRINT indexRegVal,
    ADDRINT scale,
    ADDRINT memoryEa,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineMemoryIndexedRegisterCorrespondence
      ((REG) baseReg,
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (baseRegVal),
       ADDRDELTA (displacement),
       (REG) indexReg,
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (indexRegVal),
       UINT32 (scale),
       memoryEa, insAssembly);
}

VOID analysisRoutinePrefetchMem (VOID *iseptr,
    ADDRINT memoryEa, UINT32 memReadBytes) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutinePrefetchMem (memoryEa, memReadBytes);
}

}
}
}
}
