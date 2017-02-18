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
#include "ConstraintEdge.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

ConstraintTree::ConstraintTree () :
    root (new TreeNode ()),
    tail (0),
    iterator (root),
    alwaysTrue (edu::sharif::twinner::trace::Constraint
    ::instantiateTautology (true)) {
}

ConstraintTree::~ConstraintTree () {
  delete root;
  delete alwaysTrue;
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
  if (!tail || tail == node) {
    tail = node;
    return;
  }
  if (tryToMergePath (node, tail)) {
    return;
  }
  edu::sharif::twinner::util::Logger::error ()
      << "ConstraintTree::mergePath (): Cannot merge node with the tail!\n";
  abort ();
}

bool ConstraintTree::tryToMergePath (TreeNode *node, TreeNode *target) const {
  if (!node->areInstructionsTheSame (target)) {
    return false;
  }
  return tryToMergePath (make_pair (node, target));
}

bool ConstraintTree::tryToMergePath (const NodePair lowerBound) const {
  ConstraintEdge *sourceParentEdge = lowerBound.first->getRightMostParent ();
  TreeNode *sourceParent = sourceParentEdge ? sourceParentEdge->getParent () : 0;
  if (sourceParent == 0
      || sourceParent->getChildren ().size () > 1
      || sourceParent->getParents ().size () > 1) {
    return checkSnapshotsSatisfiability (lowerBound);
  }
  const std::list < ConstraintEdge * > &targetParents =
      lowerBound.second->getParents ();
  for (std::list < ConstraintEdge * >::const_iterator it = targetParents.begin ();
      it != targetParents.end (); ++it) {
    ConstraintEdge *targetParentEdge = *it;
    TreeNode *targetParent = targetParentEdge->getParent ();
    if (!sourceParent->areInstructionsTheSame (targetParent)
        || !sourceParentEdge->areConstraintsTheSame (targetParentEdge)) {
      continue;
    }
    if (tryToMergePath (make_pair (sourceParent, targetParent))) {
      return true;
    }
  }
  return checkSnapshotsSatisfiability (lowerBound);
}

bool ConstraintTree::checkSnapshotsSatisfiability (
    const NodePair lowerBound) const {
  const edu::sharif::twinner::trace::Snapshot *snaSource =
      lowerBound.first->getSnapshot ();
  const edu::sharif::twinner::trace::Snapshot *snaTarget =
      lowerBound.second->getSnapshot ();
  if (snaSource && snaTarget) {
    if (snaSource->satisfiesMemoryRegisterCriticalExpressions (snaTarget)) {
      mergePath (lowerBound.first, lowerBound.second);
      return true;
    }
  }
  return false;
}

void ConstraintTree::mergePath (TreeNode *node, TreeNode *target) const {
  // ASSERT: node has just one parent
  ConstraintEdge *parent = node->getRightMostParent ();
  target->addParent (parent);
  parent->setChild (target);
  delete node;
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
      const std::list < ConstraintEdge * > children = it->getChildren ();
      for (std::list < ConstraintEdge * >::const_iterator edge =
          children.begin (); edge != children.end (); ++edge) {
        TreeNode *tn = (*edge)->getChild ();
        const Vertex u (tn);
        g->second.push_back (make_pair (v, u));
        nodes.push_back (tn);
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
