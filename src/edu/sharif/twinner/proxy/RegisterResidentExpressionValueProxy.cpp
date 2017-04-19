//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2017 Behnam Momeni
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
#include "edu/sharif/twinner/trace/exptoken/RegisterEmergedSymbol.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace proxy {

RegisterResidentExpressionValueProxy::RegisterResidentExpressionValueProxy (REG _reg,
    const ConcreteValue &_regVal) :
    reg (_reg), regVal (_regVal.clone (REG_Size (_reg) * 8)) {
}

RegisterResidentExpressionValueProxy::RegisterResidentExpressionValueProxy (REG _reg) :
    reg (_reg), regVal (edu::sharif::twinner::trace::cv::ConcreteValue64Bits (0)
    .clone (REG_Size (_reg) * 8)) {
}

RegisterResidentExpressionValueProxy::~RegisterResidentExpressionValueProxy () {
  delete regVal;
}

edu::sharif::twinner::trace::Expression *
RegisterResidentExpressionValueProxy::getExpression (
    edu::sharif::twinner::trace::Trace *trace,
    edu::sharif::twinner::trace::StateSummary &state) const {
  edu::sharif::twinner::trace::Expression *exp =
      trace->getSymbolicExpressionByRegister
      (getSize (), reg, *regVal, 0, state);
  if (exp) {
    return exp->clone ();
  } else {
    return 0;
  }
}

edu::sharif::twinner::trace::cv::ConcreteValue *
RegisterResidentExpressionValueProxy::getConcreteValue () const {
  return regVal->clone ();
}

edu::sharif::twinner::trace::Expression
RegisterResidentExpressionValueProxy::setExpressionWithoutChangeNotification (
    edu::sharif::twinner::trace::Trace *trace,
    const edu::sharif::twinner::trace::Expression *exp) const {
  edu::sharif::twinner::trace::Expression *newExp =
      trace->setSymbolicExpressionByRegister (getSize (), reg, exp);
  return *newExp;
}

void RegisterResidentExpressionValueProxy::putExpressionInLeastSignificantBitsOfRegister (
    edu::sharif::twinner::trace::Trace *trace, int rsize, REG r, int bits,
    const edu::sharif::twinner::trace::Expression &exp) const {
  edu::sharif::twinner::trace::Expression *dst =
      trace->getSymbolicExpressionByRegister (rsize, r);
  dst->makeLeastSignificantBitsZero (bits);
  dst->bitwiseOr (&exp);
}

void RegisterResidentExpressionValueProxy::valueIsChanged (
    edu::sharif::twinner::trace::Trace *trace,
    const edu::sharif::twinner::trace::Expression &changedExp,
    edu::sharif::twinner::trace::StateSummary &state) const {
  typedef edu::sharif::twinner::trace::exptoken::RegisterEmergedSymbol Reg;
  edu::sharif::twinner::util::Logger::loquacious () << "(register value is changed to "
      << &changedExp << ")\n";
  const int regIndex = Reg::getRegisterIndex (REG_FullRegName (reg));
  if (regIndex == -1) {
    if (Reg::is128BitsRegister (reg)) { // there is no subregister at all
      return;
    }
    edu::sharif::twinner::util::Logger::warning ()
        << "RegisterResidentExpressionValueProxy::valueIsChanged (...):"
        " Unhandled register: " << REG_StringShort (reg) << '\n';
    return;
  }
  const edu::sharif::twinner::trace::Expression *constReg16 = &changedExp;
  edu::sharif::twinner::trace::Expression *temp;
  const Reg::RegisterType regType = Reg::getRegisterType (reg, REG_Size (reg));
  switch (regType) {
#ifdef TARGET_IA32E
  case Reg::REG_32_BITS_TYPE:
    trace->setSymbolicExpressionByRegister
        (64, Reg::getOverlappingRegisterByIndex (regIndex, 0), &changedExp);
    break;
#endif
  case Reg::REG_8_BITS_UPPER_HALF_TYPE:
  {
    edu::sharif::twinner::trace::Expression *reg16 =
        trace->getSymbolicExpressionByRegister
        (16, Reg::getOverlappingRegisterByIndex (regIndex, 2));
    reg16->truncate (8);
    temp = changedExp.clone (16);
    temp->shiftToLeft (8);
    reg16->bitwiseOr (temp);
    delete temp;
    constReg16 = reg16;
    break;
  }
  default: // second switch-case will address the default case
    break;
  }
  switch (regType) {
#ifdef TARGET_IA32E
  case Reg::REG_64_BITS_TYPE:
    trace->setSymbolicExpressionByRegister
        (32, Reg::getOverlappingRegisterByIndex (regIndex, 1), &changedExp)->truncate (32);
#endif
  case Reg::REG_32_BITS_TYPE:
    trace->setSymbolicExpressionByRegister
        (16, Reg::getOverlappingRegisterByIndex (regIndex, 2), &changedExp)->truncate (16);
  case Reg::REG_16_BITS_TYPE:
    if (Reg::getOverlappingRegisterByIndex (regIndex, 3) != REG_INVALID_) {
      temp = changedExp.clone (16);
      temp->shiftToRight (8);
      trace->setSymbolicExpressionByRegister
          (8, Reg::getOverlappingRegisterByIndex (regIndex, 3), temp)->truncate (8);
      delete temp;
    }
  {
    const REG lowest8Bits = Reg::getOverlappingRegisterByIndex (regIndex, 4);
    if (lowest8Bits != REG_INVALID_) {
      trace->setSymbolicExpressionByRegister
          (8, lowest8Bits, &changedExp)->truncate (8);
    }
  }
    if (regType != Reg::REG_16_BITS_TYPE) {
      break;
    }
  case Reg::REG_8_BITS_UPPER_HALF_TYPE:
#ifdef TARGET_IA32E
    putExpressionInLeastSignificantBitsOfRegister
        (trace, 64, Reg::getOverlappingRegisterByIndex (regIndex, 0), 16, *constReg16);
#endif
    putExpressionInLeastSignificantBitsOfRegister
        (trace, 32, Reg::getOverlappingRegisterByIndex (regIndex, 1), 16, *constReg16);
    break;
  case Reg::REG_8_BITS_LOWER_HALF_TYPE:
#ifdef TARGET_IA32E
    putExpressionInLeastSignificantBitsOfRegister
        (trace, 64, Reg::getOverlappingRegisterByIndex (regIndex, 0), 8, changedExp);
#endif
    putExpressionInLeastSignificantBitsOfRegister
        (trace, 32, Reg::getOverlappingRegisterByIndex (regIndex, 1), 8, changedExp);
    putExpressionInLeastSignificantBitsOfRegister
        (trace, 16, Reg::getOverlappingRegisterByIndex (regIndex, 2), 8, changedExp);
    break;
  }
}

int RegisterResidentExpressionValueProxy::getSize () const {
  return REG_Size (reg) * 8;
}

}
}
}
}
