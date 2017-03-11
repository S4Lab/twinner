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

#include "ArithmeticShiftRightOperator.h"

#include "Constant.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

ArithmeticShiftRightOperator::ArithmeticShiftRightOperator () :
    Operator (Operator::ARITHMETIC_SHIFT_RIGHT) {
}

ArithmeticShiftRightOperator::ArithmeticShiftRightOperator (const ArithmeticShiftRightOperator &ao) :
    Operator (ao) {
}

ArithmeticShiftRightOperator::~ArithmeticShiftRightOperator () {
}

ArithmeticShiftRightOperator *ArithmeticShiftRightOperator::clone () const {
  return new ArithmeticShiftRightOperator (*this);
}

bool ArithmeticShiftRightOperator::doesSupportSimplification () const {
  return true;
}

bool ArithmeticShiftRightOperator::isCommutable () const {
  return false;
}

bool ArithmeticShiftRightOperator::apply (
    edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  return Operator::apply
      (exp,
       new edu::sharif::twinner::trace::cv::ConcreteValue64Bits
       (exp->getLastConcreteValue ().getSize ()),
       operand);
}

bool ArithmeticShiftRightOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  dst.arithmeticShiftToRight (src);
  return dst.getCarryBit ();
}

bool ArithmeticShiftRightOperator::apply (
    edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &mid,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  edu::sharif::twinner::trace::cv::ConcreteValue *cv =
      dst.clone (mid.toUint64 ());
  bool overflow = (*cv) != dst;
  apply (*cv, src);
  dst = *cv;
  overflow = overflow || ((*cv) != dst);
  delete cv;
  return overflow;
}

std::string ArithmeticShiftRightOperator::toString () const {
  return "arithmeticShiftToRight";
}

bool ArithmeticShiftRightOperator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const ArithmeticShiftRightOperator *> (&token);
}

}
}
}
}
}
