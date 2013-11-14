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
    INS ins) {
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstRegSrcReg(INS: "
      << INS_Disassemble (ins) << "): Registers:\n";
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
      RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcMem (AnalysisRoutine routine,
    REG dstReg, UINT64 dstRegVal,
    ADDRINT srcMemoryEa,
    INS ins) {
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstRegSrcMem(INS: "
      << INS_Disassemble (ins) << "): Registers:\n";
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
      MemoryResidentExpressionValueProxy (srcMemoryEa));
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcImd (AnalysisRoutine routine,
    REG dstReg, UINT64 dstRegVal,
    ADDRINT srcImmediateValue,
    INS ins) {
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstRegSrcImd(INS: "
      << INS_Disassemble (ins) << "): Registers:\n";
  edu::sharif::twinner::trace::Expression *srcexp =
      new edu::sharif::twinner::trace::Expression (srcImmediateValue);
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
      ConstantExpressionValueProxy (srcexp));
  delete srcexp;
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcReg (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    REG srcReg, UINT64 srcRegVal,
    INS ins) {
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstMemSrcReg(INS: "
      << INS_Disassemble (ins) << "): Registers:\n";
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa),
      RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcImd (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    ADDRINT srcImmediateValue,
    INS ins) {
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstMemSrcImd(INS: "
      << INS_Disassemble (ins) << "): Registers:\n";
  edu::sharif::twinner::trace::Expression *srcexp =
      new edu::sharif::twinner::trace::Expression (srcImmediateValue);
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa),
      ConstantExpressionValueProxy (srcexp));
  delete srcexp;
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcMem (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa,
    INS ins) {
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstMemSrcMem(INS: "
      << INS_Disassemble (ins) << "): Registers:\n";
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa),
      MemoryResidentExpressionValueProxy (srcMemoryEa));
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineConditionalBranch (
    ConditionalBranchAnalysisRoutine routine,
    BOOL branchTaken,
    INS ins) {
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineConditionalBranch(INS: "
      << INS_Disassemble (ins) << "): Registers:\n";
  (this->*routine) (branchTaken);
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::movAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "movAnalysisRoutine(...)\n"
      << "\tgetting src exp";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting dst exp";
  dst.setExpression (trace, srcexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::pushAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "pushAnalysisRoutine(...)\n"
      << "\tgetting src exp";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting dst exp";
  dst.setExpression (trace, srcexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::popAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "popAnalysisRoutine(...)\n"
      << "\tgetting src exp";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting dst exp";
  dst.setExpression (trace, srcexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::addAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "addAnalysisRoutine(...)\n"
      << "\tgetting src exp";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp";
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tbinary operation";
  dstexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::ADD), srcexp);
  dst.valueIsChanged ();
  //TODO: set rflags
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::subAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "subAnalysisRoutine(...)\n"
      << "\tgetting src exp";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp";
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tbinary operation";
  dstexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::MINUS), srcexp);
  dst.valueIsChanged ();
  //TODO: set rflags
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::cmpAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "cmpAnalysisRoutine(...)\n"
      << "\tgetting src exp";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp";
  const edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);

  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tbinary operation";
  edu::sharif::twinner::trace::Expression *tmpexp = dstexp->clone ();
  tmpexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::MINUS), srcexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting EFLAGS";
  eflags.setFlags (tmpexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::jnzAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::util::Logger::loquacious () << "jnzAnalysisRoutine(...)\n"
      << "\tinstantiating constraint";
  edu::sharif::twinner::trace::Constraint *cc
      = new edu::sharif::twinner::trace::Constraint
      (eflags.getFlagsUnderlyingExpression (),
       branchTaken ?
       edu::sharif::twinner::trace::Constraint::NON_ZERO :
       edu::sharif::twinner::trace::Constraint::ZERO);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tadding constraint";
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
  PIN_SafeCopy (&currentConcreteValue, (const VOID*) (memoryEa), sizeof (UINT64));
  return currentConcreteValue;
}

VOID analysisRoutineDstRegSrcReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal, UINT32 ins) {
  INS inss;
  inss.q_set ((INT32) ins);
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcReg (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    (REG) dstReg, dstRegVal,
                                    (REG) srcReg, srcRegVal, inss);
}

VOID analysisRoutineDstRegSrcMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcMemoryEa, UINT32 ins) {
  INS inss;
  inss.q_set ((INT32) ins);
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcMem (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    (REG) dstReg, dstRegVal,
                                    srcMemoryEa, inss);
}

VOID analysisRoutineDstRegSrcImd (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcImmediateValue, UINT32 ins) {
  INS inss;
  inss.q_set ((INT32) ins);
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcImd (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    (REG) dstReg, dstRegVal,
                                    srcImmediateValue, inss);
}

VOID analysisRoutineDstMemSrcReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    UINT32 srcReg, ADDRINT srcRegVal, UINT32 ins) {
  INS inss;
  inss.q_set ((INT32) ins);
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstMemSrcReg (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    dstMemoryEa,
                                    (REG) srcReg, srcRegVal, inss);
}

VOID analysisRoutineDstMemSrcImd (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcImmediateValue, UINT32 ins) {
  INS inss;
  inss.q_set ((INT32) ins);
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstMemSrcImd (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    dstMemoryEa,
                                    srcImmediateValue, inss);
}

VOID analysisRoutineDstMemSrcMem (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa, UINT32 ins) {
  INS inss;
  inss.q_set ((INT32) ins);
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstMemSrcMem (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    dstMemoryEa,
                                    srcMemoryEa, inss);
}

VOID analysisRoutineConditionalBranch (VOID *iseptr, UINT32 opcode,
    BOOL branchTaken, UINT32 ins) {
  INS inss;
  inss.q_set ((INT32) ins);
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineConditionalBranch
      (ise->convertOpcodeToConditionalBranchAnalysisRoutine ((OPCODE) opcode),
       branchTaken, inss);
}

}
}
}
}
