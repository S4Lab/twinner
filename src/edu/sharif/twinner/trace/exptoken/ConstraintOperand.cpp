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

#include "ConstraintOperand.h"

#include "edu/sharif/twinner/trace/Constraint.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue32Bits.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

ConstraintOperand::ConstraintOperand (const ConstraintOperand &co) :
    Operand (co),
    constraint (co.constraint->clone ()) {
}

ConstraintOperand::ConstraintOperand (
    const edu::sharif::twinner::trace::Constraint *c) :
    Operand (),
    constraint (c) {
}

ConstraintOperand::ConstraintOperand (
    const edu::sharif::twinner::trace::Constraint *c, bool _cv) :
    Operand (new edu::sharif::twinner::trace::cv::ConcreteValue32Bits (_cv)),
    constraint (c) {
}

ConstraintOperand::~ConstraintOperand () {
  delete constraint;
}

ConstraintOperand *ConstraintOperand::clone () const {
  return new ConstraintOperand (*this);
}

void ConstraintOperand::saveToBinaryStream (std::ofstream &out) const {
  out.write ("B", 1);
  constraint->saveToBinaryStream (out);
  Operand::saveToBinaryStream (out);
}

ConstraintOperand *ConstraintOperand::loadFromBinaryStream (std::ifstream &in) {
  ConstraintOperand *operand = new ConstraintOperand
      (edu::sharif::twinner::trace::Constraint::loadFromBinaryStream (in));
  operand->Operand::loadFromBinaryStream (in);
  return operand;
}

std::string ConstraintOperand::toString () const {
  std::stringstream ss;
  ss << '(' << constraint->toString () << ')';
  return ss.str ();
}

bool ConstraintOperand::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const ConstraintOperand *> (&token)
      && (*static_cast<const ConstraintOperand *> (&token)->constraint) ==
      (*constraint);
}

bool ConstraintOperand::isConstant () const {
  return false;
}

}
}
}
}
}
