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

#include <stdexcept>

#include "RegisterResidentExpressionValueProxy.h"

#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

RegisterResidentExpressionValueProxy::RegisterResidentExpressionValueProxy (
    REG _reg, UINT64 _regVal) :
reg (_reg), regVal (_regVal) {
}

edu::sharif::twinner::trace::Expression *
RegisterResidentExpressionValueProxy::getExpression (
    edu::sharif::twinner::trace::Trace *trace) const {
  return trace->getSymbolicExpressionByRegister (reg, regVal);
}

void RegisterResidentExpressionValueProxy::setExpression (
    edu::sharif::twinner::trace::Trace *trace,
    const edu::sharif::twinner::trace::Expression *exp) const {
  edu::sharif::twinner::trace::Expression *newExp =
      trace->setSymbolicExpressionByRegister (reg, exp);
  valueIsChanged (trace, newExp);
}

void RegisterResidentExpressionValueProxy::putExpressionInLeastSignificantBitsOfRegister
(edu::sharif::twinner::trace::Trace *trace,
    REG r, int bits,
    edu::sharif::twinner::trace::Expression *exp) const {
  edu::sharif::twinner::trace::Expression *dst =
      trace->getSymbolicExpressionByRegister (r);
  dst->makeLeastSignificantBitsZero (bits);
  dst->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::BITWISE_OR), exp);
}

void RegisterResidentExpressionValueProxy::valueIsChanged (
    edu::sharif::twinner::trace::Trace *trace,
    edu::sharif::twinner::trace::Expression *changedExp) const {
  edu::sharif::twinner::trace::Expression *ax = changedExp;
  edu::sharif::twinner::trace::Expression *temp;
  switch (reg) {
  case REG_EAX:
    trace->setSymbolicExpressionByRegister (REG_RAX, changedExp);
    break;
  case REG_AH:
    ax = trace->getSymbolicExpressionByRegister (REG_AX);
    ax->truncate (8);
    temp = changedExp->clone ();
    temp->shiftToLeft (8);
    ax->binaryOperation
        (new edu::sharif::twinner::trace::Operator
         (edu::sharif::twinner::trace::Operator::BITWISE_OR), temp);
    delete temp;
    break;
  default: // second switch-case will address the default case
    break;
  }
  switch (reg) {
  case REG_RAX:
    trace->setSymbolicExpressionByRegister (REG_EAX, changedExp)->truncate (32);
  case REG_EAX:
    ax = trace->setSymbolicExpressionByRegister (REG_AX, changedExp);
    ax->truncate (16);
  case REG_AX:
    trace->setSymbolicExpressionByRegister (REG_AH, ax)->shiftToRight (8);
    trace->setSymbolicExpressionByRegister (REG_AL, changedExp)->truncate (8);
    if (reg != REG_AX) {
      break;
    }
  case REG_AH:
    putExpressionInLeastSignificantBitsOfRegister (trace, REG_RAX, 16, ax);
    putExpressionInLeastSignificantBitsOfRegister (trace, REG_EAX, 16, ax);
    break;
  case REG_AL:
    putExpressionInLeastSignificantBitsOfRegister (trace, REG_RAX, 8, changedExp);
    putExpressionInLeastSignificantBitsOfRegister (trace, REG_EAX, 8, changedExp);
    putExpressionInLeastSignificantBitsOfRegister (trace, REG_AX, 8, changedExp);
    break;
  default:
    edu::sharif::twinner::util::Logger::warning ()
        << "RegisterResidentExpressionValueProxy::valueIsChanged (...):"
        " Unhandled register: " << REG_StringShort (reg) << '\n';
    break;
  }
}

}
}
}
}
