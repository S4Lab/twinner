//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2018 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "edu/sharif/twinner/pin-wrapper.h"

#include "ExecutionTraceGraph.h"

#include "InstructionNode.h"

#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/ExecutionTraceSegment.h"
#include "edu/sharif/twinner/trace/TraceSegmentTerminator.h"
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

ExecutionTraceGraph::ExecutionTraceGraph () :
    root (new InstructionNode ()),
    tail (0),
    iterator (root),
    alwaysTrue (edu::sharif::twinner::trace::Constraint
    ::instantiateTautology (true)) {
}

ExecutionTraceGraph::~ExecutionTraceGraph () {
  delete root;
  delete alwaysTrue;
}

TraceCriticalAddressInfo ExecutionTraceGraph::addConstraints (
    const edu::sharif::twinner::trace::Trace *trace) {
  TraceCriticalAddressInfo info;
  edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()->clearState ();
  InstructionNode *node = root;
  ConstraintEdge *edge = 0;
  int depth = 0;
  const std::list < edu::sharif::twinner::trace::ExecutionTraceSegment * > &segments =
      trace->getTraceSegments ();
  for (std::list < edu::sharif::twinner::trace::ExecutionTraceSegment * >
      ::const_reverse_iterator it = segments.rbegin (); it != segments.rend (); ++it) {
    const edu::sharif::twinner::trace::ExecutionTraceSegment *segment = *it;
    const std::list < edu::sharif::twinner::trace::Snapshot * > &snapshots =
        segment->getSnapshots ();
    InstructionNode const *preSegment = node;
    for (std::list < edu::sharif::twinner::trace::Snapshot * >
        ::const_iterator it = snapshots.begin (); it != snapshots.end (); ++it) {
      edu::sharif::twinner::trace::Snapshot *snapshot = *it;
      const std::list < edu::sharif::twinner::trace::Constraint * > &constraints =
          snapshot->getPathConstraints ();
      TraceCriticalAddressInfo::ConstraintIndices constraintsIndices;
      TraceCriticalAddressInfo::ConstraintIndex constraintIndex = 0;
      InstructionNode const *preSnapshot = node;
      for (std::list < edu::sharif::twinner::trace::Constraint * >
          ::const_iterator it2 = constraints.begin (); it2 != constraints.end (); ++it2) {
        const edu::sharif::twinner::trace::Constraint *constraint = *it2;
        InstructionNode *next = node->addConstraint
            (constraint, trace->getMemoryManager (), (depth <= 20), edge);
        if (next != node) {
          constraintsIndices.push_back (constraintIndex);
          node = next;
          depth++;
        }
        constraintIndex++;
      }
      info.effectiveConstraints.insert (make_pair (snapshot, constraintsIndices));
      if (node != preSnapshot) {
        info.nodeSnapshotPairs.push_back (make_pair (node, snapshot));
      }
    }
    if (node == preSegment) { // Each segment must have at least one constraint
      node = node->addConstraint
          (alwaysTrue, trace->getMemoryManager (), false, edge);
      info.nodeSnapshotPairs.push_back (make_pair (node, snapshots.back ()));
    }
    edge->registerCorrespondingSegment (segment);
  }
  return info;
}

void ExecutionTraceGraph::mergePath (
    const std::list < TraceCriticalAddressInfo::NodeSnapshotPair > &pairs) {
  InstructionNode *node = 0;
  for (std::list < TraceCriticalAddressInfo::NodeSnapshotPair >
      ::const_iterator it = pairs.begin (); it != pairs.end (); ++it) {
    node = it->first;
    edu::sharif::twinner::trace::Snapshot *snapshot = it->second;
    node->mergeCriticalAddresses (snapshot);
  }
  mergePath (node);
  iterator = root;
}

void ExecutionTraceGraph::mergePath (InstructionNode *node) {
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

bool ExecutionTraceGraph::tryToMergePath (InstructionNode *node, InstructionNode *target) const {
  if (!node->areInstructionsTheSame (target)) {
    return false;
  }
  return tryToMergePath (make_pair (node, target));
}

bool ExecutionTraceGraph::tryToMergePath (const NodePair lowerBound) const {
  ConstraintEdge *sourceParentEdge = lowerBound.first->getRightMostParent ();
  InstructionNode *sourceParent = sourceParentEdge ? sourceParentEdge->getParent () : 0;
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
    InstructionNode *targetParent = targetParentEdge->getParent ();
    if (!sourceParent->areInstructionsTheSame (targetParent)
        || !sourceParentEdge->areConstraintEdgesCompatible (targetParentEdge)) {
      continue;
    }
    if (tryToMergePath (make_pair (sourceParent, targetParent))) {
      return true;
    }
  }
  return checkSnapshotsSatisfiability (lowerBound);
}

bool ExecutionTraceGraph::checkSnapshotsSatisfiability (
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

void ExecutionTraceGraph::mergePath (InstructionNode *node, InstructionNode *target) const {
  // ASSERT: node has just one parent
  ConstraintEdge *parent = node->getRightMostParent ();
  target->addParent (parent);
  parent->setChild (target);
  delete node;
}

void ExecutionTraceGraph::markAbortedExecution () {
  iterator->markAsAborted ();
  iterator = root;
}

void ExecutionTraceGraph::markUnreachablePath () {
  iterator->markUnreachablePath ();
}

bool ExecutionTraceGraph::getNextConstraintsList (
    std::list < InstructionNode * > &nlist,
    std::list < const edu::sharif::twinner::trace::Constraint * > &clist) {
  //PRE-CONDITION: clist is in sync with iterator
  if (iterator == root) {
    iterator = iterator->getRightMostDeepestGrandChild (nlist, clist);
  }
  iterator = iterator->getNextNode (nlist, clist);
  if (!iterator) {
    iterator = root;
    return false;
  }
  return true;
}

void ExecutionTraceGraph::dumpTree () const {
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::debug ();
  root->dumpConstraints (logger);
  root->dumpSubTree (logger);
}

InstructionNode *ExecutionTraceGraph::getRoot () const {
  return root;
}

Graph *ExecutionTraceGraph::getEtg () const {
  Graph *g = new Graph ();
  std::list<const InstructionNode *> nodes;
  nodes.push_back (root);
  while (!nodes.empty ()) {
    const InstructionNode *it = nodes.front ();
    nodes.pop_front ();
    const Vertex v (it == root ? 0 : it, it->isAborted ());
    if (g->first.find (v) == g->first.end ()) {
      g->first.insert (v);
      if (it->isAborted ()) {
        continue;
      }
      const std::list < ConstraintEdge * > children = it->getChildren ();
      for (std::list < ConstraintEdge * >::const_iterator edge =
          children.begin (); edge != children.end (); ++edge) {
        ConstraintEdge *ce = *edge;
        InstructionNode *tn = ce->getChild ();
        if (tn->isUnreachable ()) {
          continue;
        }
        nodes.push_back (tn);
        const Vertex u (tn);
        const edu::sharif::twinner::trace::ExecutionTraceSegment *segment =
            ce->getSegment ();
        if (segment) {
          const edu::sharif::twinner::trace::TraceSegmentTerminator *term =
              segment->getTerminator ();
          if (term) {
            const Vertex z (term, term->toString ());
            g->first.insert (z);
            g->second.push_back
                (Edge (v, z, ce->getConstraint ()->toCompactString ()));
            g->second.push_back (Edge (z, u, ""));
            continue;
          }
        }
        g->second.push_back
            (Edge (v, u, ce->getConstraint ()->toCompactString ()));
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
