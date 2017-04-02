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

#include "ConstantExpressionValueProxy.h"

#include "edu/sharif/twinner/trace/ExpressionImp.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"
#include "edu/sharif/twinner/trace/StateSummary.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace proxy {

ConstantExpressionValueProxy::ConstantExpressionValueProxy (
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue, int _size) :
    exp (new edu::sharif::twinner::trace::ExpressionImp (concreteValue.clone (_size))),
    size (_size) {
}

ConstantExpressionValueProxy::~ConstantExpressionValueProxy () {
  delete exp;
}

edu::sharif::twinner::trace::Expression *ConstantExpressionValueProxy::getExpression (
    edu::sharif::twinner::trace::Trace *trace,
    edu::sharif::twinner::trace::StateSummary &state) const {
  return exp->clone ();
}

edu::sharif::twinner::trace::cv::ConcreteValue *
ConstantExpressionValueProxy::getConcreteValue () const {
  return exp->getLastConcreteValue ().clone ();
}

void ConstantExpressionValueProxy::valueIsChanged (
    edu::sharif::twinner::trace::Trace *trace,
    const edu::sharif::twinner::trace::Expression &changedExp,
    edu::sharif::twinner::trace::StateSummary &state) const {
  edu::sharif::twinner::util::Logger::error ()
      << "ConstantExpressionValueProxy::valueIsChanged (...): "
      "Constant expression is not supposed to be changed\n";
  abort ();
}

}
}
}
}
