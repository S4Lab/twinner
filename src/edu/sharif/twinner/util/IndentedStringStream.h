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

#ifndef INDENTED_STRING_STREAM_H
#define INDENTED_STRING_STREAM_H

#include <sstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

class IndentedStringStream : public std::stringstream {
private:
  int depth;
  std::string indentation;

public:
  IndentedStringStream ();

  void incrementDepth ();
  void decrementDepth ();

  IndentedStringStream &indented ();
};

}
}
}
}

#endif /* IndentedStringStream.h */
