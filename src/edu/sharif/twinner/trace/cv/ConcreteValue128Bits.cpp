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

#include <iomanip>
#include <sstream>

#include "ConcreteValue128Bits.h"

#include "ConcreteValue8Bits.h"
#include "ConcreteValue16Bits.h"
#include "ConcreteValue32Bits.h"
#include "ConcreteValue64Bits.h"

#include "edu/sharif/twinner/util/memory.h"
#include "edu/sharif/twinner/util/Logger.h"

#include <sstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace cv {

ConcreteValue128Bits::ConcreteValue128Bits () :
    ConcreteValueAbstractImp<128> (), msb (0), lsb (0) {
}

ConcreteValue128Bits::ConcreteValue128Bits (UINT64 _msb, UINT64 _lsb) :
    ConcreteValueAbstractImp<128> (), msb (_msb), lsb (_lsb) {
}

ConcreteValue128Bits::ConcreteValue128Bits (const PIN_REGISTER &value) :
    ConcreteValueAbstractImp<128> (), msb (value.qword[1]), lsb (value.qword[0]) {
}

ConcreteValue128Bits::ConcreteValue128Bits (const ConcreteValue &cv) :
    ConcreteValueAbstractImp<128> () {
  if (dynamic_cast<const ConcreteValue128Bits *> (&cv)) {
    lsb = static_cast<const ConcreteValue128Bits *> (&cv)->getLsb ();
    msb = static_cast<const ConcreteValue128Bits *> (&cv)->getMsb ();
  } else {
    lsb = cv.toUint64 ();
    msb = 0;
  }
  cf = cv.getCarryBit ();
}

ConcreteValue128Bits::~ConcreteValue128Bits () {
}

ConcreteValue128Bits &ConcreteValue128Bits::operator= (UINT64 value) {
  msb = 0;
  lsb = value;
  return *this;
}

ConcreteValue128Bits &ConcreteValue128Bits::operator= (const ConcreteValue &value) {
  ConcreteValue128Bits tmp (value);
  msb = tmp.msb;
  lsb = tmp.lsb;
  cf = tmp.cf;
  return *this;
}

void ConcreteValue128Bits::saveToBinaryStream (std::ofstream &out) const {
  const UINT32 size = 128;
  out.write ((const char *) &size, sizeof (size));
  out.write ((const char *) &lsb, sizeof (lsb));
  out.write ((const char *) &msb, sizeof (msb));
}

bool ConcreteValue128Bits::writeToMemoryAddress (ADDRINT memoryEa) const {
  const UINT64 qword[] = {lsb, msb};
  return edu::sharif::twinner::util::writeMemoryContent
      (memoryEa, (const UINT8 *) qword, 2 * sizeof (UINT64));
}

bool ConcreteValue128Bits::writeToRegister (CONTEXT *context,
    LEVEL_BASE::REG reg) const {
  PIN_REGISTER buffer;
  memset (buffer.byte, 0, sizeof (buffer));
  buffer.qword[0] = lsb;
  buffer.qword[1] = msb;
  return edu::sharif::twinner::util::writeRegisterContent
      (context, reg, buffer.byte);
}

std::basic_ostream<char> &operator<< (std::basic_ostream<char> &stream,
                                      const ConcreteValue128Bits &me) {
  std::stringstream ss;

  union {
    UINT32 v32[4];
    UINT64 v64[2];
  } value;
  value.v64[0] = me.lsb;
  value.v64[1] = me.msb;
  ss << "UINT128 (0x" << std::hex << value.v32[3] << ", 0x" << value.v32[2]
      << ", 0x" << value.v32[1] << ", 0x" << value.v32[0] << ")";
  return stream << ss.str ();
}

std::string ConcreteValue128Bits::toHexString () const {
  std::stringstream ss;
  ss << std::hex << msb << std::setw (16) << std::setfill ('0') << lsb;
  return ss.str ();
}

bool ConcreteValue128Bits::getCarryBit () const {
  return cf;
}

