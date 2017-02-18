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

#include "TreeNode.h"
#include "ConstraintEdge.h"

#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/Snapshot.h"
#include "edu/sharif/twinner/trace/ExecutionTraceSegment.h"

#include "edu/sharif/twinner/engine/smt/SmtSolver.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"
#include "edu/sharif/twinner/util/MemoryManager.h"

#include <sstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

static int lastDebugId = 0;

TreeNode::TreeNode () :
    debugId (++lastDebugId),
    insId (0),
    memoryManager (0),
    snapshot (0),
    segment (0) {
}

TreeNode::TreeNode (ConstraintEdge *p) :
    debugId (++lastDebugId),
    insId (0),
    memoryManager (0),
    snapshot (0),
    segment (0) {
  if (p) {
    parents.push_back (p);
  }
}

void delete_edge (ConstraintEdge * const &edge);

TreeNode::~TreeNode () {
  edu::sharif::twinner::util::foreach (children, delete_edge);
  children.clear ();
}

void delete_edge (ConstraintEdge * const &edge) {
  delete edge;
}

void TreeNode::registerInstructionIdIfRequired (
    const edu::sharif::twinner::trace::Constraint *c,
    const edu::sharif::twinner::util::MemoryManager *m) {
  if (insId) {
    const uint32_t firstInsId = insId;
    const uint32_t secondInsId = c->getCausingInstructionIdentifier ();
    const char *firstIns =
        memoryManager->getPointerToAllocatedMemory (firstInsId);
    const char *secondIns = m->getPointerToAllocatedMemory (secondInsId);
    if (!firstIns || !secondIns || strcmp (firstIns, secondIns) != 0) {
      edu::sharif::twinner::util::Logger::error ()
          << "TreeNode::registerInstructionIdIfRequired (): "
          "inconsistent ins-id values [old-ins-id=" << std::dec << insId
          << ", new-ins-id=" << c->getCausingInstructionIdentifier ()
          << "]\n";
      abort ();
    }
  } else {
    insId = c->getCausingInstructionIdentifier ();
    memoryManager = m;
  }
}

TreeNode *TreeNode::addConstraint (
    const edu::sharif::twinner::trace::Constraint *c,
    const edu::sharif::twinner::util::MemoryManager *m,
    bool performValidityCheck) {
  if (performValidityCheck &&
      edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
      ->checkValidity (c)) {
    return this;
  }
  edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
      ->assertConstraint (c);
  TreeNode *node = this;
  unsigned int depth = 0;
  for (;;) {
    if (node->children.empty ()) {
      return addConstraint (node, depth, c, m, performValidityCheck);
    }
    ConstraintEdge *e = node->children.back ();
    if ((*e->getConstraint ()) != (*c)) {
      if (edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
          ->checkValidity (e->getConstraint ())) {
        node = e->getChild ();
        edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
            ->popLastAssertion ();
        edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
            ->assertConstraint (e->getConstraint ());
        ++depth;
        edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
            ->assertConstraint (c);
        continue;
      }
      return addConstraint (node, depth, c, m, performValidityCheck);
    }
    if (depth > 0) {

      repeat (depth + 1) {
        edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
            ->popLastAssertion ();
      }
      edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
          ->assertConstraint (c);
    }
    return e->getChild ();
  }
}

TreeNode *TreeNode::addConstraint (
    TreeNode *parent,
    unsigned int depth,
    const edu::sharif::twinner::trace::Constraint *c,
    const edu::sharif::twinner::util::MemoryManager *m,
    bool performValidityCheck) {
  if (depth > 0) {
    edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
        ->popLastAssertion ();
    if (performValidityCheck
        && edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
        ->checkValidity (c)) {
      return parent;
    }

    repeat (depth) {
      edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
          ->popLastAssertion ();
    }
    edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
        ->assertConstraint (c);
  }
  parent->registerInstructionIdIfRequired (c, m);
  ConstraintEdge *edge = new ConstraintEdge (parent, c);
  parent->children.push_back (edge);
  TreeNode *node = new TreeNode (edge);
  edge->setChild (node);
  return node;
}

