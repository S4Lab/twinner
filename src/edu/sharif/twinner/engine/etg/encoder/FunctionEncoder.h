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

#ifndef FUNCTION_ENCODER_H
#define FUNCTION_ENCODER_H

#include "NodeEncoder.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {
namespace encoder {

class FunctionEncoder : public NodeEncoder {
private:
  const int functionIndex;

  bool firstVisit;

public:
  FunctionEncoder (const AddrToSizeMap &addressToSize);
  virtual ~FunctionEncoder ();

  virtual void finalizeInitialization ();

  virtual void encode (IndentedStream &body, IndentedStream &preamble,
      int index, bool inMain);
};

}
}
}
}
}
}

#endif /* FunctionEncoder.h */
