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

#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

class Logger;
}
namespace trace {

class Constraint;
}
namespace engine {
namespace search {

class TreeNode {

private:
  const int debugId; // FIXME: Remove this field when debug is not required

  TreeNode *parent;
  // front of list: left-most | back of list: right-most
  std::list < TreeNode * > children;
  const edu::sharif::twinner::trace::Constraint *constraint;

public:
  TreeNode (TreeNode *parent = 0, const edu::sharif::twinner::trace::Constraint *c = 0);
  ~TreeNode ();

  /**
   * Adds constraint as right-most child of this node and returns the newly instantiated
   * node. However, if the given constraint was already handled by current right-most
   * child of this node, the responsible node will be returned.
   * @param constraint The constraint which should be added.
   * @return The node (possibly newly instantiated) which is responsible for constraint.
   */
  TreeNode *addConstraint (const edu::sharif::twinner::trace::Constraint *constraint);

  /**
   * Moves downward, traversing right-most children until a leaf is visited. During this
   * traversing, visited constraints are added to the given list.
   * @param clist List of constraints which are visited during the requested traversal.
   * @return A pointer to the right-most and deepest grandchild of this node.
   */
  TreeNode *getRightMostDeepestGrandChild (
      std::list < const edu::sharif::twinner::trace::Constraint * > &clist);

  /**
   * Get next node of the current node assuming the infix visiting order among nodes.
   * Upon visiting leafs, a negated constraint (if not exist currently) is instantiated
   * and added to the tree and the target next node. If the negated node existed,
   * continues DFS and negation to find next node.
   * Constraints are remove upon moving upward and added upon moving downward.
   * If it isn't possible to instantiate a negated node (reaching the root node), null is
   * returned.
   * @param clist Constraints which are in correspondence to root-to-returned-node path.
   * @return The next node which should be visited or null if there was no such node.
   */
  TreeNode *getNextNode (
      std::list < const edu::sharif::twinner::trace::Constraint * > &clist);

  void dumpSubTree (edu::sharif::twinner::util::Logger &logger) const;
};

}
}
}
}
}

#endif	/* TreeNode.h*/
