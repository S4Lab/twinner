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

#ifndef ETG_GRAPH_H
#define ETG_GRAPH_H

#include "Edge.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

typedef std::pair<VertexSet, EdgeList> Graph;

std::ostream &operator<< (std::ostream &out, const Graph &g);

}
}
}
}
}

#endif /* Graph.h */
