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

#include "edu/sharif/twinner/pin-wrapper.h"

#include "Cvc4SmtSolverState.h"

#include "ConstraintToCvc4ExprConverter.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace smt {

Cvc4SmtSolverState::Cvc4SmtSolverState () {
}

Cvc4SmtSolverState::~Cvc4SmtSolverState () {
  for (std::list < const edu::sharif::twinner::trace::Constraint * >
      ::const_iterator it = assertions.begin ();
      it != assertions.end (); ++it) {
    const edu::sharif::twinner::trace::Constraint *c = *it;
    delete c;
  }
  assertions.clear ();
}

void Cvc4SmtSolverState::assertConstraints (
    std::list < const edu::sharif::twinner::trace::Constraint * > cons) {
  for (std::list < const edu::sharif::twinner::trace::Constraint * >
      ::const_iterator it = cons.begin (); it != cons.end (); ++it) {
    const edu::sharif::twinner::trace::Constraint *c = *it;
    assertions.push_back (c->clone ());
  }
}

void Cvc4SmtSolverState::popLastAssertion () {
  const edu::sharif::twinner::trace::Constraint *c = assertions.back ();
  delete c;
  assertions.pop_back ();
}

bool Cvc4SmtSolverState::checkValidity (
    std::list < const edu::sharif::twinner::trace::Constraint * > cons) {
  ExprManager em;
  SmtEngine smt (&em);
  /*
   * QF_ means disable quantifiers
   * BV means enable bit-vectors
   */
  smt.setLogic ("QF_BV");
  smt.setOption ("produce-models", true);
  //smt->setOption ("trace", "smt");
  //smt->setOption ("trace", "limit");
  //smt->setOption ("verbosity", 10);
  std::map<std::string, Expr> symbols;
  bool ok;
  if (!assertions.empty ()) {
    ConstraintToCvc4ExprConverter assertionsConverter (em, true, assertions);
    Expr assertionsCvc4Constraint = assertionsConverter.convert (ok, symbols);
    if (!ok) {
      return false;
    }
    smt.assertFormula (assertionsCvc4Constraint);
  }
  ConstraintToCvc4ExprConverter converter (em, true, cons);
  Expr cvc4Constraint = converter.convert (ok, symbols);
  if (!ok) {
    return false;
  }
  Result res = smt.query (cvc4Constraint);
  return res.isValid ();
}

}
}
}
}
}
