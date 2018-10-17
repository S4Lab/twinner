//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2018 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "InstructionSymbolicExecuter.h"


#include "Instrumenter.h"

#include "edu/sharif/twinner/proxy/RegisterResidentExpressionValueProxy.h"
#include "edu/sharif/twinner/proxy/MemoryResidentExpressionValueProxy.h"
#include "edu/sharif/twinner/proxy/ConstantExpressionValueProxy.h"

#include "edu/sharif/twinner/operationgroup/DummyOperationGroup.h"
#include "edu/sharif/twinner/operationgroup/SubtractOperationGroup.h"
#include "edu/sharif/twinner/operationgroup/SubtractWithBorrowOperationGroup.h"
#include "edu/sharif/twinner/operationgroup/AdditionOperationGroup.h"
#include "edu/sharif/twinner/operationgroup/AddWithCarryOperationGroup.h"
#include "edu/sharif/twinner/operationgroup/BitwiseAndOperationGroup.h"
#include "edu/sharif/twinner/operationgroup/ShiftArithmeticRightOperationGroup.h"
#include "edu/sharif/twinner/operationgroup/ShiftRightOperationGroup.h"
#include "edu/sharif/twinner/operationgroup/ShiftLeftOperationGroup.h"

#include "edu/sharif/twinner/trace/ExpressionImp.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/StateSummary.h"
#include "edu/sharif/twinner/trace/SyscallInvocation.h"
#include "edu/sharif/twinner/trace/FunctionInvocation.h"

#include "edu/sharif/twinner/trace-twintool/FunctionArgumentInfoImp.h"

#include "edu/sharif/twinner/trace/syscall/Syscall.h"
#ifdef TARGET_IS_32BITS_WINDOWS7_SP1
#include "edu/sharif/twinner/trace/syscall/X86Windows7Sp1Syscall.h"
#endif

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue128Bits.h"

#include "edu/sharif/twinner/trace-twintool/TraceImp.h"
#include "edu/sharif/twinner/trace-twintool/FunctionInfo.h"

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
    std::ifstream &symbolsFileInputStream, bool _disabled,
    const string &_searchPattern,
    bool _measureMode) :
    im (_im),
    bufferForTraceLazyLoad (std::string
    (std::istreambuf_iterator<char> (symbolsFileInputStream),
    std::istreambuf_iterator<char> ())), lazyTrace (0),
    memoryManager (edu::sharif::twinner::util::MemoryManager::getInstance ()),
    trackedReg (REG_INVALID_), operandSize (-1), hook (0),
    disabled (_disabled),
    searchPattern (_searchPattern),
    measureMode (_measureMode), numberOfExecutedInstructions (0),
    endOfSafeFuncRetAddress (0), withinSafeFunc (false) {
}

InstructionSymbolicExecuter::InstructionSymbolicExecuter (Instrumenter *_im,
    bool _disabled, const string &_searchPattern) :
    im (_im),
    lazyTrace (new edu::sharif::twinner::trace::TraceImp ()),
    memoryManager (lazyTrace->getMemoryManager ()),
    trackedReg (REG_INVALID_), operandSize (-1), hook (0),
    disabled (_disabled),
    searchPattern (_searchPattern),
    measureMode (false), numberOfExecutedInstructions (0),
    endOfSafeFuncRetAddress (0), withinSafeFunc (false) {
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

void InstructionSymbolicExecuter::setHook (Hook _hook, REG reg) {
  hook = _hook;
  trackedReg = reg;
}

void InstructionSymbolicExecuter::setHook (HookWithArg _hook, REG reg, ADDRINT _arg) {
  hookWithArg = _hook;
  trackedReg = reg;
  arg = _arg;
}

void InstructionSymbolicExecuter::disable () {
  disabled = true;
}

void InstructionSymbolicExecuter::enable () {
  disabled = false;
}

void InstructionSymbolicExecuter::syscallInvoked (const CONTEXT *context,
    edu::sharif::twinner::trace::syscall::Syscall s) {
  CONTEXT newContext;
  runHooks (context, newContext);
  if (disabled) {
    return;
  }
  getTrace ()->terminateTraceSegment
      (new edu::sharif::twinner::trace::SyscallInvocation (s));
  if (measureMode) {
    numberOfExecutedInstructions++;
  }
}

bool InstructionSymbolicExecuter::startNewTraceSegment (
    CONTEXT *context) const {
  if (disabled) {
    edu::sharif::twinner::util::Logger::warning ()
        << "startNewTraceSegment is called while the ISE is disabled\n";
    return false;
  }
  const edu::sharif::twinner::trace::Trace *trace = getTrace ();
  return trace->initializeNewTraceSegment (context);
}

edu::sharif::twinner::util::MemoryManager *
InstructionSymbolicExecuter::getTraceMemoryManager () const {
  return memoryManager;
}

void InstructionSymbolicExecuter::touchMainArguments (int argc, char **argv) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  for (int i = 0; i < argc; ++i) {
    const char *argvi = argv[i];
    const int argvilen = strlen (argvi);
    for (int j = 0; j <= argvilen; ++j) {
      const ADDRINT addr = ADDRINT (&argvi[j]);
      edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy proxy (addr, 1);
      edu::sharif::twinner::trace::Expression *exp =
          getExpression (proxy, trace);
      delete exp;
    }
  }
}

