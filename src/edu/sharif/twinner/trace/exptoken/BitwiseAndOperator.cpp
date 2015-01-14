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

#include "BitwiseAndOperator.h"

#include "Constant.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

BitwiseAndOperator::BitwiseAndOperator () :
    Operator (Operator::BITWISE_AND) {
}

BitwiseAndOperator::BitwiseAndOperator (const BitwiseAndOperator &ao) :
    Operator (ao) {
}

BitwiseAndOperator::~BitwiseAndOperator () {
}

BitwiseAndOperator *BitwiseAndOperator::clone () const {
  return new BitwiseAndOperator (*this);
}

bool BitwiseAndOperator::doesSupportSimplification () const {
  return true;
}

bool BitwiseAndOperator::apply (edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  Constant *lastConstantMask = 0;
  edu::sharif::twinner::trace::Expression::Stack &stack = exp->getStack ();
  if (!stack.empty () && dynamic_cast<Constant *> (stack.back ())) {
    lastConstantMask = static_cast<Constant *> (stack.back ());

  } else if (stack.size () > 2 && dynamic_cast<Operator *> (stack.back ())) {
    std::list < ExpressionToken * >::iterator it = stack.end ();
    const Operator *op = static_cast<Operator *> (*--it);
    if (op->getIdentifier () == Operator::BITWISE_AND) {
      lastConstantMask = dynamic_cast<Constant *> (*--it);
    } else if (deepSimplify (exp, operand)) {
      return apply (exp, operand);
    }
  }
  exp->getLastConcreteValue () &= *operand;
  if (lastConstantMask) {
    (*operand) &= lastConstantMask->getValue ();
    lastConstantMask->setValue (*operand);
    delete operand;
    return true;
  } else {
    stack.push_back (new Constant (operand));
    stack.push_back (this);
    return false;
  }
}

void BitwiseAndOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  dst &= src;
}

bool BitwiseAndOperator::deepSimplify (edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  edu::sharif::twinner::trace::Expression::Stack &stack = exp->getStack ();
  if (stack.size () > 4) {
    std::list < ExpressionToken * >::iterator it = stack.end ();
    Operator *addOrMinusOp = dynamic_cast<Operator *> (*--it);
    if (addOrMinusOp && (addOrMinusOp->getIdentifier () == Operator::ADD
        || addOrMinusOp->getIdentifier () == Operator::MINUS)) {
      Constant *second = dynamic_cast<Constant *> (*--it);
      if (second) {
        Operator *andOp = dynamic_cast<Operator *> (*--it);
        if (andOp && andOp->getIdentifier () == Operator::BITWISE_AND) {
          Constant *first = dynamic_cast<Constant *> (*--it);
          if (first && isTruncatingMask (first->getValue ().clone ())) {
            // exp == (...) & first [+-] second  and  first is similar to 0x00001111
            stack.pop_back (); // removes addOrMinusOp
            stack.pop_back (); // removes second
            stack.pop_back (); // removes andOp
            stack.pop_back (); // removes first
            if (addOrMinusOp->getIdentifier () == Operator::ADD) {
              exp->getLastConcreteValue () -= second->getValue ();
              exp->add (second->getValue ().clone ());
            } else {
              exp->getLastConcreteValue () += second->getValue ();
              exp->minus (second->getValue ().clone ());
            }
            delete addOrMinusOp;
            delete second;
            delete andOp;
            (*operand) &= first->getValue ();
            delete first;
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool BitwiseAndOperator::isTruncatingMask (
    edu::sharif::twinner::trace::cv::ConcreteValue *cv) const {
  const unsigned int s = cv->getSize ();
  for (UINT64 i = 0; i < s; ++i) {
    if ((cv->toUint64 () & 0x1) == 0) {
      if (cv->isZero ()) {
        break;
      } else {
        delete cv;
        return false;
      }
    }
    (*cv) >>= 1;
  }
  delete cv;
  return true;
}

std::string BitwiseAndOperator::toString () const {
  return "&";
}

bool BitwiseAndOperator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const BitwiseAndOperator *> (&token);
}

}
}
}
}
}
