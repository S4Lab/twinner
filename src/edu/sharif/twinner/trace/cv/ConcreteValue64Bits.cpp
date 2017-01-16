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

#include "ConcreteValue64Bits.h"

#include "ConcreteValue8Bits.h"
#include "ConcreteValue16Bits.h"
#include "ConcreteValue32Bits.h"
#include "ConcreteValue128Bits.h"

#include "edu/sharif/twinner/util/memory.h"

#include <sstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace cv {

ConcreteValue64Bits::ConcreteValue64Bits (UINT64 _value) :
    ConcreteValueAbstractImp<64, UINT64> (_value) {
}

ConcreteValue64Bits::ConcreteValue64Bits (const ConcreteValue &cv) :
    ConcreteValueAbstractImp<64, UINT64> (cv.toUint64 ()) {
  cf = cv.getCarryBit ();
}

ConcreteValue64Bits::~ConcreteValue64Bits () {
}

bool ConcreteValue64Bits::writeToRegister (CONTEXT *context,
    LEVEL_BASE::REG reg) const {
  PIN_REGISTER buffer;
  memset (buffer.byte, 0, sizeof (buffer));
  buffer.qword[0] = value;
  return edu::sharif::twinner::util::writeRegisterContent
      (context, reg, buffer.byte);
}

ConcreteValue64Bits *ConcreteValue64Bits::twosComplement () const {
  ConcreteValue64Bits *cv = new ConcreteValue64Bits ((~value) + 1);
  cv->setCarryBit (value != 0);
  return cv;
}

ConcreteValue64Bits *ConcreteValue64Bits::bitwiseNegated () const {
  return new ConcreteValue64Bits (~value);
}

ConcreteValue *ConcreteValue64Bits::signExtended (int length) const {
  if (length != 128) {
    edu::sharif::twinner::util::Logger::error ()
        << "ConcreteValue64Bits::signExtended (length=" << length << "): The "
        "only available target for sign extension from 64-bits is 128-bits\n";
    abort ();
  }
  if (isNegative ()) {
    return new ConcreteValue128Bits (-1, value);
  } else {
    return new ConcreteValue128Bits (0, value);
  }
}

ConcreteValue *ConcreteValue64Bits::realClone (int length) const {
  switch (length) {
  case 8:
    return new ConcreteValue8Bits (UINT8 (value));
  case 16:
    return new ConcreteValue16Bits (UINT16 (value));
  case 32:
    return new ConcreteValue32Bits (UINT32 (value));
  case -1:
  case 64:
    return new ConcreteValue64Bits (value);
  case 128:
    return new ConcreteValue128Bits (0, value);
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "ConcreteValue64Bits::clone (" << length << "):"
        " Unsupported length\n";
    abort ();
  }
}

ConcreteValue64Bits *ConcreteValue64Bits::loadFromBinaryStream (std::ifstream &in) {
  UINT64 value;
  in.read ((char *) &value, sizeof (value));
  return new ConcreteValue64Bits (value);
}

}
}
}
}
}
