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
  NodePair upperBound = findUpperMostMergePoint (lowerBound);
  return tryToMergePath (lowerBound, upperBound) == MERGED;
}

ConstraintTree::MergeResult ConstraintTree::tryToMergePath (
    const NodePair lowerBound, NodePair &upperBound) const {
  const edu::sharif::twinner::trace::Snapshot *snaSource =
      lowerBound.first->getSnapshot ();
  const edu::sharif::twinner::trace::Snapshot *snaTarget =
      lowerBound.second->getSnapshot ();
  if (lowerBound.first != upperBound.first) {
    MergeResult result = tryToMergePath
        (make_pair (lowerBound.first->getParent (),
                    lowerBound.second->getParent ()), upperBound);
    switch (result) {
    case HAS_NO_SNAPSHOT:
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

    case HAS_NON_CONFORMING_SNAPSHOT:
      if (snaSource && snaTarget) {
        if (snaSource->satisfiesMemoryRegisterCriticalExpressions (snaTarget)) {
          mergePath (lowerBound.first, lowerBound.second);
          return MERGED;
        } else {
          return HAS_NON_CONFORMING_SNAPSHOT;
        }
      } else if (snaSource == 0 && snaTarget == 0) {
        upperBound = lowerBound;
        return HAS_NO_SNAPSHOT;
      }
      return HAS_NON_CONFORMING_SNAPSHOT;

    case MERGED:
      return MERGED;

    default:
      edu::sharif::twinner::util::Logger::error ()
          << "ConstraintTree::tryToMergePath (): Unknown MergeResult\n";
      abort ();
    }
  } else {
    if (snaSource && snaTarget) {
      if (snaSource->satisfiesMemoryRegisterCriticalExpressions (snaTarget)) {
        mergePath (lowerBound.first, lowerBound.second);
        return MERGED;
      } else {
        return HAS_NON_CONFORMING_SNAPSHOT;
      }
    } else if (snaSource == 0 && snaTarget == 0) {
      return HAS_NO_SNAPSHOT;
    }
    return HAS_NON_CONFORMING_SNAPSHOT;
  }
}

void ConstraintTree::mergePath (TreeNode *node, TreeNode *target) const {
  TreeNode *parent = node->getParent ();
  parent->replaceChild (node, target);
  delete node;
}

ConstraintTree::NodePair ConstraintTree::findUpperMostMergePoint (
    NodePair current) const {
  TreeNode *parentNode = current.first->getParent ();
  TreeNode *parentTarget = current.second->getParent ();
  while (areNodesMergable (parentNode, parentTarget)) {
    current.first = parentNode;
    current.second = parentTarget;
    parentNode = parentNode->getParent ();
    parentTarget = parentTarget->getParent ();
  }
  return current;
}

bool ConstraintTree::areNodesMergable (const TreeNode *first,
    const TreeNode *second) const {
  // ASSERT: first != second (otherwise `first` would not be inserted)
  return first && second
      && first->getConstraint ()->getCausingInstructionIdentifier ()
      == second->getConstraint ()->getCausingInstructionIdentifier ()
      && (*first->getConstraint ()) == (*second->getConstraint ());
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
