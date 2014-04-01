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

#include "ConstraintTree.h"

#include "TreeNode.h"

#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/Constraint.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace search {

ConstraintTree::ConstraintTree () :
root (new TreeNode ()), iterator (root) {
}

ConstraintTree::~ConstraintTree () {
  delete root;
}

void ConstraintTree::addConstraints (const edu::sharif::twinner::trace::Trace *trace) {

}

bool ConstraintTree::getNextConstraintsList (
    std::list < const edu::sharif::twinner::trace::Constraint * > &clist) {
  return false;
}

}
}
}
}
}
