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

#ifndef ETG_VERTEX_H
#define ETG_VERTEX_H

#include <utility>
#include <list>
#include <set>
#include <iostream>

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

class Vertex;

typedef std::set<Vertex> VertexSet;

class Vertex {
private:
  const void *index;
  std::string label;

public:
  Vertex (const void *index);
  Vertex (const void *index, std::string label);

  std::string getName () const;
  friend std::ostream &operator<< (std::ostream &out, const Vertex &v);

  bool operator< (const Vertex &v) const;
};

}
}
}
}
}

#endif /* Vertex.h */
