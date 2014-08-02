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

#include "ConcreteValue64Bits.h"
#include "ConcreteValue128Bits.h"

#include "edu/sharif/twinner/util/memory.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

ConcreteValue32Bits::ConcreteValue32Bits (UINT32 _value) :
ConcreteValueAbstractImp (), value (_value) {
}

ConcreteValue32Bits::ConcreteValue32Bits (const ConcreteValue &cv) :
ConcreteValueAbstractImp () {
  if (dynamic_cast<const ConcreteValue32Bits *> (&cv)) {
    value = static_cast<const ConcreteValue32Bits *> (&cv)->getValue ();
  } else if (dynamic_cast<const ConcreteValue64Bits *> (&cv)) {
    value = UINT32 (static_cast<const ConcreteValue64Bits *> (&cv)->getValue ());
  } else if (dynamic_cast<const ConcreteValue128Bits *> (&cv)) {
    value = UINT32 (static_cast<const ConcreteValue128Bits *> (&cv)->getLsb ());
  } else {
    throw std::runtime_error ("ConcreteValue32Bits instantiation problem "
                              "(copying from unknown type).");
  }
}

ConcreteValue32Bits::~ConcreteValue32Bits () {
}

ConcreteValue32Bits &ConcreteValue32Bits::operator= (UINT64 v) {
  value = UINT32 (v);
  return *this;
}

ConcreteValue32Bits &ConcreteValue32Bits::operator= (const ConcreteValue &v) {
  ConcreteValue32Bits tmp (v);
  value = tmp.value;
  return *this;
}

void ConcreteValue32Bits::saveToBinaryStream (std::ofstream &out) const {
  const UINT32 size = 32;
  out.write ((const char *) &size, sizeof (size));
  out.write ((const char *) &value, sizeof (value));
}

void ConcreteValue32Bits::writeToMemoryAddress (ADDRINT memoryEa) const {
  edu::sharif::twinner::util::writeMemoryContent (memoryEa, value);
}

void ConcreteValue32Bits::writeToRegister (CONTEXT *context, REG reg) const {
  PIN_REGISTER buffer;
  memset (buffer.byte, 0, sizeof (buffer));
  buffer.dword[0] = value;
  edu::sharif::twinner::util::writeRegisterContent (context, reg, buffer.byte);
}

std::basic_ostream<char> &operator<< (std::basic_ostream<char> &stream,
    const ConcreteValue32Bits &me) {
  return stream << "0x" << std::hex << me.value;
}

ConcreteValue32Bits *ConcreteValue32Bits::twosComplement () const {
  return new ConcreteValue32Bits ((~value) + 1);
}

UINT32 ConcreteValue32Bits::getValue () const {
  return value;
}

ConcreteValue *ConcreteValue32Bits::clone (int length) const {
  switch (length) {
  case -1:
  case 32:
    return new ConcreteValue32Bits (value);
  case 64:
    return new ConcreteValue64Bits (value);
  case 128:
    return new ConcreteValue128Bits (0, value);
  default:
    throw std::runtime_error ("ConcreteValue32Bits::clone (length): Unsupported length");
  }
}

ConcreteValue32Bits *ConcreteValue32Bits::loadFromBinaryStream (std::ifstream &in) {
  UINT32 value;
  in.read ((char *) &value, sizeof (value));
  return new ConcreteValue32Bits (value);
}

bool ConcreteValue32Bits::operator> (const ConcreteValue &cv) const {
  if (getSize () < cv.getSize ()) {
    return (cv < (*this));
  }
  const ConcreteValue32Bits tmp (cv);
  return value > tmp.value;
}

bool ConcreteValue32Bits::greaterThan (const ConcreteValue &cv) const {
  if (getSize () < cv.getSize ()) {
    return cv.lessThan (*this);
  }
  const ConcreteValue32Bits tmp (cv);
  if (isNegative ()) {
    if (tmp.isNegative ()) {
      return value > tmp.value;
    } else {
      return false;
    }
  } else {
    if (tmp.isNegative ()) {
      return true;
    } else {
      return value > tmp.value;
    }
  }
}

bool ConcreteValue32Bits::operator== (const ConcreteValue &cv) const {
  if (getSize () < cv.getSize ()) {
    return (cv == (*this));
  }
  const ConcreteValue32Bits tmp (cv);
  return value == tmp.value;
}

ConcreteValue32Bits &ConcreteValue32Bits::operator-= (const ConcreteValue &cv) {
  const ConcreteValue32Bits tmp (cv);
  value -= tmp.value;
  return *this;
}

ConcreteValue32Bits &ConcreteValue32Bits::operator+= (const ConcreteValue &cv) {
  const ConcreteValue32Bits tmp (cv);
  value += tmp.value;
  return *this;
}

ConcreteValue32Bits &ConcreteValue32Bits::operator&= (const ConcreteValue &mask) {
  const ConcreteValue32Bits tmp (mask);
  value &= tmp.value;
  return *this;
}

ConcreteValue32Bits &ConcreteValue32Bits::operator|= (const ConcreteValue &mask) {
  const ConcreteValue32Bits tmp (mask);
  value |= tmp.value;
  return *this;
}

ConcreteValue32Bits &ConcreteValue32Bits::operator*= (const ConcreteValue &mul) {
  const ConcreteValue32Bits tmp (mul);
  value *= tmp.value;
  return *this;
}

ConcreteValue32Bits &ConcreteValue32Bits::operator/= (const ConcreteValue &divisor) {
  const ConcreteValue32Bits tmp (value);
  value /= tmp.value;
  return *this;
}

ConcreteValue32Bits &ConcreteValue32Bits::operator%= (const ConcreteValue &divisor) {
  const ConcreteValue32Bits tmp (divisor);
  value %= tmp.value;
  return *this;
}

ConcreteValue32Bits &ConcreteValue32Bits::operator^= (const ConcreteValue &pattern) {
  const ConcreteValue32Bits tmp (pattern);
  value ^= tmp.value;
  return *this;
}

ConcreteValue32Bits &ConcreteValue32Bits::operator>>= (const ConcreteValue &bits) {
  const ConcreteValue32Bits tmp (bits);
  value >>= tmp.value;
  return *this;
}

ConcreteValue32Bits &ConcreteValue32Bits::operator<<= (const ConcreteValue &bits) {
  const ConcreteValue32Bits tmp (bits);
  value <<= tmp.value;
  return *this;
}

}
}
}
}