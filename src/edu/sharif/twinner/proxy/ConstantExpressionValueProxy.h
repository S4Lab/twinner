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

#ifndef CONSTANT_EXPRESSION_VALUE_PROXY_H
#define CONSTANT_EXPRESSION_VALUE_PROXY_H

#include "ExpressionValueProxy.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace cv {

class ConcreteValue;
}
}
namespace proxy {

class ConstantExpressionValueProxy : public ExpressionValueProxy {
private:
  edu::sharif::twinner::trace::Expression *exp;
  int size;

public:

  /**
   * An expression object will be created to hold the given concrete value.
   * The concrete value will be cloned and casted to size-bits precision before being
   * used.
   *
   * @param val The concrete value which is hold in the served constant expression.
   * @param size The size (bit precision) of the underlying expression in bits.
   */
  ConstantExpressionValueProxy (const edu::sharif::twinner::trace::cv::ConcreteValue &val,
      int _size);
  ~ConstantExpressionValueProxy ();

  virtual edu::sharif::twinner::trace::Expression *getExpression (
      edu::sharif::twinner::trace::Trace *trace,
      edu::sharif::twinner::trace::StateSummary &state) const;

  virtual edu::sharif::twinner::trace::cv::ConcreteValue *getConcreteValue () const;

  virtual void valueIsChanged (
      edu::sharif::twinner::trace::Trace *trace,
      const edu::sharif::twinner::trace::Expression &changedExp,
      edu::sharif::twinner::trace::StateSummary &state) const;
};

}
}
}
}

#endif /* ConstantExpressionValueProxy.h */
