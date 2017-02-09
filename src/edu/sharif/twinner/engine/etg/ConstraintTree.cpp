//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2017 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "edu/sharif/twinner/pin-wrapper.h"

#include "ConstraintTree.h"

#include "TreeNode.h"

#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/ExecutionTraceSegment.h"
#include "edu/sharif/twinner/trace/Constraint.h"

#include "edu/sharif/twinner/engine/smt/SmtSolver.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"
#include "edu/sharif/twinner/util/MemoryManager.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

ConstraintTree::ConstraintTree () :
    root (new TreeNode ()), iterator (root),
    alwaysTrue (edu::sharif::twinner::trace::Constraint
    ::instantiateTautology (true)) {
}

ConstraintTree::~ConstraintTree () {
  delete root;
  delete alwaysTrue;
  lastInstructionToTreeNodes.clear ();
}

void ConstraintTree::addConstraints (const edu::sharif::twinner::trace::Trace *trace) {
  edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()->clearState ();
  TreeNode *node = root;
  int depth = 0;
  const std::list < edu::sharif::twinner::trace::ExecutionTraceSegment * > &segments =
      trace->getTraceSegments ();
  for (std::list < edu::sharif::twinner::trace::ExecutionTraceSegment * >
      ::const_reverse_iterator it = segments.rbegin (); it != segments.rend (); ++it) {
    const edu::sharif::twinner::trace::ExecutionTraceSegment *segment = *it;
    const std::list < edu::sharif::twinner::trace::Snapshot * > &snapshots =
        segment->getSnapshots ();
    TreeNode const *preSegment = node;
    for (std::list < edu::sharif::twinner::trace::Snapshot * >
        ::const_iterator it = snapshots.begin (); it != snapshots.end (); ++it) {
      edu::sharif::twinner::trace::Snapshot *snapshot = *it;
      const std::list < edu::sharif::twinner::trace::Constraint * > &constraints =
          snapshot->getPathConstraints ();
      TreeNode const *preSnapshot = node;
      for (std::list < edu::sharif::twinner::trace::Constraint * >
          ::const_iterator it2 = constraints.begin (); it2 != constraints.end (); ++it2) {
        const edu::sharif::twinner::trace::Constraint *constraint = *it2;
        TreeNode *next = node->addConstraint
            (constraint, trace->getMemoryManager (), (depth <= 10));
        if (next != node) {
          node = next;
          depth++;
        }
      }
      if (node != preSnapshot) {
        node->mergeCriticalAddresses (snapshot);
      }
    }
    if (node == preSegment) { // Each segment must have at least one constraint
      node = node->addConstraint
          (alwaysTrue, trace->getMemoryManager (), false);
      node->mergeCriticalAddresses (snapshots.back ());
    }
    node->registerCorrespondingSegment (segment);
  }
  mergePath (node);
  iterator = root;
}

void ConstraintTree::mergePath (TreeNode *node) {
  const uint32_t lastInstruction =
      node->getConstraint ()->getCausingInstructionIdentifier ();
  typedef std::multimap<uint32_t, TreeNode *>::const_iterator iterator;
  std::pair<iterator, iterator> range =
      lastInstructionToTreeNodes.equal_range (lastInstruction);
  for (iterator it = range.first; it != range.second; ++it) {
    TreeNode *mergePoint = it->second;
    if (node == mergePoint) {
      return;
    }
  }
  for (iterator it = range.first; it != range.second; ++it) {
    TreeNode *mergePoint = it->second;
    if (tryToMergePath (node, mergePoint)) {
      return;
    }
  }
  lastInstructionToTreeNodes.insert (make_pair (lastInstruction, node));
}

bool ConstraintTree::tryToMergePath (TreeNode *node, TreeNode *target) const {
  if (!areNodesMergable (node, target)) {
    return false;
  }
  const NodePair lowerBound = make_pair (node, target);
  NodePair upperBound;
  return tryToMergePath (lowerBound, upperBound) == MERGED;
}

ConstraintTree::MergeResult ConstraintTree::tryToMergePath (
    const NodePair lowerBound, NodePair &upperBound) const {
  TreeNode *sourceParent = lowerBound.first->getRightMostParent ();
  if (sourceParent == 0
      || sourceParent->getChildren ().size () > 1
      || sourceParent->getParents ().size () > 1) {
    upperBound = lowerBound;
    return checkSnapshotsSatisfiability (lowerBound, upperBound);
  }
  const std::list < TreeNode * > &targetParents = lowerBound.second->getParents ();
  NodePair lastSetUpperBound = lowerBound;
  for (std::list < TreeNode * >::const_iterator it = targetParents.begin ();
      it != targetParents.end (); ++it) {
    TreeNode *targetParent = *it;
    if (!areNodesMergable (sourceParent, targetParent)) {
      continue;
    }
    MergeResult result = tryToMergePath
        (make_pair (sourceParent, targetParent), upperBound);
    if (result == MERGED) {
      return MERGED;
    } else if (result == HAS_NO_SNAPSHOT) {
      lastSetUpperBound = upperBound;
    }
  }
  upperBound = lastSetUpperBound;
  return checkSnapshotsSatisfiability (lowerBound, upperBound);
}

