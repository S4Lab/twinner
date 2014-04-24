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

#ifndef CONSTANT_EXPRESSION_VALUE_PROXY_H
#define CONSTANT_EXPRESSION_VALUE_PROXY_H

#include "ExpressionValueProxy.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

class ConstantExpressionValueProxy : public ExpressionValueProxy {

private:
  edu::sharif::twinner::trace::Expression *exp;
  int size;

public:

  /**
   * Ownership of exp is not kept. Instantiator should take care of it.
   * @param exp The expression which getter method will return upon requests.
   * @param size The size of the underlying expression in bits.
   */
  ConstantExpressionValueProxy (edu::sharif::twinner::trace::Expression *exp, int size);

  virtual edu::sharif::twinner::trace::Expression *getExpression (
      edu::sharif::twinner::trace::Trace *trace) const;
  virtual edu::sharif::twinner::trace::Expression *getExpressionWithSignExtension (
      edu::sharif::twinner::trace::Trace *trace) const;

  virtual void valueIsChanged (
      edu::sharif::twinner::trace::Trace *trace,
      edu::sharif::twinner::trace::Expression *changedExp) const;
};

}
}
}
}

#endif /* ConstantExpressionValueProxy.h */
