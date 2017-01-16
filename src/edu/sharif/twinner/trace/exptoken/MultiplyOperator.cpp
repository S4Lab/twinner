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

#include "MultiplyOperator.h"

#include "Constant.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

#include "edu/sharif/twinner/util/max.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

MultiplyOperator::MultiplyOperator () :
    Operator (Operator::MULTIPLY) {
  initializeSimplificationRules ();
}

MultiplyOperator::MultiplyOperator (const MultiplyOperator &ao) :
    Operator (ao) {
  initializeSimplificationRules ();
}

MultiplyOperator::~MultiplyOperator () {
}

MultiplyOperator *MultiplyOperator::clone () const {
  return new MultiplyOperator (*this);
}

bool MultiplyOperator::doesSupportSimplification () const {
  return true;
}

bool MultiplyOperator::isCommutable () const {
  return false;
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
      if (second->getValue () == (*operand)) {
        stack.pop_back (); // removes divideOrBitwiseAndOp
        stack.pop_back (); // removes second
        delete divideOrBitwiseAndOp;
        delete second;
      } else {
        const int size = edu::sharif::twinner::util::max
            (exp->getLastConcreteValue ().getSize (),
             second->getValue ().getSize (),
             operand->getSize ());
        edu::sharif::twinner::trace::cv::ConcreteValue *cv =
            second->getValue ().clone (size);
        (*cv) /= (*operand);
        second->setValue (cv);
      }
      delete operand;
      return COMPLETED;
    }
  } else if (divideOrBitwiseAndOp->getIdentifier () == Operator::BITWISE_AND) {
  }
  return CAN_NOT_SIMPLIFY;
}

bool MultiplyOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  dst *= src;
  return dst.getCarryBit ();
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
