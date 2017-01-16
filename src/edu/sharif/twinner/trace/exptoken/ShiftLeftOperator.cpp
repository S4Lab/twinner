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

#include "ShiftLeftOperator.h"

#include "Constant.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

ShiftLeftOperator::ShiftLeftOperator () :
    Operator (Operator::SHIFT_LEFT) {
  initializeSimplificationRules ();
}

ShiftLeftOperator::ShiftLeftOperator (const ShiftLeftOperator &ao) :
    Operator (ao) {
  initializeSimplificationRules ();
}

ShiftLeftOperator::~ShiftLeftOperator () {
}

ShiftLeftOperator *ShiftLeftOperator::clone () const {
  return new ShiftLeftOperator (*this);
}

bool ShiftLeftOperator::doesSupportSimplification () const {
  return true;
}

bool ShiftLeftOperator::isCommutable () const {
  return false;
}

bool ShiftLeftOperator::apply (edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  if (operand->isZero ()) {
    delete operand;
    return true;
  }
  return Operator::apply (exp, operand);
}

bool ShiftLeftOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  dst <<= src;
  return dst.getCarryBit ();
}

void ShiftLeftOperator::initializeSimplificationRules () {
  simplificationRules.push_back
      (SimplificationRule (Operator::SHIFT_LEFT, Operator::ADD));
  simplificationRules.push_back
      (SimplificationRule (Operator::SHIFT_RIGHT, Operator::MINUS));
}

Operator::SimplificationStatus ShiftLeftOperator::deepSimplify (
    edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  edu::sharif::twinner::trace::Expression::Stack &stack = exp->getStack ();
  std::list < ExpressionToken * >::iterator it = stack.end ();
  if (skipBitwiseOrAndBitwiseAndOperators (stack.size (), it)) {
    std::list < ExpressionToken * >::iterator masks = it;
    if (++it != stack.end ()) {
      // (Z [|&] mask1 ... [|&] maskn) << operand
      std::list < AppliedMask > appliedMasks;
      if (aggregateMasks (appliedMasks, masks, stack.end (), *operand)) {
        std::list < ExpressionToken * >::iterator rit = stack.end ();
        while (--rit != masks) {
          delete *rit;
        }
        stack.erase (++masks, stack.end ());
        exp->shiftToLeft (operand); // concrete value is invariant at the end
        for (std::list < AppliedMask >::const_iterator am = appliedMasks.begin ();
            am != appliedMasks.end (); ++am) {
          if (am->opIsBitwiseOr) {
            exp->bitwiseOr (am->mask);
          } else {
            exp->bitwiseAnd (am->mask);
          }
        }
        return COMPLETED;
      }
    } else {
      const Operator *multiplyOrDivideOp = static_cast<Operator *> (*--it);
      if (multiplyOrDivideOp->getIdentifier () == Operator::MULTIPLY
          || multiplyOrDivideOp->getIdentifier () == Operator::DIVIDE) {
        const Constant *val = dynamic_cast<Constant *> (*--it);
        if (val) {
          int n;
          if (val->getValue ().isCompletePowerOfTwo (&n)) {
            if (multiplyOrDivideOp->getIdentifier () == Operator::MULTIPLY) {
              edu::sharif::twinner::trace::cv::ConcreteValue *cv = operand->clone ();
              (*cv) += n;
              if (cv->getCarryBit ()) {
                delete cv;
              } else {
                stack.pop_back (); // removes multiplyOrDivideOp
                stack.pop_back (); // removes val
                delete multiplyOrDivideOp;
                delete val;
                exp->shiftToLeft (cv);
                return COMPLETED;
              }
            } else {
              edu::sharif::twinner::trace::cv::ConcreteValue *cv = operand->clone ();
              (*cv) -= n;
              stack.pop_back (); // removes multiplyOrDivideOp
              stack.pop_back (); // removes val
              delete multiplyOrDivideOp;
              delete val;
              if (cv->getCarryBit ()) {
                exp->shiftToRight (-cv->toUint64 ());
                delete cv;
              } else {
                exp->shiftToLeft (cv);
              }
              return COMPLETED;
            }
          }
        }
      }
    }
  }
  return CAN_NOT_SIMPLIFY;
}

bool ShiftLeftOperator::skipBitwiseOrAndBitwiseAndOperators (int numberOfItems,
    std::list < ExpressionToken * >::iterator &it) const {
  numberOfItems -= 2;
  for (int i = 0; i < numberOfItems; i += 2) {
    const Operator *bitwiseOrOrBitwiseAndOp = dynamic_cast<Operator *> (*--it);
    if (!bitwiseOrOrBitwiseAndOp) {
      return false;
    }
    if (bitwiseOrOrBitwiseAndOp->getIdentifier () != Operator::BITWISE_OR
        && bitwiseOrOrBitwiseAndOp->getIdentifier () != Operator::BITWISE_AND) {
      return true;
    }
    if (!dynamic_cast<Constant *> (*--it)) {
      return false;
    }
  }
  return false;
}

bool ShiftLeftOperator::aggregateMasks (std::list < AppliedMask > &appliedMasks,
    std::list < ExpressionToken * >::iterator it,
    std::list < ExpressionToken * >::iterator end,
    edu::sharif::twinner::trace::cv::ConcreteValue &shiftAmount) const {
  ++it;
  do {
    const Constant *mask = static_cast<Constant *> (*it);
    ++it;
    const Operator *bitwiseOrOrBitwiseAndOp = static_cast<Operator *> (*it);
    ++it;
    edu::sharif::twinner::trace::cv::ConcreteValue *cv = mask->getValue ().clone (128);
    (*cv) <<= shiftAmount;
    (*cv) >>= shiftAmount;
    const bool shiftingMaskDoesNotCausePrecisionLoss = ((*cv) == mask->getValue ());
    if (shiftingMaskDoesNotCausePrecisionLoss) {
      (*cv) <<= shiftAmount;
      edu::sharif::twinner::trace::cv::ConcreteValue *cv64 = cv->clone (64);
      if ((*cv) == (*cv64)) {
        delete cv;
        cv = cv64;
      } else {
        delete cv64;
      }
      AppliedMask am;
      am.mask = cv;
      am.opIsBitwiseOr =
          (bitwiseOrOrBitwiseAndOp->getIdentifier () == Operator::BITWISE_OR);
      appliedMasks.push_back (am);
    } else {
      delete cv;
      while (!appliedMasks.empty ()) {
        delete appliedMasks.back ().mask;
        appliedMasks.pop_back ();
      }
      return false;
    }
  } while (it != end);
  return true;
}

std::string ShiftLeftOperator::toString () const {
  return "<<";
}

bool ShiftLeftOperator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const ShiftLeftOperator *> (&token);
}

}
}
}
}
}
