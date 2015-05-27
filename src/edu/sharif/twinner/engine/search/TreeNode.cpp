//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2015  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "TreeNode.h"

#include "edu/sharif/twinner/trace/Constraint.h"

#include "edu/sharif/twinner/engine/smt/SmtSolver.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"
#include "edu/sharif/twinner/util/MemoryManager.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace search {

static int lastDebugId = 0;

TreeNode::TreeNode (TreeNode *p, const edu::sharif::twinner::trace::Constraint *c,
    const edu::sharif::twinner::util::MemoryManager *m) :
debugId (++lastDebugId),
parent (p), constraint (c), memoryManager (m) {
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
    const edu::sharif::twinner::util::MemoryManager *m) {
  if (c->isTrivial ()) {
    return this;
  }
  std::list < const edu::sharif::twinner::trace::Constraint * > sc =
      edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
      ->simplifyConstraint (c);
  const bool isTrivial = sc.empty ()
      || (sc.size () == 1 && sc.front ()->isTrivial ());
  while (!sc.empty ()) {
    delete sc.back ();
    sc.pop_back ();
  }
  if (isTrivial) {
    return this;
  }
  if (children.empty () || (*children.back ()->constraint) != (*c)) {
    new TreeNode (this, c, m);
  }
  return children.back ();
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

}
}
}
}
}
