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

#include "ConstantExpressionValueProxy.h"

#include "edu/sharif/twinner/trace/ExpressionImp.h"
#include "edu/sharif/twinner/trace/ConcreteValue.h"

#include <stdexcept>

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

ConstantExpressionValueProxy::ConstantExpressionValueProxy (
    const edu::sharif::twinner::trace::ConcreteValue &concreteValue, int _size) :
    exp (new edu::sharif::twinner::trace::ExpressionImp (concreteValue.clone (_size))),
    size (_size) {
}

ConstantExpressionValueProxy::~ConstantExpressionValueProxy () {
  delete exp;
}

edu::sharif::twinner::trace::Expression *ConstantExpressionValueProxy::getExpression (
    edu::sharif::twinner::trace::Trace *trace) const {
  return exp->clone ();
}

void ConstantExpressionValueProxy::valueIsChanged (
    edu::sharif::twinner::trace::Trace *trace,
    const edu::sharif::twinner::trace::Expression &changedExp) const {
  throw std::runtime_error ("Constant expression is not supposed to be changed");
}

}
}
}
}
