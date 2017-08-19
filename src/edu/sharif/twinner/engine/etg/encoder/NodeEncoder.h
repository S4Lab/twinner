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

#ifndef NODE_ENCODER_H
#define NODE_ENCODER_H

#include "Encoder.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

class InstructionNode;

namespace encoder {

class ConstraintEncoder;

class NodeEncoder : public Encoder {
public:
  NodeEncoder (const AddrToSizeMap &addressToSize);
  virtual ~NodeEncoder ();
};

}
}
}
}
}
}

#endif /* NodeEncoder.h */
