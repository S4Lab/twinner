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

#include "ConcreteValue16Bits.h"

#include "ConcreteValue8Bits.h"
#include "ConcreteValue32Bits.h"
#include "ConcreteValue64Bits.h"
#include "ConcreteValue128Bits.h"

#include "edu/sharif/twinner/util/memory.h"

#include <sstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace cv {

ConcreteValue16Bits::ConcreteValue16Bits (UINT16 _value) :
    ConcreteValueAbstractImp<16, UINT16> (_value) {
}

ConcreteValue16Bits::ConcreteValue16Bits (const ConcreteValue &cv) :
    ConcreteValueAbstractImp<16, UINT16> (cv.toUint64 ()) {
  cf = cv.getCarryBit ();
}

ConcreteValue16Bits::~ConcreteValue16Bits () {
}

bool ConcreteValue16Bits::writeToRegister (CONTEXT *context,
    LEVEL_BASE::REG reg) const {
  PIN_REGISTER buffer;
  memset (buffer.byte, 0, sizeof (buffer));
  buffer.word[0] = value;
  return edu::sharif::twinner::util::writeRegisterContent
      (context, reg, buffer.byte);
}

ConcreteValue16Bits *ConcreteValue16Bits::twosComplement () const {
  ConcreteValue16Bits *cv = new ConcreteValue16Bits ((~value) + 1);
  cv->setCarryBit (value != 0);
  return cv;
}

ConcreteValue16Bits *ConcreteValue16Bits::bitwiseNegated () const {
  return new ConcreteValue16Bits (~value);
}

ConcreteValue *ConcreteValue16Bits::realClone (int length) const {
  switch (length) {
  case 8:
    return new ConcreteValue8Bits (UINT8 (value));
  case -1:
  case 16:
    return new ConcreteValue16Bits (value);
  case 32:
    return new ConcreteValue32Bits (value);
  case 64:
    return new ConcreteValue64Bits (value);
  case 128:
    return new ConcreteValue128Bits (0, value);
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "ConcreteValue16Bits::clone (" << length << "):"
        " Unsupported length\n";
    abort ();
  }
}

ConcreteValue16Bits *ConcreteValue16Bits::loadFromBinaryStream (std::ifstream &in) {
  UINT16 value;
  in.read ((char *) &value, sizeof (value));
  return new ConcreteValue16Bits (value);
}

}
}
}
}
}
