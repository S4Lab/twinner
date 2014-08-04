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

#ifndef REGISTER_RESIDENT_EXPRESSION_VALUE_PROXY_H
#define REGISTER_RESIDENT_EXPRESSION_VALUE_PROXY_H

#include "MutableExpressionValueProxy.h"

#include "pin.H"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class ConcreteValue;
}
namespace twintool {

class RegisterResidentExpressionValueProxy : public MutableExpressionValueProxy {

private:
  typedef edu::sharif::twinner::trace::ConcreteValue ConcreteValue;

  REG reg;
  ConcreteValue *regVal;

public:
  /**
   * Constructs a proxy for register expression values. It clones and cast the regVal
   * based on bit-length of the reg.
   *
   * @param reg The underlying registers which access to it is wrapped by this proxy
   * @param regVal The concrete value which is supposed to be found in the reg
   */
  RegisterResidentExpressionValueProxy (REG reg, const ConcreteValue &regVal);
  RegisterResidentExpressionValueProxy (REG reg);
  virtual ~RegisterResidentExpressionValueProxy ();

  virtual edu::sharif::twinner::trace::Expression *getExpression (
      edu::sharif::twinner::trace::Trace *trace) const;

  virtual edu::sharif::twinner::trace::Expression
  setExpressionWithoutChangeNotification (
      edu::sharif::twinner::trace::Trace *trace,
      const edu::sharif::twinner::trace::Expression *exp) const;

  virtual void valueIsChanged (
      edu::sharif::twinner::trace::Trace *trace,
      const edu::sharif::twinner::trace::Expression &changedExp) const;

  virtual int getSize () const;

private:
  void putExpressionInLeastSignificantBitsOfRegister (
      edu::sharif::twinner::trace::Trace *trace, int rsize, REG r, int bits,
      const edu::sharif::twinner::trace::Expression &exp) const;

  enum RegisterType {

    REG_64_BITS_TYPE = 8,
    REG_32_BITS_TYPE = 4,
    REG_16_BITS_TYPE = 2,
    REG_8_BITS_LOWER_HALF_TYPE = 1,
    REG_8_BITS_UPPER_HALF_TYPE = 3,
  };

  RegisterType getRegisterType (REG reg) const;
  REG getOverlappingRegisterByIndex (int external, int internal) const;
  int getRegisterIndex (REG reg) const;
};

}
}
}
}

#endif /* RegisterResidentExpressionValueProxy.h */
