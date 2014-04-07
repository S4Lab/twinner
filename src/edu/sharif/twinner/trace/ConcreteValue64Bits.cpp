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

#include "ConcreteValue64Bits.h"

#include "ConcreteValue128Bits.h"

#include "edu/sharif/twinner/util/memory.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

ConcreteValue64Bits::ConcreteValue64Bits (UINT64 _value) :
ConcreteValue (), value (_value) {
}

ConcreteValue64Bits::ConcreteValue64Bits (const ConcreteValue &cv) :
ConcreteValue () {
  if (dynamic_cast<const ConcreteValue64Bits *> (&cv)) {
    value = static_cast<const ConcreteValue64Bits *> (&cv)->getValue ();
  } else if (dynamic_cast<const ConcreteValue128Bits *> (&cv)) {
    value = static_cast<const ConcreteValue128Bits *> (&cv)->getLsb ();
  } else {
    throw std::runtime_error ("ConcreteValue64Bits instantiation problem "
                              "(copying from unknown type).");
  }
}

ConcreteValue64Bits::~ConcreteValue64Bits () {
}

ConcreteValue64Bits &ConcreteValue64Bits::operator= (UINT64 v) {
  value = v;
  return *this;
}

ConcreteValue64Bits &ConcreteValue64Bits::operator= (const ConcreteValue &v) {
  ConcreteValue64Bits tmp (v);
  value = tmp.value;
  return *this;
}

void ConcreteValue64Bits::saveToBinaryStream (std::ofstream &out) const {
  const UINT32 size = 64;
  out.write ((const char *) &size, sizeof (size));
  out.write ((const char *) &value, sizeof (value));
}

void ConcreteValue64Bits::writeToMemoryAddress (ADDRINT memoryEa) const {
  edu::sharif::twinner::util::writeMemoryContent (memoryEa, value);
}

void ConcreteValue64Bits::writeToRegister (CONTEXT *context, REG reg) const {
  PIN_REGISTER buffer;
  memset (buffer.byte, 0, sizeof (buffer));
  buffer.qword[0] = value;
  edu::sharif::twinner::util::writeRegisterContent (context, reg, buffer.byte);
}

std::basic_ostream<char> &operator<< (std::basic_ostream<char> &stream,
    const ConcreteValue64Bits &me) {
  return stream << "0x" << std::hex << me.value;
}

bool ConcreteValue64Bits::isNegative (int size) const {
  return (value >= (1ull << (size - 1)));
}

bool ConcreteValue64Bits::isZero () const {
  return (value == 0);
}

ConcreteValue64Bits *ConcreteValue64Bits::twosComplement () const {
  return new ConcreteValue64Bits ((~value) + 1);
}

UINT64 ConcreteValue64Bits::getValue () const {
  return value;
}

ConcreteValue64Bits *ConcreteValue64Bits::clone () const {
  return new ConcreteValue64Bits (value);
}

ConcreteValue64Bits *ConcreteValue64Bits::loadFromBinaryStream (std::ifstream &in) {
  UINT64 value;
  in.read ((char *) &value, sizeof (value));
  return new ConcreteValue64Bits (value);
}

int ConcreteValue64Bits::getSize () const {
  return 64;
}

bool ConcreteValue64Bits::operator> (const ConcreteValue &cv) const {
  if (getSize () < cv.getSize ()) {
    return (cv < (*this));
  }
  const ConcreteValue64Bits tmp (cv);
  return value > tmp.value;
}

bool ConcreteValue64Bits::operator== (const ConcreteValue &cv) const {
  if (getSize () < cv.getSize ()) {
    return (cv == (*this));
  }
  const ConcreteValue64Bits tmp (cv);
  return value == tmp.value;
}

ConcreteValue64Bits &ConcreteValue64Bits::operator-= (const ConcreteValue &cv) {
  const ConcreteValue64Bits tmp (cv);
  value -= tmp.value;
  return *this;
}

ConcreteValue64Bits &ConcreteValue64Bits::operator+= (const ConcreteValue &cv) {
  const ConcreteValue64Bits tmp (cv);
  value += tmp.value;
  return *this;
}

ConcreteValue64Bits &ConcreteValue64Bits::operator&= (const ConcreteValue &mask) {
  const ConcreteValue64Bits tmp (mask);
  value &= tmp.value;
  return *this;
}

ConcreteValue64Bits &ConcreteValue64Bits::operator|= (const ConcreteValue &mask) {
  const ConcreteValue64Bits tmp (mask);
  value |= tmp.value;
  return *this;
}

ConcreteValue &ConcreteValue64Bits::operator*= (const ConcreteValue &mul) {
  const ConcreteValue64Bits tmp (mul);
  value *= tmp.value;
  return *this;
}

ConcreteValue &ConcreteValue64Bits::operator/= (const ConcreteValue &divisor) {
  const ConcreteValue64Bits tmp (value);
  value /= tmp.value;
  return *this;
}

ConcreteValue &ConcreteValue64Bits::operator%= (const ConcreteValue &divisor) {
  const ConcreteValue64Bits tmp (divisor);
  value %= tmp.value;
  return *this;
}

ConcreteValue &ConcreteValue64Bits::operator^= (const ConcreteValue &pattern) {
  const ConcreteValue64Bits tmp (pattern);
  value ^= tmp.value;
  return *this;
}

ConcreteValue64Bits &ConcreteValue64Bits::operator>>= (const ConcreteValue &bits) {
  const ConcreteValue64Bits tmp (bits);
  value >>= tmp.value;
  return *this;
}

ConcreteValue64Bits &ConcreteValue64Bits::operator<<= (const ConcreteValue &bits) {
  const ConcreteValue64Bits tmp (bits);
  value <<= tmp.value;
  return *this;
}

}
}
}
}
