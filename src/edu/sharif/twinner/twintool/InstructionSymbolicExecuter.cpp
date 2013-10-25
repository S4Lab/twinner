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

void InstructionSymbolicExecuter::movToRegisterFromMemoryAddress (REG reg,
    ADDRINT memoryEa) {
  const edu::sharif::twinner::trace::Expression *srcexp =
      trace->getSymbolicExpressionByMemoryAddress (memoryEa,
          readMemoryContent (memoryEa));
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
      trace->getSymbolicExpressionByMemoryAddress (memoryEa,
          readMemoryContent (memoryEa));
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
      trace->getSymbolicExpressionByMemoryAddress (stackEa, readMemoryContent (stackEa));
  trace->setSymbolicExpressionByMemoryAddress (memoryEa, srcexp);
}

void InstructionSymbolicExecuter::addToRegisterFromImmediateValue (REG reg, UINT64 regval,
    ADDRINT immediate) {
  edu::sharif::twinner::trace::Expression *srcexp =
      new edu::sharif::twinner::trace::Expression (immediate);
  edu::sharif::twinner::trace::Expression *dstexp =
      trace->getSymbolicExpressionByRegister (reg, regval);
  dstexp->binaryOperation (
      new edu::sharif::twinner::trace::Operator (
          edu::sharif::twinner::trace::Operator::ADD), srcexp);
  delete srcexp; // binary operation clones the expression contents
  //TODO: set rflags
}

UINT64 InstructionSymbolicExecuter::readMemoryContent (ADDRINT memoryEa) const {
  UINT64 currentConcreteValue;
  PIN_SafeCopy (&currentConcreteValue, (const VOID*) (memoryEa), sizeof(UINT64));
  return currentConcreteValue;
}

VOID movToRegisterFromMemoryAddress (VOID *iseptr, UINT32 regi32, ADDRINT memoryEa) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->movToRegisterFromMemoryAddress ((REG) regi32, memoryEa);
}

VOID movToMemoryAddressFromRegister (VOID *iseptr, ADDRINT memoryEa, UINT32 regi32,
    ADDRINT regval) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->movToMemoryAddressFromRegister (memoryEa, (REG) regi32, regval);
}

VOID movToMemoryAddressFromImmediateValue (VOID *iseptr, ADDRINT memoryEa,
    ADDRINT immediate) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->movToMemoryAddressFromImmediateValue (memoryEa, immediate);
}

VOID movToRegisterFromImmediateValue (VOID *iseptr, UINT32 regi32, ADDRINT immediate) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->movToRegisterFromImmediateValue ((REG) regi32, immediate);
}

VOID movToRegisterFromRegister (VOID *iseptr, UINT32 regdsti32, UINT32 regsrci32,
    ADDRINT regsrcval) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->movToRegisterFromRegister ((REG) regdsti32, (REG) regsrci32, regsrcval);
}

VOID pushToStackFromRegister (VOID *iseptr, ADDRINT stackEa, UINT32 regi32,
    ADDRINT regval) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->pushToStackFromRegister (stackEa, (REG) regi32, regval);
}

VOID pushToStackFromImmediateValue (VOID *iseptr, ADDRINT stackEa, ADDRINT immediate) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->pushToStackFromImmediateValue (stackEa, immediate);
}

VOID pushToStackFromMemoryAddress (VOID *iseptr, ADDRINT stackEa, ADDRINT memoryEa) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->pushToStackFromMemoryAddress (stackEa, memoryEa);
}

VOID popToRegisterFromStack (VOID *iseptr, UINT32 regi32, ADDRINT stackEa) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->popToRegisterFromStack ((REG) regi32, stackEa);
}

VOID popToMemoryAddressFromStack (VOID *iseptr, ADDRINT memoryEa, ADDRINT stackEa) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->popToMemoryAddressFromStack (memoryEa, stackEa);
}

VOID addToRegisterFromImmediateValue (VOID *iseptr, UINT32 regi32, ADDRINT regval,
    ADDRINT immediate) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->addToRegisterFromImmediateValue ((REG) regi32, regval, immediate);
}

}
}
}
}
