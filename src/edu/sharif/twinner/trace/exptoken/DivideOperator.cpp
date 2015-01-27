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

#include "DivideOperator.h"

#include "Constant.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

DivideOperator::DivideOperator () :
    Operator (Operator::DIVIDE) {
}

DivideOperator::DivideOperator (const DivideOperator &ao) :
    Operator (ao) {
}

DivideOperator::~DivideOperator () {
}

DivideOperator *DivideOperator::clone () const {
  return new DivideOperator (*this);
}

bool DivideOperator::doesSupportSimplification () const {
  return true;
}

void DivideOperator::initializeSimplificationRules () {
  simplificationRules.push_back
      (SimplificationRule (Operator::MULTIPLY, Operator::DIVIDE));
}

void DivideOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  dst /= src;
}

std::string DivideOperator::toString () const {
  return "/";
}

bool DivideOperator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const DivideOperator *> (&token);
}

}
}
}
}
}
