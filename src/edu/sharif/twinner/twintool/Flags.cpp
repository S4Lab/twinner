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

#include "Flags.h"

#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/ConcreteValue.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

Flags::Flags () :
    leftExp (0), rightExp (0), op (UNDEFINED_OPGROUP),
    of (DEFAULT_FSTATE), df (CLEAR_FSTATE), sf (DEFAULT_FSTATE), zf (DEFAULT_FSTATE),
    pf (DEFAULT_FSTATE), cf (DEFAULT_FSTATE) {
}

Flags::~Flags () {
  delete leftExp;
  delete rightExp;
}

bool Flags::getDirectionFlag () const {
  return df == SET_FSTATE;
}

void Flags::setFlags (OperationGroup operation,
    edu::sharif::twinner::trace::Expression *exp1,
    edu::sharif::twinner::trace::Expression *exp2) {
  delete leftExp;
  leftExp = exp1;
  delete rightExp;
  rightExp = exp2;

  op = operation;
  of = sf = zf = pf = cf = DEFAULT_FSTATE;
}

edu::sharif::twinner::trace::Constraint *Flags::instantiateConstraintForZeroCase (
    bool &zero, uint32_t instruction) const {
  return edu::sharif::twinner::trace::Constraint::instantiateEqualConstraint
      (zero, leftExp, rightExp, instruction);
}

edu::sharif::twinner::trace::Constraint *Flags::instantiateConstraintForLessOrEqualCase (
    bool &lessOrEqual, uint32_t instruction) const {
  return edu::sharif::twinner::trace::Constraint::instantiateLessOrEqualConstraint
      (lessOrEqual, leftExp, rightExp, instruction);
}

edu::sharif::twinner::trace::Constraint *Flags::instantiateConstraintForLessCase (
    bool &less, uint32_t instruction) const {
  return edu::sharif::twinner::trace::Constraint::instantiateLessConstraint
      (less, leftExp, rightExp, instruction);
}

edu::sharif::twinner::trace::Constraint *Flags::instantiateConstraintForBelowOrEqualCase (
    bool &belowOrEqual, uint32_t instruction) const {
  return edu::sharif::twinner::trace::Constraint::instantiateBelowOrEqualConstraint
      (belowOrEqual, leftExp, rightExp, instruction);
}

edu::sharif::twinner::trace::Constraint *Flags::instantiateConstraintForBelowCase (
    bool &below, uint32_t instruction) const {
  return edu::sharif::twinner::trace::Constraint::instantiateBelowConstraint
      (below, leftExp, rightExp, instruction);
}

edu::sharif::twinner::trace::Constraint *Flags::instantiateConstraintForSignCase (
    bool &sign, uint32_t instruction) const {
  return edu::sharif::twinner::trace::Constraint::instantiateSignConstraint
      (sign, leftExp, rightExp, instruction);
}

}
}
}
}
