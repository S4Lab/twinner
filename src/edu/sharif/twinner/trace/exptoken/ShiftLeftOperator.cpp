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
}

Operator::SimplificationStatus ShiftLeftOperator::deepSimplify (
    edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  edu::sharif::twinner::trace::Expression::Stack &stack = exp->getStack ();
  std::list < ExpressionToken * >::iterator it = stack.end ();
  const Operator *bitwiseOrOrBitwiseAndOp = static_cast<Operator *> (*--it);
  if (bitwiseOrOrBitwiseAndOp->getIdentifier () == Operator::BITWISE_OR
      || bitwiseOrOrBitwiseAndOp->getIdentifier () == Operator::BITWISE_AND) {
    Constant *mask = dynamic_cast<Constant *> (*--it);
    if (mask) {
      Operator *shiftOp = dynamic_cast<Operator *> (*--it);
      if (shiftOp && shiftOp->getIdentifier () == Operator::SHIFT_LEFT) {
        Constant *first = dynamic_cast<Constant *> (*--it);
        if (first) {
          // ((Z << first) [|&] mask) << operand
          edu::sharif::twinner::trace::cv::ConcreteValue *cv =
              mask->getValue ().clone (128);
          (*cv) <<= (*operand);
          (*cv) >>= (*operand);
          const bool shiftingMaskDoesNotCausePrecisionLoss = ((*cv) == mask->getValue ());
          if (shiftingMaskDoesNotCausePrecisionLoss) {
            (*cv) <<= (*operand);
            edu::sharif::twinner::trace::cv::ConcreteValue *cv64 = cv->clone (64);
            if ((*cv) == (*cv64)) {
              delete cv;
              cv = cv64;
            } else {
              delete cv64;
            }
            stack.pop_back (); // removes bitwiseOrOrBitwiseAndOp
            stack.pop_back (); // removes mask
            exp->shiftToLeft (operand); // concrete value is invariant after this op.
            const bool opIsBitwiseOr =
                (bitwiseOrOrBitwiseAndOp->getIdentifier () == Operator::BITWISE_OR);
            delete bitwiseOrOrBitwiseAndOp;
            delete mask;
            if (opIsBitwiseOr) {
              exp->bitwiseOr (cv);
            } else {
              exp->bitwiseAnd (cv);
            }
            return COMPLETED;
          }
          delete cv;
        }
      }
    }
  }
  return CAN_NOT_SIMPLIFY;
}

std::string ShiftLeftOperator::toString () const {
  return "logicalShiftToLeft";
}

bool ShiftLeftOperator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const ShiftLeftOperator *> (&token);
}

}
}
}
}
}
