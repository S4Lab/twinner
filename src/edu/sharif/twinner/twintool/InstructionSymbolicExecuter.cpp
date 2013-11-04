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
    REG srcReg, UINT64 srcRegVal) {
  edu::sharif::twinner::util::Logger::debug () << "analysisRoutineDstRegSrcReg (...)\n";
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
      RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcMem (AnalysisRoutine routine,
    REG dstReg, UINT64 dstRegVal,
    ADDRINT srcMemoryEa) {
  edu::sharif::twinner::util::Logger::debug () << "analysisRoutineDstRegSrcMem (...)\n";
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
      MemoryResidentExpressionValueProxy (srcMemoryEa));
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcImd (AnalysisRoutine routine,
    REG dstReg, UINT64 dstRegVal,
    ADDRINT srcImmediateValue) {
  edu::sharif::twinner::util::Logger::debug () << "analysisRoutineDstRegSrcImd (...)\n";
  edu::sharif::twinner::trace::Expression *srcexp =
      new edu::sharif::twinner::trace::Expression (srcImmediateValue);
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
      ConstantExpressionValueProxy (srcexp));
  delete srcexp;
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcReg (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    REG srcReg, UINT64 srcRegVal) {
  edu::sharif::twinner::util::Logger::debug () << "analysisRoutineDstMemSrcReg (...)\n";
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa),
      RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcImd (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    ADDRINT srcImmediateValue) {
  edu::sharif::twinner::util::Logger::debug () << "analysisRoutineDstMemSrcImd (...)\n";
  edu::sharif::twinner::trace::Expression *srcexp =
      new edu::sharif::twinner::trace::Expression (srcImmediateValue);
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa),
      ConstantExpressionValueProxy (srcexp));
  delete srcexp;
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcMem (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa) {
  edu::sharif::twinner::util::Logger::debug () << "analysisRoutineDstMemSrcMem (...)\n";
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa),
      MemoryResidentExpressionValueProxy (srcMemoryEa));
}

void InstructionSymbolicExecuter::analysisRoutineConditionalBranch (
    ConditionalBranchAnalysisRoutine routine,
    BOOL branchTaken) {
  edu::sharif::twinner::util::Logger::debug () <<
      "analysisRoutineConditionalBranch (...)\n";
  (this->*routine) (branchTaken);
}

void InstructionSymbolicExecuter::movAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  dst.setExpression (trace, srcexp);
}

void InstructionSymbolicExecuter::pushAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  dst.setExpression (trace, srcexp);
}

void InstructionSymbolicExecuter::popAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  dst.setExpression (trace, srcexp);
}

void InstructionSymbolicExecuter::addAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);
  dstexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::ADD), srcexp);
  //TODO: set rflags
}

void InstructionSymbolicExecuter::subAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);
  dstexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::MINUS), srcexp);
  //TODO: set rflags
}

void InstructionSymbolicExecuter::cmpAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  const edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);

  edu::sharif::twinner::trace::Expression *tmpexp = dstexp->clone ();
  tmpexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::MINUS), srcexp);
  eflags.setFlags (tmpexp);
}

void InstructionSymbolicExecuter::jnzAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::trace::Constraint *cc
      = new edu::sharif::twinner::trace::Constraint
      (eflags.getFlagsUnderlyingExpression (),
       branchTaken ?
       edu::sharif::twinner::trace::Constraint::NON_ZERO :
       edu::sharif::twinner::trace::Constraint::ZERO);
  trace->addPathConstraint (cc);
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
    edu::sharif::twinner::util::Logger::debug () << "Analysis routine: Unknown opcode: "
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
    edu::sharif::twinner::util::Logger::debug () << "Analysis routine: "
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
    UINT32 srcReg, ADDRINT srcRegVal) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcReg (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    (REG) dstReg, dstRegVal,
                                    (REG) srcReg, srcRegVal);
}

VOID analysisRoutineDstRegSrcMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcMemoryEa) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcMem (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    (REG) dstReg, dstRegVal,
                                    srcMemoryEa);
}

VOID analysisRoutineDstRegSrcImd (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcImmediateValue) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstRegSrcImd (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    (REG) dstReg, dstRegVal,
                                    srcImmediateValue);
}

VOID analysisRoutineDstMemSrcReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    UINT32 srcReg, ADDRINT srcRegVal) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstMemSrcReg (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    dstMemoryEa,
                                    (REG) srcReg, srcRegVal);
}

VOID analysisRoutineDstMemSrcImd (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcImmediateValue) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstMemSrcImd (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    dstMemoryEa,
                                    srcImmediateValue);
}

VOID analysisRoutineDstMemSrcMem (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineDstMemSrcMem (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    dstMemoryEa,
                                    srcMemoryEa);
}

VOID analysisRoutineConditionalBranch (VOID *iseptr, UINT32 opcode,
    BOOL branchTaken) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineConditionalBranch
      (ise->convertOpcodeToConditionalBranchAnalysisRoutine ((OPCODE) opcode),
       branchTaken);
}

}
}
}
}
