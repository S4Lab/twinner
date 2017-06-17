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

#ifndef REGISTER_RESIDENT_EXPRESSION_VALUE_PROXY_H
#define REGISTER_RESIDENT_EXPRESSION_VALUE_PROXY_H

#include "MutableExpressionValueProxy.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace cv {

class ConcreteValue;
}
}
namespace proxy {

class RegisterResidentExpressionValueProxy : public MutableExpressionValueProxy {
private:
  typedef edu::sharif::twinner::trace::cv::ConcreteValue ConcreteValue;

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
      edu::sharif::twinner::trace::Trace *trace,
      edu::sharif::twinner::trace::StateSummary &state) const;
  virtual edu::sharif::twinner::trace::cv::ConcreteValue *getConcreteValue () const;

  virtual edu::sharif::twinner::trace::Expression
  setExpressionWithoutChangeNotification (
      edu::sharif::twinner::trace::Trace *trace,
      const edu::sharif::twinner::trace::Expression *exp) const;

  virtual void valueIsChanged (
      edu::sharif::twinner::trace::Trace *trace,
      const edu::sharif::twinner::trace::Expression &changedExp,
      edu::sharif::twinner::trace::StateSummary &state) const;

  virtual int getSize () const;

private:
  void putExpressionInLeastSignificantBitsOfRegister (
      edu::sharif::twinner::trace::Trace *trace, int rsize, REG r, int bits,
      const edu::sharif::twinner::trace::Expression *exp) const;
};

}
}
}
}

#endif /* RegisterResidentExpressionValueProxy.h */
