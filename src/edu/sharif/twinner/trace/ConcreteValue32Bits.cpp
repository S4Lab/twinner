//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2014  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "ConcreteValue32Bits.h"

#include "ConcreteValue16Bits.h"
#include "ConcreteValue64Bits.h"
#include "ConcreteValue128Bits.h"

#include "edu/sharif/twinner/util/memory.h"

#include <sstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

ConcreteValue32Bits::ConcreteValue32Bits (UINT32 _value) :
ConcreteValueAbstractImp<32, UINT32> (_value) {
}

ConcreteValue32Bits::ConcreteValue32Bits (const ConcreteValue &cv) :
ConcreteValueAbstractImp<32, UINT32> (cv.toUint64 ()) {
}

ConcreteValue32Bits::~ConcreteValue32Bits () {
}

void ConcreteValue32Bits::writeToRegister (CONTEXT *context, REG reg) const {
  PIN_REGISTER buffer;
  memset (buffer.byte, 0, sizeof (buffer));
  buffer.dword[0] = value;
  edu::sharif::twinner::util::writeRegisterContent (context, reg, buffer.byte);
}

ConcreteValue32Bits *ConcreteValue32Bits::twosComplement () const {
  return new ConcreteValue32Bits ((~value) + 1);
}

ConcreteValue *ConcreteValue32Bits::clone (int length) const {
  switch (length) {
  case 16:
    return new ConcreteValue16Bits (UINT16 (value));
  case -1:
  case 32:
    return new ConcreteValue32Bits (value);
  case 64:
    return new ConcreteValue64Bits (value);
  case 128:
    return new ConcreteValue128Bits (0, value);
  default:
    std::stringstream ss;
    ss << "ConcreteValue32Bits::clone (" << length << "): Unsupported length";
    throw std::runtime_error (ss.str ());
  }
}

ConcreteValue32Bits *ConcreteValue32Bits::loadFromBinaryStream (std::ifstream &in) {
  UINT32 value;
  in.read ((char *) &value, sizeof (value));
  return new ConcreteValue32Bits (value);
}

}
}
}
}
