//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2014  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "InstructionSymbolicExecuter.h"


#include "RegisterResidentExpressionValueProxy.h"
#include "MemoryResidentExpressionValueProxy.h"
#include "ConstantExpressionValueProxy.h"

#include "edu/sharif/twinner/trace-twintool/TraceImp.h"
#include "edu/sharif/twinner/trace-twintool/ExpressionImp.h"
#include "edu/sharif/twinner/trace/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/ConcreteValue128Bits.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/Syscall.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/memory.h"
#include "edu/sharif/twinner/util/MemoryManager.h"
#include "edu/sharif/twinner/trace/WrongStateException.h"

#include <stdexcept>

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

InstructionSymbolicExecuter::InstructionSymbolicExecuter (
    std::ifstream &symbolsFileInputStream, bool _disabled) :
    trace (new edu::sharif::twinner::trace::TraceImp (symbolsFileInputStream)),
    trackedReg (REG_INVALID_), operandSize (-1), hook (0),
    disabled (_disabled) {
}

InstructionSymbolicExecuter::InstructionSymbolicExecuter (bool _disabled) :
    trace (new edu::sharif::twinner::trace::TraceImp ()),
    trackedReg (REG_INVALID_), operandSize (-1), hook (0),
    disabled (_disabled) {
}

edu::sharif::twinner::trace::Trace *InstructionSymbolicExecuter::getTrace () const {
  return trace;
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
  trace->syscallInvoked (s);
}

