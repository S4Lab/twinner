/*********************                                                        */
/*! \file decision_engine.h
 ** \verbatim
 ** Original author: Kshitij Bansal
 ** Major contributors: none
 ** Minor contributors (to current version): Clark Barrett, Dejan Jovanovic, Morgan Deters, Tim King
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Decision engine
 **
 ** Decision engine
 **/

#include "cvc4_private.h"

#ifndef __CVC4__DECISION__DECISION_ENGINE_H
#define __CVC4__DECISION__DECISION_ENGINE_H

#include <vector>

#include "decision/decision_strategy.h"

#include "expr/node.h"
#include "prop/cnf_stream.h"
#include "prop/prop_engine.h"
#include "prop/sat_solver_types.h"
#include "theory/decision_attributes.h"
#include "util/ite_removal.h"
#include "util/output.h"

using namespace std;
using namespace CVC4::prop;
using namespace CVC4::decision;

namespace CVC4 {

class DecisionEngine {

  vector <DecisionStrategy* > d_enabledStrategies;
  vector <ITEDecisionStrategy* > d_needIteSkolemMap;
  RelevancyStrategy* d_relevancyStrategy;

  typedef context::CDList<Node> AssertionsList;
  AssertionsList d_assertions;

  // PropEngine* d_propEngine;
  CnfStream* d_cnfStream;
  DPLLSatSolverInterface* d_satSolver;

  context::Context* d_satContext;
  context::UserContext* d_userContext;

  // Does decision engine know the answer?
  context::CDO<SatValue> d_result;

  // Disable creating decision engine without required parameters
  DecisionEngine();

  // init/shutdown state
  unsigned d_engineState;    // 0=pre-init; 1=init,pre-shutdown; 2=shutdown
public:
  // Necessary functions

  /** Constructor */
  DecisionEngine(context::Context *sc, context::UserContext *uc);

  /** Destructor, currently does nothing */
  ~DecisionEngine() {
    Trace("decision") << "Destroying decision engine" << std::endl;
  }
  
  // void setPropEngine(PropEngine* pe) {
  //   // setPropEngine should not be called more than once
  //   Assert(d_propEngine == NULL);
  //   Assert(pe != NULL);
  //   d_propEngine = pe;
  // }

  void setSatSolver(DPLLSatSolverInterface* ss) {
    // setPropEngine should not be called more than once
    Assert(d_satSolver == NULL);
    Assert(ss != NULL);
    d_satSolver = ss;
  }

  void setCnfStream(CnfStream* cs) {
    // setPropEngine should not be called more than once
    Assert(d_cnfStream == NULL);
    Assert(cs != NULL);
    d_cnfStream = cs;
  }

  /* enables decision stragies based on options */
  void init();

  /* clears all of the strategies */
  void clearStrategies();


  /**
   * This is called by SmtEngine, at shutdown time, just before
   * destruction.  It is important because there are destruction
   * ordering issues between some parts of the system.
   */
  void shutdown() {
    Assert(d_engineState == 1);
    d_engineState = 2;

    Trace("decision") << "Shutting down decision engine" << std::endl;
    clearStrategies();
  }

  // Interface for External World to use our services

  /** Gets the next decision based on strategies that are enabled */
  SatLiteral getNext(bool &stopSearch) {
    Assert(d_cnfStream != NULL,
           "Forgot to set cnfStream for decision engine?");
    Assert(d_satSolver != NULL,
           "Forgot to set satSolver for decision engine?");

    SatLiteral ret = undefSatLiteral;
    for(unsigned i = 0; 
        i < d_enabledStrategies.size() 
          and ret == undefSatLiteral
          and stopSearch == false; ++i) {
      ret = d_enabledStrategies[i]->getNext(stopSearch);
    }
    return ret;
  }

  /** Is a sat variable relevant */
  bool isRelevant(SatVariable var);

  /**
   * Try to get tell SAT solver what polarity to try for a
   * decision. Return SAT_VALUE_UNKNOWN if it can't help 
   */
  SatValue getPolarity(SatVariable var);

  /** Is the DecisionEngine in a state where it has solved everything? */
  bool isDone() {
    Trace("decision") << "DecisionEngine::isDone() returning "
		      << (d_result != SAT_VALUE_UNKNOWN)
		      << (d_result != SAT_VALUE_UNKNOWN ? "true" : "false")
		      << std::endl;
    return (d_result != SAT_VALUE_UNKNOWN);
  }

  /** */
  Result getResult() {
    switch(d_result.get()) {
    case SAT_VALUE_TRUE: return Result(Result::SAT);
    case SAT_VALUE_FALSE: return Result(Result::UNSAT);
    case SAT_VALUE_UNKNOWN: return Result(Result::SAT_UNKNOWN, Result::UNKNOWN_REASON);
    default: Assert(false, "d_result is garbage");
    }
    return Result();
  }

  /** */
  void setResult(SatValue val) {
    d_result = val;
  }

  // External World helping us help the Strategies

  /** If one of the enabled strategies needs them  */
  /* bool needIteSkolemMap() { */
  /*   return d_needIteSkolemMap.size() > 0; */
  /* } */

  /* add a set of assertions */
  void addAssertions(const vector<Node> &assertions);

  /**
   * add a set of assertions, while providing a mapping between skolem
   * variables and corresponding assertions. It is assumed that all
   * assertions after assertionEnd were generated by ite
   * removal. Hence, iteSkolemMap maps into only these.
   */
  void addAssertions(const vector<Node> &assertions,
                     unsigned assertionsEnd,
                     IteSkolemMap iteSkolemMap);

  /* add a single assertion */
  void addAssertion(Node n);

  // Interface for Strategies to use stuff stored in Decision Engine
  // (which was possibly requested by them on initialization)

  /**
   * Get the assertions. Strategies are notified when these are available. 
   */
  AssertionsList& getAssertions() {
    return d_assertions;
  }


  // Interface for Strategies to get information about External World

  bool hasSatLiteral(TNode n) {
    return d_cnfStream->hasLiteral(n);
  }
  SatLiteral getSatLiteral(TNode n) {
    return d_cnfStream->getLiteral(n);
  }
  SatValue getSatValue(SatLiteral l) {
    return d_satSolver->value(l);
  }
  SatValue getSatValue(TNode n) {
    return getSatValue(getSatLiteral(n));
  }
  Node getNode(SatLiteral l) {
    return d_cnfStream->getNode(l);
  }

private:
  /**
   * Enable a particular decision strategy, also updating
   * corresponding vector<DecisionStrategy*>-s is the engine
   */
  void enableStrategy(DecisionStrategy* ds);

};/* DecisionEngine class */

}/* CVC4 namespace */

#endif /* __CVC4__DECISION__DECISION_ENGINE_H */
