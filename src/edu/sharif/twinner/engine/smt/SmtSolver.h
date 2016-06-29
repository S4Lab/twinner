//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2016 Behnam Momeni
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
   * @return True if and only if constraints are satisfiable.
   */
  virtual bool solveConstraints (
      const std::list < const edu::sharif::twinner::trace::Constraint * > &constraints,
      std::set < const edu::sharif::twinner::trace::exptoken::Symbol * > &symbols) const = 0;

  /**
   * Simplifies the given constraint and returns an equivalent but simpler constraint.
   * Caller is responsible for deleting the instantiated returned constraint object.
   *
   * @param ok False if and only if an error occurs during the simplification.
   * @param constraint The constraint which should be simplified.
   * @return A constraint which is equivalent to given one, but simpler; owned by caller
   */
  virtual std::list < const edu::sharif::twinner::trace::Constraint * >
  simplifyConstraint (bool &ok,
      const edu::sharif::twinner::trace::Constraint *constraint) const = 0;

  /**
   * Same as above method, but allows a list of constraints to be inputted.
   * These constraints will be BOOLEAN-ANDed before simplification. The returned
   * constraint is owned by the caller and should be "delete"ed.
   *
   * @param ok False if and only if an error occurs during the simplification.
   * @param constraints List of constraints that their AND should be simplified.
   * @return A constraints which is equivalent to AND of given ones, but simpler.
   */
  virtual std::list < const edu::sharif::twinner::trace::Constraint * >
  simplifyConstraints (bool &ok,
      std::list < const edu::sharif::twinner::trace::Constraint * >
      constraints) const = 0;

  /**
   * Clears the internal state of the SMT solver.
   * This cancels all asserted constraints.
   */
  virtual void clearState () = 0;

  /**
   * Asserts the given constraint in this SMT solver context.
   * All asserted constraints from the last call to clearState () method
   * are accumulated.
   *
   * @param constraint The constraint to be asserted in this SMT solver context.
   */
  virtual void assertConstraint (const edu::sharif::twinner::trace::Constraint *
      constraint) = 0;

  /**
   * Cancels the effect of the last asserted constraint.
   *
   * If n constraints had been asserted, after this call the SMT solver context
   * will become equivalent to when only n-1 constraints have been asserted.
   */
  virtual void popLastAssertion () = 0;

  /**
   * Checks validity of the given constraint based on asserted constraints in
   * the current context. The constraint is valid if it can be proven to be
   * always true based on the asserted constraints.
   *
   * @param constraint The constraints which its validity is queried.
   * @return true iff the given constraint is always valid in current context.
   */
  virtual bool checkValidity (const edu::sharif::twinner::trace::Constraint *
      constraint) = 0;

  static void init (SmtSolver *instance);
  static SmtSolver *getInstance ();
  void destroy ();
};

}
}
}
}
}

#endif /* SmtSolver.h */
