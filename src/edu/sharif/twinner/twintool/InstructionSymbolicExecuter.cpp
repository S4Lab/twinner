//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013  Behnam Momeni
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

#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/Constraint.h"

#include "edu/sharif/twinner/util/Logger.h"

#include <stdexcept>

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

InstructionSymbolicExecuter::InstructionSymbolicExecuter () :
trace (new edu::sharif::twinner::trace::Trace ()) {
}

edu::sharif::twinner::trace::Trace *InstructionSymbolicExecuter::getTrace () const {
  return trace;
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcReg (AnalysisRoutine routine,
    REG dstReg, UINT64 dstRegVal,
    REG srcReg, UINT64 srcRegVal,
    std::string *insAssembly) {
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstRegSrcReg(INS: "
      << *insAssembly << "): dst reg: " << REG_StringShort (dstReg)
      << ", src reg: " << REG_StringShort (srcReg) << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
      RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcMem (AnalysisRoutine routine,
    REG dstReg, UINT64 dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    std::string *insAssembly) {
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstRegSrcMem(INS: "
      << *insAssembly << "): dst reg: " << REG_StringShort (dstReg)
      << ", src mem addr: " << srcMemoryEa << ", mem read bytes: " << memReadBytes
      << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
      MemoryResidentExpressionValueProxy (srcMemoryEa, (int) memReadBytes));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcImd (AnalysisRoutine routine,
    REG dstReg, UINT64 dstRegVal,
    ADDRINT srcImmediateValue,
    std::string *insAssembly) {
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstRegSrcImd(INS: "
      << *insAssembly << "): dst reg: " << REG_StringShort (dstReg)
      << ", src imd: " << srcImmediateValue << '\n';
  edu::sharif::twinner::trace::Expression *srcexp =
      new edu::sharif::twinner::trace::Expression (srcImmediateValue);
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
      ConstantExpressionValueProxy (srcexp));
  delete srcexp;
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcReg (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    REG srcReg, UINT64 srcRegVal,
    UINT32 memReadBytes,
    std::string *insAssembly) {
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstMemSrcReg(INS: "
      << *insAssembly << "): dst mem addr: " << dstMemoryEa
      << ", src reg: " << REG_StringShort (srcReg) << '\n';
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
      RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcImd (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    ADDRINT srcImmediateValue,
    UINT32 memReadBytes,
    std::string *insAssembly) {
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstMemSrcImd(INS: "
      << *insAssembly << "): dst mem addr: " << dstMemoryEa
      << ", src imd: " << srcImmediateValue << '\n';
  edu::sharif::twinner::trace::Expression *srcexp =
      new edu::sharif::twinner::trace::Expression (srcImmediateValue);
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
      ConstantExpressionValueProxy (srcexp));
  delete srcexp;
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcMem (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    std::string *insAssembly) {
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstMemSrcMem(INS: "
      << *insAssembly << "): dst mem addr: " << dstMemoryEa
      << ", src mem addr: " << srcMemoryEa << ", mem read bytes: " << memReadBytes
      << '\n';
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa),
      MemoryResidentExpressionValueProxy (srcMemoryEa, (int) memReadBytes));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineConditionalBranch (
    ConditionalBranchAnalysisRoutine routine,
    BOOL branchTaken,
    std::string *insAssembly) {
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineConditionalBranch(INS: "
      << *insAssembly << "): branch taken: " << branchTaken << '\n';
  (this->*routine) (branchTaken);
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcAdg (AnalysisRoutine routine,
    REG dstReg, UINT64 dstRegVal,
    std::string *insAssembly) {
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstRegSrcAdg(INS: "
      << *insAssembly << ") [AFTER execution of instruction]: dst reg: "
      << REG_StringShort (dstReg) << ", dst reg value: " << dstRegVal << '\n';
  edu::sharif::twinner::trace::Expression *srcexp =
      new edu::sharif::twinner::trace::Expression (dstRegVal);
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
      ConstantExpressionValueProxy (srcexp));
  delete srcexp;
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineWhenCallIsInvoked (UINT64 rspRegVal,
    std::string *insAssembly) {
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineWhenCallIsInvoked(INS: "
      << *insAssembly << ") [AFTER execution of instruction]: rsp reg value: 0x"
      << std::hex << rspRegVal << '\n';
  edu::sharif::twinner::trace::Expression *rsp =
      trace->tryToGetSymbolicExpressionByRegister (REG_RSP);
  if (rsp) { // If we are not tracking RSP yet, it's not required to adjust its value
    UINT64 oldVal = rsp->getLastConcreteValue ();
    if (oldVal > rspRegVal) {
      // some items have been pushed into stack by CALL and so RSP is decremented
      rsp->minus (oldVal - rspRegVal);
      // TODO: call valueIsChanged from an expression proxy to address ESP, SP, and SPL

    } else {
      edu::sharif::twinner::util::Logger::warning ()
          << "RSP is not decremented at all after CALL instruction!\n";
    }
  }
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::movAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "movAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting dst exp...";
  dst.setExpression (trace, srcexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
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
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tadjusting rsp...";
  //TODO: check for current value of RSP too
  edu::sharif::twinner::trace::Expression *rsp =
      trace->tryToGetSymbolicExpressionByRegister (REG_RSP);
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
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tadjusting rsp...";
  //TODO: check for current value of RSP too
  edu::sharif::twinner::trace::Expression *rsp =
      trace->tryToGetSymbolicExpressionByRegister (REG_RSP);
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
  dst.valueIsChanged (trace, dstexp);
  //TODO: set rflags
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
  dst.valueIsChanged (trace, dstexp);
  //TODO: set rflags
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::cmpAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "cmpAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);

  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tbinary operation...";
  edu::sharif::twinner::trace::Expression *tmpexp = dstexp->clone ();
  tmpexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::MINUS), srcexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting EFLAGS...";
  eflags.setFlags (tmpexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
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
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::jnzAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::util::Logger::loquacious () << "jnzAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  edu::sharif::twinner::trace::Constraint *cc
      = new edu::sharif::twinner::trace::Constraint
      (eflags.getFlagsUnderlyingExpression (),
       branchTaken ?
       edu::sharif::twinner::trace::Constraint::NON_ZERO :
       edu::sharif::twinner::trace::Constraint::ZERO);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tadding constraint...";
  trace->addPathConstraint (cc);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

InstructionSymbolicExecuter::AnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToAnalysisRoutine (OPCODE op) const {
  switch (op) {
  case XED_ICLASS_MOV:
  case XED_ICLASS_MOVZX:
  case XED_ICLASS_MOVSX:
    return &InstructionSymbolicExecuter::movAnalysisRoutine;
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
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: Unknown opcode: "
        << OPCODE_StringShort (op) << '\n';
    throw std::runtime_error ("Unknown opcode given to analysis routine");
  }
}

InstructionSymbolicExecuter::ConditionalBranchAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToConditionalBranchAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_JNZ:
    return &InstructionSymbolicExecuter::jnzAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Conditional Branch: Unknown opcode: " << OPCODE_StringShort (op) << '\n';
    throw std::runtime_error ("Unknown opcode (for Jcc) given to analysis routine");
  }
}

UINT64 InstructionSymbolicExecuter::readMemoryContent (ADDRINT memoryEa) {
  UINT64 currentConcreteValue;
  PIN_SafeCopy (&currentConcreteValue, (const VOID *) (memoryEa), sizeof (UINT64));
  return currentConcreteValue;
}

UINT64 InstructionSymbolicExecuter::truncateValue (UINT64 value, int countOfBytes) {
  switch (countOfBytes) {
  case 8:
    break; // no cast is required in 64-bits mode
  case 4:
    value = (UINT32) value;
    break;
  case 2:
    value = (UINT16) value;
    break;
  case 1:
    value = (UINT8) value;
    break;
  default:
    throw std::runtime_error ("Can not truncate value to given count of bytes");
  }
  return value;
}

VOID analysisRoutineDstRegSrcReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    VOID *insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  std::string *insAssemblyStr = (std::string *) insAssembly;
  ise->analysisRoutineDstRegSrcReg (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    (REG) dstReg, dstRegVal,
                                    (REG) srcReg, srcRegVal,
                                    insAssemblyStr);
}

VOID analysisRoutineDstRegSrcMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    VOID *insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  std::string *insAssemblyStr = (std::string *) insAssembly;
  ise->analysisRoutineDstRegSrcMem (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    (REG) dstReg, dstRegVal,
                                    srcMemoryEa, memReadBytes,
                                    insAssemblyStr);
}

VOID analysisRoutineDstRegSrcImd (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcImmediateValue,
    VOID *insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  std::string *insAssemblyStr = (std::string *) insAssembly;
  ise->analysisRoutineDstRegSrcImd (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    (REG) dstReg, dstRegVal,
                                    srcImmediateValue,
                                    insAssemblyStr);
}

VOID analysisRoutineDstMemSrcReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 memReadBytes,
    VOID *insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  std::string *insAssemblyStr = (std::string *) insAssembly;
  ise->analysisRoutineDstMemSrcReg (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    dstMemoryEa,
                                    (REG) srcReg, srcRegVal,
                                    memReadBytes,
                                    insAssemblyStr);
}

VOID analysisRoutineDstMemSrcImd (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcImmediateValue,
    UINT32 memReadBytes,
    VOID *insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  std::string *insAssemblyStr = (std::string *) insAssembly;
  ise->analysisRoutineDstMemSrcImd (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    dstMemoryEa,
                                    srcImmediateValue,
                                    memReadBytes,
                                    insAssemblyStr);
}

VOID analysisRoutineDstMemSrcMem (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    VOID *insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  std::string *insAssemblyStr = (std::string *) insAssembly;
  ise->analysisRoutineDstMemSrcMem (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    dstMemoryEa,
                                    srcMemoryEa, memReadBytes,
                                    insAssemblyStr);
}

VOID analysisRoutineConditionalBranch (VOID *iseptr, UINT32 opcode,
    BOOL branchTaken,
    VOID *insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  std::string *insAssemblyStr = (std::string *) insAssembly;
  ise->analysisRoutineConditionalBranch
      (ise->convertOpcodeToConditionalBranchAnalysisRoutine ((OPCODE) opcode),
       branchTaken,
       insAssemblyStr);
}

VOID analysisRoutineDstRegSrcAdg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    VOID *insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  std::string *insAssemblyStr = (std::string *) insAssembly;
  ise->analysisRoutineDstRegSrcAdg (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    (REG) dstReg, dstRegVal,
                                    insAssemblyStr);
}

VOID analysisRoutineWhenCallIsInvoked (VOID *iseptr, UINT32 opcode,
    ADDRINT rspRegVal,
    VOID *insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  std::string *insAssemblyStr = (std::string *) insAssembly;
  ise->analysisRoutineWhenCallIsInvoked (rspRegVal,
                                         insAssemblyStr);
}

}
}
}
}
