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

#ifndef CVC4_SMT_SOLVER_STATE_H
#define CVC4_SMT_SOLVER_STATE_H

#include "Cvc4Header.h"

#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Constraint;
}
namespace engine {
namespace smt {

class Cvc4SmtSolverState {
private:
  ExprManager *em;
  SmtEngine *smt;
  std::map<std::string, Expr> *symbols;

public:
  Cvc4SmtSolverState ();
  ~Cvc4SmtSolverState ();

  void assertConstraints
  (std::list < const edu::sharif::twinner::trace::Constraint * > constraints);
  bool checkValidity
  (std::list < const edu::sharif::twinner::trace::Constraint * > constraints);
};

}
}
}
}
}

#endif	/* Cvc4SmtSolverState.h */
