//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2014  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "TreeNode.h"

#include "edu/sharif/twinner/trace/Constraint.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace search {

static int lastDebugId = 0;

TreeNode::TreeNode (TreeNode *p, const edu::sharif::twinner::trace::Constraint *c) :
debugId (++lastDebugId),
parent (p), constraint (c) {
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
    const edu::sharif::twinner::trace::Constraint *c) {
  if (c->isTrivial ()) {
    return this;
  }
  if (children.empty () || (*children.back ()->constraint) != (*c)) {
    new TreeNode (this, c);
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
  TreeNode *n = new TreeNode (node, negatedConstraint);
  clist.push_back (negatedConstraint);
  return n;
}

void TreeNode::dumpSubTree (edu::sharif::twinner::util::Logger &logger) const {
  logger << "Node(" << debugId << "): ";
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