void InstructionSymbolicExecuter::analysisRoutineBeforeCallingSafeFunction (
    ADDRINT retAddress, const FunctionInfo &fi,
    UINT32 insAssembly, const CONTEXT *context) {
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
  logger << std::hex << "analysisRoutineBeforeCallingSafeFunction(INS: "
      << insAssemblyStr << "): before calling " << fi << '\n';
  registerSafeFunction (fi, context);
  endOfSafeFuncRetAddress = retAddress;
  withinSafeFunc = true;
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineSyscall (ADDRINT syscallNumber,
    ADDRINT arg0, ADDRINT arg1, ADDRINT arg2, ADDRINT arg3,
    ADDRINT arg4, ADDRINT arg5,
    UINT32 insAssembly) {
  // we should report syscalls even while the ise is disabled
  disassembledInstruction = insAssembly;
  //  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  if (measureMode) {
    numberOfExecutedInstructions++;
    return;
  }
  const char *insAssemblyStr =
      memoryManager->getPointerToAllocatedMemory (insAssembly);
#ifdef TARGET_IS_32BITS_WINDOWS7_SP1
  edu::sharif::twinner::trace::syscall::Syscall const &syscall =
      edu::sharif::twinner::trace::syscall::X86Windows7Sp1Syscall
      (syscallNumber, arg0, arg1, arg2, arg3, arg4, arg5);
#else
  edu::sharif::twinner::trace::syscall::Syscall const &syscall =
      edu::sharif::twinner::trace::syscall::Syscall
      (syscallNumber, arg0, arg1, arg2, arg3, arg4, arg5);
#endif
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineSyscall(INS: "
      << insAssemblyStr << "): syscall-representation: "
      << syscall.getRepresentation () << '\n';
  syscallAnalysisRoutine (syscall);
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
  (this->*routine) (edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
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
  (this->*routine) (edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
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
  (this->*routine) (edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (srcReg, srcRegVal),
                    edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (auxReg, auxRegVal));
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
  (this->*routine) (edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (srcReg, srcRegVal),
                    edu::sharif::twinner::proxy::ConstantExpressionValueProxy (auxImmediateValue, 64));
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
  (this->*routine) (edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy (srcMemoryEa, (int) memReadBytes));
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
  (this->*routine) (edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy (srcMemoryEa, (int) memReadBytes),
                    edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (auxReg, auxRegVal));
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
  (this->*routine) (edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy (srcMemoryEa, memReadBytes),
                    edu::sharif::twinner::proxy::ConstantExpressionValueProxy (auxImmediateValue, 8));
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
  (this->*routine) (edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    edu::sharif::twinner::proxy::ConstantExpressionValueProxy (srcImmediateValue, REG_Size (dstReg) * 8));
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
  (this->*routine) (edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
                    edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
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
  (this->*routine) (edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
                    edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
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
  (this->*routine) (edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
                    edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (srcReg, srcRegVal),
                    edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (auxReg, auxRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcRegAuxImd (
    DoubleSourcesAnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    REG srcReg, const ConcreteValue &srcRegVal,
    const ConcreteValue &auxImmediateValue,
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
  logger << std::hex << "analysisRoutineDstMemSrcRegAuxImd(INS: "
      << insAssemblyStr << "): dst mem addr: 0x" << dstMemoryEa
      << ", src reg: " << REG_StringShort (srcReg)
      << ", aux imd: " << auxImmediateValue << '\n';
  (this->*routine) (edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
                    edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (srcReg, srcRegVal),
                    edu::sharif::twinner::proxy::ConstantExpressionValueProxy (auxImmediateValue, memReadBytes * 8));
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
  }
  logger << std::hex << ": dst mem addr: 0x" << dstMemoryEa
      << ", src imd: 0x" << srcImmediateValue << '\n';
  (this->*routine) (edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
                    edu::sharif::twinner::proxy::ConstantExpressionValueProxy (srcImmediateValue, memReadBytes * 8));
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
  }
  logger << std::hex << ": dst mem addr: 0x" << dstMemoryEa
      << ", src imd: 0x" << srcImmediateValue
      << ", aux reg: " << REG_StringShort (auxReg) << '\n';
  (this->*routine) (edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
                    edu::sharif::twinner::proxy::ConstantExpressionValueProxy (srcImmediateValue, memReadBytes * 8),
                    edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (auxReg, auxRegVal));
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
  (this->*routine) (edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy (dstMemoryEa, (int) memReadBytes),
                    edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy (srcMemoryEa, (int) memReadBytes));
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
  (this->*routine) (edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy (dstMemoryEa, (int) memReadBytes),
                    edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy (srcMemoryEa, (int) memReadBytes),
                    edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (auxReg, auxRegVal));
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
  (this->*routine) (edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    edu::sharif::twinner::proxy::ConstantExpressionValueProxy (dstRegVal, REG_Size (dstReg) * 8));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineBeforeRet (REG reg) {
  if (withinSafeFunc) {
    edu::sharif::twinner::util::Logger::debug ()
        << "analysisRoutineBeforeRet\n";
    setHook (&InstructionSymbolicExecuter::checkForEndOfSafeFunc, reg);
  }
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
  setHook (routine, reg);
  logger << "done\n";
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineBeforeChangeOfRegWithArg (
    SuddenlyChangedRegWithArgAnalysisRoutine routine,
    REG reg, ADDRINT argImmediateValue,
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
  logger << "analysisRoutineBeforeChangeOfRegWithArg(INS: "
      << insAssemblyStr << ")\n"
      "\tregistering register to be tracked...";
  setHook (routine, reg, argImmediateValue);
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
  (this->*routine) (edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (dstLeftReg, dstLeftRegVal),
                    edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (dstRightReg, dstRightRegVal),
                    edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
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
  (this->*routine) (edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (dstLeftReg, dstLeftRegVal),
                    edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (dstRightReg, dstRightRegVal),
                    edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy (srcMemoryEa, memReadBytes));
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
  (this->*routine) (edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
                    edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
                    edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
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
  (this->*routine) (edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (dstLeftReg, dstLeftRegVal),
                    edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (dstRightReg, dstRightRegVal),
                    edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
                    edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy (srcMemoryEa, memReadBytes));
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
  (this->*routine) (edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (dstReg, dstRegVal));
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
  (this->*routine) (edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy (srcMemoryEa, (int) memReadBytes));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineRunHooks (const CONTEXT *context) {
  CONTEXT newContext;
  if (runHooks (context, newContext)) {
    PIN_ExecuteAt (&newContext); // never returns
  }
}

void InstructionSymbolicExecuter::analysisRoutineInitializeRegisters (
    CONTEXT *context) const {
  edu::sharif::twinner::util::Logger::loquacious () << "analysisRoutineInitializeRegisters\n";
  startNewTraceSegment (context);
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
  repAnalysisRoutine (edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (repReg, repRegVal),
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
      (edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (baseReg, baseRegVal),
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
      (edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (baseReg, baseRegVal), displacement,
       edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (indexReg, indexRegVal), scale,
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
  edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy memory (memoryEa, memReadBytes);
  memory.checkForOverwritingMemory (trace);
}

edu::sharif::twinner::trace::Expression *
InstructionSymbolicExecuter::getExpression (
    const edu::sharif::twinner::proxy::ExpressionValueProxy &proxy,
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
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    edu::sharif::twinner::trace::Trace *trace,
    edu::sharif::twinner::trace::Expression *exp, bool shouldDeleteExp) const {
  edu::sharif::twinner::trace::StateSummary state;
  if (!shouldDeleteExp) {
    exp = exp->clone ();
  }
  dst.truncate (exp);
  // following call clones the exp and so we should delete ours
  dst.setExpression (trace, exp, state);
  delete exp;
  if (state.isWrongState ()) {
    edu::sharif::twinner::util::Logger::error () << state.getMessage () << '\n';
    abort ();
  }
}

void InstructionSymbolicExecuter::findPatternInStack (
    const CONTEXT *context) {
  if (searchPattern.empty ()) {
    return;
  }
  if (!searchedInstructions.insert (disassembledInstruction).second) {
    return; // this instruction has been already inspected
  }
  ConcreteValue *value =
#ifdef TARGET_IA32E
      edu::sharif::twinner::util::readRegisterContent (context, REG_RSP);
#else
      edu::sharif::twinner::util::readRegisterContent (context, REG_ESP);
#endif
  const UINT64 stackPointer = value->toUint64 ();
  delete value;
  findPatternInStack (stackPointer);
}

void InstructionSymbolicExecuter::findPatternInStack (
    const UINT64 stackPointer) const {
  char content[4 * 1024];
  const size_t size =
      PIN_SafeCopy (content, (const VOID *) (stackPointer), sizeof (content));
  const std::string stackContent (content, size);

  for (std::string::size_type patternPos = stackContent.find (searchPattern, 16);
      patternPos != std::string::npos;
      patternPos = stackContent.find (searchPattern, patternPos + 1)) {
    const ADDRINT patternAddress = stackPointer + patternPos;

    const std::string patternAddressBytesStr
        (reinterpret_cast<const char *> (&patternAddress), sizeof (patternAddress));

    for (std::string::size_type argv1Pos = stackContent.find (patternAddressBytesStr, 8);
        argv1Pos != std::string::npos;
        argv1Pos = stackContent.find (patternAddressBytesStr, argv1Pos + 1)) {
      const ADDRINT argv1Address = stackPointer + argv1Pos;
      edu::sharif::twinner::util::Logger::debug ()
          << "Found argv[1] at 0x" << std::hex << argv1Address << '\n';
      Instrumenter::printHexAscii (stackPointer, content, size);

      const char *argv2Value =
          *reinterpret_cast<const char **> (content + argv1Pos + sizeof (patternAddress));
      if (argv2Value == 0 || argv2Value
          == reinterpret_cast<const char *> (patternAddress) + searchPattern.length () + 1) {
        const ADDRINT argv0Address = argv1Address - sizeof (patternAddress);

        const std::string argv0AddressBytesStr
            (reinterpret_cast<const char *> (&argv0Address), sizeof (argv0Address));

        for (std::string::size_type argvPos = stackContent.find (argv0AddressBytesStr);
            argvPos != std::string::npos;
            argvPos = stackContent.find (argv0AddressBytesStr, argvPos + 1)) {
          const ADDRINT argvAddress = stackPointer + argvPos;

          const char *insAssemblyStr =
              memoryManager->getPointerToAllocatedMemory (disassembledInstruction);
          std::stringstream ss;
          ss << std::setw (16) << std::setfill ('0') << std::hex
              << "Pattern found in stack at address: 0x" << patternAddress
              << ", argv[1] is stored at address: 0x" << argv1Address
              << ", argv is stored at address: 0x" << argvAddress
              << ", disassembly: " << insAssemblyStr;
          const int argIndex = argvPos / 4 - 1;
          edu::sharif::twinner::util::Logger::info () << ss.str ()
              << ", offset in bytes: " << std::dec << argvPos
              << ", offset as arg-index: " << std::dec << argIndex
              << (argIndex < 25 ? ", close-pattern\n" : ", far-pattern\n");
        }
      }
    }
  }
}

bool InstructionSymbolicExecuter::runHooks (const CONTEXT *context,
    CONTEXT &newContext) {
  findPatternInStack (context);
  bool isContextModified = false;
  if (trackedReg != REG_INVALID_) {
    ConcreteValue *value =
        edu::sharif::twinner::util::readRegisterContent (context, trackedReg);
    if (hook) {
      Hook hfunc = hook;
      trackedReg = REG_INVALID_;
      hook = 0;
      isContextModified = (this->*hfunc) (context, *value, newContext);
    } else {
      HookWithArg hfunc = hookWithArg;
      trackedReg = REG_INVALID_;
      hookWithArg = 0;
      (this->*hfunc) (context, *value, arg);
    }
    delete value;

  } else if (operandSize > 0) {
    edu::sharif::twinner::trace::cv::ConcreteValue64Bits os (operandSize);
    Hook hfunc = hook;
    operandSize = -1;
    hook = 0;
    isContextModified = (this->*hfunc) (context, os, newContext);
  }
  return isContextModified;
}

void InstructionSymbolicExecuter::registerSafeFunction (const FunctionInfo &fi,
    const CONTEXT *context) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::trace::FunctionInvocation *f =
      instantiateFunctionInvocation (fi, trace, context);
  edu::sharif::twinner::util::Logger::loquacious ()
      << '\t' << f->getCallingLine (false);
  getTrace ()->terminateTraceSegment (f);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

edu::sharif::twinner::trace::FunctionInvocation *
InstructionSymbolicExecuter::instantiateFunctionInvocation (
    const FunctionInfo &fi, edu::sharif::twinner::trace::Trace *trace,
    const CONTEXT *context) const {
  std::string name = fi.getName ();
  std::list<edu::sharif::twinner::trace::FunctionArgumentInfo *> args;
  if (!fi.isAutoArgs ()) {
    const int argsNo = fi.getArgsNo ();
    for (int i = 0; i < argsNo; ++i) {
      args.push_back (fi.getArgument (i, trace, context));
    }
    return new edu::sharif::twinner::trace::FunctionInvocation
        (name, args);
  }
  if (name == "printf") {
    /*
     * Assuming that printf arguments are marked by simple %d, %s, etc.
     * in the format string. Pointing to arguments by their position numbers
     * causes wrong number of arguments to be guessed here.
     */
    // TODO: Support all cases of the printf format string
    args.push_back (edu::sharif::twinner::trace::FunctionArgumentInfoImp
                    ("const char *", true).resolve (0, trace, context));
    std::string formatString = args.back ()->getResolvedString ();
    if (formatString == "") {
      return new edu::sharif::twinner::trace::FunctionInvocation
          (name, args);
    }
    int argsNo = 0; // extra args after the format string
    for (int i = 0, len = formatString.length (); i < len; ++i) {
      if (formatString[i] == '%') {
        if (i + 1 < len && formatString[i + 1] == '%') {
          ++i;
          continue;
        }
        ++argsNo;
        args.push_back (edu::sharif::twinner::trace::FunctionArgumentInfoImp
                        ("UINT64", false).resolve (argsNo, trace, context));
      }
    }
    return new edu::sharif::twinner::trace::FunctionInvocation
        (name, args);

  } else if (name == "puts") {
    args.push_back (edu::sharif::twinner::trace::FunctionArgumentInfoImp
                    ("const char *", true).resolve (0, trace, context));
    return new edu::sharif::twinner::trace::FunctionInvocation
        (name, args);

  } else {
    edu::sharif::twinner::util::Logger::warning () << "argsNo=auto but "
        << name << " function is not supported by auto yet";
    return new edu::sharif::twinner::trace::FunctionInvocation (name);
  }
}

void InstructionSymbolicExecuter::syscallAnalysisRoutine (
    edu::sharif::twinner::trace::syscall::Syscall const &syscall) {
  edu::sharif::twinner::util::Logger::loquacious ()
      << "syscallAnalysisRoutine(...)\n";
  if (syscall.isProcessTerminatingSyscall ()) {
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tsyscall is a process terminating one; exit code is 0x"
        << std::hex << syscall.getExitCodeArgument () << '\n';
  }
}

void InstructionSymbolicExecuter::cmpxchgAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &aux) {
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

void InstructionSymbolicExecuter::palignrAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &shift) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "palignrAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      getExpression (dst, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting shift imd...";
  const edu::sharif::twinner::trace::Expression *shiftexp =
      getExpression (shift, trace);
  const int bits = shiftexp->getLastConcreteValue ().toUint64 () * 8;
  delete shiftexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tshifting...";
  srcexp->shiftToRight (bits);
  dstexp->shiftToLeft (dst.getSize () - bits);
  srcexp->bitwiseOr (dstexp);
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  setExpression (dst, trace, srcexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::pshufdAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &order) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::shldAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &bits) {
  shldAnalysisRoutine
      (dst, src, static_cast<const edu::sharif::twinner::proxy::ExpressionValueProxy &> (bits));
}

void InstructionSymbolicExecuter::shldAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &shift) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "shldAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp = getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexpOrig =
      getExpression (dst, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting shift exp...";
  edu::sharif::twinner::trace::Expression *shiftexp =
      getExpression (shift, trace);
  if (dst.getSize () == 64) {
    shiftexp->bitwiseAnd (0x3F); // % 64
  } else {
    shiftexp->bitwiseAnd (0x1F); // % 32
  }
  edu::sharif::twinner::trace::Expression *dstexp = dstexpOrig->clone ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tshifting operation...";
  dstexp->shiftToLeft (shiftexp);
  // truncate bits which are shifted left, outside of dst boundaries
  dst.truncate (dstexp);
  // fill lower bits with src
  edu::sharif::twinner::trace::Expression *fillexp =
      new edu::sharif::twinner::trace::ExpressionImp
      (srcexp->getLastConcreteValue ().getSize ());
  fillexp->minus (shiftexp);
  srcexp->shiftToRight (fillexp);
  delete fillexp;
  dstexp->bitwiseOr (srcexp);
  delete srcexp;
  setExpression (dst, trace, dstexp);
  delete dstexpOrig;
  delete shiftexp;
  eflags.setFlags (new edu::sharif::twinner::operationgroup::DummyOperationGroup
                   ("ShiftDoubleLeftOperationGroup"));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::xchgAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "xchgAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp = getExpression (dst, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  setExpression (dst, trace, srcexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting src exp...";
  setExpression (src, trace, dstexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::xaddAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "xaddAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp = getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp = getExpression (dst, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting src exp...";
  srcexp->add (dstexp);
  setExpression (src, trace, dstexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  setExpression (dst, trace, srcexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::movlpdAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "movlpdAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  const int size = dst.getSize ();
  edu::sharif::twinner::trace::Expression *res;
  if (size == 128) {
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tmoving 64-bits from src to low 64-bits of dst..."
        << "\tgetting dst exp...";
    edu::sharif::twinner::trace::Expression *dstexp =
        getExpression (dst, trace);
    ConcreteValue *mask =
        new edu::sharif::twinner::trace::cv::ConcreteValue128Bits (-1, 0);
    dstexp->bitwiseAnd (mask);
    res = srcexp->clone (128);
    res->bitwiseOr (dstexp);
    delete dstexp;
  } else {
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tmoving low 64-bits of src to low 64-bits of dst...";
    srcexp->truncate (64);
    res = srcexp->clone (64);
  }
  delete srcexp;
  setExpression (dst, trace, res);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::movhpdAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "movhpdAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  const int size = dst.getSize ();
  edu::sharif::twinner::trace::Expression *res;
  if (size == 128) {
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tmoving 64-bits from src to high 64-bits of dst..."
        << "\tgetting dst exp...";
    edu::sharif::twinner::trace::Expression *dstexp =
        getExpression (dst, trace);
    dstexp->truncate (64);
    res = srcexp->clone (128);
    res->shiftToLeft (64);
    res->bitwiseOr (dstexp);
    delete dstexp;
  } else {
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tmoving high 64-bits of src to low 64-bits of dst...";
    srcexp->shiftToRight (64);
    res = srcexp->clone (64);
  }
  delete srcexp;
  setExpression (dst, trace, res);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::movAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "movAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  setExpression (dst, trace, srcexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::movsxAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::cdqAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "cdqAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp = getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tinstantiating constraint...";
  bool sign;
  std::list <edu::sharif::twinner::trace::Constraint *> cc;
  cc.push_back
      (edu::sharif::twinner::trace::Constraint::instantiateLessConstraint
       (sign, srcexp, disassembledInstruction));
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  delete srcexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp;
  if (sign) {
    edu::sharif::twinner::trace::cv::ConcreteValue64Bits fullOne (UINT64 (-1));
    dstexp = new edu::sharif::twinner::trace::ExpressionImp
        (fullOne.clone (dst.getSize ()));
  } else {
    edu::sharif::twinner::trace::cv::ConcreteValue64Bits fullZero (UINT64 (0));
    dstexp = new edu::sharif::twinner::trace::ExpressionImp
        (fullZero.clone (dst.getSize ()));
  }
  setExpression (dst, trace, dstexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::movsAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &rdi,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &rsi,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dstMem,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &srcMem) {
  edu::sharif::twinner::util::Logger::loquacious () << "movsAnalysisRoutine(...)\n";
  movAnalysisRoutine (dstMem, srcMem);
  const int size = dstMem.getSize () / 8;
  adjustRsiRdiRegisters (size, rdi, rsi);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::cmpsAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &rdi,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &rsi,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dstMem,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &srcMem) {
  edu::sharif::twinner::util::Logger::loquacious () << "cmpsAnalysisRoutine(...)\n";
  cmpAnalysisRoutine (dstMem, srcMem);
  const int size = dstMem.getSize () / 8;
  adjustRsiRdiRegisters (size, rdi, rsi);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::adjustRsiRdiRegisters (int size,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &rdi,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &rsi) {
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
}

void InstructionSymbolicExecuter::pushfdAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &stack,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &flags,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &rsp) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "pushfdAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::cv::ConcreteValue *cv = flags.getConcreteValue ();
  uint32_t flagsConcreteValue = cv->toUint64 ();
  delete cv;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.getFlagsExpression (flagsConcreteValue, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraints...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  setExpression (stack, trace, new edu::sharif::twinner::trace::ExpressionImp
                 (UINT64 (flagsConcreteValue)));
  edu::sharif::twinner::util::Logger::loquacious () << "\tadjusting rsp...";
  edu::sharif::twinner::trace::Expression *rspexp = getExpression (rsp, trace);
  rspexp->minus (stack.getSize () / 8);
  setExpression (rsp, trace, rspexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::pushAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &rsp) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "pushAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  setExpression (dst, trace, srcexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadjusting rsp...";
  edu::sharif::twinner::trace::Expression *rspexp = getExpression (rsp, trace);
  rspexp->minus (STACK_OPERATION_UNIT_SIZE);
  setExpression (rsp, trace, rspexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::popAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &rsp) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "popAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  setExpression (dst, trace, srcexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadjusting rsp...";
  edu::sharif::twinner::trace::Expression *rspexp = getExpression (rsp, trace);
  rspexp->add (STACK_OPERATION_UNIT_SIZE);
  setExpression (rsp, trace, rspexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::lodsdAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dstReg,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &srcMem,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &rsi) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::addAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
  eflags.setFlags
      (new edu::sharif::twinner::operationgroup::AdditionOperationGroup
       (dstexpOrig, srcexp));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::adcAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
  eflags.setFlags
      (new edu::sharif::twinner::operationgroup::AddWithCarryOperationGroup
       (dstexp, srcexp, carryexp));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::subAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
  eflags.setFlags
      (new edu::sharif::twinner::operationgroup::SubtractOperationGroup
       (dstexpOrig, srcexp));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::sbbAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
  eflags.setFlags
      (new edu::sharif::twinner::operationgroup::SubtractWithBorrowOperationGroup
       (dstexp, srcexp, carryexp));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::cmpAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
      (new edu::sharif::twinner::operationgroup::SubtractOperationGroup
       (dstexp, srcexp));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::leaAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "leaAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  setExpression (dst, trace, srcexp);
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

void InstructionSymbolicExecuter::joAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "joAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool overflow;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForOverflowCase (overflow, disassembledInstruction);
  if (overflow != branchTaken) {
    edu::sharif::twinner::util::Logger::error ()
        << "InstructionSymbolicExecuter::joAnalysisRoutine"
        " (branchTaken=" << branchTaken << "):"
        "JO branching and last known EFLAGS state do not match\n";
    abort ();
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jpAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "jpAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool parity;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForParityCase (parity, disassembledInstruction);
  if (parity != branchTaken) {
    edu::sharif::twinner::util::Logger::error ()
        << "InstructionSymbolicExecuter::jpAnalysisRoutine"
        " (branchTaken=" << branchTaken << "):"
        "JP branching and last known EFLAGS state do not match\n";
    abort ();
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jnpAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "jnpAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool parity;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForParityCase (parity, disassembledInstruction);
  if (parity == branchTaken) {
    edu::sharif::twinner::util::Logger::error ()
        << "InstructionSymbolicExecuter::jnpAnalysisRoutine"
        " (branchTaken=" << branchTaken << "):"
        "JNP branching and last known EFLAGS state do not match\n";
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

bool InstructionSymbolicExecuter::callAnalysisRoutine (const CONTEXT *context,
    const ConcreteValue &rspRegVal, CONTEXT &newContext) {
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
        edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy stack
            (rspRegVal.toUint64 (), STACK_OPERATION_UNIT_SIZE);
        edu::sharif::twinner::trace::StateSummary state;
        edu::sharif::twinner::trace::Expression *exp =
            stack.getExpression (trace, state);
        if (exp) {
          delete exp;
        }
        if (state.isWrongState ()) {
          edu::sharif::twinner::trace::Expression *exp =
              new edu::sharif::twinner::trace::ExpressionImp
              (state.getExpectedStateValue ());
          setExpression (stack, trace, exp);
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
  return false;
}

bool InstructionSymbolicExecuter::checkForEndOfSafeFunc (const CONTEXT *context,
    const ConcreteValue &ripRegVal, CONTEXT &newContext) {
  bool isContextModified = false;
  if (endOfSafeFuncRetAddress == ripRegVal.toUint64 ()) {
    isContextModified = im->afterSafeFunction (context, newContext);
    withinSafeFunc = false;
  }
  return isContextModified;
}

bool InstructionSymbolicExecuter::retAnalysisRoutine (const CONTEXT *context,
    const ConcreteValue &rspRegVal, CONTEXT &newContext) {
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
  return false;
}

void InstructionSymbolicExecuter::retWithArgAnalysisRoutine (
    const CONTEXT *context, const ConcreteValue &rspRegVal, ADDRINT offset) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "retWithArgAnalysisRoutine(...)\n"
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
      const bool normalRetInstruction =
          ((*cv) == STACK_OPERATION_UNIT_SIZE + offset)
          || ((*cv) == 2 * STACK_OPERATION_UNIT_SIZE + offset);
      if (!normalRetInstruction) {
        edu::sharif::twinner::util::Logger::error ()
            << "InstructionSymbolicExecuter::retWithArgAnalysisRoutine (...): "
            "ret instruction must pop either "
            << (STACK_OPERATION_UNIT_SIZE + offset) << " or "
            << (2 * STACK_OPERATION_UNIT_SIZE + offset) << " bytes\n";
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

bool InstructionSymbolicExecuter::jmpAnalysisRoutine (const CONTEXT *context,
    const ConcreteValue &rspRegVal, CONTEXT &newContext) {
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
  return false;
}

void InstructionSymbolicExecuter::repAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    bool executing, bool repEqual) {
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
    setExpression (dst, trace, dstexp, false);
  }
  delete dstexp;
  std::list <edu::sharif::twinner::trace::Constraint *> ccList;
  ccList.push_front (cc);
  trace->addPathConstraints (ccList);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::pslldqAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "pslldqAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexpOrig =
      getExpression (dst, trace);
  edu::sharif::twinner::trace::Expression *dstexp = dstexpOrig->clone ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tshifting operation...";
  if (dynamic_cast<const edu::sharif::twinner::proxy::ConstantExpressionValueProxy *> (&src) == 0) {
    edu::sharif::twinner::util::Logger::error ()
        << "\tthe PSLLDQ src is not an immediate value!";
    abort ();
  }
  srcexp->multiply (8); // convert byte to bits
  edu::sharif::twinner::trace::cv::ConcreteValue *cv =
      srcexp->getLastConcreteValue ().clone ();
  dstexp->shiftToLeft (cv);
  // truncate bits which are shifted left, outside of dst boundaries
  dst.truncate (dstexp);
  setExpression (dst, trace, dstexp);
  eflags.setFlags
      (new edu::sharif::twinner::operationgroup::ShiftLeftOperationGroup
       (dstexpOrig, srcexp));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::psrldqAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "psrldqAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexpOrig =
      getExpression (dst, trace);
  edu::sharif::twinner::trace::Expression *dstexp = dstexpOrig->clone ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tshifting operation...";
  if (dynamic_cast<const edu::sharif::twinner::proxy::ConstantExpressionValueProxy *> (&src) == 0) {
    edu::sharif::twinner::util::Logger::error ()
        << "\tthe PSRLDQ src is not an immediate value!";
    abort ();
  }
  srcexp->multiply (8); // convert byte to bits
  edu::sharif::twinner::trace::cv::ConcreteValue *cv =
      srcexp->getLastConcreteValue ().clone ();
  dstexp->shiftToRight (cv);
  setExpression (dst, trace, dstexp);
  eflags.setFlags
      (new edu::sharif::twinner::operationgroup::ShiftLeftOperationGroup
       (dstexpOrig, srcexp));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::shlAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "shlAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexpOrig =
      getExpression (dst, trace);
  const int mask =
      dstexpOrig->getLastConcreteValue ().getSize () > 32 ? 0x3f : 0x1f;
  srcexp->bitwiseAnd (mask);
  edu::sharif::twinner::trace::Expression *dstexp = dstexpOrig->clone ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tshifting operation...";
  if (dynamic_cast<const edu::sharif::twinner::proxy::ConstantExpressionValueProxy *> (&src) != 0) {
    // src was an immediate value
    dstexp->shiftToLeft (srcexp->getLastConcreteValue ().clone ());
  } else {
    // src was CL register
    dstexp->shiftToLeft (srcexp);
  }
  // truncate bits which are shifted left, outside of dst boundaries
  dst.truncate (dstexp);
  setExpression (dst, trace, dstexp);
  eflags.setFlags
      (new edu::sharif::twinner::operationgroup::ShiftLeftOperationGroup
       (dstexpOrig, srcexp));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::shrAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "shrAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexpOrig =
      getExpression (dst, trace);
  const int mask =
      dstexpOrig->getLastConcreteValue ().getSize () > 32 ? 0x3f : 0x1f;
  srcexp->bitwiseAnd (mask);
  edu::sharif::twinner::trace::Expression *dstexp = dstexpOrig->clone ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tshifting operation...";
  dstexp->shiftToRight (srcexp);
  setExpression (dst, trace, dstexp);
  eflags.setFlags
      (new edu::sharif::twinner::operationgroup::ShiftRightOperationGroup
       (dstexpOrig, srcexp));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::sarAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "sarAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexpOrig =
      getExpression (dst, trace);
  edu::sharif::twinner::trace::Expression *dstexp = dstexpOrig->clone ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tshifting operation...";
  dstexp->arithmeticShiftToRight (srcexp);
  setExpression (dst, trace, dstexp);
  eflags.setFlags (new edu::sharif::twinner::operationgroup
                   ::ShiftArithmeticRightOperationGroup (dstexpOrig, srcexp));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::rorAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::rolAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::andAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
  setExpression (dst, trace, dstexp, false);
  eflags.setFlags
      (new edu::sharif::twinner::operationgroup::BitwiseAndOperationGroup
       (dstexp));
  eflags.setOverflowFlag (false);
  eflags.setCarryFlag (false);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::orAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
  setExpression (dst, trace, dstexp, false);
  eflags.setFlags
      (new edu::sharif::twinner::operationgroup::BitwiseAndOperationGroup
       (dstexp));
  eflags.setOverflowFlag (false);
  eflags.setCarryFlag (false);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::xorAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
  setExpression (dst, trace, dstexp, false);
  eflags.setFlags
      (new edu::sharif::twinner::operationgroup::BitwiseAndOperationGroup
       (dstexp));
  eflags.setOverflowFlag (false);
  eflags.setCarryFlag (false);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::testAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
      (new edu::sharif::twinner::operationgroup::BitwiseAndOperationGroup
       (dstexp));
  eflags.setOverflowFlag (false);
  eflags.setCarryFlag (false);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::btAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &bitstring,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &offset) {
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

void InstructionSymbolicExecuter::btrAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &bitstring,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &offset) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "btrAnalysisRoutine(...)\n"
      << "\tgetting offset exp...";
  edu::sharif::twinner::trace::Expression *offsetexp =
      getExpression (offset, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting bitstring exp...";
  edu::sharif::twinner::trace::Expression *bitstringexpOrig =
      getExpression (bitstring, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tfinding requested bit...";
  edu::sharif::twinner::trace::Expression *bitstringexp =
      bitstringexpOrig->clone ();
  offsetexp->bitwiseAnd (bitstring.getSize () - 1);
  bitstringexp->shiftToRight (offsetexp);
  bitstringexp->bitwiseAnd (0x1);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting EFLAGS...";
  eflags.setCarryFlag (bitstringexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tresetting selected bit...";
  edu::sharif::twinner::trace::Expression *mask =
      new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  mask->shiftToLeft (offsetexp);
  delete offsetexp;
  mask->bitwiseNegate ();
  bitstringexpOrig->bitwiseAnd (mask);
  delete mask;
  setExpression (bitstring, trace, bitstringexpOrig);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::btsAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &bitstring,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &offset) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "btsAnalysisRoutine(...)\n"
      << "\tgetting offset exp...";
  edu::sharif::twinner::trace::Expression *offsetexp =
      getExpression (offset, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting bitstring exp...";
  edu::sharif::twinner::trace::Expression *bitstringexpOrig =
      getExpression (bitstring, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tfinding requested bit...";
  edu::sharif::twinner::trace::Expression *bitstringexp =
      bitstringexpOrig->clone ();
  offsetexp->bitwiseAnd (bitstring.getSize () - 1);
  bitstringexp->shiftToRight (offsetexp);
  bitstringexp->bitwiseAnd (0x1);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting EFLAGS...";
  eflags.setCarryFlag (bitstringexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting selected bit...";
  edu::sharif::twinner::trace::Expression *mask =
      new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  mask->shiftToLeft (offsetexp);
  delete offsetexp;
  bitstringexpOrig->bitwiseOr (mask);
  delete mask;
  setExpression (bitstring, trace, bitstringexpOrig);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::pmovmskbAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "pmovmskbAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tpreparing mask-byte(src)...";
  edu::sharif::twinner::trace::Expression *maskbyte =
      new edu::sharif::twinner::trace::ExpressionImp (); // zero-filled
  // src is a reg and is mutable
  const int size = static_cast<const edu::sharif::twinner::proxy::MutableExpressionValueProxy &> (src).getSize ();
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::pcmpeqbAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
  edu::sharif::twinner::trace::Expression *resexp =
      new edu::sharif::twinner::trace::ExpressionImp (result);
  setExpression (dst, trace, resexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::pcmpgtbAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "pcmpgtbAnalysisRoutine(...)\n"
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
  for (int i = 0; i < size; i += 8) {
    edu::sharif::twinner::trace::Expression *ithByteSrc = srcexp->clone ();
    edu::sharif::twinner::trace::Expression *ithByteDst = dstexp->clone ();
    ithByteSrc->shiftToRight (i);
    ithByteSrc->bitwiseAnd (0xFF);
    ithByteDst->shiftToRight (i);
    ithByteDst->bitwiseAnd (0xFF);
    bool lessOrEqual;
    edu::sharif::twinner::trace::Constraint *ithDstByteIsGreaterThanSrc =
        edu::sharif::twinner::trace::Constraint::instantiateLessOrEqualConstraint
        (lessOrEqual, ithByteDst, ithByteSrc, disassembledInstruction);
    cc.push_back (ithDstByteIsGreaterThanSrc);
    ConcreteValue *c = dstexp->getLastConcreteValue ().clone ();
    *c = lessOrEqual ? 0x00 : 0xFF;
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
  edu::sharif::twinner::trace::Expression *resexp =
      new edu::sharif::twinner::trace::ExpressionImp (result);
  setExpression (dst, trace, resexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::pminubAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::psubbAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "psubbAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      getExpression (src, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexp =
      getExpression (dst, trace);
  const int size = dst.getSize ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsubtracting byte-by-byte...";
  edu::sharif::twinner::trace::Expression *res = 0;
  const int bytesNumber = size / 8;
  ConcreteValue *mask = dstexp->getLastConcreteValue ().clone ();
  (*mask) = 0xFF;
  for (int i = 0; i < bytesNumber; ++i) {
    edu::sharif::twinner::trace::Expression *nextDstByte = dstexp->clone (size);
    edu::sharif::twinner::trace::Expression *nextSrcByte = srcexp->clone (size);
    nextDstByte->bitwiseAnd (mask->clone ());
    nextSrcByte->bitwiseAnd (mask->clone ());
    nextDstByte->minus (nextSrcByte);
    delete nextSrcByte;
    nextDstByte->bitwiseAnd (mask->clone ());
    if (res == 0) {
      res = nextDstByte;
    } else {
      res->bitwiseOr (nextDstByte);
      delete nextDstByte;
    }
    (*mask) <<= 8;
  }
  delete mask;
  setExpression (dst, trace, res);
  delete srcexp;
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::punpcklbwAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
  delete srcexp;
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::punpcklwdAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
  delete srcexp;
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::bsfAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &leftDst,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &rightDst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
  leftDstExp->truncate (operandSize);
  leftDst.setExpressionWithoutChangeNotification (trace, leftDstExp);
  delete leftDstExp;
  rightDstExp->truncate (operandSize);
  rightDst.setExpressionWithoutChangeNotification (trace, rightDstExp);
  delete rightDstExp;
  // At this point, symbolic quotient and remainder are calculated correctly.
  // but concrete values are not! So we need to register a hook to synchronize concrete
  // values too (we can also calculate them in assembly, but it's not required).

  hook = &InstructionSymbolicExecuter::adjustDivisionMultiplicationOperands;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::idivAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &leftDst,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &rightDst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
  leftDstExp->truncate (operandSize);
  leftDst.setExpressionWithoutChangeNotification (trace, leftDstExp);
  delete leftDstExp;
  rightDstExp->truncate (operandSize);
  rightDst.setExpressionWithoutChangeNotification (trace, rightDstExp);
  delete rightDstExp;
  // At this point, symbolic quotient and remainder are calculated correctly.
  // but concrete values are not! So we need to register a hook to synchronize concrete
  // values too (we can also calculate them in assembly, but it's not required).

  hook = &InstructionSymbolicExecuter::adjustDivisionMultiplicationOperands;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

bool InstructionSymbolicExecuter::adjustDivisionMultiplicationOperands (
    const CONTEXT *context, const ConcreteValue &operandSize,
    CONTEXT &newContext) {
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
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &ax =
        edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (REG_AX);
    setExpression (ax, trace, leftExp, false); // this deletes unused expressions by itself
  } else {
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &left =
        edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (leftReg, *leftVal);
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &right =
        edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (rightReg, *rightVal);
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
  return false;
}

void InstructionSymbolicExecuter::mulAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &leftDst,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &rightDst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
  leftDstExp->truncate (operandSize);
  leftDst.setExpressionWithoutChangeNotification (trace, leftDstExp);
  delete leftDstExp;
  rightDstExp->truncate (operandSize);
  rightDst.setExpressionWithoutChangeNotification (trace, rightDstExp);
  delete rightDstExp;
  // At this point, symbolic multiplication result is calculated correctly.
  // but concrete values are not! So we need to register a hook to synchronize concrete
  // values too (we can also calculate them in assembly, but it's not required).

  hook = &InstructionSymbolicExecuter::adjustDivisionMultiplicationOperands;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::imulAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &leftDst,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &rightDst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
  leftDstExp->truncate (operandSize);
  leftDst.setExpressionWithoutChangeNotification (trace, leftDstExp);
  delete leftDstExp;
  rightDstExp->truncate (operandSize);
  rightDst.setExpressionWithoutChangeNotification (trace, rightDstExp);
  delete rightDstExp;
  // At this point, symbolic multiplication result is calculated correctly.
  // but concrete values are not! So we need to register a hook to synchronize concrete
  // values too (we can also calculate them in assembly, but it's not required).

  hook = &InstructionSymbolicExecuter::adjustDivisionMultiplicationOperands;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::imulAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::imulAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dst,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &src,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &imd) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::scasAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dstReg,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &srcReg,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &srcMem) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tchecking eflags...";
  bool zero;
  std::list <edu::sharif::twinner::trace::Constraint *> ccList =
      eflags.instantiateConstraintForZeroCase (zero, disassembledInstruction);
  trace->addPathConstraints (ccList);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::stosAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &dstMem,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &rdireg,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &srcReg) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::leaveAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &fpReg,
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &spReg,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &srcMem) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "leaveAnalysisRoutine(...)\n"
      << "\tgetting frame pointer (to be set in stack pointer)...";
  edu::sharif::twinner::trace::Expression *rsp = getExpression (fpReg, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tpopping frame pointer...";
  edu::sharif::twinner::trace::Expression *memexp =
      getExpression (srcMem, trace);
  setExpression (fpReg, trace, memexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadjusting rsp...";
  rsp->add (STACK_OPERATION_UNIT_SIZE);
  setExpression (spReg, trace, rsp);
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

  const edu::sharif::twinner::proxy::MutableExpressionValueProxy &edx =
      edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (REG_EDX, *edxVal);
  const edu::sharif::twinner::proxy::MutableExpressionValueProxy &eax =
      edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (REG_EAX, *eaxVal);
  setExpression (edx, trace, edxNewExp);
  setExpression (eax, trace, eaxNewExp);
}

void InstructionSymbolicExecuter::stdAnalysisRoutine (const CONTEXT *context) {
  edu::sharif::twinner::util::Logger::loquacious () << "stdAnalysisRoutine(...)\n";
  eflags.setDirectionFlag (true);
}

void InstructionSymbolicExecuter::cldAnalysisRoutine (const CONTEXT *context) {
  edu::sharif::twinner::util::Logger::loquacious () << "cldAnalysisRoutine(...)\n";
  /**
   * Now, we are right after the CLD instruction. This is a decision to match with other
   * operand-less instructions. Anyway, our implementation for CLD works independent of
   * being executed before or after the CLD instruction itself.
   */
  eflags.setDirectionFlag (false);
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

  const edu::sharif::twinner::proxy::MutableExpressionValueProxy &eax =
      edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (REG_EAX, *eaxVal);
  const edu::sharif::twinner::proxy::MutableExpressionValueProxy &ebx =
      edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (REG_EBX, *ebxVal);
  const edu::sharif::twinner::proxy::MutableExpressionValueProxy &ecx =
      edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (REG_ECX, *ecxVal);
  const edu::sharif::twinner::proxy::MutableExpressionValueProxy &edx =
      edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy (REG_EDX, *edxVal);
  setExpression (eax, trace, eaxNewExp);
  setExpression (ebx, trace, ebxNewExp);
  setExpression (ecx, trace, ecxNewExp);
  setExpression (edx, trace, edxNewExp);
}

void InstructionSymbolicExecuter::incAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "incAnalysisRoutine(...)\n"
      << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexpOrig =
      getExpression (opr, trace);
  edu::sharif::twinner::trace::Expression *dstexp = dstexpOrig->clone ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tincrementing...";
  dstexp->add (1);
  setExpression (opr, trace, dstexp);
  const edu::sharif::twinner::trace::Expression *one =
      new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  eflags.setFlags
      (new edu::sharif::twinner::operationgroup::AdditionOperationGroup
       (dstexpOrig, one));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::decAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "decAnalysisRoutine(...)\n"
      << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexpOrig =
      getExpression (opr, trace);
  edu::sharif::twinner::trace::Expression *dstexp = dstexpOrig->clone ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tdecrementing...";
  dstexp->minus (1);
  setExpression (opr, trace, dstexp);
  const edu::sharif::twinner::trace::Expression *one =
      new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  eflags.setFlags
      (new edu::sharif::twinner::operationgroup::SubtractOperationGroup
       (dstexpOrig, one));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::negAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "negAnalysisRoutine(...)\n"
      << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexpOrig =
      getExpression (opr, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\ttwo's complementing...";
  edu::sharif::twinner::trace::Expression *dstexp = dstexpOrig->twosComplement ();
  setExpression (opr, trace, dstexp);
  eflags.setFlags
      (new edu::sharif::twinner::operationgroup::SubtractOperationGroup
       (new edu::sharif::twinner::trace::ExpressionImp (UINT64 (0)), dstexpOrig));
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setoAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "setoAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool overflow;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForOverflowCase (overflow, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp;
  if (overflow) {
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  } else { // shouldSetToZero
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
  }
  setExpression (opr, trace, dstexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setpAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "setpAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool parity;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForParityCase (parity, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp;
  if (parity) {
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  } else { // shouldSetToZero
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
  }
  setExpression (opr, trace, dstexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setnpAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "setnpAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool parity;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForParityCase (parity, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp;
  if (!parity) {
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  } else { // shouldSetToZero
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
  }
  setExpression (opr, trace, dstexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setsAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "setsAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool sign;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForSignCase (sign, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp;
  if (sign) {
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  } else { // shouldSetToZero
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
  }
  setExpression (opr, trace, dstexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setnsAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "setnsAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool sign;
  std::list <edu::sharif::twinner::trace::Constraint *> cc =
      eflags.instantiateConstraintForSignCase (sign, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraints (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp;
  if (!sign) {
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  } else { // shouldSetToZero
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
  }
  setExpression (opr, trace, dstexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setnzAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &opr) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setzAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &opr) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setleAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &opr) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setnleAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &opr) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setlAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &opr) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setbeAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &opr) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setnbeAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &opr) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setbAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &opr) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setnlAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &opr) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setnbAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &opr) {
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
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::notAnalysisRoutine (
    const edu::sharif::twinner::proxy::MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::trace::Trace *trace = getTrace ();
  edu::sharif::twinner::util::Logger::loquacious () << "notAnalysisRoutine(...)\n"
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp = getExpression (opr, trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tnegating...";
  dstexp->bitwiseNegate ();
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  setExpression (opr, trace, dstexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::memoryRegisterCorrespondenceAnalysisRoutine (
    const edu::sharif::twinner::proxy::ExpressionValueProxy &baseReg,
    ADDRDELTA displacement,
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
    const edu::sharif::twinner::proxy::ExpressionValueProxy &baseReg,
    ADDRDELTA displacement,
    const edu::sharif::twinner::proxy::ExpressionValueProxy &indexReg,
    UINT32 scale,
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
  case XED_ICLASS_MOVQ:
  case XED_ICLASS_MOVAPS:
  case XED_ICLASS_LDDQU:
  case XED_ICLASS_MOVDQU:
  case XED_ICLASS_MOVDQA:
  case XED_ICLASS_MOVSD_XMM:
  case XED_ICLASS_MOVSS:
  case XED_ICLASS_CMOVBE:
  case XED_ICLASS_CMOVNBE:
    return &InstructionSymbolicExecuter::movAnalysisRoutine;
  case XED_ICLASS_MOVLPD:
    return &InstructionSymbolicExecuter::movlpdAnalysisRoutine;
  case XED_ICLASS_MOVHPD:
    return &InstructionSymbolicExecuter::movhpdAnalysisRoutine;
  case XED_ICLASS_IMUL:
    return &InstructionSymbolicExecuter::imulAnalysisRoutine;
  case XED_ICLASS_CDQE:
  case XED_ICLASS_MOVSX:
  case XED_ICLASS_MOVSXD:
    return &InstructionSymbolicExecuter::movsxAnalysisRoutine;
  case XED_ICLASS_CDQ:
    return &InstructionSymbolicExecuter::cdqAnalysisRoutine;
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
  case XED_ICLASS_PSLLDQ:
    return &InstructionSymbolicExecuter::pslldqAnalysisRoutine;
  case XED_ICLASS_PSRLDQ:
    return &InstructionSymbolicExecuter::psrldqAnalysisRoutine;
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
  case XED_ICLASS_PAND:
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
  case XED_ICLASS_BTR:
    return &InstructionSymbolicExecuter::btrAnalysisRoutine;
  case XED_ICLASS_BTS:
    return &InstructionSymbolicExecuter::btsAnalysisRoutine;
  case XED_ICLASS_PMOVMSKB:
    return &InstructionSymbolicExecuter::pmovmskbAnalysisRoutine;
  case XED_ICLASS_PCMPEQB:
    return &InstructionSymbolicExecuter::pcmpeqbAnalysisRoutine;
  case XED_ICLASS_PCMPGTB:
    return &InstructionSymbolicExecuter::pcmpgtbAnalysisRoutine;
  case XED_ICLASS_PMINUB:
    return &InstructionSymbolicExecuter::pminubAnalysisRoutine;
  case XED_ICLASS_PSUBB:
    return &InstructionSymbolicExecuter::psubbAnalysisRoutine;
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
  case XED_ICLASS_XADD:
    return &InstructionSymbolicExecuter::xaddAnalysisRoutine;
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
  case XED_ICLASS_PUSHFD:
    return &InstructionSymbolicExecuter::pushfdAnalysisRoutine;
  case XED_ICLASS_PUSH:
    return &InstructionSymbolicExecuter::pushAnalysisRoutine;
  case XED_ICLASS_POP:
    return &InstructionSymbolicExecuter::popAnalysisRoutine;
  case XED_ICLASS_LODSD:
    return &InstructionSymbolicExecuter::lodsdAnalysisRoutine;
  case XED_ICLASS_SHLD:
    return &InstructionSymbolicExecuter::shldAnalysisRoutine;
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
  case XED_ICLASS_SCASW:
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
  case XED_ICLASS_PALIGNR:
    return &InstructionSymbolicExecuter::palignrAnalysisRoutine;
  case XED_ICLASS_PSHUFD:
    return &InstructionSymbolicExecuter::pshufdAnalysisRoutine;
  case XED_ICLASS_SHLD:
    return &InstructionSymbolicExecuter::shldAnalysisRoutine;
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
  case XED_ICLASS_MOVSD:
  case XED_ICLASS_MOVSW:
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
  case XED_ICLASS_JO:
    return &InstructionSymbolicExecuter::joAnalysisRoutine;
  case XED_ICLASS_JP:
    return &InstructionSymbolicExecuter::jpAnalysisRoutine;
  case XED_ICLASS_JNP:
    return &InstructionSymbolicExecuter::jnpAnalysisRoutine;
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

InstructionSymbolicExecuter::SuddenlyChangedRegWithArgAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToSuddenlyChangedRegWithArgAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_RET_NEAR:
    return &InstructionSymbolicExecuter::retWithArgAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Suddenly Changed Register (with arg): Unknown opcode: "
        << OPCODE_StringShort (op) << '\n';
    abort ();
  }
}

InstructionSymbolicExecuter::OperandLessAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToOperandLessAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_RDTSC:
    return &InstructionSymbolicExecuter::rdtscAnalysisRoutine;
  case XED_ICLASS_STD:
    return &InstructionSymbolicExecuter::stdAnalysisRoutine;
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
  case XED_ICLASS_SETO:
    return &InstructionSymbolicExecuter::setoAnalysisRoutine;
  case XED_ICLASS_SETP:
    return &InstructionSymbolicExecuter::setpAnalysisRoutine;
  case XED_ICLASS_SETNP:
    return &InstructionSymbolicExecuter::setnpAnalysisRoutine;
  case XED_ICLASS_SETS:
    return &InstructionSymbolicExecuter::setsAnalysisRoutine;
  case XED_ICLASS_SETNS:
    return &InstructionSymbolicExecuter::setnsAnalysisRoutine;
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

VOID analysisRoutineSyscall (VOID *iseptr,
    ADDRINT syscallNumber,
    ADDRINT arg0, ADDRINT arg1, ADDRINT arg2, ADDRINT arg3,
    ADDRINT arg4, ADDRINT arg5,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineSyscall
      (syscallNumber, arg0, arg1, arg2, arg3, arg4, arg5, insAssembly);
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

VOID analysisRoutineDstLargeRegSrcImd (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, const PIN_REGISTER *dstRegVal,
    ADDRINT srcImmediateValue,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcImd
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg,
       edu::sharif::twinner::trace::cv::ConcreteValue128Bits (*dstRegVal),
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (srcImmediateValue),
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

VOID analysisRoutineDstMemSrcRegAuxImd (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    UINT32 srcReg, ADDRINT srcRegVal,
    ADDRINT auxImmediateValue,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstMemSrcRegAuxImd
      (ise->convertOpcodeToDoubleSourcesAnalysisRoutine ((OPCODE) opcode),
       dstMemoryEa,
       (REG) srcReg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (srcRegVal),
       edu::sharif::twinner::trace::cv::ConcreteValue64Bits (auxImmediateValue),
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
  if (opcode == XED_ICLASS_RET_NEAR || opcode == XED_ICLASS_RET_FAR) {
#ifdef TARGET_IA32E
    ise->analysisRoutineBeforeRet (REG_RIP);
#else
    ise->analysisRoutineBeforeRet (REG_EIP);
#endif
  }
}

VOID analysisRoutineBeforeChangeOfRegWithArg (VOID *iseptr, UINT32 opcode,
    UINT32 reg, ADDRINT argImmediateValue,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineBeforeChangeOfRegWithArg
      (ise->convertOpcodeToSuddenlyChangedRegWithArgAnalysisRoutine ((OPCODE) opcode),
       (REG) reg, argImmediateValue,
       insAssembly);
  if (opcode == XED_ICLASS_RET_NEAR || opcode == XED_ICLASS_RET_FAR) {
#ifdef TARGET_IA32E
    ise->analysisRoutineBeforeRet (REG_RIP);
#else
    ise->analysisRoutineBeforeRet (REG_EIP);
#endif
  }
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
  ise->analysisRoutineTwoDstRegOneSrcMem
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
