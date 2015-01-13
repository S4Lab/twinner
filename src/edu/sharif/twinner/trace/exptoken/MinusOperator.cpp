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

#include "MinusOperator.h"

#include "Constant.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

MinusOperator::MinusOperator () :
    Operator (Operator::MINUS) {
}

MinusOperator::MinusOperator (const MinusOperator &ao) :
    Operator (ao) {
}

MinusOperator::~MinusOperator () {
}

MinusOperator *MinusOperator::clone () const {
  return new MinusOperator (*this);
}

bool MinusOperator::doesSupportSimplification () const {
  return true;
}

bool MinusOperator::apply (edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  Constant *lastConstant = 0;
  edu::sharif::twinner::trace::Expression::Stack &stack = exp->getStack ();
  if (!stack.empty () && dynamic_cast<Constant *> (stack.back ())) {
    lastConstant = static_cast<Constant *> (stack.back ());

  } else if (stack.size () > 2 && dynamic_cast<Operator *> (stack.back ())) {
    std::list < ExpressionToken * >::iterator it = stack.end ();
    const Operator *op = static_cast<Operator *> (*--it);
    if (op->getIdentifier () == Operator::ADD) {
      lastConstant = dynamic_cast<Constant *> (*--it);
    }
  }
  exp->getLastConcreteValue () -= *operand;
  if (lastConstant) {
    edu::sharif::twinner::trace::cv::ConcreteValue *cv =
        lastConstant->getValue ().clone ();
    (*cv) -= (*operand);
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

void MinusOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  dst -= src;
}

std::string MinusOperator::toString () const {
  return "-";
}

bool MinusOperator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const MinusOperator *> (&token);
}

}
}
}
}
}
