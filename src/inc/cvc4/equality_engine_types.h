/*********************                                                        */
/*! \file equality_engine_types.h
 ** \verbatim
 ** Original author: Dejan Jovanovic
 ** Major contributors: none
 ** Minor contributors (to current version): Morgan Deters
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief [[ Add one-line brief description here ]]
 **
 ** [[ Add lengthier description here ]]
 ** \todo document this file
 **/

#include "cvc4_private.h"

#include <string>
#include <iostream>
#include <sstream>

namespace CVC4 {
namespace theory {
namespace eq {

/** Default type for the size of the sizes (size_t replacement) */
typedef uint32_t DefaultSizeType;

/** Id of the node */
typedef DefaultSizeType EqualityNodeId;

/** Id of the use list */
typedef DefaultSizeType UseListNodeId;

/** The trigger ids */
typedef DefaultSizeType TriggerId;

/** The equality edge ids */
typedef DefaultSizeType EqualityEdgeId;

/** The null node */
static const EqualityNodeId null_id = (EqualityNodeId)(-1);

/** The null use list node */
static const EqualityNodeId null_uselist_id = (EqualityNodeId)(-1);

/** The null trigger */
static const TriggerId null_trigger = (TriggerId)(-1);

/** The null edge id */
static const EqualityEdgeId null_edge = (EqualityEdgeId)(-1);

/**
 * A reason for a merge. Either an equality x = y, a merge of two
 * function applications f(x1, x2), f(y1, y2) due to congruence, 
 * or a merge of an equality to false due to both sides being
 * (different) constants.
 */
enum MergeReasonType {
  /** Terms were merged due to application of congruence closure */
  MERGED_THROUGH_CONGRUENCE,
  /** Terms were merged due to application of pure equality */
  MERGED_THROUGH_EQUALITY,
  /** Equality was merged to true, due to both sides of equality being in the same class */
  MERGED_THROUGH_REFLEXIVITY,
  /** Equality was merged to false, due to both sides of equality being a constant */
  MERGED_THROUGH_CONSTANTS,
};

inline std::ostream& operator << (std::ostream& out, MergeReasonType reason) {
  switch (reason) {
  case MERGED_THROUGH_CONGRUENCE:
    out << "congruence";
    break;
  case MERGED_THROUGH_EQUALITY:
    out << "pure equality";
    break;
  case MERGED_THROUGH_REFLEXIVITY:
    out << "reflexivity";
    break;
  case MERGED_THROUGH_CONSTANTS:
    out << "constants disequal";
    break;
  default:
    Unreachable();
  }
  return out;
}

/**
 * A candidate for merging two equivalence classes, with the necessary
 * additional information.
 */
struct MergeCandidate {
  EqualityNodeId t1Id, t2Id;
  MergeReasonType type;
  TNode reason;
  MergeCandidate(EqualityNodeId x, EqualityNodeId y, MergeReasonType type, TNode reason)
  : t1Id(x), t2Id(y), type(type), reason(reason) 
  {}
};

/**
 * Just an index into the reasons array, and the number of merges to consume.
 */
struct DisequalityReasonRef {
  DefaultSizeType mergesStart;
  DefaultSizeType mergesEnd;
  DisequalityReasonRef(DefaultSizeType mergesStart = 0, DefaultSizeType mergesEnd = 0)
  : mergesStart(mergesStart), mergesEnd(mergesEnd) {}
};

/** 
 * We maintain uselist where a node appears in, and this is the node
 * of such a list. 
 */
class UseListNode {

private:

  /** The id of the application node where this representative is at */
  EqualityNodeId d_applicationId;

  /** The next one in the class */
  UseListNodeId d_nextUseListNodeId;

public:

  /**
   * Creates a new node, which is in a list of it's own.
   */
  UseListNode(EqualityNodeId nodeId = null_id, UseListNodeId nextId = null_uselist_id)
  : d_applicationId(nodeId), d_nextUseListNodeId(nextId) {}

  /**
   * Returns the next node in the circular list.
   */
  UseListNodeId getNext() const {
    return d_nextUseListNodeId;
  }

  /**
   * Returns the id of the function application.
   */
  EqualityNodeId getApplicationId() const {
    return d_applicationId;
  }
};

/**
 * Main class for representing nodes in the equivalence class. The 
 * nodes are a circular list, with the representative carrying the
 * size. Each individual node carries with itself the uselist of
 * function applications it appears in and the list of asserted 
 * disequalities it belongs to. In order to get these lists one must
 * traverse the entire class and pick up all the individual lists. 
 */
class EqualityNode {

private:

  /** The size of this equivalence class (if it's a representative) */
  DefaultSizeType d_size;

  /** The id (in the eq-manager) of the representative equality node */
  EqualityNodeId d_findId;

  /** The next equality node in this class */
  EqualityNodeId d_nextId;

  /** The use list of this node */
  UseListNodeId d_useList;

public:

