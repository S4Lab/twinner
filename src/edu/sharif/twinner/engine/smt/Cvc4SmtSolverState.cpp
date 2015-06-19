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

#include "Cvc4SmtSolverState.h"

#include "ConstraintToCvc4ExprConverter.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace smt {

Cvc4SmtSolverState::Cvc4SmtSolverState () :
em (new ExprManager ()), smt (new SmtEngine (em)),
symbols (new std::map<std::string, Expr> ()) {
  /*
   * QF_ means disable quantifiers
   * BV means enable bit-vectors
   */
  smt->setLogic ("QF_BV");
  smt->setOption ("produce-models", true);
}

Cvc4SmtSolverState::~Cvc4SmtSolverState () {
  delete symbols;
  delete smt;
  delete em;
}

void Cvc4SmtSolverState::assertConstraints (
    std::list < const edu::sharif::twinner::trace::Constraint * > constraints) {
  ConstraintToCvc4ExprConverter converter (*em, false, constraints);
  Expr cvc4Constraint = converter.convert (*symbols);
  smt->assertFormula (cvc4Constraint);
}

bool Cvc4SmtSolverState::checkValidity (
    std::list < const edu::sharif::twinner::trace::Constraint * > constraints) {
  ConstraintToCvc4ExprConverter converter (*em, false, constraints);
  Expr cvc4Constraint = converter.convert (*symbols);
  Result res = smt->query (cvc4Constraint);
  return res.isValid ();
}

}
}
}
}
}
