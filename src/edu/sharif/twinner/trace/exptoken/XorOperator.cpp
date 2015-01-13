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

#include "XorOperator.h"

#include "Constant.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

XorOperator::XorOperator () :
    Operator (Operator::XOR) {
}

XorOperator::XorOperator (const XorOperator &ao) :
    Operator (ao) {
}

XorOperator::~XorOperator () {
}

XorOperator *XorOperator::clone () const {
  return new XorOperator (*this);
}

bool XorOperator::doesSupportSimplification () const {
  return true;
}

bool XorOperator::apply (edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  Constant *lastConstantMask = 0;
  edu::sharif::twinner::trace::Expression::Stack &stack = exp->getStack ();
  if (!stack.empty () && dynamic_cast<Constant *> (stack.back ())) {
    lastConstantMask = static_cast<Constant *> (stack.back ());

  } else if (stack.size () > 2 && dynamic_cast<Operator *> (stack.back ())) {
    std::list < ExpressionToken * >::iterator it = stack.end ();
    const Operator *op = static_cast<Operator *> (*--it);
    if (op->getIdentifier () == Operator::XOR) {
      lastConstantMask = dynamic_cast<Constant *> (*--it);
    }
  }
  exp->getLastConcreteValue () ^= *operand;
  if (lastConstantMask) {
    (*operand) ^= lastConstantMask->getValue ();
    lastConstantMask->setValue (*operand);
    delete operand;
    return true;
  } else {
    stack.push_back (new Constant (operand));
    stack.push_back (this);
    return false;
  }
}

void XorOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  dst ^= src;
}

std::string XorOperator::toString () const {
  return "^";
}

bool XorOperator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const XorOperator *> (&token);
}

}
}
}
}
}
