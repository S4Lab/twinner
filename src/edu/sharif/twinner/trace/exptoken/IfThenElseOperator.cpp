//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2018 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "IfThenElseOperator.h"

#include "Constant.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

IfThenElseOperator::IfThenElseOperator () :
    Operator (Operator::IF_THEN_ELSE) {
}

IfThenElseOperator::IfThenElseOperator (const IfThenElseOperator &iteo) :
    Operator (iteo) {
}

IfThenElseOperator::~IfThenElseOperator () {
}

IfThenElseOperator *IfThenElseOperator::clone () const {
  return new IfThenElseOperator (*this);
}

bool IfThenElseOperator::doesSupportSimplification () const {
  return false;
}

bool IfThenElseOperator::isCommutable () const {
  return false;
}

bool IfThenElseOperator::apply (edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  edu::sharif::twinner::util::Logger::error ()
      << "IfThenElseOperator::apply(Expression *, ConcreteValue *):"
      " Not supported operation!\n";
  abort ();
}

bool IfThenElseOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  return false; // ITE does not affect concrete values (keeping then-case val)
}

std::string IfThenElseOperator::toString () const {
  return "?:";
}

bool IfThenElseOperator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const IfThenElseOperator *> (&token);
}

}
}
}
}
}
