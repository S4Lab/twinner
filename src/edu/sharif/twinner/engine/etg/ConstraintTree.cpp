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

#include "ConstraintTree.h"

#include "TreeNode.h"

#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/ExecutionTraceSegment.h"
#include "edu/sharif/twinner/trace/Constraint.h"

#include "edu/sharif/twinner/engine/smt/SmtSolver.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

ConstraintTree::ConstraintTree () :
    root (new TreeNode ()), iterator (root),
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
    const std::list < edu::sharif::twinner::trace::Constraint * > &constraints =
        segment->getPathConstraints ();
    TreeNode const *pre = node;
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
    if (node == pre) { // Each segment must have at least one constraint
      node = node->addConstraint
          (alwaysTrue, trace->getMemoryManager (), false);
    }
    node->registerCorrespondingSegment (segment);
  }
  iterator = root;
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

}
}
}
}
}