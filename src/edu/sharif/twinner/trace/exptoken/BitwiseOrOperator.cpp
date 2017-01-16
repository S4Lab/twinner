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

#include "BitwiseOrOperator.h"

#include "Constant.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

BitwiseOrOperator::BitwiseOrOperator () :
    Operator (Operator::BITWISE_OR) {
  initializeSimplificationRules ();
}

BitwiseOrOperator::BitwiseOrOperator (const BitwiseOrOperator &ao) :
    Operator (ao) {
  initializeSimplificationRules ();
}

BitwiseOrOperator::~BitwiseOrOperator () {
}

BitwiseOrOperator *BitwiseOrOperator::clone () const {
  return new BitwiseOrOperator (*this);
}

bool BitwiseOrOperator::doesSupportSimplification () const {
  return true;
}

bool BitwiseOrOperator::isCommutable () const {
  return true;
}

bool BitwiseOrOperator::apply (edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  if (operand->isZero ()) {
    delete operand;
    return true;
  }
  return Operator::apply (exp, operand);
}

void BitwiseOrOperator::initializeSimplificationRules () {
  simplificationRules.push_back
      (SimplificationRule (Operator::BITWISE_OR, Operator::BITWISE_OR));
}

bool BitwiseOrOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  dst |= src;
  return dst.getCarryBit ();
}

std::string BitwiseOrOperator::toString () const {
  return "|";
}

bool BitwiseOrOperator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const BitwiseOrOperator *> (&token);
}

}
}
}
}
}
