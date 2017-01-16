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

#include "MinusOperator.h"

#include "Constant.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

MinusOperator::MinusOperator () :
    Operator (Operator::MINUS) {
  initializeSimplificationRules ();
}

MinusOperator::MinusOperator (const MinusOperator &ao) :
    Operator (ao) {
  initializeSimplificationRules ();
}

MinusOperator::~MinusOperator () {
}

MinusOperator *MinusOperator::clone () const {
  return new MinusOperator (*this);
}

bool MinusOperator::doesSupportSimplification () const {
  return true;
}

bool MinusOperator::isCommutable () const {
  return false;
}

void MinusOperator::initializeSimplificationRules () {
  simplificationRules.push_back (SimplificationRule (Operator::ADD, Operator::MINUS));
  simplificationRules.push_back (SimplificationRule (Operator::MINUS, Operator::ADD));
}

bool MinusOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  dst -= src;
  return dst.getCarryBit ();
}

std::string MinusOperator::toString () const {
  return "-";
}

bool MinusOperator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const MinusOperator *> (&token);
}

}
}
}
}
}
