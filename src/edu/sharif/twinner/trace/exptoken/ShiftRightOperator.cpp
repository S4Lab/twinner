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

#include "ShiftRightOperator.h"

#include "Constant.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

ShiftRightOperator::ShiftRightOperator () :
    Operator (Operator::SHIFT_RIGHT) {
  initializeSimplificationRules ();
}

ShiftRightOperator::ShiftRightOperator (const ShiftRightOperator &ao) :
    Operator (ao) {
  initializeSimplificationRules ();
}

ShiftRightOperator::~ShiftRightOperator () {
}

ShiftRightOperator *ShiftRightOperator::clone () const {
  return new ShiftRightOperator (*this);
}

bool ShiftRightOperator::doesSupportSimplification () const {
  return true;
}

bool ShiftRightOperator::apply (edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  if (operand->isZero ()) {
    delete operand;
    return true;
  }
  return Operator::apply (exp, operand);
}

bool ShiftRightOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  dst >>= src;
  return dst.getCarryBit ();
}

void ShiftRightOperator::initializeSimplificationRules () {
  simplificationRules.push_back
      (SimplificationRule (Operator::SHIFT_RIGHT, Operator::ADD));
  simplificationRules.push_back
      (SimplificationRule (Operator::SHIFT_LEFT, Operator::MINUS));
}

Operator::SimplificationStatus ShiftRightOperator::deepSimplify (
    edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  edu::sharif::twinner::trace::Expression::Stack &stack = exp->getStack ();
  std::list < ExpressionToken * >::iterator it = stack.end ();
  const Operator *op = static_cast<Operator *> (*--it);
  if (op->getIdentifier () == Operator::BITWISE_AND) {
    Constant *mask = dynamic_cast<Constant *> (*--it);
    if (mask) {
      edu::sharif::twinner::trace::cv::ConcreteValue *cv = mask->getValue ().clone ();
      (*cv) >>= (*operand);
      stack.pop_back (); // removes op
      stack.pop_back (); // removes mask
      delete op;
      delete mask;
      exp->shiftToRight (operand);
      exp->bitwiseAnd (cv);
      return COMPLETED;
    }
  } else if (op->getIdentifier () == Operator::MULTIPLY
      || op->getIdentifier () == Operator::DIVIDE) {
    const Constant *val = dynamic_cast<Constant *> (*--it);
    if (val) {
      int n;
      if (val->getValue ().isCompletePowerOfTwo (&n)) {
        if (op->getIdentifier () == Operator::DIVIDE) {
          edu::sharif::twinner::trace::cv::ConcreteValue *cv = operand->clone ();
          (*cv) += n;
          if (cv->getCarryBit ()) {
            delete cv;
          } else {
            exp->getLastConcreteValue () *= val->getValue ();
            stack.pop_back (); // removes op
            stack.pop_back (); // removes val
            delete op;
            delete val;
            exp->shiftToRight (cv);
            return COMPLETED;
          }
        } else {
          edu::sharif::twinner::trace::cv::ConcreteValue *cv = operand->clone ();
          (*cv) -= n;
          exp->getLastConcreteValue () /= val->getValue ();
          stack.pop_back (); // removes op
          stack.pop_back (); // removes val
          delete op;
          delete val;
          if (cv->getCarryBit ()) {
            exp->shiftToLeft (-cv->toUint64 ());
            delete cv;
          } else {
            exp->shiftToRight (cv);
          }
          return COMPLETED;
        }
      }
    }
  }
  return CAN_NOT_SIMPLIFY;
}

std::string ShiftRightOperator::toString () const {
  return ">>";
}

bool ShiftRightOperator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const ShiftRightOperator *> (&token);
}

}
}
}
}
}
