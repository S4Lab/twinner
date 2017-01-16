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

#include "TraceSegmentTerminator.h"
#include "SyscallInvocation.h"
#include "FunctionInvocation.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

TraceSegmentTerminator::TraceSegmentTerminator () {
}

TraceSegmentTerminator::~TraceSegmentTerminator () {
}

TraceSegmentTerminator *TraceSegmentTerminator::loadFromBinaryStream (
    std::ifstream &in) {
  char terminatorMagicString[3];
  in.read (terminatorMagicString, 3);
  if (strncmp (terminatorMagicString, "NTM", 3) == 0) {
    return 0; // no terminator

  } else if (strncmp (terminatorMagicString, "SYS", 3) == 0) {
    return SyscallInvocation::loadFromBinaryStream (in);

  } else if (strncmp (terminatorMagicString, "FUN", 3) == 0) {
    return FunctionInvocation::loadFromBinaryStream (in);

  } else {
    edu::sharif::twinner::util::Logger::error ()
        << "TraceSegmentTerminator::loadFromBinaryStream (...): Unexpected "
        "magic string while loading trace segment terminator"
        " from the binary stream\n";
    abort ();
  }
}

}
}
}
}
