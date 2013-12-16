/*********************                                                        */
/*! \file theory_arrays.h
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: Dejan Jovanovic, Clark Barrett
 ** Minor contributors (to current version): Tim King, Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Theory of arrays
 **
 ** Theory of arrays.
 **/

#include "cvc4_private.h"

#ifndef __CVC4__THEORY__ARRAYS__THEORY_ARRAYS_H
#define __CVC4__THEORY__ARRAYS__THEORY_ARRAYS_H

#include "theory/theory.h"
#include "theory/arrays/array_info.h"
#include "util/statistics_registry.h"
#include "theory/uf/equality_engine.h"
#include "context/cdhashmap.h"
#include "context/cdhashset.h"
#include "context/cdqueue.h"

namespace CVC4 {
namespace theory {
namespace arrays {

/**
 * Decision procedure for arrays.
 *
 * Overview of decision procedure:
 *
 * Preliminary notation:
 *   Stores(a)  = {t | a ~ t and t = store( _ _ _ )}
 *   InStores(a) = {t | t = store (b _ _) and a ~ b }
 *   Indices(a) = {i | there exists a term b[i] such that a ~ b or store(b i v)}
 *   ~ represents the equivalence relation based on the asserted equalities in the
 *   current context.
 *
 * The rules implemented are the following:
 *             store(b i v)
 *     Row1 -------------------
 *          store(b i v)[i] = v
 *
 *           store(b i v)  a'[j]
 *     Row ---------------------- [ a' ~ store(b i v) or a' ~ b ]
 *           i = j OR a[j] = b[j]
 *
 *          a  b same kind arrays
 *     Ext ------------------------ [ a!= b in current context, k new var]
 *           a = b OR a[k] != b[k]p
 *
 *
 *  The Row1 one rule is implemented implicitly as follows:
 *     - for each store(b i v) term add the following equality to the congruence
 *       closure store(b i v)[i] = v
 *     - if one of the literals in a conflict is of the form store(b i v)[i] = v
 *       remove it from the conflict
 *
 *  Because new store terms are not created, we need to check if we need to
 *  instantiate a new Row axiom in the following cases:
 *     1. the congruence relation changes (i.e. two terms get merged)
 *         - when a new equality between array terms a = b is asserted we check if
 *           we can instantiate a Row lemma for all pairs of indices i where a is
 *           being read and stores
 *         - this is only done during full effort check
 *     2. a new read term is created either as a consequences of an Ext lemma or a
 *        Row lemma
 *         - this is implemented in the checkRowForIndex method which is called
 *           when preregistering a term of the form a[i].
 *         - as a consequence lemmas are instantiated even before full effort check
 *
 *  The Ext axiom is instantiated when a disequality is asserted during full effort
 *  check. Ext lemmas are stored in a cache to prevent instantiating essentially
 *  the same lemma multiple times.
 */

static inline std::string spaces(int level)
{
  std::string indentStr(level, ' ');
  return indentStr;
}

class TheoryArrays : public Theory {

  /////////////////////////////////////////////////////////////////////////////
  // MISC
  /////////////////////////////////////////////////////////////////////////////

  private:

  /** True node for predicates = true */
  Node d_true;

  /** True node for predicates = false */
  Node d_false;

  // Statistics

  /** number of Row lemmas */
  IntStat d_numRow;
  /** number of Ext lemmas */
  IntStat d_numExt;
  /** number of propagations */
  IntStat d_numProp;
  /** number of explanations */
  IntStat d_numExplain;
  /** calls to non-linear */
  IntStat d_numNonLinear;
  /** splits on array variables */
  IntStat d_numSharedArrayVarSplits;
  /** splits in getModelVal */
  IntStat d_numGetModelValSplits;
  /** conflicts in getModelVal */
  IntStat d_numGetModelValConflicts;
  /** splits in setModelVal */
  IntStat d_numSetModelValSplits;
  /** conflicts in setModelVal */
  IntStat d_numSetModelValConflicts;
  /** time spent in check() */
  TimerStat d_checkTimer;

  public:

  TheoryArrays(context::Context* c, context::UserContext* u, OutputChannel& out, Valuation valuation, const LogicInfo& logicInfo, QuantifiersEngine* qe);
  ~TheoryArrays();

  void setMasterEqualityEngine(eq::EqualityEngine* eq);

  std::string identify() const { return std::string("TheoryArrays"); }

  /////////////////////////////////////////////////////////////////////////////
  // PREPROCESSING
  /////////////////////////////////////////////////////////////////////////////

  private:

