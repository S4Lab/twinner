//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2018 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "Edge.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

Edge::Edge (Vertex _src, Vertex _dst, std::string _constraint) :
    src (_src), dst (_dst), constraint (_constraint) {
}

std::ostream &operator<< (std::ostream &out, const Edge &e) {
  return out << e.src.getName () << " -> " << e.dst.getName ()
      << " [label = \"" << e.constraint << "\"];";
}

}
}
}
}
}