TreeNode *TreeNode::getRightMostDeepestGrandChild (
    std::list < const edu::sharif::twinner::trace::Constraint * > &clist) {
  if (children.empty ()) {
    return this;
  }
  ConstraintEdge *edge = children.back ();
  clist.push_back (edge->getConstraint ());
  return edge->getChild ()->getRightMostDeepestGrandChild (clist);
}

TreeNode *TreeNode::getNextNode (
    std::list < const edu::sharif::twinner::trace::Constraint * > &clist) {
  if (parents.empty ()) {
    return 0;
  }
  TreeNode *node = getRightMostParent ()->getParent ();
  while (node->children.size () > 1) {
    if (node->parents.empty ()) {
      return 0;
    }
    node = node->getRightMostParent ()->getParent ();
    clist.pop_back ();
  }
  const edu::sharif::twinner::trace::Constraint *negatedConstraint =
      clist.back ()->instantiateNegatedConstraint ();
  clist.pop_back ();
  ConstraintEdge *edge = new ConstraintEdge (node, negatedConstraint);
  node->children.push_back (edge);
  TreeNode *n = new TreeNode (edge);
  edge->setChild (n);
  clist.push_back (negatedConstraint);
  return n;
}

const edu::sharif::twinner::util::MemoryManager *TreeNode::getMemoryManager () const {
  return memoryManager;
}

void TreeNode::dumpConstraints (
    edu::sharif::twinner::util::Logger &logger) const {
  for (std::list < ConstraintEdge * >::const_iterator it = children.begin ();
      it != children.end (); ++it) {
    const ConstraintEdge *edge = *it;
    edge->dumpConstraints (logger);
  }
}

void TreeNode::dumpSubTree (edu::sharif::twinner::util::Logger &logger,
    unsigned int pad) const {

  repeat (pad) {
    logger << "  ";
  }
  logger << "Node(" << debugId << "{" << toString () << "}) -> \n";
  for (std::list < ConstraintEdge * >::const_iterator it = children.begin ();
      it != children.end (); ++it) {
    const ConstraintEdge *edge = *it;
    edge->dumpSubTree (logger, pad + 1);
  }
}

bool TreeNode::hasAnyChild () const {
  return !children.empty ();
}

void TreeNode::mergeCriticalAddresses (
    edu::sharif::twinner::trace::Snapshot *sna) {
  if (snapshot == 0) {
    snapshot = sna;
    return;
  }
  snapshot->addCriticalSymbols (sna->getCriticalSymbols ());
}

void TreeNode::registerCorrespondingSegment (
    const edu::sharif::twinner::trace::ExecutionTraceSegment *_segment) {
  segment = _segment;
}

const edu::sharif::twinner::trace::ExecutionTraceSegment *
TreeNode::getSegment () const {
  return segment;
}

const edu::sharif::twinner::trace::Snapshot *TreeNode::getSnapshot () const {
  return snapshot;
}

const std::list < ConstraintEdge * > &TreeNode::getChildren () const {
  return children;
}

void TreeNode::addParent (ConstraintEdge *newParent) {
  parents.push_back (newParent);
}

bool TreeNode::areInstructionsTheSame (const TreeNode *tn) const {
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
  const uint32_t firstInsId = insId;
  const uint32_t secondInsId = tn->insId;
  if (firstInsId == 0 && secondInsId == 0) { // both are tautologies
    return true;
  }
  const char *firstIns =
      memoryManager->getPointerToAllocatedMemory (firstInsId);
  const char *secondIns =
      tn->memoryManager->getPointerToAllocatedMemory (secondInsId);
  return firstIns && secondIns && strcmp (firstIns, secondIns) == 0;
}

ConstraintEdge *TreeNode::getRightMostParent () {
  return parents.empty () ? 0 : parents.back ();
}

const std::list < ConstraintEdge * > &TreeNode::getParents () const {
  return parents;
}

std::string TreeNode::toString () const {
  std::stringstream ss;
  ss << insId;
  if (insId) {
    ss << "; " << memoryManager->getPointerToAllocatedMemory (insId);
  }
  return ss.str ();
}


}
}
}
}
}
