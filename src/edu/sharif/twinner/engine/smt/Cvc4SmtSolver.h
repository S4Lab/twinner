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

#ifndef CVC4_SMT_SOLVER_H
#define CVC4_SMT_SOLVER_H

#include "SmtSolver.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace smt {

class Cvc4SmtSolver : public SmtSolver {

public:
  Cvc4SmtSolver ();
  virtual ~Cvc4SmtSolver ();

  virtual void solveConstraints (
      const std::list < const edu::sharif::twinner::trace::Constraint * > &constraints,
      std::set < const edu::sharif::twinner::trace::exptoken::Symbol * > &symbols) const
  /* @throws (UnsatisfiableConstraintsException) */;

  virtual std::list < const edu::sharif::twinner::trace::Constraint * >
  simplifyConstraint (const edu::sharif::twinner::trace::Constraint *constraint) const;
  virtual std::list < const edu::sharif::twinner::trace::Constraint * >
  simplifyConstraints (
      std::list < const edu::sharif::twinner::trace::Constraint * > constraints) const;
};

}
}
}
}
}

#endif	/* Cvc4SmtSolver.h */
