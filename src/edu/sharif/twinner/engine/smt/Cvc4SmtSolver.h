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

#ifndef CVC4_SMT_SOLVER_H
#define CVC4_SMT_SOLVER_H

#include "SmtSolver.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace smt {

class Cvc4SmtSolverState;

class Cvc4SmtSolver : public SmtSolver {
private:
  Cvc4SmtSolverState *state;

public:
  Cvc4SmtSolver ();
  virtual ~Cvc4SmtSolver ();

  virtual bool solveConstraints (
      const std::list < const edu::sharif::twinner::trace::Constraint * > &constraints,
      std::set < const edu::sharif::twinner::trace::exptoken::Symbol * > &symbols) const;

  virtual std::list < const edu::sharif::twinner::trace::Constraint * >
  simplifyConstraint (bool &ok,
      const edu::sharif::twinner::trace::Constraint *constraint) const;
  virtual std::list < const edu::sharif::twinner::trace::Constraint * >
  simplifyConstraints (bool &ok,
      std::list < const edu::sharif::twinner::trace::Constraint * > constraints) const;

  virtual void clearState ();
  virtual void assertConstraint (const edu::sharif::twinner::trace::Constraint *constraint);
  virtual void popLastAssertion ();

  virtual bool checkValidity (const edu::sharif::twinner::trace::Constraint *constraint);
};

}
}
}
}
}

#endif /* Cvc4SmtSolver.h */
