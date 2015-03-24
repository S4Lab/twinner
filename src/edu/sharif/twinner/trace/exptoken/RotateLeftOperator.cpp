//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2015  Behnam Momeni
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

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

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

bool RotateLeftOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  dst.rotateToLeft (src);
  return dst.getCarryBit ();
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