  // PPNotifyClass: dummy template class for d_ppEqualityEngine - notifications not used
  class PPNotifyClass {
  public:
    bool notify(TNode propagation) { return true; }
    void notify(TNode t1, TNode t2) { }
  };

  /** The notify class for d_ppEqualityEngine */
  PPNotifyClass d_ppNotify;

  /** Equaltity engine */
  eq::EqualityEngine d_ppEqualityEngine;

  // List of facts learned by preprocessor - needed for permanent ref for benefit of d_ppEqualityEngine
  context::CDList<Node> d_ppFacts;

  Node preprocessTerm(TNode term);
  Node recursivePreprocessTerm(TNode term);
  bool ppDisequal(TNode a, TNode b);
  Node solveWrite(TNode term, bool solve1, bool solve2, bool ppCheck);

  public:

  PPAssertStatus ppAssert(TNode in, SubstitutionMap& outSubstitutions);
  Node ppRewrite(TNode atom);

  /////////////////////////////////////////////////////////////////////////////
  // T-PROPAGATION / REGISTRATION
  /////////////////////////////////////////////////////////////////////////////

  private:

  /** Literals to propagate */
  context::CDList<Node> d_literalsToPropagate;

  /** Index of the next literal to propagate */
  context::CDO<unsigned> d_literalsToPropagateIndex;

  /** Should be called to propagate the literal.  */
  bool propagate(TNode literal);

  /** Explain why this literal is true by adding assumptions */
  void explain(TNode literal, std::vector<TNode>& assumptions);

  /** For debugging only- checks invariants about when things are preregistered*/
  context::CDHashSet<Node, NodeHashFunction > d_isPreRegistered;

  /** Helper for preRegisterTerm, also used internally */
  void preRegisterTermInternal(TNode n);

  public:

  void preRegisterTerm(TNode n);
  void propagate(Effort e);
  Node explain(TNode n);

  /////////////////////////////////////////////////////////////////////////////
  // SHARING
  /////////////////////////////////////////////////////////////////////////////

  private:

  class MayEqualNotifyClass {
  public:
    bool notify(TNode propagation) { return true; }
    void notify(TNode t1, TNode t2) { }
  };

  /** The notify class for d_mayEqualEqualityEngine */
  MayEqualNotifyClass d_mayEqualNotify;

  /** Equaltity engine for determining if two arrays might be equal */
  eq::EqualityEngine d_mayEqualEqualityEngine;

  public:

  void addSharedTerm(TNode t);
  EqualityStatus getEqualityStatus(TNode a, TNode b);
  void computeCareGraph();
  bool isShared(TNode t)
    { return (d_sharedArrays.find(t) != d_sharedArrays.end()); }


  /////////////////////////////////////////////////////////////////////////////
  // MODEL GENERATION
  /////////////////////////////////////////////////////////////////////////////

  private:

  public:

  void collectModelInfo(TheoryModel* m, bool fullModel);

  /////////////////////////////////////////////////////////////////////////////
  // NOTIFICATIONS
  /////////////////////////////////////////////////////////////////////////////

  private:
  public:

  Node getNextDecisionRequest();

  void presolve();
  void shutdown() { }

  /////////////////////////////////////////////////////////////////////////////
  // MAIN SOLVER
  /////////////////////////////////////////////////////////////////////////////

  public:

  void check(Effort e);

  private:

  // NotifyClass: template helper class for d_equalityEngine - handles call-back from congruence closure module
  class NotifyClass : public eq::EqualityEngineNotify {
    TheoryArrays& d_arrays;
  public:
    NotifyClass(TheoryArrays& arrays): d_arrays(arrays) {}

    bool eqNotifyTriggerEquality(TNode equality, bool value) {
      Debug("arrays::propagate") << spaces(d_arrays.getSatContext()->getLevel()) << "NotifyClass::eqNotifyTriggerEquality(" << equality << ", " << (value ? "true" : "false") << ")" << std::endl;
      // Just forward to arrays
      if (value) {
        return d_arrays.propagate(equality);
      } else {
        return d_arrays.propagate(equality.notNode());
      }
    }

    bool eqNotifyTriggerPredicate(TNode predicate, bool value) {
      Unreachable();
    }

    bool eqNotifyTriggerTermEquality(TheoryId tag, TNode t1, TNode t2, bool value) {
      Debug("arrays::propagate") << spaces(d_arrays.getSatContext()->getLevel()) << "NotifyClass::eqNotifyTriggerTermEquality(" << t1 << ", " << t2 << ", " << (value ? "true" : "false") << ")" << std::endl;
      if (value) {
        if (t1.getType().isArray()) {
          d_arrays.mergeArrays(t1, t2);
          if (!d_arrays.isShared(t1) || !d_arrays.isShared(t2)) {
            return true;
          }
        }
        // Propagate equality between shared terms
        return d_arrays.propagate(t1.eqNode(t2));
      } else {
        if (t1.getType().isArray()) {
          if (!d_arrays.isShared(t1) || !d_arrays.isShared(t2)) {
            return true;
          }
        }
        return d_arrays.propagate(t1.eqNode(t2).notNode());
      }
      return true;
    }