  /**
   * Creates a new node, which is in a list of it's own.
   */
  EqualityNode(EqualityNodeId nodeId = null_id)
  : d_size(1)
  , d_findId(nodeId) 
  , d_nextId(nodeId)
  , d_useList(null_uselist_id)
  {}

  /**
   * Returns the requested uselist.
   */
  UseListNodeId getUseList() const {
    return d_useList;
  }

  /**
   * Returns the next node in the class circular list.
   */
  EqualityNodeId getNext() const {
    return d_nextId;
  }

  /**
   * Returns the size of this equivalence class (only valid if this is the representative).
   */
  DefaultSizeType getSize() const {
    return d_size;
  }

  /**
   * Merges the two lists. If add size is true the size of this node is increased by the size of
   * the other node, otherwise the size is decreased by the size of the other node.
   */
  template<bool addSize>
  void merge(EqualityNode& other) {
    EqualityNodeId tmp = d_nextId; d_nextId = other.d_nextId; other.d_nextId = tmp;
    if (addSize) {
      d_size += other.d_size;
    } else {
      d_size -= other.d_size;
    }
  }

  /**
   * Returns the class representative.
   */
  EqualityNodeId getFind() const { return d_findId; }

  /**
   * Set the class representative.
   */
  void setFind(EqualityNodeId findId) { d_findId = findId; }

  /**
   * Note that this node is used in a function application funId, or
   * a negatively asserted equality (dis-equality) with funId. 
   */
  template<typename memory_class>
  void usedIn(EqualityNodeId funId, memory_class& memory) {
    UseListNodeId newUseId = memory.size();
    memory.push_back(UseListNode(funId, d_useList));
    d_useList = newUseId;
  }

  /**
   * For backtracking: remove the first element from the uselist and pop the memory.
   */
  template<typename memory_class>
  void removeTopFromUseList(memory_class& memory) {
    Assert ((int) d_useList == (int)memory.size() - 1);
    d_useList = memory.back().getNext();
    memory.pop_back();
  }
};

/** A pair of ids */
typedef std::pair<EqualityNodeId, EqualityNodeId> EqualityPair;

struct EqualityPairHashFunction {
  size_t operator () (const EqualityPair& pair) const {
    size_t hash = 0;
    hash = 0x9e3779b9 + pair.first;
    hash ^= 0x9e3779b9 + pair.second + (hash << 6) + (hash >> 2);
    return hash;
  }
};

enum FunctionApplicationType {
  /** This application is an equality a = b */
  APP_EQUALITY,
  /** This is a part of an uninterpreted application f(t1, ...., tn) */
  APP_UNINTERPRETED,
  /** This is a part of an interpreted application f(t1, ..., tn) */
  APP_INTERPRETED
};

/**
 * Represents the function APPLY a b. If isEquality is true then it
 * represents the predicate (a = b). Note that since one can not 
 * construct the equality over function terms, the equality and hash 
 * function below are still well defined.
 */
struct FunctionApplication {
  /** Type of application */
  FunctionApplicationType type;
  /** The actual application elements */
  EqualityNodeId a, b;

  /** Construct an application */
  FunctionApplication(FunctionApplicationType type = APP_EQUALITY, EqualityNodeId a = null_id, EqualityNodeId b = null_id)
  : type(type), a(a), b(b) {}

  /** Equality of two applications */
  bool operator == (const FunctionApplication& other) const {
    return type == other.type && a == other.a && b == other.b;
  }

  /** Is this a null application */
  bool isNull() const {
    return a == null_id || b == null_id;
  }

  /** Is this an equality */
  bool isEquality() const {
    return type == APP_EQUALITY;
  }

  /** Is this an interpreted application (equality is special, i.e. not interpreted) */
  bool isInterpreted() const {
    return type == APP_INTERPRETED;
  }

};

struct FunctionApplicationHashFunction {
  size_t operator () (const FunctionApplication& app) const {
    size_t hash = 0;
    hash = 0x9e3779b9 + app.a;
    hash ^= 0x9e3779b9 + app.b + (hash << 6) + (hash >> 2);
    return hash;
  }
};

/**
 * At time of addition a function application can already normalize to something, so
 * we keep both the original, and the normalized version.
 */
struct FunctionApplicationPair {
  FunctionApplication original;
  FunctionApplication normalized;
  FunctionApplicationPair() {}
  FunctionApplicationPair(const FunctionApplication& original, const FunctionApplication& normalized)
  : original(original), normalized(normalized) {}
  bool isNull() const {
    return original.isNull();
  }
};

/**
 * Information about the added triggers.
 */
struct TriggerInfo {
  /** The trigger itself */
  Node trigger;
  /** Polarity of the trigger */
  bool polarity;
  TriggerInfo() {}
  TriggerInfo(Node trigger, bool polarity)
  : trigger(trigger), polarity(polarity) {}
  };


} // namespace eq
} // namespace theory
} // namespace CVC4

