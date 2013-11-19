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
  edu::sharif::twinner::util::Logger::loquacious () << "(register value is changed to "
      << changedExp << ")\n";
  edu::sharif::twinner::trace::Expression *reg16 = changedExp;
  edu::sharif::twinner::trace::Expression *temp;
  switch (reg) {
  case REG_EAX:
  case REG_R12D:
  case REG_EDI:
    trace->setSymbolicExpressionByRegister (getOverlappingRegisterByIndex (reg, 1),
                                            changedExp);
    break;
  case REG_AH:
    reg16 = trace->getSymbolicExpressionByRegister
        (getOverlappingRegisterByIndex (reg, 3));
    reg16->truncate (8);
    temp = changedExp->clone ();
    temp->shiftToLeft (8);
    reg16->binaryOperation
        (new edu::sharif::twinner::trace::Operator
         (edu::sharif::twinner::trace::Operator::BITWISE_OR), temp);
    delete temp;
    break;
  default: // second switch-case will address the default case
    break;
  }
  switch (reg) {
  case REG_RAX:
  case REG_R12:
  case REG_RDI:
    trace->setSymbolicExpressionByRegister (getOverlappingRegisterByIndex (reg, 2),
                                            changedExp)->truncate (32);
  case REG_EAX:
  case REG_R12D:
  case REG_EDI:
    reg16 = trace->setSymbolicExpressionByRegister
        (getOverlappingRegisterByIndex (reg, 3), changedExp);
    reg16->truncate (16);
  case REG_AX:
  case REG_R12W:
  case REG_DI:
    if (getOverlappingRegisterByIndex (reg, 4) != REG_INVALID_) {
      trace->setSymbolicExpressionByRegister (getOverlappingRegisterByIndex (reg, 4),
                                              reg16)->shiftToRight (8);
    }
    trace->setSymbolicExpressionByRegister (getOverlappingRegisterByIndex (reg, 5),
                                            changedExp)->truncate (8);
    if (!REG_is_gr16 (reg)) {
      break;
    }
  case REG_AH:
    putExpressionInLeastSignificantBitsOfRegister
        (trace, getOverlappingRegisterByIndex (reg, 1), 16, reg16);
    putExpressionInLeastSignificantBitsOfRegister
        (trace, getOverlappingRegisterByIndex (reg, 2), 16, reg16);
    break;
  case REG_AL:
  case REG_R12B:
  case REG_DIL:
    putExpressionInLeastSignificantBitsOfRegister
        (trace, getOverlappingRegisterByIndex (reg, 1), 8, changedExp);
    putExpressionInLeastSignificantBitsOfRegister
        (trace, getOverlappingRegisterByIndex (reg, 2), 8, changedExp);
    putExpressionInLeastSignificantBitsOfRegister
        (trace, getOverlappingRegisterByIndex (reg, 3), 8, changedExp);
    break;
  default:
    edu::sharif::twinner::util::Logger::warning ()
        << "RegisterResidentExpressionValueProxy::valueIsChanged (...):"
        " Unhandled register: " << REG_StringShort (reg) << '\n';
    break;
  }
}

REG RegisterResidentExpressionValueProxy::getOverlappingRegisterByIndex (REG reg,
    int index) const {
  switch (reg) {
  case REG_RAX:
  case REG_EAX:
  case REG_AX:
  case REG_AH:
  case REG_AL:
    switch (index) {
    case 1:
      return REG_RAX;
    case 2:
      return REG_EAX;
    case 3:
      return REG_AX;
    case 4:
      return REG_AH;
    case 5:
      return REG_AL;
    default:
      break;
    }
    break;
  case REG_R12:
  case REG_R12D:
  case REG_R12W:
  case REG_R12B:
    switch (index) {
    case 1:
      return REG_R12;
    case 2:
      return REG_R12D;
    case 3:
      return REG_R12W;
    case 5:
      return REG_R12B;
    default:
      break;
    }
    break;
  case REG_RDI:
  case REG_EDI:
  case REG_DI:
  case REG_DIL:
    switch (index) {
    case 1:
      return REG_RDI;
    case 2:
      return REG_EDI;
    case 3:
      return REG_DI;
    case 5:
      return REG_DIL;
    default:
      break;
    }
    break;
  default:
    break;
  }
  return REG_INVALID_;
}

void RegisterResidentExpressionValueProxy::truncate (
    edu::sharif::twinner::trace::Expression *exp) const {
  exp->truncate (REG_Size (reg) * 8);
}

}
}
}
}