void ConcreteValue128Bits::setCarryBit (bool cf) {
  this->cf = cf;
}

bool ConcreteValue128Bits::isNegative () const {
  //return msb != 0 || (lsb >= (1ull << (size - 1)));
  return (msb >= (1ull << 63));
}

bool ConcreteValue128Bits::isZero () const {
  return (msb == 0) && (lsb == 0);
}

bool ConcreteValue128Bits::isCompletePowerOfTwo (int *n) const {
  if ((msb == 0) == (lsb == 0)) {
    return false;
  }
  const UINT64 org = lsb ? lsb : msb;
  UINT64 v = org;
  int bits = 0;
  while (v > 0) {
    v >>= 1;
    bits++;
  }
  if (bits == 1 || org == (1ull << (bits - 1))) {
    if (n) {
      if (lsb) {
        *n = bits - 1;
      } else {
        *n = bits - 1 + 64;
      }
    }
    return true;
  } else {
    return false;
  }
}

ConcreteValue128Bits *ConcreteValue128Bits::twosComplement () const {
  ConcreteValue128Bits *tmp = bitwiseNegated ();
  ConcreteValue &tt = *tmp;
  tt += UINT64 (1);
  tmp->setCarryBit (msb != 0 || lsb != 0);
  return tmp;
}

ConcreteValue128Bits *ConcreteValue128Bits::bitwiseNegated () const {
  return new ConcreteValue128Bits (~msb, ~lsb);
}

ConcreteValue *ConcreteValue128Bits::signExtended (int length) const {
  edu::sharif::twinner::util::Logger::error () << "Larger than 128-bits"
      " concrete values are not supported for sign-extension\n";
  abort ();
}

ConcreteValue *ConcreteValue128Bits::realClone (int length) const {
  switch (length) {
  case 8:
    return new ConcreteValue8Bits (UINT8 (lsb));
  case 16:
    return new ConcreteValue16Bits (UINT16 (lsb));
  case 32:
    return new ConcreteValue32Bits (UINT32 (lsb));
  case 64:
    return new ConcreteValue64Bits (lsb);
  case -1:
  case 128:
    return new ConcreteValue128Bits (msb, lsb);
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "ConcreteValue128Bits::clone (" << length << "):"
        " Unsupported length\n";
    abort ();
  }
}

UINT64 ConcreteValue128Bits::getLsb () const {
  return lsb;
}

UINT64 ConcreteValue128Bits::getMsb () const {
  return msb;
}

ConcreteValue128Bits *ConcreteValue128Bits::loadFromBinaryStream (std::ifstream &in) {
  UINT64 msb, lsb;
  in.read ((char *) &lsb, sizeof (lsb));
  in.read ((char *) &msb, sizeof (msb));
  return new ConcreteValue128Bits (msb, lsb);
}

bool ConcreteValue128Bits::operator> (const ConcreteValue &cv) const {
  if (getSize () < cv.getSize ()) {
    return (cv < (*this));
  }
  const ConcreteValue128Bits tmp (cv);
  return (msb > tmp.msb) || ((msb == tmp.msb) && lsb > tmp.lsb);
}

bool ConcreteValue128Bits::greaterThan (const ConcreteValue &cv) const {
  if (getSize () < cv.getSize ()) {
    return cv.lessThan (*this);
  }
  const ConcreteValue128Bits tmp (cv);
  if (isNegative ()) {
    if (tmp.isNegative ()) {
      return (msb > tmp.msb) || ((msb == tmp.msb) && lsb > tmp.lsb);
    } else {
      return false;
    }
  } else {
    if (tmp.isNegative ()) {
      return true;
    } else {
      return (msb > tmp.msb) || ((msb == tmp.msb) && lsb > tmp.lsb);
    }
  }
}

bool ConcreteValue128Bits::operator== (const ConcreteValue &cv) const {
  if (getSize () < cv.getSize ()) {
    return (cv == (*this));
  }
  const ConcreteValue128Bits tmp (cv);
  return (msb == tmp.msb) && (lsb == tmp.lsb);
}

