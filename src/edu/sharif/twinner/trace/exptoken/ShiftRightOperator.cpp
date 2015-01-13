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

#include "ShiftRightOperator.h"

#include "Constant.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

ShiftRightOperator::ShiftRightOperator () :
    Operator (Operator::SHIFT_RIGHT) {
}

ShiftRightOperator::ShiftRightOperator (const ShiftRightOperator &ao) :
    Operator (ao) {
}

ShiftRightOperator::~ShiftRightOperator () {
}

ShiftRightOperator *ShiftRightOperator::clone () const {
  return new ShiftRightOperator (*this);
}

bool ShiftRightOperator::doesSupportSimplification () const {
  return true;
}

bool ShiftRightOperator::apply (edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  if ((*operand) >= 64) {
    edu::sharif::twinner::trace::Expression::Stack &stack = exp->getStack ();
    exp->getLastConcreteValue () >>= *operand;
    if (!stack.empty () && dynamic_cast<Constant *> (stack.back ())) {
      Constant *lastConstant = static_cast<Constant *> (stack.back ());
      edu::sharif::twinner::trace::cv::ConcreteValue *cv =
          lastConstant->getValue ().clone ();
      (*cv) >>= (*operand);
      lastConstant->setValue (*cv);
      delete operand;
      delete cv;
      return true; // means that this operator is not used and can be deleted.
    } else {
      stack.push_back (new Constant (operand));
      stack.push_back (this);
      return false; // means that this operator is owned by exp since now
    }
  } else {
    // shift-to-right by n bits is equivalent to division by 2^n
    const UINT64 val = (1ull << operand->toUint64 ());
    delete operand;
    if (val > 1) {
      exp->divide (val);
    }
    return true; // means that this operator is not used and can be deleted.
  }
}

void ShiftRightOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  dst >>= src;
}

std::string ShiftRightOperator::toString () const {
  return ">>";
}

bool ShiftRightOperator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const ShiftRightOperator *> (&token);
}

}
}
}
}
}
