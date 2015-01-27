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

#include "MultiplyOperator.h"

#include "Constant.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

MultiplyOperator::MultiplyOperator () :
    Operator (Operator::MULTIPLY) {
}

MultiplyOperator::MultiplyOperator (const MultiplyOperator &ao) :
    Operator (ao) {
}

MultiplyOperator::~MultiplyOperator () {
}

MultiplyOperator *MultiplyOperator::clone () const {
  return new MultiplyOperator (*this);
}

bool MultiplyOperator::doesSupportSimplification () const {
  return true;
}

void MultiplyOperator::initializeSimplificationRules () {
  simplificationRules.push_back
      (SimplificationRule (Operator::MULTIPLY, Operator::MULTIPLY));
}

Operator::SimplificationStatus MultiplyOperator::deepSimplify (
    edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  edu::sharif::twinner::trace::Expression::Stack &stack = exp->getStack ();
  std::list < ExpressionToken * >::iterator it = stack.end ();
  Operator *divideOrBitwiseAndOp = static_cast<Operator *> (*--it);
  if (divideOrBitwiseAndOp->getIdentifier () == Operator::DIVIDE) {
    Constant *second = dynamic_cast<Constant *> (*--it);
    if (second) {
      exp->getLastConcreteValue () *= *operand;
      edu::sharif::twinner::trace::cv::ConcreteValue *cv = second->getValue ().clone ();
      if ((*cv) == (*operand)) {
        stack.pop_back (); // removes divideOrBitwiseAndOp
        stack.pop_back (); // removes second
        delete divideOrBitwiseAndOp;
        delete second;
      } else {
        (*cv) /= (*operand);
        second->setValue (*cv);
      }
      delete operand;
      delete cv;
      return COMPLETED;
    }
  } else if (divideOrBitwiseAndOp->getIdentifier () == Operator::BITWISE_AND) {
  }
  return CAN_NOT_SIMPLIFY;
}

void MultiplyOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  dst *= src;
}

std::string MultiplyOperator::toString () const {
  return "*";
}

bool MultiplyOperator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const MultiplyOperator *> (&token);
}

}
}
}
}
}
