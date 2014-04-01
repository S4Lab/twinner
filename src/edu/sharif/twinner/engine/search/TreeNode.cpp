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

#include "edu/sharif/twinner/util/iterationtools.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace search {

TreeNode::TreeNode () {
}

void delete_tree_node (TreeNode * const &node);

TreeNode::~TreeNode () {
  edu::sharif::twinner::util::foreach (children, delete_tree_node);
  children.clear ();
}

void delete_tree_node (TreeNode * const &node) {
  delete node;
}

}
}
}
}
}