void InstructionSymbolicExecuter::syscallReturned (CONTEXT *context) const {
  if (disabled) {
    return;
  }
  trace->syscallReturned (context);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcReg (AnalysisRoutine routine,
    REG dstReg, const ConcreteValue &dstRegVal,
    REG srcReg, const ConcreteValue &srcRegVal,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
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
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  if (insAssemblyStr) {
    logger << "analysisRoutineDstMemSrcImd(INS: "
        << insAssemblyStr << ")";
  } else {
    disassembledInstruction = 0;
    logger << "analysisRoutineDstMemSrcImd()";
  }
  const UINT32 maxReadSizeInBytes = srcImmediateValue.getSize () / 8;
  if (memReadBytes > maxReadSizeInBytes) {
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

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcMem (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
  const char *insAssemblyStr =
      trace->getMemoryManager ()->getPointerToAllocatedMemory (insAssembly);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << std::hex << "analysisRoutineDstMemSrcMem(INS: "
      << insAssemblyStr << "): dst mem addr: 0x" << dstMemoryEa
      << ", src mem addr: 0x" << srcMemoryEa << ", mem read bytes: 0x" << memReadBytes
      << '\n';
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa),
      MemoryResidentExpressionValueProxy (srcMemoryEa, (int) memReadBytes));
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

void InstructionSymbolicExecuter::analysisRoutineAfterOperandLessInstruction (
    OperandLessAnalysisRoutine routine,
    const CONTEXT *context,
    UINT32 insAssembly) {
  if (disabled) {
    return;
  }
  disassembledInstruction = insAssembly;
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

void InstructionSymbolicExecuter::runHooks (const CONTEXT *context) {
  if (trackedReg != REG_INVALID_) {
    ConcreteValue *value =
        edu::sharif::twinner::util::readRegisterContent (context, trackedReg);
    (this->*hook) (context, *value);
    delete value;
    trackedReg = REG_INVALID_;

  } else if (operandSize > 0) {
    (this->*hook) (context,
        edu::sharif::twinner::trace::ConcreteValue64Bits (operandSize));
    operandSize = -1;
  }
  hook = 0;
}

void InstructionSymbolicExecuter::cmovbeAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "cmovbeAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool belowOrEqual;
  edu::sharif::twinner::trace::Constraint *cc =
      eflags.instantiateConstraintForBelowOrEqualCase
      (belowOrEqual, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraint (cc);
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
  edu::sharif::twinner::util::Logger::loquacious () << "cmovnbeAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool belowOrEqual;
  edu::sharif::twinner::trace::Constraint *cc =
      eflags.instantiateConstraintForBelowOrEqualCase
      (belowOrEqual, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraint (cc);
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
  edu::sharif::twinner::util::Logger::loquacious () << "cmpxchgAnalysisRoutine(...)\n"
      << "\tcomparison part...";
  cmpAnalysisRoutine (aux, dst);
  edu::sharif::twinner::util::Logger::loquacious () << "\texchange part...";
  bool zero;
  edu::sharif::twinner::trace::Constraint *cc =
      eflags.instantiateConstraintForZeroCase (zero, disassembledInstruction);
  if (zero) { // equal
    movAnalysisRoutine (dst, src);
  } else {
    movAnalysisRoutine (aux, dst);
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraint (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::pshufdAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src,
    const ExpressionValueProxy &order) {
  edu::sharif::twinner::util::Logger::loquacious () << "pshufdAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp = src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting order byte...";
  const edu::sharif::twinner::trace::Expression *orderexp = order.getExpression (trace);

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
      (new edu::sharif::twinner::trace::ConcreteValue128Bits ());
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
  dst.setExpression (trace, res);
  delete res;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::xchgAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const MutableExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "xchgAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp = src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp = dst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  dst.setExpression (trace, srcexp);
  delete srcexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting src exp...";
  src.setExpression (trace, dstexp);
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::movAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "movAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp = src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  dst.setExpression (trace, srcexp);
  delete srcexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::movsxAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "movsxAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp = src.getExpression (trace);
  // src is either reg or mem. So src is mutable
  const int size = static_cast<const MutableExpressionValueProxy &> (src).getSize ();
  // size is at most 32 bits
  const bool isNegative = srcexp->getLastConcreteValue ().isNegative ();
  edu::sharif::twinner::trace::Expression *conditionExp = srcexp->clone ();
  conditionExp->minus (1ull << (size - 1));
  edu::sharif::twinner::trace::Expression *signExtendedExp =
      srcexp->clone (dst.getSize ());
  delete srcexp;
  edu::sharif::twinner::trace::Constraint *cc;
  if (isNegative) {
    edu::sharif::twinner::util::Logger::loquacious () << "\tdummy negative condition...";
    cc = new edu::sharif::twinner::trace::Constraint
        (conditionExp, edu::sharif::twinner::trace::Constraint::NON_NEGATIVE,
         disassembledInstruction);
    edu::sharif::twinner::util::Logger::loquacious () << "\tbinary operations...";
    signExtendedExp->minus (1ull << size);
  } else {
    edu::sharif::twinner::util::Logger::loquacious () << "\tdummy positive condition...";
    cc = new edu::sharif::twinner::trace::Constraint
        (conditionExp, edu::sharif::twinner::trace::Constraint::NEGATIVE,
         disassembledInstruction);
  }
  delete conditionExp; // this is cloned by cc and is not required anymore
  trace->addPathConstraint (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  dst.setExpression (trace, signExtendedExp);
  delete signExtendedExp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::pushAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "pushAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting dst exp...";
  dst.setExpression (trace, srcexp);
  delete srcexp;
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tadjusting rsp...";
  //TODO: check for current value of RSP too
  edu::sharif::twinner::trace::Expression *rsp =
      trace->tryToGetSymbolicExpressionByRegister (64, REG_RSP);
  if (rsp) { // If we are not tracking RSP yet, it's not required to adjust its value
    // When check for current value of RSP get added, this condition can be removed.
    rsp->minus (8);
  }
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::popAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "popAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting dst exp...";
  dst.setExpression (trace, srcexp);
  delete srcexp;
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tadjusting rsp...";
  //TODO: check for current value of RSP too
  edu::sharif::twinner::trace::Expression *rsp =
      trace->tryToGetSymbolicExpressionByRegister (64, REG_RSP);
  if (rsp) { // If we are not tracking RSP yet, it's not required to adjust its value
    // When check for current value of RSP get added, this condition can be removed.
    rsp->add (8);
  }
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::addAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "addAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tbinary operation...";
  dstexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::ADD), srcexp);
  delete srcexp;
  dst.setExpression (trace, dstexp);
  eflags.setFlags (dstexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::subAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "subAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tbinary operation...";
  dstexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::MINUS), srcexp);
  delete srcexp;
  dst.setExpression (trace, dstexp);
  eflags.setFlags (dstexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::cmpAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "cmpAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp = src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp = dst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting EFLAGS...";
  eflags.setFlags (dstexp, srcexp); // dstexp - srcexp
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::leaAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "leaAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting dst exp...";
  dst.setExpression (trace, srcexp);
  delete srcexp;
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::jnzAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::util::Logger::loquacious () << "jnzAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool zero;
  edu::sharif::twinner::trace::Constraint *cc =
      eflags.instantiateConstraintForZeroCase (zero, disassembledInstruction);
  if (zero == branchTaken) {
    throw std::runtime_error ("JNZ branching and last known EFLAGS state do not match");
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraint (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jzAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::util::Logger::loquacious () << "jzAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool zero;
  edu::sharif::twinner::trace::Constraint *cc =
      eflags.instantiateConstraintForZeroCase (zero, disassembledInstruction);
  if (zero != branchTaken) {
    throw std::runtime_error ("JZ branching and last known EFLAGS state do not match");
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraint (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jleAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::util::Logger::loquacious () << "jleAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool lessOrEqual;
  edu::sharif::twinner::trace::Constraint *cc =
      eflags.instantiateConstraintForLessOrEqualCase
      (lessOrEqual, disassembledInstruction);
  if (lessOrEqual != branchTaken) {
    throw std::runtime_error ("JLE branching and last known EFLAGS state do not match");
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraint (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jnleAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::util::Logger::loquacious () << "jnleAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool lessOrEqual;
  edu::sharif::twinner::trace::Constraint *cc =
      eflags.instantiateConstraintForLessOrEqualCase
      (lessOrEqual, disassembledInstruction);
  if (lessOrEqual == branchTaken) {
    throw std::runtime_error ("JNLE branching and last known EFLAGS state do not match");
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraint (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jnlAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::util::Logger::loquacious () << "jnlAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool less;
  edu::sharif::twinner::trace::Constraint *cc =
      eflags.instantiateConstraintForLessCase (less, disassembledInstruction);
  if (less == branchTaken) {
    throw std::runtime_error ("JNL branching and last known EFLAGS state do not match");
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraint (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jbeAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::util::Logger::loquacious () << "jbeAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool belowOrEqual;
  edu::sharif::twinner::trace::Constraint *cc =
      eflags.instantiateConstraintForBelowOrEqualCase
      (belowOrEqual, disassembledInstruction);
  if (belowOrEqual != branchTaken) {
    throw std::runtime_error ("JBE branching and last known EFLAGS state do not match");
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraint (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jnbeAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::util::Logger::loquacious () << "jnbeAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool belowOrEqual;
  edu::sharif::twinner::trace::Constraint *cc =
      eflags.instantiateConstraintForBelowOrEqualCase
      (belowOrEqual, disassembledInstruction);
  if (belowOrEqual == branchTaken) {
    throw std::runtime_error ("JNBE branching and last known EFLAGS state do not match");
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraint (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jnbAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::util::Logger::loquacious () << "jnbAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool below;
  edu::sharif::twinner::trace::Constraint *cc =
      eflags.instantiateConstraintForBelowCase (below, disassembledInstruction);
  if (below == branchTaken) {
    throw std::runtime_error ("JNB branching and last known EFLAGS state do not match");
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraint (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::jsAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::util::Logger::loquacious () << "jsAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool sign;
  edu::sharif::twinner::trace::Constraint *cc =
      eflags.instantiateConstraintForSignCase (sign, disassembledInstruction);
  if (sign != branchTaken) {
    throw std::runtime_error ("JS branching and last known EFLAGS state do not match");
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraint (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::callAnalysisRoutine (const CONTEXT *context,
    const ConcreteValue &rspRegVal) {
  edu::sharif::twinner::util::Logger::loquacious () << "callAnalysisRoutine(...)\n"
      << "\tgetting rsp reg exp...";
  edu::sharif::twinner::trace::Expression *rsp =
      trace->tryToGetSymbolicExpressionByRegister (64, REG_RSP);
  if (rsp) { // If we are not tracking RSP yet, it's not required to adjust its value
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tadjusting rsp...";
    const ConcreteValue &oldVal = rsp->getLastConcreteValue ();
    if (oldVal > rspRegVal) {
      // some items have been pushed into stack by CALL and so RSP is decremented
      ConcreteValue *cv = oldVal.clone ();
      (*cv) -= rspRegVal;
      if ((*cv) == 8) {
        edu::sharif::twinner::util::Logger::loquacious ()
            << "\tupdating stack (pushing the ret address)...";
        MemoryResidentExpressionValueProxy stack (rspRegVal.toUint64 (), 8);
        try {
          edu::sharif::twinner::trace::Expression *exp = stack.getExpression (trace);
          delete exp;

        } catch (const edu::sharif::twinner::trace::WrongStateException &e) {
          const edu::sharif::twinner::trace::Expression *exp =
              new edu::sharif::twinner::trace::ExpressionImp (e.getExpectedStateValue ());
          stack.setExpression (trace, exp);
          delete exp;
        }
      } else {
        edu::sharif::twinner::util::Logger::warning ()
            << "CALL decremented RSP more/less than 8 bytes;"
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
  edu::sharif::twinner::util::Logger::loquacious () << "retAnalysisRoutine(...)\n"
      << "\tgetting rsp reg exp...";
  edu::sharif::twinner::trace::Expression *rsp =
      trace->tryToGetSymbolicExpressionByRegister (64, REG_RSP);
  if (rsp) { // If we are not tracking RSP yet, it's not required to adjust its value
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tadjusting rsp...";
    const ConcreteValue &oldVal = rsp->getLastConcreteValue ();
    if (oldVal < rspRegVal) {
      // some items have been popped out from the stack by RET and so RSP is incremented
      ConcreteValue *cv = rspRegVal.clone ();
      (*cv) -= oldVal;
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
  edu::sharif::twinner::util::Logger::loquacious () << "jmpAnalysisRoutine(...)\n"
      << "\tgetting rsp reg exp...";
  edu::sharif::twinner::trace::Expression *rsp =
      trace->tryToGetSymbolicExpressionByRegister (64, REG_RSP);
  if (rsp) { // If we are not tracking RSP yet, it's not required to adjust its value
    const ConcreteValue &oldVal = rsp->getLastConcreteValue ();
    if (oldVal != rspRegVal) { // This jump had side-effect on RSP
      edu::sharif::twinner::util::Logger::debug () << "\tadjusting rsp...";
      if (oldVal < rspRegVal) {
        ConcreteValue *cv = rspRegVal.clone ();
        (*cv) -= oldVal;
        rsp->add (cv); // some items have been popped out of stack and RSP is incremented
      } else { // oldVal > rspRegVal
        ConcreteValue *cv = oldVal.clone ();
        (*cv) -= rspRegVal;
        rsp->minus (cv); // some items have been pushed into stack and RSP is decremented
      }
      // TODO: call valueIsChanged from an expression proxy to address ESP, SP, and SPL
    }
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::repAnalysisRoutine (
    const MutableExpressionValueProxy &dst, bool executing, bool repEqual) {
  edu::sharif::twinner::util::Logger::loquacious () << "repAnalysisRoutine(...)\n"
      << "\tgetting dst (rep) reg exp...";
  edu::sharif::twinner::trace::Expression *dstexp = dst.getExpression (trace);
  bool zero;
  edu::sharif::twinner::trace::Constraint *cc =
      edu::sharif::twinner::trace::Constraint::instantiateEqualConstraint
      (zero, dstexp, 0, disassembledInstruction);
  if (zero == executing) {
    throw std::runtime_error ("REP count and executing state do not match");
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraint (cc);
  if (executing) {
    edu::sharif::twinner::util::Logger::loquacious () << "\tchecking eflags...";
    cc = eflags.instantiateConstraintForZeroCase (zero, disassembledInstruction);
    // if zero == repEqual then predicated instruction will be executed once again
    trace->addPathConstraint (cc);
    edu::sharif::twinner::util::Logger::loquacious () << "\tdecrementing count reg...";
    dstexp->minus (1);
    dst.setExpression (trace, dstexp);
  }
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::shlAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "shlAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tshifting operation...";
  if (dynamic_cast<const ConstantExpressionValueProxy *> (&src) != 0) {
    // src was an immediate value
    dstexp->shiftToLeft (srcexp->getLastConcreteValue ().clone ());
  } else {
    // src was CL register
    dstexp->shiftToLeft (srcexp);
  }
  delete srcexp;
  // truncate bits which are shifted left, outside of dst boundaries
  dst.truncate (dstexp);
  dst.setExpression (trace, dstexp);
  eflags.setFlags (dstexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::shrAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "shrAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tshifting operation...";
  dstexp->shiftToRight (srcexp);
  delete srcexp;
  dst.setExpression (trace, dstexp);
  eflags.setFlags (dstexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::sarAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "sarAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);
  const int size = dst.getSize ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tshifting operation...";
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
  delete srcexp;
  dst.setExpression (trace, dstexp);
  eflags.setFlags (dstexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::andAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "andAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tbinary operation...";
  dstexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::BITWISE_AND), srcexp);
  delete srcexp;
  dst.setExpression (trace, dstexp);
  eflags.setFlags (dstexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::orAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "orAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tbinary operation...";
  dstexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::BITWISE_OR), srcexp);
  delete srcexp;
  dst.setExpression (trace, dstexp);
  eflags.setFlags (dstexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::xorAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "xorAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tbinary operation...";
  dstexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::XOR), srcexp);
  delete srcexp;
  dst.setExpression (trace, dstexp);
  eflags.setFlags (dstexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::cdqeAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "cdqeAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp = src.getExpression (trace);
  // src is reg; which is mutable
  const int size = static_cast<const MutableExpressionValueProxy &> (src).getSize ();
  // size is supposed to be 32 bits
  const bool isNegative = srcexp->getLastConcreteValue ().isNegative ();
  edu::sharif::twinner::trace::Expression *conditionExp = srcexp->clone ();
  conditionExp->minus (1ull << (size - 1));
  edu::sharif::twinner::trace::Expression *signExtendedExp =
      srcexp->clone (dst.getSize ());
  delete srcexp;
  edu::sharif::twinner::trace::Constraint *cc;
  if (isNegative) {
    edu::sharif::twinner::util::Logger::loquacious () << "\tdummy negative condition...";
    cc = new edu::sharif::twinner::trace::Constraint
        (conditionExp, edu::sharif::twinner::trace::Constraint::NON_NEGATIVE,
         disassembledInstruction);
    edu::sharif::twinner::util::Logger::loquacious () << "\tbinary operations...";
    signExtendedExp->minus (1ull << size);
  } else {
    edu::sharif::twinner::util::Logger::loquacious () << "\tdummy positive condition...";
    cc = new edu::sharif::twinner::trace::Constraint
        (conditionExp, edu::sharif::twinner::trace::Constraint::NEGATIVE,
         disassembledInstruction);
  }
  delete conditionExp; // this is cloned by cc and is not required anymore
  trace->addPathConstraint (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  dst.setExpression (trace, signExtendedExp);
  delete signExtendedExp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::testAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "testAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);

  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tbinary operation...";
  edu::sharif::twinner::trace::Expression *tmpexp = dstexp;
  tmpexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::BITWISE_AND), srcexp);
  delete srcexp;
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting EFLAGS...";
  eflags.setFlags (tmpexp);
  // TODO: Inform eflags about CF and OF which must be zero.
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::pmovmskbAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "pmovmskbAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp = src.getExpression (trace);
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
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting dst exp...";
  dst.setExpression (trace, maskbyte);
  delete maskbyte;
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::pcmpeqbAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "pcmpeqbAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp = src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexp = dst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tcomparing...";
  const int size = dst.getSize ();
  ConcreteValue *result = dstexp->getLastConcreteValue ().clone ();
  *result = 0;
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
    ConcreteValue *c = dstexp->getLastConcreteValue ().clone ();
    if (ithByteSrc->getLastConcreteValue ().isZero ()) { // equal
      trace->addPathConstraint
          (new edu::sharif::twinner::trace::Constraint
           (ithByteSrc, edu::sharif::twinner::trace::Constraint::ZERO,
            disassembledInstruction));
      *c = 0xFF;
    } else { // non-equal
      trace->addPathConstraint
          (new edu::sharif::twinner::trace::Constraint
           (ithByteSrc, edu::sharif::twinner::trace::Constraint::NON_ZERO,
            disassembledInstruction));
      *c = 0x00;
    }
    delete ithByteSrc;
    delete ithByteDst;
    (*c) <<= i;
    (*result) |= (*c);
    delete c;
  }
  delete srcexp;
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting dst exp...";
  const edu::sharif::twinner::trace::Expression *resexp =
      new edu::sharif::twinner::trace::ExpressionImp (result);
  dst.setExpression (trace, resexp);
  delete resexp;
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::pminubAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "pminubAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp = src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp = dst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tcalculating the minimum...";
  const int size = dst.getSize ();
  ConcreteValue *mask = dstexp->getLastConcreteValue ().clone ();
  *mask = 0;
  for (int i = 0; i < size; i += 8) {
    edu::sharif::twinner::trace::Expression *ithByteSrc = srcexp->clone ();
    edu::sharif::twinner::trace::Expression *ithByteDst = dstexp->clone ();
    ithByteSrc->shiftToRight (i);
    ithByteSrc->bitwiseAnd (0xFF);
    ithByteDst->shiftToRight (i);
    ithByteDst->bitwiseAnd (0xFF);
    bool below;
    edu::sharif::twinner::trace::Constraint *cc =
        edu::sharif::twinner::trace::Constraint::instantiateBelowConstraint
        (below, ithByteSrc, ithByteDst, disassembledInstruction);
    trace->addPathConstraint (cc);
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
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\ttransferring (mask & src) to (dst) for mask=" << (*mask);
  dstexp->bitwiseAnd (mask->bitwiseNegated ());
  srcexp->bitwiseAnd (mask);
  dstexp->bitwiseOr (srcexp);
  delete srcexp;
  dst.setExpression (trace, dstexp);
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::punpcklbwAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "punpcklbwAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp = src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexp = dst.getExpression (trace);
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
  dst.setExpression (trace, res);
  delete res;
  delete srcexp;
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::punpcklwdAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "punpcklwdAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp = src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexp = dst.getExpression (trace);
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
  dst.setExpression (trace, res);
  delete res;
  delete srcexp;
  delete dstexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::bsfAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "bsfAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  const edu::sharif::twinner::trace::ConcreteValue &cv = srcexp->getLastConcreteValue ();
  UINT64 i = 0;
  for (unsigned int s = cv.getSize (); i < s; ++i) {
    edu::sharif::twinner::trace::ConcreteValue *bit = cv.clone ();
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
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting dst exp...";
  dst.setExpression (trace, indexexp);
  delete indexexp;
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tadding constraint...";
  edu::sharif::twinner::trace::Expression *conditionExp = srcexp;
  edu::sharif::twinner::trace::ConcreteValue *bit = cv.clone ();
  (*bit) = 1;
  (*bit) <<= i;
  conditionExp->truncate (i + 1);
  conditionExp->minus (bit); // takes ownership of bit
  edu::sharif::twinner::trace::Constraint *cc =
      new edu::sharif::twinner::trace::Constraint
      (conditionExp, edu::sharif::twinner::trace::Constraint::ZERO,
       disassembledInstruction);
  delete conditionExp;
  trace->addPathConstraint (cc);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::divAnalysisRoutine (
    const MutableExpressionValueProxy &leftDst,
    const MutableExpressionValueProxy &rightDst,
    const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "divAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp = src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting left dst exp...";
  edu::sharif::twinner::trace::Expression *leftDstExp = leftDst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting right dst exp...";
  edu::sharif::twinner::trace::Expression *rightDstExp = rightDst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tpreparing left-right in both dst regs...";
  operandSize = leftDst.getSize ();
  leftDstExp->shiftToLeft (operandSize);
  leftDstExp->bitwiseOr (rightDstExp);
  delete rightDstExp;
  rightDstExp = leftDstExp->clone ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tcalculating quotient (right) and remainder (left) of division...";
  leftDstExp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::REMAINDER), srcexp);
  rightDstExp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::DIVIDE), srcexp);
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
  edu::sharif::twinner::util::Logger::loquacious ()
      << "adjustDivisionMultiplicationOperands(...) hook...";
  const edu::sharif::twinner::trace::ConcreteValue64Bits os = operandSize;
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
  case 64:
    leftReg = REG_RDX;
    rightReg = REG_RAX;
    break;
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "adjustDivisionMultiplicationOperands(...) hook: "
        "unsupported operand size: " << operandSize << '\n';
    throw std::runtime_error
        ("Unsupported operand size in division/multiplication instruction");
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
    leftExp->binaryOperation
        (new edu::sharif::twinner::trace::Operator
         (edu::sharif::twinner::trace::Operator::BITWISE_OR), rightExp);
    const MutableExpressionValueProxy &ax = RegisterResidentExpressionValueProxy (REG_AX);
    ax.setExpression (trace, leftExp); // this deletes unused expressions by itself
  } else {
    const MutableExpressionValueProxy &left =
        RegisterResidentExpressionValueProxy (leftReg, *leftVal);
    const MutableExpressionValueProxy &right =
        RegisterResidentExpressionValueProxy (rightReg, *rightVal);
    left.valueIsChanged (trace, *leftExp);
    right.valueIsChanged (trace, *rightExp);
  }
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\toverlapping registers are updated.\n";
}

void InstructionSymbolicExecuter::mulAnalysisRoutine (
    const MutableExpressionValueProxy &leftDst,
    const MutableExpressionValueProxy &rightDst,
    const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "mulAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp = src.getExpression (trace);
  edu::sharif::twinner::trace::Expression *leftDstExp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tgetting right dst exp...";
  edu::sharif::twinner::trace::Expression *rightDstExp = rightDst.getExpression (trace);
  operandSize = leftDst.getSize ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tmultiplying (left-right = right * src; size=0x"
      << std::hex << operandSize << ")...";
  rightDstExp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::MULTIPLY), srcexp);
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
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::scasAnalysisRoutine (
    const MutableExpressionValueProxy &dstReg,
    const MutableExpressionValueProxy &srcReg,
    const ExpressionValueProxy &srcMem) {
  edu::sharif::twinner::util::Logger::loquacious () << "scasAnalysisRoutine(...)\n";
  cmpAnalysisRoutine (dstReg, srcMem); // comparing AL/AX/EAX/RAX with memory
  edu::sharif::twinner::trace::Expression *exp = srcReg.getExpression (trace);
  if (eflags.getDirectionFlag ()) { // DF == 1
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tdecrementing index register...";
    exp->minus (dstReg.getSize () / 8);
  } else { // DF == 0
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tincrementing index register...";
    exp->add (dstReg.getSize () / 8);
  }
  srcReg.setExpression (trace, exp);
  delete exp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::leaveAnalysisRoutine (
    const MutableExpressionValueProxy &fpReg,
    const MutableExpressionValueProxy &spReg,
    const ExpressionValueProxy &srcMem) {
  edu::sharif::twinner::util::Logger::loquacious () << "leaveAnalysisRoutine(...)\n"
      << "\tgetting frame pointer (to be set in stack pointer)...";
  edu::sharif::twinner::trace::Expression *rsp = fpReg.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tpopping frame pointer...";
  const edu::sharif::twinner::trace::Expression *memexp = srcMem.getExpression (trace);
  fpReg.setExpression (trace, memexp);
  delete memexp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tadjusting rsp...";
  rsp->add (8);
  spReg.setExpression (trace, rsp);
  delete rsp;
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::rdtscAnalysisRoutine (const CONTEXT *context) {
  edu::sharif::twinner::util::Logger::loquacious () << "rdtscAnalysisRoutine(...)\n";
  /**
   * Now, we are right after the RDTSC instruction and time-stamp is loaded in
   * the edx:eax registers. These registers should be loaded as immediate values
   * in symbolic expressions.
   */
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
  edx.setExpression (trace, edxNewExp);
  eax.setExpression (trace, eaxNewExp);
  delete edxNewExp; // expressions are cloned by above setter methods
  delete eaxNewExp;
}

void InstructionSymbolicExecuter::incAnalysisRoutine (
    const MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::util::Logger::loquacious () << "incAnalysisRoutine(...)\n"
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      opr.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tincrementing...";
  dstexp->add (1);
  opr.setExpression (trace, dstexp);
  eflags.setFlags (dstexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::decAnalysisRoutine (
    const MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::util::Logger::loquacious () << "decAnalysisRoutine(...)\n"
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp = opr.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdecrementing...";
  dstexp->minus (1);
  opr.setExpression (trace, dstexp);
  eflags.setFlags (dstexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::setnzAnalysisRoutine (
    const MutableExpressionValueProxy &opr) {
  edu::sharif::twinner::util::Logger::loquacious () << "setnzAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  bool zero;
  edu::sharif::twinner::trace::Constraint *cc =
      eflags.instantiateConstraintForZeroCase (zero, disassembledInstruction);
  edu::sharif::twinner::util::Logger::loquacious () << "\tadding constraint...";
  trace->addPathConstraint (cc);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp;
  if (!zero) {
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (1));
  } else { // shouldSetToZero
    dstexp = new edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
  }
  opr.setExpression (trace, dstexp);
  delete dstexp;
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
    return &InstructionSymbolicExecuter::movAnalysisRoutine;
  case XED_ICLASS_MOVSX:
  case XED_ICLASS_MOVSXD:
    return &InstructionSymbolicExecuter::movsxAnalysisRoutine;
  case XED_ICLASS_CMOVBE:
    return &InstructionSymbolicExecuter::cmovbeAnalysisRoutine;
  case XED_ICLASS_CMOVNBE:
    return &InstructionSymbolicExecuter::cmovnbeAnalysisRoutine;
  case XED_ICLASS_PUSH:
    return &InstructionSymbolicExecuter::pushAnalysisRoutine;
  case XED_ICLASS_POP:
    return &InstructionSymbolicExecuter::popAnalysisRoutine;
  case XED_ICLASS_ADD:
    return &InstructionSymbolicExecuter::addAnalysisRoutine;
  case XED_ICLASS_SUB:
    return &InstructionSymbolicExecuter::subAnalysisRoutine;
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
  case XED_ICLASS_AND:
    return &InstructionSymbolicExecuter::andAnalysisRoutine;
  case XED_ICLASS_OR:
  case XED_ICLASS_POR:
    return &InstructionSymbolicExecuter::orAnalysisRoutine;
  case XED_ICLASS_PXOR:
  case XED_ICLASS_XOR:
    return &InstructionSymbolicExecuter::xorAnalysisRoutine;
  case XED_ICLASS_CDQE:
    return &InstructionSymbolicExecuter::cdqeAnalysisRoutine;
  case XED_ICLASS_TEST:
    return &InstructionSymbolicExecuter::testAnalysisRoutine;
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
    throw std::runtime_error ("Unknown opcode given to analysis routine");
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
    throw std::runtime_error
        ("Unknown opcode (for mutable source case) given to analysis routine");
  }
}

InstructionSymbolicExecuter::AuxOperandHavingAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToAuxOperandHavingAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_CMPXCHG:
    return &InstructionSymbolicExecuter::cmpxchgAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Having an auxiliary third (dst) operand: Unknown opcode: "
        << OPCODE_StringShort (op) << '\n';
    throw std::runtime_error
        ("Unknown opcode (for aux-operand case) given to analysis routine");
  }
}

InstructionSymbolicExecuter::DoubleDestinationsAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToDoubleDestinationsAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_DIV:
    return &InstructionSymbolicExecuter::divAnalysisRoutine;
  case XED_ICLASS_MUL:
    return &InstructionSymbolicExecuter::mulAnalysisRoutine;
  case XED_ICLASS_SCASB:
    return &InstructionSymbolicExecuter::scasAnalysisRoutine;
  case XED_ICLASS_LEAVE:
    return &InstructionSymbolicExecuter::leaveAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Double Destinations: Unknown opcode: " << OPCODE_StringShort (op) << '\n';
    throw std::runtime_error
        ("Unknown opcode (for double dests) given to analysis routine");
  }
}

InstructionSymbolicExecuter::DoubleSourcesAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToDoubleSourcesAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_PSHUFD:
    return &InstructionSymbolicExecuter::pshufdAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Double Sources: Unknown opcode: " << OPCODE_StringShort (op) << '\n';
    throw std::runtime_error
        ("Unknown opcode (for double sources) given to analysis routine");
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
  case XED_ICLASS_JNL:
    return &InstructionSymbolicExecuter::jnlAnalysisRoutine;
  case XED_ICLASS_JBE:
    return &InstructionSymbolicExecuter::jbeAnalysisRoutine;
  case XED_ICLASS_JNBE:
    return &InstructionSymbolicExecuter::jnbeAnalysisRoutine;
  case XED_ICLASS_JNB:
    return &InstructionSymbolicExecuter::jnbAnalysisRoutine;
  case XED_ICLASS_JS:
    return &InstructionSymbolicExecuter::jsAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Conditional Branch: Unknown opcode: " << OPCODE_StringShort (op) << '\n';
    throw std::runtime_error ("Unknown opcode (for Jcc) given to analysis routine");
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
    throw std::runtime_error
        ("Unknown opcode (for changedReg) given to analysis routine");
  }
}

InstructionSymbolicExecuter::OperandLessAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToOperandLessAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_RDTSC:
    return &InstructionSymbolicExecuter::rdtscAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Operand Less: Unknown opcode: " << OPCODE_StringShort (op) << '\n';
    throw std::runtime_error
        ("Unknown opcode (for operand-less) given to analysis routine");
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
  case XED_ICLASS_SETNZ:
    return &InstructionSymbolicExecuter::setnzAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Single Operand: Unknown opcode: " << OPCODE_StringShort (op) << '\n';
    throw std::runtime_error
        ("Unknown opcode (for single-operand) given to analysis routine");
  }
}

VOID analysisRoutineDstRegSrcReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcReg
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::ConcreteValue64Bits (dstRegVal),
       (REG) srcReg, edu::sharif::twinner::trace::ConcreteValue64Bits (srcRegVal),
       insAssembly);
}

VOID analysisRoutineDstRegSrcMutableReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcMutableReg
      (ise->convertOpcodeToMutableSourceAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::ConcreteValue64Bits (dstRegVal),
       (REG) srcReg, edu::sharif::twinner::trace::ConcreteValue64Bits (srcRegVal),
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
       (REG) dstReg, edu::sharif::twinner::trace::ConcreteValue64Bits (dstRegVal),
       (REG) srcReg, edu::sharif::twinner::trace::ConcreteValue64Bits (srcRegVal),
       (REG) auxReg, edu::sharif::twinner::trace::ConcreteValue64Bits (auxRegVal),
       insAssembly);
}

VOID analysisRoutineDstRegSrcLargeReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, const PIN_REGISTER *srcRegVal,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcReg
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::ConcreteValue64Bits (dstRegVal),
       (REG) srcReg, edu::sharif::twinner::trace::ConcreteValue128Bits (*srcRegVal),
       insAssembly);
}

VOID analysisRoutineDstLargeRegSrcReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, const PIN_REGISTER *dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcReg
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::ConcreteValue128Bits (*dstRegVal),
       (REG) srcReg, edu::sharif::twinner::trace::ConcreteValue64Bits (srcRegVal),
       insAssembly);
}

VOID analysisRoutineDstLargeRegSrcLargeReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, const PIN_REGISTER *dstRegVal,
    UINT32 srcReg, const PIN_REGISTER *srcRegVal,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcReg
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::ConcreteValue128Bits (*dstRegVal),
       (REG) srcReg, edu::sharif::twinner::trace::ConcreteValue128Bits (*srcRegVal),
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
       (REG) dstReg, edu::sharif::twinner::trace::ConcreteValue128Bits (*dstRegVal),
       (REG) srcReg, edu::sharif::twinner::trace::ConcreteValue128Bits (*srcRegVal),
       edu::sharif::twinner::trace::ConcreteValue64Bits (auxImmediateValue),
       insAssembly);
}

VOID analysisRoutineDstLargeRegSrcMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, const PIN_REGISTER *dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcMem
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::ConcreteValue128Bits (*dstRegVal),
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
       (REG) dstReg, edu::sharif::twinner::trace::ConcreteValue128Bits (*dstRegVal),
       srcMemoryEa, memReadBytes,
       edu::sharif::twinner::trace::ConcreteValue64Bits (auxImmediateValue),
       insAssembly);
}

VOID analysisRoutineDstRegSrcMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcMem
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::ConcreteValue64Bits (dstRegVal),
       srcMemoryEa, memReadBytes,
       insAssembly);
}

VOID analysisRoutineDstRegSrcImd (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcImmediateValue,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcImd
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::ConcreteValue64Bits (dstRegVal),
       edu::sharif::twinner::trace::ConcreteValue64Bits (srcImmediateValue),
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
       (REG) srcReg, edu::sharif::twinner::trace::ConcreteValue64Bits (srcRegVal),
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
       (REG) srcReg, edu::sharif::twinner::trace::ConcreteValue64Bits (srcRegVal),
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
       (REG) srcReg, edu::sharif::twinner::trace::ConcreteValue64Bits (srcRegVal),
       (REG) auxReg, edu::sharif::twinner::trace::ConcreteValue64Bits (auxRegVal),
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
       (REG) srcReg, edu::sharif::twinner::trace::ConcreteValue128Bits (*srcRegVal),
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
       edu::sharif::twinner::trace::ConcreteValue64Bits (srcImmediateValue),
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
       (REG) dstReg, edu::sharif::twinner::trace::ConcreteValue64Bits (dstRegVal),
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
       edu::sharif::twinner::trace::ConcreteValue64Bits (dstLeftRegVal),
       (REG) dstRightReg,
       edu::sharif::twinner::trace::ConcreteValue64Bits (dstRightRegVal),
       (REG) srcReg,
       edu::sharif::twinner::trace::ConcreteValue64Bits (srcRegVal),
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
       edu::sharif::twinner::trace::ConcreteValue64Bits (dstLeftRegVal),
       (REG) dstRightReg,
       edu::sharif::twinner::trace::ConcreteValue64Bits (dstRightRegVal),
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
       (REG) dstReg, edu::sharif::twinner::trace::ConcreteValue64Bits (dstRegVal),
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
       edu::sharif::twinner::trace::ConcreteValue64Bits (dstRegVal),
       (REG) srcReg,
       edu::sharif::twinner::trace::ConcreteValue64Bits (srcRegVal),
       srcMemoryEa, memReadBytes,
       insAssembly);
}

VOID analysisRoutineRepPrefix (VOID *iseptr, UINT32 opcode,
    UINT32 repReg, ADDRINT repRegVal,
    UINT32 executing, UINT32 repEqual,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineRepEqualOrRepNotEqualPrefix
      ((REG) repReg,
       edu::sharif::twinner::trace::ConcreteValue64Bits (repRegVal),
       executing, repEqual,
       insAssembly);
}

}
}
}
}
