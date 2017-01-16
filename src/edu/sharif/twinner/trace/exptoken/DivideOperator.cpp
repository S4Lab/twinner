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

#include "DivideOperator.h"

#include "Constant.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

DivideOperator::DivideOperator (bool _signedArithmetic) :
    Operator (_signedArithmetic ? Operator::SIGNED_DIVIDE : Operator::DIVIDE),
    signedArithmetic (_signedArithmetic) {
  initializeSimplificationRules ();
}

DivideOperator::DivideOperator (const DivideOperator &ao) :
    Operator (ao), signedArithmetic (ao.signedArithmetic) {
  initializeSimplificationRules ();
}

DivideOperator::~DivideOperator () {
}

DivideOperator *DivideOperator::clone () const {
  return new DivideOperator (*this);
}

bool DivideOperator::doesSupportSimplification () const {
  return !signedArithmetic;
}

bool DivideOperator::isCommutable () const {
  return false;
}

void DivideOperator::initializeSimplificationRules () {
  if (!signedArithmetic) {
    simplificationRules.push_back
        (SimplificationRule (Operator::MULTIPLY, Operator::DIVIDE));
  }
}

bool DivideOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  if (signedArithmetic) {
    dst.signedDivide (src);
  } else {
    dst /= src;
  }
  return dst.getCarryBit ();
}

std::string DivideOperator::toString () const {
  if (signedArithmetic) {
    return "signedDivide";
  } else {
    return "/";
  }
}

bool DivideOperator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const DivideOperator *> (&token)
      && signedArithmetic ==
      static_cast<const DivideOperator *> (&token)->signedArithmetic;
}

}
}
}
}
}
