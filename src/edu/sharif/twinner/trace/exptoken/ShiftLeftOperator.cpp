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
