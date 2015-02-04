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

#include "edu/sharif/twinner/trace/ExpressionImp.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

BitwiseAndOperator::BitwiseAndOperator () :
    Operator (Operator::BITWISE_AND) {
  initializeSimplificationRules ();
}

BitwiseAndOperator::BitwiseAndOperator (const BitwiseAndOperator &ao) :
    Operator (ao) {
  initializeSimplificationRules ();
}

BitwiseAndOperator::~BitwiseAndOperator () {
}

BitwiseAndOperator *BitwiseAndOperator::clone () const {
  return new BitwiseAndOperator (*this);
}

bool BitwiseAndOperator::doesSupportSimplification () const {
  return true;
}

void BitwiseAndOperator::initializeSimplificationRules () {
  simplificationRules.push_back
      (SimplificationRule (Operator::BITWISE_AND, Operator::BITWISE_AND));
}

bool BitwiseAndOperator::apply (edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  if (operand->isZero ()) {
    delete operand;
    (*exp) = edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
    return true;
  }
  return Operator::apply (exp, operand);
}

bool BitwiseAndOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  dst &= src;
  return dst.getCarryBit ();
}

Operator::SimplificationStatus BitwiseAndOperator::deepSimplify (
    edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  edu::sharif::twinner::trace::Expression::Stack &stack = exp->getStack ();
  if (stack.size () > 4) {
    std::list < ExpressionToken * >::iterator it = stack.end ();
    Operator *addOrMinusOrBitwiseOrOrMulOp = dynamic_cast<Operator *> (*--it);
    if (addOrMinusOrBitwiseOrOrMulOp
        && (addOrMinusOrBitwiseOrOrMulOp->getIdentifier () == Operator::ADD
        || addOrMinusOrBitwiseOrOrMulOp->getIdentifier () == Operator::MINUS
        || addOrMinusOrBitwiseOrOrMulOp->getIdentifier () == Operator::BITWISE_OR
        || addOrMinusOrBitwiseOrOrMulOp->getIdentifier () == Operator::MULTIPLY)) {
      Constant *second = dynamic_cast<Constant *> (*--it);
      if (second) {
        Operator *andOp = dynamic_cast<Operator *> (*--it);
        if (andOp->getIdentifier () == Operator::BITWISE_AND) {
          Constant *first = dynamic_cast<Constant *> (*--it);
          if (first) {
            // exp == (...) & first [+-|*] second
            if (addOrMinusOrBitwiseOrOrMulOp->getIdentifier () == Operator::BITWISE_OR) {
              const int size = operand->getSize ();
              edu::sharif::twinner::trace::cv::ConcreteValue *secondCv =
                  second->getValue ().clone (size);
              (*secondCv) &= (*operand);
              if ((*secondCv) != (*operand)) {
                edu::sharif::twinner::trace::cv::ConcreteValue *firstCv =
                    first->getValue ().clone (size);
                (*firstCv) |= (*secondCv);
                (*operand) &= (*firstCv);
                delete firstCv;
                stack.pop_back (); // removes addOrMinusOrBitwiseOrOp
                stack.pop_back (); // removes second
                stack.pop_back (); // removes andOp
                stack.pop_back (); // removes first
                exp->bitwiseOr (secondCv);
                delete addOrMinusOrBitwiseOrOrMulOp;
                delete second;
                delete andOp;
                delete first;
                return RESTART_SIMPLIFICATION;
              } else {
                delete secondCv;
                while (!stack.empty ()) {
                  delete stack.back ();
                  stack.pop_back ();
                }
                stack.push_back (new Constant (operand));
                exp->getLastConcreteValue () = (*operand);
                return COMPLETED;
              }
            } else if (addOrMinusOrBitwiseOrOrMulOp->getIdentifier ()
                == Operator::MULTIPLY && isTruncatingMask (operand->clone ())) {
              // exp == (...) & first * second
              const int maxNumberOfBitsOfResult =
                  numberOfBits (first->getValue ().clone ()) +
                  numberOfBits (second->getValue ().clone ());
              if (maxNumberOfBitsOfResult <= numberOfBits (operand->clone ())) {
                delete operand;
                return COMPLETED;
              }
            } else if (isTruncatingMask (first->getValue ().clone ())
                && isEquallyOrMoreLimitedThan (*operand, first->getValue ())) {
              // first is similar to 0x00001111
              stack.pop_back (); // removes addOrMinusOrBitwiseOrOp
              stack.pop_back (); // removes second
              stack.pop_back (); // removes andOp
              stack.pop_back (); // removes first
              if (addOrMinusOrBitwiseOrOrMulOp->getIdentifier () == Operator::ADD) {
                exp->getLastConcreteValue () -= second->getValue ();
                exp->add (second->getValue ().clone ());
              } else {
                exp->getLastConcreteValue () += second->getValue ();
                exp->minus (second->getValue ().clone ());
              }
              delete addOrMinusOrBitwiseOrOrMulOp;
              delete second;
              delete andOp;
              delete first;
              return RESTART_SIMPLIFICATION;
            }
          }
        } else if (andOp->getIdentifier () == Operator::ADD
            || andOp->getIdentifier () == Operator::MINUS) {
          if (addOrMinusOrBitwiseOrOrMulOp->getIdentifier () == Operator::ADD
              || addOrMinusOrBitwiseOrOrMulOp->getIdentifier () == Operator::MINUS) {
            Constant *first = dynamic_cast<Constant *> (*--it);
            if (first && isTruncatingMask (operand->clone ())) {
              // exp == (...) [+-] first [+-] second
              edu::sharif::twinner::trace::cv::ConcreteValue *firstCv =
                  first->getValue ().clone (operand->getSize ());
              if ((*andOp) == (*addOrMinusOrBitwiseOrOrMulOp)) {
                // exp == (...) [+-] (first + second)
                (*firstCv) += second->getValue ();
              } else {
                (*firstCv) -= second->getValue ();
              }
              first->setValue (firstCv);
              stack.pop_back (); // removes addOrMinusOrBitwiseOrOp
              stack.pop_back (); // removes second
              delete addOrMinusOrBitwiseOrOrMulOp;
              delete second;
              return RESTART_SIMPLIFICATION;
            }
          }
        }
      }
    }
  }
  return CAN_NOT_SIMPLIFY;
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

int BitwiseAndOperator::numberOfBits (
    edu::sharif::twinner::trace::cv::ConcreteValue *cv) const {
  int bits = 0;
  while (!cv->isZero ()) {
    (*cv) >>= 1;
    bits++;
  }
  delete cv;
  return bits;
}

bool BitwiseAndOperator::isEquallyOrMoreLimitedThan (
    const edu::sharif::twinner::trace::cv::ConcreteValue &first,
    const edu::sharif::twinner::trace::cv::ConcreteValue &second) const {
  edu::sharif::twinner::trace::cv::ConcreteValue *moreLimited = first.clone ();
  (*moreLimited) &= second;
  const bool firstIsEquallyOrMoreLimited = ((*moreLimited) == first);
  delete moreLimited;
  return firstIsEquallyOrMoreLimited;
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
