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
      trace->getSymbolicExpressionByMemoryAddress (memoryEa);
  trace->setSymbolicExpressionByRegister (reg, srcexp);
}

void InstructionSymbolicExecuter::movToMemoryAddressFromRegister (ADDRINT memoryEa,
    REG reg) {
  const edu::sharif::twinner::trace::Expression *srcexp =
      trace->getSymbolicExpressionByRegister (reg);
  trace->setSymbolicExpressionByMemoryAddress (memoryEa, srcexp);
}

void InstructionSymbolicExecuter::movToMemoryAddressFromImmediateValue (ADDRINT memoryEa,
    ADDRINT immediate) {
  throw "Not yet implemented";
}

void InstructionSymbolicExecuter::movToRegisterFromImmediateValue (REG reg,
    ADDRINT immediate) {
  throw "Not yet implemented";
}

void InstructionSymbolicExecuter::movToRegisterFromRegister (REG dreg, REG sreg) {
  const edu::sharif::twinner::trace::Expression *srcexp =
      trace->getSymbolicExpressionByRegister (sreg);
  trace->setSymbolicExpressionByRegister (dreg, srcexp);
}

VOID movToRegisterFromMemoryAddress (VOID *iseptr, UINT32 regi32, ADDRINT memoryEa) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->movToRegisterFromMemoryAddress ((REG) regi32, memoryEa);
}

VOID movToMemoryAddressFromRegister (VOID *iseptr, ADDRINT memoryEa, UINT32 regi32) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->movToMemoryAddressFromRegister (memoryEa, (REG) regi32);
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

VOID movToRegisterFromRegister (VOID *iseptr, UINT32 regdsti32, UINT32 regsrci32) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->movToRegisterFromRegister ((REG) regdsti32, (REG) regsrci32);
}

}
}
}
}
