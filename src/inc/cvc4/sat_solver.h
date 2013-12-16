/*********************                                                        */
/*! \file sat_solver.h
 ** \verbatim
 ** Original author: Dejan Jovanovic
 ** Major contributors: Morgan Deters, Liana Hadarean
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief SAT Solver.
 **
 ** SAT Solver.
 **/

#include "cvc4_private.h"

#ifndef __CVC4__PROP__SAT_SOLVER_H
#define __CVC4__PROP__SAT_SOLVER_H

#include <string>
#include <stdint.h>
#include "util/statistics_registry.h"
#include "context/cdlist.h"
#include "prop/sat_solver_types.h"

namespace CVC4 {
namespace prop {

class TheoryProxy;

class SatSolver {

public:

  /** Virtual destructor */
  virtual ~SatSolver() { }

  /** Assert a clause in the solver. */
  virtual void addClause(SatClause& clause, bool removable) = 0;

  /**
   * Create a new boolean variable in the solver.
   * @param isTheoryAtom is this a theory atom that needs to be asserted to theory
   * @param preRegister whether to preregister the atom with the theory
   * @param canErase whether the sat solver can safely eliminate this variable
   *
   */
  virtual SatVariable newVar(bool isTheoryAtom, bool preRegister, bool canErase) = 0;

  /** Create a new (or return an existing) boolean variable representing the constant true */
  virtual SatVariable trueVar() = 0;

  /** Create a new (or return an existing) boolean variable representing the constant false */
  virtual SatVariable falseVar() = 0;

  /** Check the satisfiability of the added clauses */
  virtual SatValue solve() = 0;

  /** Check the satisfiability of the added clauses */
  virtual SatValue solve(long unsigned int&) = 0;

  /** Interrupt the solver */
  virtual void interrupt() = 0;

  /** Call value() during the search.*/
  virtual SatValue value(SatLiteral l) = 0;

  /** Call modelValue() when the search is done.*/
  virtual SatValue modelValue(SatLiteral l) = 0;

  /** Get the current assertion level */
  virtual unsigned getAssertionLevel() const = 0;

};/* class SatSolver */


class BVSatSolverInterface: public SatSolver {
public:

  /** Interface for notifications */
  class Notify {
  public:

    virtual ~Notify() {};

    /**
     * If the notify returns false, the solver will break out of whatever it's currently doing
     * with an "unknown" answer.
     */
    virtual bool notify(SatLiteral lit) = 0;

    /**
     * Notify about a learnt clause.
     */
    virtual void notify(SatClause& clause) = 0;
    virtual void safePoint() = 0;
    
  };/* class BVSatSolverInterface::Notify */

  virtual void setNotify(Notify* notify) = 0;

  virtual void markUnremovable(SatLiteral lit) = 0;

  virtual void getUnsatCore(SatClause& unsatCore) = 0;

  virtual void addMarkerLiteral(SatLiteral lit) = 0;

  virtual SatValue propagate() = 0;

  virtual void explain(SatLiteral lit, std::vector<SatLiteral>& explanation) = 0;

  virtual SatValue assertAssumption(SatLiteral lit, bool propagate = false) = 0;

  virtual void popAssumption() = 0;

};/* class BVSatSolverInterface */


class DPLLSatSolverInterface: public SatSolver {
public:
  virtual void initialize(context::Context* context, prop::TheoryProxy* theoryProxy) = 0;

  virtual void push() = 0;

  virtual void pop() = 0;

  virtual bool properExplanation(SatLiteral lit, SatLiteral expl) const = 0;

  virtual void requirePhase(SatLiteral lit) = 0;

  virtual bool flipDecision() = 0;

  virtual bool isDecision(SatVariable decn) const = 0;

};/* class DPLLSatSolverInterface */

inline std::ostream& operator <<(std::ostream& out, prop::SatLiteral lit) {
  out << lit.toString();
  return out;
}

inline std::ostream& operator <<(std::ostream& out, const prop::SatClause& clause) {
  out << "clause:";
  for(unsigned i = 0; i < clause.size(); ++i) {
    out << " " << clause[i];
  }
  out << ";";
  return out;
}

inline std::ostream& operator <<(std::ostream& out, prop::SatValue val) {
  std::string str;
  switch(val) {
  case prop::SAT_VALUE_UNKNOWN:
    str = "_";
    break;
  case prop::SAT_VALUE_TRUE:
    str = "1";
    break;
  case prop::SAT_VALUE_FALSE:
    str = "0";
    break;
  default:
    str = "Error";
    break;
  }

  out << str;
  return out;
}

}/* CVC4::prop namespace */
}/* CVC4 namespace */

#endif /* __CVC4__PROP__SAT_MODULE_H */
