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

#include "IndentedStringStream.h"

#include "edu/sharif/twinner/util/iterationtools.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

IndentedStringStream::IndentedStringStream () :
    depth (0), indentation ("") {
}

void IndentedStringStream::incrementDepth () {
  indentation += '\t';
  ++depth;
}

void IndentedStringStream::decrementDepth () {
  indentation.erase (0, 1);
  --depth;
}

IndentedStringStream &IndentedStringStream::indented () {
  (*this) << (indentation);
  return *this;
}

}
}
}
}
