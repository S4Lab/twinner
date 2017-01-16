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

#include "AddOperator.h"

#include "Constant.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

AddOperator::AddOperator () :
    Operator (Operator::ADD) {
  initializeSimplificationRules ();
}

AddOperator::AddOperator (const AddOperator &ao) :
    Operator (ao) {
  initializeSimplificationRules ();
}

AddOperator::~AddOperator () {
}

AddOperator *AddOperator::clone () const {
  return new AddOperator (*this);
}

bool AddOperator::doesSupportSimplification () const {
  return true;
}

bool AddOperator::isCommutable () const {
  return true;
}

void AddOperator::initializeSimplificationRules () {
  simplificationRules.push_back (SimplificationRule (Operator::ADD, Operator::ADD));
  simplificationRules.push_back (SimplificationRule (Operator::MINUS, Operator::MINUS));
}

bool AddOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  dst += src;
  return dst.getCarryBit ();
}

std::string AddOperator::toString () const {
  return "+";
}

bool AddOperator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const AddOperator *> (&token);
}

}
}
}
}
}