    void eqNotifyConstantTermMerge(TNode t1, TNode t2) {
      Debug("arrays::propagate") << spaces(d_arrays.getSatContext()->getLevel()) << "NotifyClass::eqNotifyConstantTermMerge(" << t1 << ", " << t2 << ")" << std::endl;
      d_arrays.conflict(t1, t2);
    }

    void eqNotifyNewClass(TNode t) { }
    void eqNotifyPreMerge(TNode t1, TNode t2) { }
    void eqNotifyPostMerge(TNode t1, TNode t2) { }
    void eqNotifyDisequal(TNode t1, TNode t2, TNode reason) { }
  };

  /** The notify class for d_equalityEngine */
  NotifyClass d_notify;

  /** Equaltity engine */
  eq::EqualityEngine d_equalityEngine;

  /** Are we in conflict? */
  context::CDO<bool> d_conflict;

  /** Conflict when merging constants */
  void conflict(TNode a, TNode b);

  /** The conflict node */
  Node d_conflictNode;

  /**
   * Context dependent map from a congruence class canonical representative of
   * type array to an Info pointer that keeps track of information useful to axiom
   * instantiation
   */

  Backtracker<TNode> d_backtracker;
  ArrayInfo d_infoMap;

  context::CDQueue<Node> d_mergeQueue;

  bool d_mergeInProgress;

  typedef quad<TNode, TNode, TNode, TNode> RowLemmaType;

  context::CDQueue<RowLemmaType> d_RowQueue;
  context::CDHashSet<RowLemmaType, RowLemmaTypeHashFunction > d_RowAlreadyAdded;

  typedef context::CDHashSet<Node, NodeHashFunction> CDNodeSet;

  CDNodeSet d_sharedArrays;
  CDNodeSet d_sharedOther;
  context::CDO<bool> d_sharedTerms;
  context::CDList<TNode> d_reads;
  std::hash_map<Node, Node, NodeHashFunction> d_skolemCache;
  context::CDO<unsigned> d_skolemIndex;
  std::vector<Node> d_skolemAssertions;

  // The decision requests we have for the core
  context::CDQueue<Node> d_decisionRequests;

  // List of nodes that need permanent references in this context
  context::CDList<Node> d_permRef;
  context::CDList<Node> d_modelConstraints;
  context::CDHashSet<Node, NodeHashFunction > d_lemmasSaved;
  std::vector<Node> d_lemmas;

  Node getSkolem(TNode ref, const std::string& name, const TypeNode& type, const std::string& comment, bool makeEqual = true);
  Node mkAnd(std::vector<TNode>& conjunctions, bool invert = false, unsigned startIndex = 0);
  void setNonLinear(TNode a);
  void checkRIntro1(TNode a, TNode b);
  Node removeRepLoops(TNode a, TNode rep);
  Node expandStores(TNode s, std::vector<TNode>& assumptions, bool checkLoop = false, TNode a = TNode(), TNode b = TNode());
  void mergeArrays(TNode a, TNode b);
  void checkStore(TNode a);
  void checkRowForIndex(TNode i, TNode a);
  void checkRowLemmas(TNode a, TNode b);
  void queueRowLemma(RowLemmaType lem);
  void dischargeLemmas();

  std::vector<Node> d_decisions;
  bool d_inCheckModel;
  int d_topLevel;
  void convertNodeToAssumptions(TNode node, std::vector<TNode>& assumptions, TNode nodeSkip);
  void preRegisterStores(TNode s);
  void checkModel(Effort e);
  bool hasLoop(TNode node, TNode target);
  typedef std::hash_map<Node, Node, NodeHashFunction> NodeMap;
  NodeMap d_getModelValCache;
  NodeMap d_lastVal;
  Node getModelVal(TNode node);
  Node getModelValRec(TNode node);
  bool setModelVal(TNode node, TNode val, bool invert,
                   bool explain, std::vector<TNode>& assumptions);

  public:

  eq::EqualityEngine* getEqualityEngine() {
    return &d_equalityEngine;
  }

};/* class TheoryArrays */

}/* CVC4::theory::arrays namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */

#endif /* __CVC4__THEORY__ARRAYS__THEORY_ARRAYS_H */
