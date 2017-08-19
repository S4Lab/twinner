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

#ifndef CONSTRAINT_ENCODER_H
#define CONSTRAINT_ENCODER_H

#include "NodeEncoder.h"

#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

class ConstraintEdge;

namespace encoder {

class ConstraintEncoder : public Encoder {
public:
  ConstraintEncoder (edu::sharif::twinner::engine::etg::ConstraintEdge *edge,
      const AddrToSizeMap &addressToSize,
      bool bypassConstraint);
  virtual ~ConstraintEncoder ();
};

}
}
}
}
}
}

#endif /* ConstraintEncoder.h */