ConcreteValue128Bits &ConcreteValue128Bits::operator-= (const ConcreteValue &cv) {
  const ConcreteValue128Bits tmp1 (cv);
  const ConcreteValue128Bits *tmp2 = tmp1.twosComplement ();
  (*this) += *tmp2;
  delete tmp2;
  cf = !cf;
  return (*this);
}

ConcreteValue128Bits &ConcreteValue128Bits::operator+= (const ConcreteValue &cv) {
  const ConcreteValue128Bits tmp (cv);
  const int carry = wouldSummationHaveCarry (lsb, tmp.lsb) ? 1 : 0;
  const bool sumOfMsbBitsHasCarry = wouldSummationHaveCarry (msb, tmp.msb);
  lsb += tmp.lsb;
  msb += tmp.msb;
  cf = sumOfMsbBitsHasCarry || wouldSummationHaveCarry (msb, carry);
  msb += carry;
  return *this;
}

bool ConcreteValue128Bits::wouldSummationHaveCarry (UINT64 first, UINT64 second) const {
  const bool firstMsbIsSet = ((first & (1ull << 63)) != 0);
  const bool secondMsbIsSet = ((second & (1ull << 63)) != 0);
  if (firstMsbIsSet && secondMsbIsSet) {
    return true;
  } else if (!firstMsbIsSet && !secondMsbIsSet) {
    return false;
  } else { // one MSB is set and another is not set
    const UINT64 first63BitsOfFirstValue = (first & ((1ull << 63) - 1));
    const UINT64 first63BitsOfSecondValue = (second & ((1ull << 63) - 1));
    if (first63BitsOfFirstValue + first63BitsOfSecondValue >= (1ull << 63)) {
      return true;
    } else {
      return false;
    }
  }
}

ConcreteValue128Bits &ConcreteValue128Bits::operator&= (const ConcreteValue &mask) {
  const ConcreteValue128Bits tmp (mask);
  lsb &= tmp.lsb;
  msb &= tmp.msb;
  cf = false;
  return *this;
}

ConcreteValue128Bits &ConcreteValue128Bits::operator|= (const ConcreteValue &mask) {
  const ConcreteValue128Bits tmp (mask);
  lsb |= tmp.lsb;
  msb |= tmp.msb;
  cf = false;
  return *this;
}

ConcreteValue128Bits &ConcreteValue128Bits::operator*= (const ConcreteValue &mul) {
  const ConcreteValue128Bits tmp (mul);
  const UINT64 b4 = tmp.lsb & 0xFFFFFFFF;
  const UINT64 b3 = tmp.lsb >> 32;
  const UINT64 b2 = tmp.msb & 0xFFFFFFFF;
  const UINT64 b1 = tmp.msb >> 32;
  const UINT64 a4 = lsb & 0xFFFFFFFF;
  const UINT64 a3 = lsb >> 32;
  const UINT64 a2 = msb & 0xFFFFFFFF;
  const UINT64 a1 = msb >> 32;
  //   b1 b2 b3 b4
  // * a1 a2 a3 a4
  const ResultCarry d4 (a4 * b4);

  const ResultCarry c43 (a4 * b3);
  const ResultCarry c34 (a3 * b4);
  const ResultCarry d3 (c43.result + c34.result
                        + d4.carry);

  const ResultCarry c24 (a2 * b4);
  const ResultCarry c33 (a3 * b3);
  const ResultCarry c42 (a4 * b2);
  const ResultCarry d2 (c42.result + c33.result + c24.result
                        + c43.carry + c34.carry
                        + d3.carry);

  const ResultCarry c14 (a1 * b4);
  const ResultCarry c23 (a2 * b3);
  const ResultCarry c32 (a3 * b2);
  const ResultCarry c41 (a4 * b1);
  const ResultCarry d1 (c41.result + c32.result + c23.result + c14.result
                        + c42.carry + c33.carry + c24.carry
                        + d2.carry);

  const ResultCarry c13 (a1 * b3);
  const ResultCarry c22 (a2 * b2);
  const ResultCarry c31 (a3 * b1);
  const ResultCarry d0 (c31.result + c22.result + c13.result
                        + c41.carry + c32.carry + c23.carry + c14.carry
                        + d1.carry);

  const ResultCarry c12 (a1 * b2);
  const ResultCarry c21 (a2 * b1);
  const ResultCarry dm1 (c21.result + c12.result
                         + c31.carry + c22.carry + c13.carry
                         + d0.carry);

  const ResultCarry c11 (a1 * b1);
  const ResultCarry dm2 (c11.result
                         + c21.carry + c12.carry
                         + dm1.carry);

  const ResultCarry dm3 (c11.carry
                         + dm2.carry);
  //d0 and other terms are overflowed and ignored
  lsb = (d3.result << 32) | d4.result;
  msb = (d1.result << 32) | d2.result;
  cf = (d0 || dm1 || dm2 || dm3);
  return *this;
}

