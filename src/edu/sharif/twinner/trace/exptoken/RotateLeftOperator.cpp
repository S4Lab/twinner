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

#include "RotateLeftOperator.h"

#include "Constant.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

RotateLeftOperator::RotateLeftOperator () :
    Operator (Operator::ROTATE_LEFT) {
}

RotateLeftOperator::RotateLeftOperator (const RotateLeftOperator &ao) :
    Operator (ao) {
}

RotateLeftOperator::~RotateLeftOperator () {
}

RotateLeftOperator *RotateLeftOperator::clone () const {
  return new RotateLeftOperator (*this);
}

bool RotateLeftOperator::doesSupportSimplification () const {
  return true;
}

bool RotateLeftOperator::isCommutable () const {
  return false;
}

bool RotateLeftOperator::apply (edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  return Operator::apply
      (exp,
       new edu::sharif::twinner::trace::cv::ConcreteValue64Bits
       (exp->getLastConcreteValue ().getSize ()),
       operand);
}

bool RotateLeftOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  dst.rotateToLeft (src);
  return dst.getCarryBit ();
}

bool RotateLeftOperator::apply (
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

std::string RotateLeftOperator::toString () const {
  return "rotateToLeft";
}

bool RotateLeftOperator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const RotateLeftOperator *> (&token);
}

}
}
}
}
}
