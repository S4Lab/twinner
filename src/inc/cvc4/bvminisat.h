/*********************                                                        */
/*! \file bvminisat.h
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
 ** Implementation of the minisat for cvc4 (bitvectors).
 **/

#include "cvc4_private.h"

#pragma once

#include "prop/sat_solver.h"
#include "prop/sat_solver_registry.h"
#include "prop/bvminisat/simp/SimpSolver.h"
#include "context/cdo.h"

namespace CVC4 {
namespace prop {

class BVMinisatSatSolver : public BVSatSolverInterface, public context::ContextNotifyObj {

private:

  class MinisatNotify : public BVMinisat::Notify {
    BVSatSolverInterface::Notify* d_notify;
  public:
    MinisatNotify(BVSatSolverInterface::Notify* notify)
    : d_notify(notify)
    {}
    bool notify(BVMinisat::Lit lit) {
      return d_notify->notify(toSatLiteral(lit));
    }
    void notify(BVMinisat::vec<BVMinisat::Lit>& clause) {
      SatClause satClause;
      toSatClause(clause, satClause);
      d_notify->notify(satClause);
    }

    void safePoint() {
      d_notify->safePoint(); 
    }
  };

  BVMinisat::SimpSolver* d_minisat;
  MinisatNotify* d_minisatNotify;

  unsigned d_solveCount;
  unsigned d_assertionsCount;
  context::CDO<unsigned> d_assertionsRealCount;
  context::CDO<unsigned> d_lastPropagation;

protected:

  void contextNotifyPop();

public:

  BVMinisatSatSolver() :
    ContextNotifyObj(NULL, false),
    d_assertionsRealCount(NULL, (unsigned)0),
    d_lastPropagation(NULL, (unsigned)0)
  { Unreachable(); }
  BVMinisatSatSolver(context::Context* mainSatContext);
  ~BVMinisatSatSolver() throw(AssertionException);

  void setNotify(Notify* notify);

  void addClause(SatClause& clause, bool removable);

  SatValue propagate();

  SatVariable newVar(bool isTheoryAtom = false, bool preRegister = false, bool canErase = true);

  SatVariable trueVar() { return d_minisat->trueVar(); }
  SatVariable falseVar() { return d_minisat->falseVar(); }

  void markUnremovable(SatLiteral lit);

  void interrupt();
  
  SatValue solve();
  SatValue solve(long unsigned int&);
  SatValue solve(bool quick_solve);
  void getUnsatCore(SatClause& unsatCore);

  SatValue value(SatLiteral l);
  SatValue modelValue(SatLiteral l);

  void unregisterVar(SatLiteral lit);
  void renewVar(SatLiteral lit, int level = -1);
  unsigned getAssertionLevel() const;


  // helper methods for converting from the internal Minisat representation

  static SatVariable     toSatVariable(BVMinisat::Var var);
  static BVMinisat::Lit    toMinisatLit(SatLiteral lit);
  static SatLiteral      toSatLiteral(BVMinisat::Lit lit);
  static SatValue toSatLiteralValue(bool res);
  static SatValue toSatLiteralValue(BVMinisat::lbool res);

  static void  toMinisatClause(SatClause& clause, BVMinisat::vec<BVMinisat::Lit>& minisat_clause);
  static void  toSatClause    (BVMinisat::vec<BVMinisat::Lit>& clause, SatClause& sat_clause);
  void addMarkerLiteral(SatLiteral lit);

  void explain(SatLiteral lit, std::vector<SatLiteral>& explanation);

  SatValue assertAssumption(SatLiteral lit, bool propagate);
  
  void popAssumption();

  class Statistics {
  public:
    ReferenceStat<uint64_t> d_statStarts, d_statDecisions;
    ReferenceStat<uint64_t> d_statRndDecisions, d_statPropagations;
    ReferenceStat<uint64_t> d_statConflicts, d_statClausesLiterals;
    ReferenceStat<uint64_t> d_statLearntsLiterals,  d_statMaxLiterals;
    ReferenceStat<uint64_t> d_statTotLiterals;
    ReferenceStat<int> d_statEliminatedVars;
    IntStat d_statCallsToSolve;
    BackedStat<double> d_statSolveTime; 
    Statistics();
    ~Statistics();
    void init(BVMinisat::SimpSolver* minisat);
  };

  Statistics d_statistics;
};

}
}




