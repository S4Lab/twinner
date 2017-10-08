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

#ifndef ETG_EDGE_H
#define ETG_EDGE_H

#include "Vertex.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

class Edge;

typedef std::list<Edge> EdgeList;

class Edge {
private:
  Vertex src;
  Vertex dst;

public:
  Edge (Vertex src, Vertex dst);

  friend std::ostream &operator<< (std::ostream &out, const Edge &v);
};

}
}
}
}
}

#endif /* Edge.h */

