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

#include "ConstantExpressionValueProxy.h"

#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

ConstantExpressionValueProxy::ConstantExpressionValueProxy (
    edu::sharif::twinner::trace::Expression *_exp, int _size) :
exp (_exp), size (_size) {
  exp->truncate (_size);
}

edu::sharif::twinner::trace::Expression *ConstantExpressionValueProxy::getExpression (
    edu::sharif::twinner::trace::Trace *trace) const {
  return exp->clone ();
}

edu::sharif::twinner::trace::Expression *
ConstantExpressionValueProxy::getExpressionWithSignExtension (
    edu::sharif::twinner::trace::Trace *trace) const {
  edu::sharif::twinner::trace::Expression *exp = getExpression (trace);
  if (size < 64) {
    if (exp->getLastConcreteValue ().isNegative (size)) {
      exp->minus (1ull << size); // sign-extend
    }
  }
  return exp;
}

void ConstantExpressionValueProxy::valueIsChanged (
    edu::sharif::twinner::trace::Trace *trace,
    edu::sharif::twinner::trace::Expression *changedExp) const {
  throw std::runtime_error ("Constant expression is not supposed to be changed");
}

}
}
}
}
