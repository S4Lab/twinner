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

#include "ConcreteValue.h"

#include "ConcreteValue8Bits.h"
#include "ConcreteValue16Bits.h"
#include "ConcreteValue32Bits.h"
#include "ConcreteValue64Bits.h"
#include "ConcreteValue128Bits.h"

#include "edu/sharif/twinner/util/Logger.h"

#include <fstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace cv {

ConcreteValue::ConcreteValue () {
}

ConcreteValue::~ConcreteValue () {
}

ConcreteValue *ConcreteValue::loadFromBinaryStream (std::ifstream &in) {
  UINT32 size;
  in.read ((char *) &size, sizeof (size));
  switch (size) {
  case 8:
    return ConcreteValue8Bits::loadFromBinaryStream (in);
  case 16:
    return ConcreteValue16Bits::loadFromBinaryStream (in);
  case 32:
    return ConcreteValue32Bits::loadFromBinaryStream (in);
  case 64:
    return ConcreteValue64Bits::loadFromBinaryStream (in);
  case 128:
    return ConcreteValue128Bits::loadFromBinaryStream (in);
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "ConcreteValue::loadFromBinaryStream(...): Unsupported size.\n";
    abort ();
  }
}

std::basic_ostream<char> &operator<< (std::basic_ostream<char> &stream,
                                      const ConcreteValue &cv) {
  if (dynamic_cast<const ConcreteValue8Bits *> (&cv)) {
    return stream << static_cast<const ConcreteValue8Bits &> (cv);

  } else if (dynamic_cast<const ConcreteValue16Bits *> (&cv)) {
    return stream << static_cast<const ConcreteValue16Bits &> (cv);

  } else if (dynamic_cast<const ConcreteValue32Bits *> (&cv)) {
    return stream << static_cast<const ConcreteValue32Bits &> (cv);

  } else if (dynamic_cast<const ConcreteValue64Bits *> (&cv)) {
    return stream << static_cast<const ConcreteValue64Bits &> (cv);

  } else if (dynamic_cast<const ConcreteValue128Bits *> (&cv)) {
    return stream << static_cast<const ConcreteValue128Bits &> (cv);

  } else {
    return stream << "ConcreteValue(unknown-type)";
  }
}

bool ConcreteValue::operator< (UINT64 value) const {
  return (*this) < ConcreteValue64Bits (value);
}

bool ConcreteValue::lessThan (UINT64 value) const {
  return lessThan (ConcreteValue64Bits (value));
}

bool ConcreteValue::operator<= (UINT64 value) const {
  return (*this) <= ConcreteValue64Bits (value);
}

bool ConcreteValue::lessThanOrEqualTo (UINT64 value) const {
  return lessThanOrEqualTo (ConcreteValue64Bits (value));
}

bool ConcreteValue::operator>= (UINT64 value) const {
  return (*this) >= ConcreteValue64Bits (value);
}

bool ConcreteValue::greaterThanOrEqualTo (UINT64 value) const {
  return greaterThanOrEqualTo (ConcreteValue64Bits (value));
}

bool ConcreteValue::operator> (UINT64 value) const {
  return (*this) > ConcreteValue64Bits (value);
}

bool ConcreteValue::greaterThan (UINT64 value) const {
  return greaterThan (ConcreteValue64Bits (value));
}

bool ConcreteValue::operator== (UINT64 value) const {
  return (*this) == ConcreteValue64Bits (value);
}

ConcreteValue &ConcreteValue::operator-= (UINT64 cv) {
  return (*this) -= ConcreteValue64Bits (cv);
}

ConcreteValue &ConcreteValue::operator+= (UINT64 cv) {
  return (*this) += ConcreteValue64Bits (cv);
}

ConcreteValue &ConcreteValue::operator&= (UINT64 mask) {
  return (*this) &= ConcreteValue64Bits (mask);
}

ConcreteValue &ConcreteValue::operator|= (UINT64 mask) {
  return (*this) |= ConcreteValue64Bits (mask);
}

ConcreteValue &ConcreteValue::operator*= (UINT64 mul) {
  return (*this) *= ConcreteValue64Bits (mul);
}

ConcreteValue &ConcreteValue::operator/= (UINT64 divisor) {
  return (*this) /= ConcreteValue64Bits (divisor);
}

ConcreteValue &ConcreteValue::signedDivide (UINT64 divisor) {
  return this->signedDivide (ConcreteValue64Bits (divisor));
}

ConcreteValue &ConcreteValue::operator%= (UINT64 divisor) {
  return (*this) %= ConcreteValue64Bits (divisor);
}

ConcreteValue &ConcreteValue::signedRemainder (UINT64 divisor) {
  return this->signedRemainder (ConcreteValue64Bits (divisor));
}

ConcreteValue &ConcreteValue::operator^= (UINT64 pattern) {
  return (*this) ^= ConcreteValue64Bits (pattern);
}

ConcreteValue &ConcreteValue::operator>>= (UINT64 bits) {
  return (*this) >>= ConcreteValue64Bits (bits);
}

ConcreteValue &ConcreteValue::operator<<= (UINT64 bits) {
  return (*this) <<= ConcreteValue64Bits (bits);
}

ConcreteValue &ConcreteValue::arithmeticShiftToRight (UINT64 bits) {
  return this->arithmeticShiftToRight (ConcreteValue64Bits (bits));
}

ConcreteValue &ConcreteValue::rotateToRight (UINT64 bits) {
  return this->rotateToRight (ConcreteValue64Bits (bits));
}

ConcreteValue &ConcreteValue::rotateToLeft (UINT64 bits) {
  return this->rotateToLeft (ConcreteValue64Bits (bits));
}

}
}
}
}
}
