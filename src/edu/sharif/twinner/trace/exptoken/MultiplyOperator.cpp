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

bool MultiplyOperator::apply (edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  Constant *lastConstant = 0;
  edu::sharif::twinner::trace::Expression::Stack &stack = exp->getStack ();
  if (!stack.empty () && dynamic_cast<Constant *> (stack.back ())) {
    lastConstant = static_cast<Constant *> (stack.back ());

  } else if (stack.size () > 2 && dynamic_cast<Operator *> (stack.back ())) {
    std::list < ExpressionToken * >::iterator it = stack.end ();
    const Operator *op = static_cast<Operator *> (*--it);
    if (op->getIdentifier () == Operator::MULTIPLY) {
      lastConstant = dynamic_cast<Constant *> (*--it);
    } else {
      switch (deepSimplify (exp, operand)) {
      case CAN_NOT_SIMPLIFY:
        break;
      case RESTART_SIMPLIFICATION:
        return apply (exp, operand);
      case COMPLETED:
        return true;
      }
    }
  }
  exp->getLastConcreteValue () *= *operand;
  if (lastConstant) {
    (*operand) *= lastConstant->getValue ();
    lastConstant->setValue (*operand);
    delete operand;
    return true;
  } else {
    stack.push_back (new Constant (operand));
    stack.push_back (this);
    return false;
  }
}

MultiplyOperator::SimplificationStatus MultiplyOperator::deepSimplify (
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
