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

#ifndef SMT_SOLVER_H
#define SMT_SOLVER_H

#include <list>
#include <set>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Constraint;

namespace exptoken {

class Symbol;
}
}
namespace engine {
namespace smt {

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
   * These values will be encoded in Symbol objects and will be added to the given set.
   *
   * @param constraints List of constraints which are supposed to be satisfied.
   * @param symbols Set of symbols values of the example assignment (output).
   *
   * @except Throws UnsatisfiableConstraintsException if constraints are unsatisfiable.
   */
  virtual void solveConstraints (
      const std::list < const edu::sharif::twinner::trace::Constraint * > &constraints,
      std::set < const edu::sharif::twinner::trace::exptoken::Symbol * > &symbols) const
  /* @throw (UnsatisfiableConstraintsException) */ = 0;

  /**
   * Simplifies the given constraint and returns an equivalent but simpler constraint.
   * Caller is responsible for deleting the instantiated returned constraint object.
   *
   * @param constraint The constraint which should be simplified.
   * @return A constraint which is equivalent to given one, but simpler; owned by caller
   */
  virtual std::list < const edu::sharif::twinner::trace::Constraint * >
  simplifyConstraint (const edu::sharif::twinner::trace::Constraint *
      constraint) const = 0;

  /**
   * Same as above method, but allows a list of constraints to be inputted.
   * These constraints will be BOOLEAN-ANDed before simplification. The returned
   * constraint is owned by the caller and should be "delete"ed.
   *
   * @param constraints List of constraints that their AND should be simplified.
   * @return A constraints which is equivalent to AND of given ones, but simpler.
   */
  virtual std::list < const edu::sharif::twinner::trace::Constraint * >
  simplifyConstraints (std::list < const edu::sharif::twinner::trace::Constraint * >
      constraints) const = 0;

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
