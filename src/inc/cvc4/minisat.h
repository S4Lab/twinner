/*********************                                                        */
/*! \file minisat.h
 ** \verbatim
 ** Original author: dejan
 ** Major contributors:
 ** Minor contributors (to current version):
 ** This file is part of the CVC4 prototype.
 ** Copyright (c) 2009, 2010, 2011  The Analysis of Computer Systems Group (ACSys)
 ** Courant Institute of Mathematical Sciences
 ** New York University
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief SAT Solver.
 **
 ** Implementation of the minisat for cvc4.
 **/

#pragma once

#include "prop/sat_solver.h"
#include "prop/sat_solver_registry.h"
#include "prop/minisat/simp/SimpSolver.h"

namespace CVC4 {
namespace prop {

class MinisatSatSolver : public DPLLSatSolverInterface {

  /** The SatSolver used */
  Minisat::SimpSolver* d_minisat;

  /** The SatSolver uses this to communicate with the theories */
  TheoryProxy* d_theoryProxy;

  /** Context we will be using to synchronize the sat solver */
  context::Context* d_context;

public:

  MinisatSatSolver();
  ~MinisatSatSolver();

  static SatVariable     toSatVariable(Minisat::Var var);
  static Minisat::Lit    toMinisatLit(SatLiteral lit);
  static SatLiteral      toSatLiteral(Minisat::Lit lit);
  static SatValue        toSatLiteralValue(bool res);
  static SatValue        toSatLiteralValue(Minisat::lbool res);
  static Minisat::lbool  toMinisatlbool(SatValue val);
  //(Commented because not in use) static bool            tobool(SatValue val);

  static void  toMinisatClause(SatClause& clause, Minisat::vec<Minisat::Lit>& minisat_clause);
  static void  toSatClause    (Minisat::vec<Minisat::Lit>& clause, SatClause& sat_clause);
  static void  toSatClause    (const Minisat::Clause& clause, SatClause& sat_clause);
  void initialize(context::Context* context, TheoryProxy* theoryProxy);

  void addClause(SatClause& clause, bool removable);

  SatVariable newVar(bool isTheoryAtom, bool preRegister, bool canErase);
  SatVariable trueVar() { return d_minisat->trueVar(); }
  SatVariable falseVar() { return d_minisat->falseVar(); }

  SatValue solve();
  SatValue solve(long unsigned int&);

  void interrupt();

  SatValue value(SatLiteral l);

  SatValue modelValue(SatLiteral l);

  bool properExplanation(SatLiteral lit, SatLiteral expl) const;

  /** Incremental interface */

  unsigned getAssertionLevel() const;

  void push();

  void pop();

  void requirePhase(SatLiteral lit);

  bool flipDecision();

  bool isDecision(SatVariable decn) const;

  class Statistics {
  private:
    ReferenceStat<uint64_t> d_statStarts, d_statDecisions;
    ReferenceStat<uint64_t> d_statRndDecisions, d_statPropagations;
    ReferenceStat<uint64_t> d_statConflicts, d_statClausesLiterals;
    ReferenceStat<uint64_t> d_statLearntsLiterals,  d_statMaxLiterals;
    ReferenceStat<uint64_t> d_statTotLiterals;
  public:
    Statistics();
    ~Statistics();
    void init(Minisat::SimpSolver* d_minisat);
  };
  Statistics d_statistics;

};

} // prop namespace
} // cvc4 namespace

