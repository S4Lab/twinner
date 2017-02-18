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

#ifndef CONSTRAINT_TREE_H
#define CONSTRAINT_TREE_H

#include "Vertex.h"

#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Trace;
class Constraint;
}
namespace engine {
namespace etg {

class TreeNode;

class ConstraintTree {
private:
  TreeNode *root;
  TreeNode *tail;
  TreeNode *iterator;
  const edu::sharif::twinner::trace::Constraint * const alwaysTrue;

public:
  ConstraintTree ();
  ~ConstraintTree ();

  void addConstraints (const edu::sharif::twinner::trace::Trace *trace);
  bool getNextConstraintsList (
      std::list < const edu::sharif::twinner::trace::Constraint * > &clist);

  void dumpTree () const;

  const TreeNode *getRoot () const;

  Graph *getEtg () const;

private:
  void mergePath (TreeNode *node);
  bool tryToMergePath (TreeNode *node, TreeNode *target) const;

  typedef std::pair<TreeNode *, TreeNode *> NodePair;
  bool tryToMergePath (const NodePair lowerBound) const;
  bool checkSnapshotsSatisfiability (const NodePair lowerBound) const;

  void mergePath (TreeNode *node, TreeNode *target) const;
};

}
}
}
}
}

#endif /* ConstraintTree.h*/
