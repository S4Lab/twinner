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

#include "ConstantExpressionValueProxy.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

ConstantExpressionValueProxy::ConstantExpressionValueProxy (
    edu::sharif::twinner::trace::Expression *_exp) :
exp (_exp) {
}

edu::sharif::twinner::trace::Expression *ConstantExpressionValueProxy::getExpression (
    edu::sharif::twinner::trace::Trace *trace) const {
  return exp;
}

void ConstantExpressionValueProxy::valueIsChanged () const {
  throw std::runtime_error ("Constant expression is not supposed to be changed");
}

}
}
}
}
