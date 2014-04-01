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
namespace engine {
namespace search {

class TreeNode {

private:
  std::list < TreeNode * > children;

public:
  TreeNode ();
  ~TreeNode ();
};

}
}
}
}
}

#endif	/* TreeNode.h*/