void ConcreteValue128Bits::twosComplement (ConcreteValue128Bits &num) const {
  ConcreteValue128Bits *tmp = num.twosComplement ();
  num = static_cast<ConcreteValue &> (*tmp);
  delete tmp;
}

bool ConcreteValue128Bits::absolute (ConcreteValue128Bits &num) const {
  const bool sign = num.isNegative ();
  if (sign) {
    twosComplement (num);
  }
  return sign;
}

void ConcreteValue128Bits::divide (
    ConcreteValue128Bits dividend, ConcreteValue128Bits divisor,
    ConcreteValue128Bits &quotient, ConcreteValue128Bits &remainder,
    bool signedArithmetic) const {
  bool dividendSign, divisorSign;
  if (signedArithmetic) {
    dividendSign = absolute (dividend);
    divisorSign = absolute (divisor);
  }
  if (divisor.isZero ()) {
    edu::sharif::twinner::util::Logger::error ()
        << "ConcreteValue128Bits::divide(...) method: Division by zero!\n";
    abort ();
  } else if (divisor > dividend) {
    quotient = 0;
    remainder = (ConcreteValue &) dividend;
  } else if (divisor == dividend) {
    quotient = 1;
    remainder = 0;
  } else { // divisor < dividend
    ConcreteValue128Bits tmpDivisor (divisor);
    for (int i = 0;;) {
      const ConcreteValue128Bits previousDivisor (tmpDivisor);
      tmpDivisor.doubleIt ();
      i++;
      if (tmpDivisor == dividend) {
        quotient = (1 << i);
        remainder = 0;
      } else if (tmpDivisor > dividend || previousDivisor > tmpDivisor) {
        quotient = (1 << (i - 1));
        dividend -= previousDivisor;
        ConcreteValue128Bits tmpquotient;
        divide (dividend, divisor, tmpquotient, remainder);
        quotient += tmpquotient;
      } else {
        continue;
      }
      break;
    }
  }
  if (signedArithmetic) {
    if (dividendSign != divisorSign) {
      twosComplement (quotient);
    }
    if (dividendSign) {
      twosComplement (remainder);
    }
  }
}

void ConcreteValue128Bits::doubleIt () {
  int carry;
  if ((lsb & (1ull << 63)) != 0) {
    carry = 1;
  } else {
    carry = 0;
  }
  lsb <<= 1;
  msb <<= 1;
  msb += carry;
}

ConcreteValue128Bits &ConcreteValue128Bits::operator/= (const ConcreteValue &divisor) {
  ConcreteValue128Bits remainder;
  divide (*this, divisor, *this, remainder);
  cf = false;
  return *this;
}

ConcreteValue128Bits &ConcreteValue128Bits::signedDivide (const ConcreteValue &divisor) {
  ConcreteValue128Bits remainder;
  divide (*this, divisor, *this, remainder, true);
  cf = false;
  return *this;
}

