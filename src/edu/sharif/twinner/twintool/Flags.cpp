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

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

Flags::Flags () :
exp (0) {
}

Flags::~Flags () {
  if (exp) {
    delete exp;
  }
}

void Flags::setFlags (edu::sharif::twinner::trace::Expression *_exp) {
  if (exp) {
    delete exp;
  }
  exp = _exp;
}

const edu::sharif::twinner::trace::Expression *
Flags::getFlagsUnderlyingExpression () const {
  return exp;
}

edu::sharif::twinner::trace::Constraint *
Flags::instantiateConstraintForZeroFlag (bool zfIsSet) const {
  return new edu::sharif::twinner::trace::Constraint
      (getFlagsUnderlyingExpression (),
       zfIsSet ?
       edu::sharif::twinner::trace::Constraint::ZERO :
       edu::sharif::twinner::trace::Constraint::NON_ZERO);
}

edu::sharif::twinner::trace::Constraint *
Flags::instantiateConstraintForBelowOrEqual () const {
  const edu::sharif::twinner::trace::Expression *exp = getFlagsUnderlyingExpression ();
  const edu::sharif::twinner::trace::ConcreteValue &cv = exp->getLastConcreteValue ();
  return new edu::sharif::twinner::trace::Constraint
      (exp, (cv == 0 || cv.isNegative (cv.getSize ())) ?
       edu::sharif::twinner::trace::Constraint::NON_POSITIVE :
       edu::sharif::twinner::trace::Constraint::POSITIVE);
}

}
}
}
}
