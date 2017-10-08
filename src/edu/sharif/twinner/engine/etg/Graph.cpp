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

#include "Graph.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

std::ostream &operator<< (std::ostream &out, const Graph &g) {
  out << "digraph \"Execution Trace Graph\" {\n"
      "\tlabel=\"Execution Trace Graph\";\n\n";
  for (VertexSet::const_iterator it = g.first.begin ();
      it != g.first.end (); ++it) {
    const Vertex &v = *it;
    out << '\t' << v << '\n';
  }
  for (EdgeList::const_iterator it = g.second.begin ();
      it != g.second.end (); ++it) {
    const Edge &e = *it;
    out << '\t' << e << '\n';
  }
  out << "}\n";
  return out;
}


}
}
}
}
}
