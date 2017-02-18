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

#include "ConstraintEdge.h"

#include "TreeNode.h"

#include "edu/sharif/twinner/trace/Constraint.h"

#include "edu/sharif/twinner/util/iterationtools.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

static int lastDebugId = 0;

ConstraintEdge::ConstraintEdge (TreeNode *p,
    const edu::sharif::twinner::trace::Constraint *c) :
    debugId (++lastDebugId),
    constraint (c),
    parent (p),
    child (0) {
}

ConstraintEdge::~ConstraintEdge () {
  delete child;
}

void ConstraintEdge::setChild (TreeNode *node) {
  child = node;
}

const edu::sharif::twinner::trace::Constraint *
ConstraintEdge::getConstraint () const {
  return constraint;
}

TreeNode *ConstraintEdge::getChild () {
  return child;
}

TreeNode *ConstraintEdge::getParent () {
  return parent;
}

bool ConstraintEdge::areConstraintsTheSame (const ConstraintEdge *edge) const {
  return (*constraint) == (*edge->constraint);
}

void ConstraintEdge::dumpConstraints (
    edu::sharif::twinner::util::Logger &logger) const {
  if (constraint) {
    logger << debugId << " -> " << constraint << '\n';
  }
  child->dumpConstraints (logger);
}

void ConstraintEdge::dumpSubTree (edu::sharif::twinner::util::Logger &logger,
    unsigned int pad) const {

  repeat (pad) {
    logger << "  ";
  }
  logger << "Edge(" << debugId << ") -> " << constraint << "\n";
  child->dumpSubTree (logger, pad + 1);
}

}
}
}
}
}
