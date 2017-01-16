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

#include "RemainderOperator.h"

#include "Constant.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

RemainderOperator::RemainderOperator (bool _signedArithmetic) :
    Operator (_signedArithmetic ? Operator::SIGNED_REMAINDER : Operator::REMAINDER),
    signedArithmetic (_signedArithmetic) {
  initializeSimplificationRules ();
}

RemainderOperator::RemainderOperator (const RemainderOperator &ao) :
    Operator (ao), signedArithmetic (ao.signedArithmetic) {
  initializeSimplificationRules ();
}

RemainderOperator::~RemainderOperator () {
}

RemainderOperator *RemainderOperator::clone () const {
  return new RemainderOperator (*this);
}

bool RemainderOperator::doesSupportSimplification () const {
  return false;
}

bool RemainderOperator::isCommutable () const {
  return false;
}

void RemainderOperator::initializeSimplificationRules () {
}

bool RemainderOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  if (signedArithmetic) {
    dst.signedRemainder (src);
  } else {
    dst %= src;
  }
  return dst.getCarryBit ();
}

std::string RemainderOperator::toString () const {
  if (signedArithmetic) {
    return "signedRemainder";
  } else {
    return "%";
  }
}

bool RemainderOperator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const RemainderOperator *> (&token)
      && signedArithmetic ==
      static_cast<const RemainderOperator *> (&token)->signedArithmetic;
}

}
}
}
}
}
