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

#include "SignExtendOperator.h"

#include "Constant.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

SignExtendOperator::SignExtendOperator () :
    Operator (Operator::SIGN_EXTEND) {
}

SignExtendOperator::SignExtendOperator (const SignExtendOperator &ao) :
    Operator (ao) {
}

SignExtendOperator::~SignExtendOperator () {
}

SignExtendOperator *SignExtendOperator::clone () const {
  return new SignExtendOperator (*this);
}

bool SignExtendOperator::doesSupportSimplification () const {
  return true;
}

bool SignExtendOperator::isCommutable () const {
  return false;
}

bool SignExtendOperator::apply (edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  return Operator::apply
      (exp,
       new edu::sharif::twinner::trace::cv::ConcreteValue64Bits
       (exp->getLastConcreteValue ().getSize ()),
       operand);
}

bool SignExtendOperator::apply (
    edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  edu::sharif::twinner::trace::cv::ConcreteValue *cv =
      dst.signExtended (src.toUint64 ());
  dst = *cv;
  const bool overflow = (*cv) != dst;
  delete cv;
  return overflow;
}

bool SignExtendOperator::apply (
    edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &mid,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  edu::sharif::twinner::trace::cv::ConcreteValue *cv =
      dst.clone (mid.toUint64 ());
  bool overflow = (*cv) != dst;
  edu::sharif::twinner::trace::cv::ConcreteValue *se =
      cv->signExtended (src.toUint64 ());
  delete cv;
  dst = *se;
  overflow = overflow || ((*se) != dst);
  delete se;
  return overflow;
}

std::string SignExtendOperator::toString () const {
  return "signExtend";
}

bool SignExtendOperator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const SignExtendOperator *> (&token);
}


}
}
}
}
}
