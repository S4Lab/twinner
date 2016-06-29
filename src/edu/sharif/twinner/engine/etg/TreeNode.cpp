//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2016 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "edu/sharif/twinner/pin-wrapper.h"

#include "TreeNode.h"

#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/ExecutionTraceSegment.h"

#include "edu/sharif/twinner/engine/smt/SmtSolver.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"
#include "edu/sharif/twinner/util/MemoryManager.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

static int lastDebugId = 0;

TreeNode::TreeNode (TreeNode *p, const edu::sharif::twinner::trace::Constraint *c,
    const edu::sharif::twinner::util::MemoryManager *m) :
    debugId (++lastDebugId),
    parent (p), constraint (c), memoryManager (m),
    segment (0) {
  if (p) {
    p->children.push_back (this);
  }
}

void delete_tree_node (TreeNode * const &node);

TreeNode::~TreeNode () {
  edu::sharif::twinner::util::foreach (children, delete_tree_node);
  children.clear ();
}

void delete_tree_node (TreeNode * const &node) {
  delete node;
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
  for (TreeNode *n = this; true; n = n->children.back ()) {
    if (n->children.empty ()) {
      return addConstraint (n, c, m, performValidityCheck);
    } else if ((*n->children.back ()->constraint) != (*c)) {
      if (edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
          ->checkValidity (n->children.back ()->constraint)) {
        continue;
      }
      return addConstraint (n, c, m, performValidityCheck);
    }
    return n->children.back ();
  }
}

TreeNode *TreeNode::addConstraint (TreeNode *parent,
    const edu::sharif::twinner::trace::Constraint *c,
    const edu::sharif::twinner::util::MemoryManager *m,
    bool performValidityCheck) {
  if (performValidityCheck && parent != this) {
    edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
        ->popLastAssertion ();
    edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
        ->assertConstraint (parent->constraint);
    if (edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
        ->checkValidity (c)) {
      return parent;
    }
    edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
        ->popLastAssertion ();
    edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
        ->assertConstraint (c);
  }
  return new TreeNode (parent, c, m);
}

TreeNode *TreeNode::getRightMostDeepestGrandChild (
    std::list < const edu::sharif::twinner::trace::Constraint * > &clist) {
  if (children.empty ()) {
    return this;
  }
  clist.push_back (children.back ()->constraint);
  return children.back ()->getRightMostDeepestGrandChild (clist);
}

TreeNode *TreeNode::getNextNode (
    std::list < const edu::sharif::twinner::trace::Constraint * > &clist) {
  TreeNode *node = parent;
  while (node && node->children.size () > 1) {
    node = node->parent;
    clist.pop_back ();
  }
  if (!node) {
    return 0;
  }
  const edu::sharif::twinner::trace::Constraint *negatedConstraint =
      clist.back ()->instantiateNegatedConstraint ();
  clist.pop_back ();
  TreeNode *n = new TreeNode (node, negatedConstraint, memoryManager);
  clist.push_back (negatedConstraint);
  return n;
}

void TreeNode::dumpConstraints (edu::sharif::twinner::util::Logger &logger) const {
  if (constraint) {
    logger << debugId << " -> " << constraint << '\n';
  }
  for (std::list < TreeNode * >::const_iterator it = children.begin ();
      it != children.end (); ++it) {
    const TreeNode *node = *it;
    node->dumpConstraints (logger);
  }
}

void TreeNode::dumpSubTree (edu::sharif::twinner::util::Logger &logger) const {
  logger << "Node(" << debugId;
  if (constraint) {
    const uint32_t ins = constraint->getCausingInstructionIdentifier ();
    if (ins) {
      logger << "<" << ins << ">; " << memoryManager->getPointerToAllocatedMemory (ins);
    }
  }
  logger << "): ";
  bool first = true;
  for (std::list < TreeNode * >::const_iterator it = children.begin ();
      it != children.end (); ++it) {
    const TreeNode *node = *it;
    if (!first) {
      logger << ", ";
    }
    logger << node->debugId;
    first = false;
  }
  logger << '\n';
  for (std::list < TreeNode * >::const_iterator it = children.begin ();
      it != children.end (); ++it) {
    const TreeNode *node = *it;
    node->dumpSubTree (logger);
  }
}

bool TreeNode::hasAnyChild () const {
  return !children.empty ();
}

void TreeNode::registerCorrespondingSegment (
    const edu::sharif::twinner::trace::ExecutionTraceSegment *_segment) {
  segment = _segment;
}

const edu::sharif::twinner::trace::ExecutionTraceSegment *
TreeNode::getSegment () const {
  return segment;
}

const std::list < TreeNode * > &TreeNode::getChildren () const {
  return children;
}

const edu::sharif::twinner::trace::Constraint *
TreeNode::getConstraint () const {
  return constraint;
}

}
}
}
}
}
