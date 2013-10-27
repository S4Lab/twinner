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

#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/Expression.h"

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
  throw std::runtime_error ("Not yet implemented");
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcMem (AnalysisRoutine routine,
    REG dstReg, UINT64 dstRegVal,
    ADDRINT srcMemoryEa) {
  throw std::runtime_error ("Not yet implemented");
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcImd (AnalysisRoutine routine,
    REG dstReg, UINT64 dstRegVal,
    ADDRINT srcImmediateValue) {
  throw std::runtime_error ("Not yet implemented");
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcReg (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    REG srcReg, UINT64 srcRegVal) {
  throw std::runtime_error ("Not yet implemented");
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcImd (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    ADDRINT srcImmediateValue) {
  throw std::runtime_error ("Not yet implemented");
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcMem (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa) {
  throw std::runtime_error ("Not yet implemented");
}

void InstructionSymbolicExecuter::movToRegisterFromMemoryAddress (REG reg,
    ADDRINT memoryEa) {
  const edu::sharif::twinner::trace::Expression *srcexp =
      trace->getSymbolicExpressionByMemoryAddress
      (memoryEa, readMemoryContent (memoryEa));
  trace->setSymbolicExpressionByRegister (reg, srcexp);
}

void InstructionSymbolicExecuter::movToMemoryAddressFromRegister (ADDRINT memoryEa,
    REG reg, UINT64 regval) {
  const edu::sharif::twinner::trace::Expression *srcexp =
      trace->getSymbolicExpressionByRegister (reg, regval);
  trace->setSymbolicExpressionByMemoryAddress (memoryEa, srcexp);
}

void InstructionSymbolicExecuter::movToMemoryAddressFromImmediateValue (ADDRINT memoryEa,
    ADDRINT immediate) {
  edu::sharif::twinner::trace::Expression *exp =
      new edu::sharif::twinner::trace::Expression (immediate);
  trace->setSymbolicExpressionByMemoryAddress (memoryEa, exp);
  delete exp; // Above setter method, has cloned the expression object for itself.
}

void InstructionSymbolicExecuter::movToRegisterFromImmediateValue (REG reg,
    ADDRINT immediate) {
  edu::sharif::twinner::trace::Expression *exp =
      new edu::sharif::twinner::trace::Expression (immediate);
  trace->setSymbolicExpressionByRegister (reg, exp);
  delete exp; // Above setter method, has cloned the expression object for itself.
}

void InstructionSymbolicExecuter::movToRegisterFromRegister (REG dreg, REG sreg,
    UINT64 regsrcval) {
  const edu::sharif::twinner::trace::Expression *srcexp =
      trace->getSymbolicExpressionByRegister (sreg, regsrcval);
  trace->setSymbolicExpressionByRegister (dreg, srcexp);
}

void InstructionSymbolicExecuter::pushToStackFromRegister (ADDRINT stackEa, REG reg,
    UINT64 regval) {
  const edu::sharif::twinner::trace::Expression *srcexp =
      trace->getSymbolicExpressionByRegister (reg, regval);
  trace->setSymbolicExpressionByMemoryAddress (stackEa, srcexp);
}

void InstructionSymbolicExecuter::pushToStackFromImmediateValue (ADDRINT stackEa,
    ADDRINT immediate) {
  edu::sharif::twinner::trace::Expression *srcexp =
      new edu::sharif::twinner::trace::Expression (immediate);
  trace->setSymbolicExpressionByMemoryAddress (stackEa, srcexp);
}

void InstructionSymbolicExecuter::pushToStackFromMemoryAddress (ADDRINT stackEa,
    ADDRINT memoryEa) {
  const edu::sharif::twinner::trace::Expression *srcexp =
      trace->getSymbolicExpressionByMemoryAddress
      (memoryEa, readMemoryContent (memoryEa));
  trace->setSymbolicExpressionByMemoryAddress (stackEa, srcexp);
}

void InstructionSymbolicExecuter::popToRegisterFromStack (REG reg, ADDRINT stackEa) {
  const edu::sharif::twinner::trace::Expression *srcexp =
      trace->getSymbolicExpressionByMemoryAddress (stackEa, readMemoryContent (stackEa));
  trace->setSymbolicExpressionByRegister (reg, srcexp);
}

void InstructionSymbolicExecuter::popToMemoryAddressFromStack (ADDRINT memoryEa,
    ADDRINT stackEa) {
  const edu::sharif::twinner::trace::Expression *srcexp =
      trace->getSymbolicExpressionByMemoryAddress
      (stackEa, readMemoryContent (stackEa));
  trace->setSymbolicExpressionByMemoryAddress (memoryEa, srcexp);
}

void InstructionSymbolicExecuter::addToRegisterFromImmediateValue (REG reg, UINT64 regval,
    ADDRINT immediate) {
  const edu::sharif::twinner::trace::Expression *srcexp =
      new edu::sharif::twinner::trace::Expression (immediate);
  edu::sharif::twinner::trace::Expression *dstexp =
      trace->getSymbolicExpressionByRegister (reg, regval);
  dstexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::ADD), srcexp);
  delete srcexp; // binary operation clones the expression contents
  //TODO: set rflags
}

void InstructionSymbolicExecuter::addToRegisterFromRegister (REG dstreg,
    UINT64 dstregval, REG srcreg, UINT64 srcregval) {
  const edu::sharif::twinner::trace::Expression *srcexp =
      trace->getSymbolicExpressionByRegister (srcreg, srcregval);
  edu::sharif::twinner::trace::Expression *dstexp =
      trace->getSymbolicExpressionByRegister (dstreg, dstregval);
  dstexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::ADD), srcexp);
  //TODO: set rflags
}

void InstructionSymbolicExecuter::subToRegisterFromImmediateValue (REG reg,
    UINT64 regval, ADDRINT immediate) {
  const edu::sharif::twinner::trace::Expression *srcexp =
      new edu::sharif::twinner::trace::Expression (immediate);
  edu::sharif::twinner::trace::Expression *dstexp =
      trace->getSymbolicExpressionByRegister (reg, regval);
  dstexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::MINUS), srcexp);
  delete srcexp; // binary operation clones the expression contents
  //TODO: set rflags
}

void InstructionSymbolicExecuter::cmpToRegisterFromMemoryAddress (REG reg,
    UINT64 regval, ADDRINT memoryEa) {
  const edu::sharif::twinner::trace::Expression *dstexp =
      trace->getSymbolicExpressionByRegister (reg, regval);
  const edu::sharif::twinner::trace::Expression *srcexp =
      trace->getSymbolicExpressionByMemoryAddress
      (memoryEa, readMemoryContent (memoryEa));

  edu::sharif::twinner::trace::Expression *tmpexp = dstexp->clone ();
  tmpexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::MINUS), srcexp);
  eflags.setFlags (tmpexp);
}

InstructionSymbolicExecuter::AnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToAnalysisRoutine (OPCODE op) const {
  switch (op) {
  case XED_ICLASS_POP:
    return &InstructionSymbolicExecuter::popAnalysisRoutine;
  default:
    throw std::runtime_error ("Unknown opcode given to analysis routine");
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

}
}
}
}
