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

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

ShiftLeftOperator::ShiftLeftOperator () :
    Operator (Operator::SHIFT_LEFT) {
}

ShiftLeftOperator::ShiftLeftOperator (const ShiftLeftOperator &ao) :
    Operator (ao) {
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
  if ((*operand) >= 64) {
    edu::sharif::twinner::trace::Expression::Stack &stack = exp->getStack ();
    exp->getLastConcreteValue () <<= *operand;
    stack.push_back (new Constant (operand));
    stack.push_back (this);
    return false;
  } else {
    // shift-to-left by n bits is equivalent to multiplication by 2^n
    UINT64 val = (1ull << operand->toUint64 ());
    delete operand;
    if (val > 1) {
      exp->multiply (val);
    }
    return true;
  }
}

void ShiftLeftOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  dst <<= src;
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
