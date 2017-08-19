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

#ifndef ENCODER_H
#define ENCODER_H

#include "edu/sharif/twinner/util/IndentedStringStream.h"

#include <stdint.h>
#include <map>
#include <set>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Expression;
class ExecutionTraceSegment;
class Constraint;
}
namespace engine {
namespace etg {

class InstructionNode;

namespace encoder {

class Encoder {
protected:
  typedef int Size;
  typedef int Index;
  typedef uint64_t Value;

#ifdef TARGET_IA32E
  typedef uint64_t Address;
#else
  typedef uint32_t Address;
#endif

  typedef std::pair < Address, Size > AddrToSize;

public:
  typedef std::map < Index, std::set < AddrToSize > > AddrToSizeMap;
  typedef std::map < AddrToSize, Value > MemoryValueMap;

protected:
  const AddrToSizeMap &addressToSize;

  Encoder (const AddrToSizeMap &addressToSize);

public:
  virtual ~Encoder ();
};

}
}
}
}
}
}

#endif /* Encoder.h */
