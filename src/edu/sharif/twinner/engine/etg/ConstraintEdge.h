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

#ifndef CONSTRAINT_EDGE_H
#define CONSTRAINT_EDGE_H

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
namespace etg {

class TreeNode;

class ConstraintEdge {
private:
  const int debugId;

  const edu::sharif::twinner::trace::Constraint *constraint;
  TreeNode *parent;
  TreeNode *child;

public:
  ConstraintEdge (TreeNode *p,
      const edu::sharif::twinner::trace::Constraint *c);
  virtual ~ConstraintEdge ();

  void setChild (TreeNode *node);

  const edu::sharif::twinner::trace::Constraint *getConstraint () const;
  TreeNode *getChild ();
  TreeNode *getParent ();

  bool areConstraintsTheSame (const ConstraintEdge *edge) const;

  void dumpConstraints (edu::sharif::twinner::util::Logger &logger) const;
  void dumpSubTree (edu::sharif::twinner::util::Logger &logger,
      unsigned int pad) const;
};

}
}
}
}
}

#endif /* ConstraintEdge.h */
