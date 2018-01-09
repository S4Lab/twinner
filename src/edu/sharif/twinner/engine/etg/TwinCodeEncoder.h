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

#ifndef TWIN_CODE_ENCODER_H
#define TWIN_CODE_ENCODER_H

#include "edu/sharif/twinner/engine/etg/encoder/Encoder.h"

#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Trace;
}
namespace engine {
namespace etg {

class InstructionNode;
class Variable;

class TwinCodeEncoder {
public:
  typedef edu::sharif::twinner::trace::Trace Trace;
  typedef const Trace ConstTrace;
  typedef edu::sharif::twinner::engine::etg::encoder
  ::Encoder::AddrToSizeMap AddrToSizeMap;
  typedef edu::sharif::twinner::engine::etg::encoder
  ::Encoder::MemoryValueMap MemoryValueMap;

private:
  const MemoryValueMap &initialValues;
  InstructionNode *root;

  AddrToSizeMap addressToSize;

public:
  TwinCodeEncoder (const std::list < ConstTrace * > &traces,
      const MemoryValueMap &initialValues,
      InstructionNode *root);

  void encodeToFile (std::ofstream &fileout);
};

}
}
}
}
}

#endif /* TwinCodeEncoder.h */
