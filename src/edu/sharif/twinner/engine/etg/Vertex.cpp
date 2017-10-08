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

#include "Vertex.h"

#include <sstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

Vertex::Vertex (const void *_index) :
    index (_index) {
}

template<int n>
std::ostream &operator<< (std::ostream &out, const char str[n]) {
  return out.write (str, n - 1); // skip the terminating null character
}

std::string Vertex::getName () const {
  std::stringstream ss;
  ss << "Node" << std::dec << index;
  return ss.str ();
}

std::ostream &operator<< (std::ostream &out, const Vertex &v) {
  return out << v.getName ()
      << " [label = \"" << v.index << "\"];";
}

bool Vertex::operator< (const Vertex &v) const {
  return index < v.index;
}

}
}
}
}
}