ConstraintTree::MergeResult ConstraintTree::checkSnapshotsSatisfiability (
    const NodePair lowerBound, NodePair upperBound) const {
  const edu::sharif::twinner::trace::Snapshot *snaSource =
      lowerBound.first->getSnapshot ();
  const edu::sharif::twinner::trace::Snapshot *snaTarget =
      lowerBound.second->getSnapshot ();
  if (snaSource && snaTarget) {
    if (snaSource->satisfiesMemoryRegisterCriticalExpressions (snaTarget)) {
      mergePath (upperBound.first, upperBound.second);
      return MERGED;
    } else {
      return HAS_NON_CONFORMING_SNAPSHOT;
    }
  } else if (snaSource == 0 && snaTarget == 0) {
    return HAS_NO_SNAPSHOT;
  }
  return HAS_NON_CONFORMING_SNAPSHOT;
}

void ConstraintTree::mergePath (TreeNode *node, TreeNode *target) const {
  // ASSERT: node has just one parent
  TreeNode *parent = node->getRightMostParent ();
  parent->replaceChild (node, target);
  delete node;
}

bool ConstraintTree::areNodesMergable (const TreeNode *first,
    const TreeNode *second) const {
  // ASSERT: first != second (otherwise `first` would not be inserted)
  return first && second
      && areInstructionsTheSame (first, second)
      && (*first->getConstraint ()) == (*second->getConstraint ());
}

bool ConstraintTree::areInstructionsTheSame (const TreeNode *first,
    const TreeNode *second) const {
  /* TODO: Support self changing instructions as described below
   * At each snapshot point, accumulate the concrete/symbolic values which
   * are stored (at that exact moment) at all locations which are going to
   * be executed during all possible executions paths which are located in
   * the subgraph started at that snapshot point. This information can be
   * acquired (for example) by a two pass execution in which the first run
   * finds out which instruction locations will be executed and the second
   * run reads those interesting addresses at the snapshot point.
   * If all such interesting instruction values (concrete/symbolic) match
   * between two given snapshots, then they will perform the same changes on the
   * equal past instruction values and although instructions can be modified
   * on the fly, they will be the same between the following snapshot points.
   * Then this method should check for the equality of all such values starting
   * from the first/second subgraphs.
   * Note: The performance can be increased in the above scenario by calculating
   * and storing a hash of the above values instead of comparing them in a one
   * by one basis if the possibility of collision is acceptable.
   */
  const uint32_t firstInsId =
      first->getConstraint ()->getCausingInstructionIdentifier ();
  const uint32_t secondInsId =
      second->getConstraint ()->getCausingInstructionIdentifier ();
  if (firstInsId == 0 && secondInsId == 0) { // both are tautologies
    return true;
  }
  const char *firstIns =
      first->getMemoryManager ()->getPointerToAllocatedMemory (firstInsId);
  const char *secondIns =
      second->getMemoryManager ()->getPointerToAllocatedMemory (secondInsId);
  return firstIns && secondIns && strcmp (firstIns, secondIns) == 0;
}

bool ConstraintTree::getNextConstraintsList (
    std::list < const edu::sharif::twinner::trace::Constraint * > &clist) {
  //PRE-CONDITION: clist is in sync with iterator
  if (iterator == root) {
    iterator = iterator->getRightMostDeepestGrandChild (clist);
  }
  iterator = iterator->getNextNode (clist);
  if (!iterator) {
    iterator = root;
    return false;
  }
  return true;
}

void ConstraintTree::dumpTree () const {
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::debug ();
  root->dumpConstraints (logger);
  root->dumpSubTree (logger);
}

const TreeNode *ConstraintTree::getRoot () const {
  return root;
}

Graph *ConstraintTree::getEtg () const {
  Graph *g = new Graph ();
  std::list<const TreeNode *> nodes;
  nodes.push_back (root);
  while (!nodes.empty ()) {
    const TreeNode *it = nodes.front ();
    nodes.pop_front ();
    const Vertex v (it == root ? 0 : it);
    if (g->first.find (v) == g->first.end ()) {
      g->first.insert (v);
      const std::list < TreeNode * > children = it->getChildren ();
      for (std::list < TreeNode * >::const_iterator tn = children.begin ();
          tn != children.end (); ++tn) {
        const Vertex u (*tn);
        g->second.push_back (make_pair (v, u));
        nodes.push_back (*tn);
      }
    }
  }
  return g;
}

}
}
}
}
}
