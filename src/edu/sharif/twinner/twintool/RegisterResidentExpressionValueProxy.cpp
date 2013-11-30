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

edu::sharif::twinner::trace::Expression *
RegisterResidentExpressionValueProxy::setExpressionWithoutChangeNotification (
    edu::sharif::twinner::trace::Trace *trace,
    const edu::sharif::twinner::trace::Expression *exp) const {
  return trace->setSymbolicExpressionByRegister (reg, exp);
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
  int regIndex = getRegisterIndex (reg);
  if (regIndex == -1) {
    edu::sharif::twinner::util::Logger::warning ()
        << "RegisterResidentExpressionValueProxy::valueIsChanged (...):"
        " Unhandled register: " << REG_StringShort (reg) << '\n';
    return;
  }
  edu::sharif::twinner::trace::Expression *reg16 = changedExp;
  edu::sharif::twinner::trace::Expression *temp;
  RegisterType regType = getRegisterType (reg);
  switch (regType) {
  case REG_32_BITS_TYPE:
    trace->setSymbolicExpressionByRegister (getOverlappingRegisterByIndex (regIndex, 0),
                                            changedExp);
    break;
  case REG_8_BITS_UPPER_HALF_TYPE:
    reg16 = trace->getSymbolicExpressionByRegister
        (getOverlappingRegisterByIndex (regIndex, 2));
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
  switch (regType) {
  case REG_64_BITS_TYPE:
    trace->setSymbolicExpressionByRegister (getOverlappingRegisterByIndex (regIndex, 1),
                                            changedExp)->truncate (32);
  case REG_32_BITS_TYPE:
    reg16 = trace->setSymbolicExpressionByRegister
        (getOverlappingRegisterByIndex (regIndex, 2), changedExp);
    reg16->truncate (16);
  case REG_16_BITS_TYPE:
    if (getOverlappingRegisterByIndex (regIndex, 3) != REG_INVALID_) {
      trace->setSymbolicExpressionByRegister (getOverlappingRegisterByIndex (regIndex, 3),
                                              reg16)->shiftToRight (8);
    }
    trace->setSymbolicExpressionByRegister (getOverlappingRegisterByIndex (regIndex, 4),
                                            changedExp)->truncate (8);
    if (regType != REG_16_BITS_TYPE) {
      break;
    }
  case REG_8_BITS_UPPER_HALF_TYPE:
    putExpressionInLeastSignificantBitsOfRegister
        (trace, getOverlappingRegisterByIndex (regIndex, 0), 16, reg16);
    putExpressionInLeastSignificantBitsOfRegister
        (trace, getOverlappingRegisterByIndex (regIndex, 1), 16, reg16);
    break;
  case REG_8_BITS_LOWER_HALF_TYPE:
    putExpressionInLeastSignificantBitsOfRegister
        (trace, getOverlappingRegisterByIndex (regIndex, 0), 8, changedExp);
    putExpressionInLeastSignificantBitsOfRegister
        (trace, getOverlappingRegisterByIndex (regIndex, 1), 8, changedExp);
    putExpressionInLeastSignificantBitsOfRegister
        (trace, getOverlappingRegisterByIndex (regIndex, 2), 8, changedExp);
    break;
  }
}

RegisterResidentExpressionValueProxy::RegisterType
RegisterResidentExpressionValueProxy::getRegisterType (REG reg) const {
  RegisterType regType = (RegisterType) REG_Size (reg); // 8, 4, 2, 1
  if (regType == REG_8_BITS_LOWER_HALF_TYPE && REG_is_Upper8 (reg)) {
    regType = REG_8_BITS_UPPER_HALF_TYPE;
  }
  return regType;
}

REG RegisterResidentExpressionValueProxy::getOverlappingRegisterByIndex (int external,
    int internal) const {
  const REG registers[][5] = {
    {REG_RAX, REG_EAX, REG_AX, REG_AH, REG_AL}, // 0
    {REG_RBX, REG_EBX, REG_BX, REG_BH, REG_BL},
    {REG_RDX, REG_EDX, REG_DX, REG_DH, REG_DL},
    {REG_R14, REG_R14D, REG_R14W, REG_INVALID_, REG_R14B},
    {REG_R12, REG_R12D, REG_R12W, REG_INVALID_, REG_R12B},
    {REG_RDI, REG_EDI, REG_DI, REG_INVALID_, REG_DIL}, // 5
    {REG_RSI, REG_ESI, REG_SI, REG_INVALID_, REG_SIL},
  };
  return registers[external][internal];
}

int RegisterResidentExpressionValueProxy::getRegisterIndex (REG reg) const {
  switch (REG_FullRegName (reg)) {
  case REG_RAX:
    return 0;
  case REG_RBX:
    return 1;
  case REG_RDX:
    return 2;
  case REG_R14:
    return 3;
  case REG_R12:
    return 4;
  case REG_RDI:
    return 5;
  case REG_RSI:
    return 6;
  default:
    return -1;
  }
}

void RegisterResidentExpressionValueProxy::truncate (
    edu::sharif::twinner::trace::Expression *exp) const {
  exp->truncate (getSize ());
}

int RegisterResidentExpressionValueProxy::getSize () const {
  return REG_Size (reg) * 8;
}

}
}
}
}
