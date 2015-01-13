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

#include "ArithmeticShiftRight.h"

#include "Constant.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

ArithmeticShiftRight::ArithmeticShiftRight () :
    Operator (Operator::ARITHMETIC_SHIFT_RIGHT) {
}

ArithmeticShiftRight::ArithmeticShiftRight (const ArithmeticShiftRight &ao) :
    Operator (ao) {
}

ArithmeticShiftRight::~ArithmeticShiftRight () {
}

ArithmeticShiftRight *ArithmeticShiftRight::clone () const {
  return new ArithmeticShiftRight (*this);
}

bool ArithmeticShiftRight::doesSupportSimplification () const {
  return true;
}

bool ArithmeticShiftRight::apply (edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  edu::sharif::twinner::trace::Expression::Stack &stack = exp->getStack ();
  exp->getLastConcreteValue ().arithmeticShiftToRight (*operand);
  if (!stack.empty () && dynamic_cast<Constant *> (stack.back ())) {
    Constant *lastConstant = static_cast<Constant *> (stack.back ());
    edu::sharif::twinner::trace::cv::ConcreteValue *cv =
        lastConstant->getValue ().clone ();
    cv->arithmeticShiftToRight (*operand);
    lastConstant->setValue (*cv);
    delete operand;
    delete cv;
    return true;
  } else {
    stack.push_back (new Constant (operand));
    stack.push_back (this);
    return false;
  }
}

void ArithmeticShiftRight::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  dst.arithmeticShiftToRight (src);
}

std::string ArithmeticShiftRight::toString () const {
  return "arithmeticShiftToRight";
}

bool ArithmeticShiftRight::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const ArithmeticShiftRight *> (&token);
}

}
}
}
}
}
