//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef SMT_SOLVER_H
#define SMT_SOLVER_H

#include <list>
#include <set>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Constraint;
class MemoryEmergedSymbol;
}
namespace engine {
namespace smt {

/**
 * This exception indicates that constraints are unsatisfiable simultaneously.
 */
class UnsatisfiableConstraintsException : public std::exception {

public:

  const char *what () const throw () {
    return "Constraints are unsatisfiable simultaneously.";
  }
};

/**
 * @interface Satisfiability Modulo Theories (SMT) solver is an interface to
 * interact (bridge pattern) with external SMT solver programs/libraries.
 * It also implemnets singleton functionality to hide implementation more transparent.
 */
class SmtSolver {

private:
  static SmtSolver *me;

protected:
  SmtSolver ();
  virtual ~SmtSolver ();

public:
  /**
   * Passes list of constraints (using AND between different constraints) to the real
   * SMT solver implementation. It tries to find an assignment of concrete values to
   * different symbols/variables to make all constraints satisfiable simultaneously.
   * These values will be encoded in MemoryEmergedSymbol objects and will be added to
   * the given set.
   * Values of symbols which are not represented by MemoryEmergedSymbol class, are
   * ignored.
   * @param constraints List of constraints which are supposed to be satisfied.
   * @param symbols Set of symbols values of the example assignment (output).
   * 
   * @except Throws UnsatisfiableConstraintsException if constraints are unsatisfiable.
   */
  virtual void solveConstraints (
      const std::list < const edu::sharif::twinner::trace::Constraint * > &constraints,
      std::set < const edu::sharif::twinner::trace::MemoryEmergedSymbol * > &symbols)
  throw (UnsatisfiableConstraintsException) = 0;

  static void init (SmtSolver *instance);
  static SmtSolver *getInstance ();
  void destroy ();
};

}
}
}
}
}

#endif	/* SmtSolver.h */