ConcreteValue128Bits &ConcreteValue128Bits::operator%= (const ConcreteValue &divisor) {
  ConcreteValue128Bits quotient;
  divide (*this, divisor, quotient, *this);
  cf = false;
  return *this;
}

ConcreteValue128Bits &ConcreteValue128Bits::signedRemainder (const ConcreteValue &divisor) {
  ConcreteValue128Bits quotient;
  divide (*this, divisor, quotient, *this, true);
  cf = false;
  return *this;
}

ConcreteValue128Bits &ConcreteValue128Bits::operator^= (const ConcreteValue &pattern) {
  const ConcreteValue128Bits tmp (pattern);
  lsb ^= tmp.lsb;
  msb ^= tmp.msb;
  cf = false;
  return *this;
}

ConcreteValue128Bits &ConcreteValue128Bits::operator>>= (const ConcreteValue &bits) {
  if (bits > 128) {
    msb = lsb = cf = 0;
  } else if (bits == 128) {
    cf = (msb >> 63) & 0x1;
    msb = lsb = 0;
  } else {
    int n = bits.toUint64 ();
    if (n > 63) {
      n -= 64;
      cf = getNthBit (msb, n);
      lsb = (msb >> n);
      msb = 0;
    } else if (n > 0) {
      cf = getNthBit (lsb, n);
      lsb = (msb << (64 - n)) | (lsb >> n);
      msb >>= n;
    }
  }
  return *this;
}

bool ConcreteValue128Bits::getNthBit (UINT64 value, int n) const {
  if (n == 1) {
    return value & 0x1;
  } else {
    return (value >> (n - 1)) & 0x1;
  }
}

ConcreteValue128Bits &ConcreteValue128Bits::operator<<= (const ConcreteValue &bits) {
  if (bits > 128) {
    msb = lsb = cf = 0;
  } else if (bits == 128) {
    cf = lsb & 0x1;
    msb = lsb = 0;
  } else {
    int n = bits.toUint64 ();
    if (n > 63) {
      n -= 64;
      cf = (lsb >> (64 - n)) & 0x1;
      msb = (lsb << n);
      lsb = 0;
    } else if (n > 0) {
      cf = (msb >> (64 - n)) & 0x1;
      msb = (msb << n) | (lsb >> (64 - n));
      lsb <<= n;
    }
  }
  return *this;
}

ConcreteValue128Bits &ConcreteValue128Bits::arithmeticShiftToRight (
    const ConcreteValue &cv) {
  const bool signBit = msb >> 63;
  (*this) >>= cv;
  if (signBit) {
    int n = cv.toUint64 ();
    UINT64 *pattern;
    if (n > 63) {
      msb = UINT64 (-1);
      pattern = &lsb;
      n -= 64;
    } else {
      pattern = &msb;
    }
    if (n > 0) { // n most significant bits of *pattern should be set
      UINT64 mask = 1;
      mask <<= n;
      mask -= 1;
      mask <<= 64 - n;
      (*pattern) |= mask;
    }
  }
  return *this;
}

ConcreteValue128Bits &ConcreteValue128Bits::rotateToRight (const ConcreteValue &bits) {
  int n = bits.toUint64 ();
  if (n > 63) {
    UINT64 tmp = lsb;
    lsb = msb;
    msb = tmp;
    n -= 64;
  }
  if (n > 0) {
    lsb = (msb << (64 - n)) | (lsb >> n);
    msb = (lsb << (64 - n)) | (msb >> n);
  }
  return *this;
}

ConcreteValue128Bits &ConcreteValue128Bits::rotateToLeft (const ConcreteValue &bits) {
  //TODO: handle CF
  int n = bits.toUint64 ();
  if (n > 63) {
    UINT64 tmp = lsb;
    lsb = msb;
    msb = tmp;
    n -= 64;
  }
  if (n > 0) {
    lsb = (lsb << n) | (msb >> (64 - n));
    msb = (msb << n) | (lsb >> (64 - n));
  }
  return *this;
}

}
}
}
}
}
