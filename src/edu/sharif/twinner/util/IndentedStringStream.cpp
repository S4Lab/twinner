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

#include "IndentedStringStream.h"

#include "edu/sharif/twinner/util/iterationtools.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

IndentedStringStream::IndentedStringStream (int _depth) :
    depth (0), indentation ("") {
  setDepth (_depth);
}

void IndentedStringStream::setDepth (int _depth) {
  if (_depth > depth) {
    const unsigned int diff = _depth - depth;
    std::stringstream ss;

    repeat (diff) {
      ss << '\t';
    }
    indentation += ss.str ();
  } else if (_depth < depth) {
    indentation = indentation.substr (0, _depth);
  }
  depth = _depth;
}

IndentedStringStream &IndentedStringStream::indented () {
  (*this) << (indentation);
  return *this;
}

}
}
}
}
