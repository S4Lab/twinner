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

#include <stdexcept>

#include "RegisterResidentExpressionValueProxy.h"

#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/trace/ConcreteValue64Bits.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

RegisterResidentExpressionValueProxy::RegisterResidentExpressionValueProxy (REG _reg,
    const ConcreteValue &_regVal) :
reg (_reg), regVal (_regVal) {
}

RegisterResidentExpressionValueProxy::RegisterResidentExpressionValueProxy (REG _reg) :
reg (_reg), regVal (edu::sharif::twinner::trace::ConcreteValue64Bits (0)) {
}

edu::sharif::twinner::trace::Expression *
RegisterResidentExpressionValueProxy::getExpression (
    edu::sharif::twinner::trace::Trace *trace) const {
  return trace->getSymbolicExpressionByRegister (reg, regVal)->clone ();
}

edu::sharif::twinner::trace::Expression *
RegisterResidentExpressionValueProxy::getExpressionWithSignExtension (
    edu::sharif::twinner::trace::Trace *trace) const {
  edu::sharif::twinner::trace::Expression *exp = getExpression (trace);
  const int size = getSize ();
  if (size < 64) {
    if (exp->getLastConcreteValue ().isNegative (size)) {
      exp->minus (1ull << size); // sign-extend
    }
  }
  return exp;
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
  truncate (changedExp);
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
    {REG_RCX, REG_ECX, REG_CX, REG_CH, REG_CL},
    {REG_RDX, REG_EDX, REG_DX, REG_DH, REG_DL},
    {REG_RDI, REG_EDI, REG_DI, REG_INVALID_, REG_DIL}, // 4
    {REG_RSI, REG_ESI, REG_SI, REG_INVALID_, REG_SIL},
    {REG_RSP, REG_ESP, REG_SP, REG_INVALID_, REG_SPL},
    {REG_RBP, REG_EBP, REG_BP, REG_INVALID_, REG_BPL},
    {REG_R8, REG_R8D, REG_R8W, REG_INVALID_, REG_R8B}, // 8
    {REG_R9, REG_R9D, REG_R9W, REG_INVALID_, REG_R9B},
    {REG_R10, REG_R10D, REG_R10W, REG_INVALID_, REG_R10B},
    {REG_R11, REG_R11D, REG_R11W, REG_INVALID_, REG_R11B},
    {REG_R12, REG_R12D, REG_R12W, REG_INVALID_, REG_R12B}, // 12
    {REG_R13, REG_R13D, REG_R13W, REG_INVALID_, REG_R13B},
    {REG_R14, REG_R14D, REG_R14W, REG_INVALID_, REG_R14B},
    {REG_R15, REG_R15D, REG_R15W, REG_INVALID_, REG_R15B},
  };
  return registers[external][internal];
}

int RegisterResidentExpressionValueProxy::getRegisterIndex (REG reg) const {
  switch (REG_FullRegName (reg)) {
  case REG_RAX:
    return 0;
  case REG_RBX:
    return 1;
  case REG_RCX:
    return 2;
  case REG_RDX:
    return 3;
  case REG_RDI:
    return 4;
  case REG_RSI:
    return 5;
  case REG_RSP:
    return 6;
  case REG_RBP:
    return 7;
  case REG_R8:
    return 8;
  case REG_R9:
    return 9;
  case REG_R10:
    return 10;
  case REG_R11:
    return 11;
  case REG_R12:
    return 12;
  case REG_R13:
    return 13;
  case REG_R14:
    return 14;
  case REG_R15:
    return 15;
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
