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

#include "ConcreteValue128Bits.h"

#include "ConcreteValue64Bits.h"

#include "edu/sharif/twinner/util/memory.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

ConcreteValue128Bits::ConcreteValue128Bits () :
ConcreteValue (), msb (0), lsb (0) {
}

ConcreteValue128Bits::ConcreteValue128Bits (UINT64 _msb, UINT64 _lsb) :
ConcreteValue (), msb (_msb), lsb (_lsb) {
}

ConcreteValue128Bits::ConcreteValue128Bits (const PIN_REGISTER &value) :
ConcreteValue (), msb (value.qword[1]), lsb (value.qword[0]) {
}

ConcreteValue128Bits::ConcreteValue128Bits (const ConcreteValue &cv) :
ConcreteValue () {
  if (dynamic_cast<const ConcreteValue64Bits *> (&cv)) {
    lsb = static_cast<const ConcreteValue64Bits *> (&cv)->getValue ();
    msb = 0;
  } else if (dynamic_cast<const ConcreteValue128Bits *> (&cv)) {
    lsb = static_cast<const ConcreteValue128Bits *> (&cv)->getLsb ();
    msb = static_cast<const ConcreteValue128Bits *> (&cv)->getMsb ();
  } else {
    throw std::runtime_error ("ConcreteValue64Bits instantiation problem "
                              "(copying from unknown type).");
  }
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
  return *this;
}

void ConcreteValue128Bits::saveToBinaryStream (std::ofstream &out) const {
  const UINT32 size = 128;
  out.write ((const char *) &size, sizeof (size));
  out.write ((const char *) &lsb, sizeof (lsb));
  out.write ((const char *) &lsb, sizeof (msb));
}

void ConcreteValue128Bits::writeToMemoryAddress (ADDRINT memoryEa) const {
  UINT64 qword[] = {lsb, msb};
  edu::sharif::twinner::util::writeMemoryContent
      (memoryEa, (UINT8 *) qword, sizeof (qword));
}

std::basic_ostream<char> &operator<< (std::basic_ostream<char> &stream,
    const ConcreteValue128Bits &me) {
  return stream << "0x" << std::hex << me.msb << me.lsb;
}

bool ConcreteValue128Bits::isNegative (int size) const {
  if (size <= 64) {
    return msb != 0 || (lsb >= (1ull << (size - 1)));
  } else {
    return (msb >= (1ull << (size - 64 - 1)));
  }
}

bool ConcreteValue128Bits::isZero () const {
  return (msb == 0) && (lsb == 0);
}

ConcreteValue128Bits *ConcreteValue128Bits::twosComplement () const {
  ConcreteValue128Bits *tmp = new ConcreteValue128Bits (~msb, ~lsb);
  ConcreteValue &tt = *tmp;
  tt += UINT64 (1);
  return tmp;
}

ConcreteValue128Bits *ConcreteValue128Bits::clone () const {
  return new ConcreteValue128Bits (*this);
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

int ConcreteValue128Bits::getSize () const {
  return 128;
}

bool ConcreteValue128Bits::operator> (const ConcreteValue &cv) const {
  if (getSize () < cv.getSize ()) {
    return (cv < (*this));
  }
  const ConcreteValue128Bits tmp (cv);
  return (msb > tmp.msb) || ((msb == tmp.msb) && lsb > tmp.lsb);
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
  return (*this);
}

ConcreteValue128Bits &ConcreteValue128Bits::operator+= (const ConcreteValue &cv) {
  const ConcreteValue128Bits tmp (cv);
  int carry;
  if ((lsb & (1ull << 63)) != 0 && (tmp.lsb & (1ull << 63)) != 0) {
    carry = 1;
  } else {
    carry = 0;
  }
  lsb += tmp.lsb;
  msb += tmp.msb + carry;
  return *this;
}

ConcreteValue128Bits &ConcreteValue128Bits::operator&= (const ConcreteValue &mask) {
  const ConcreteValue128Bits tmp (mask);
  lsb &= tmp.lsb;
  msb &= tmp.msb;
  return *this;
}

ConcreteValue128Bits &ConcreteValue128Bits::operator|= (const ConcreteValue &mask) {
  const ConcreteValue128Bits tmp (mask);
  lsb |= tmp.lsb;
  msb |= tmp.msb;
  return *this;
}

ConcreteValue &ConcreteValue128Bits::operator*= (const ConcreteValue &mul) {
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
  const ResultCarry d3 (c43.result + c34.result + d4.carry);

  const ResultCarry c24 (a2 * b4);
  const ResultCarry c33 (a3 * b3);
  const ResultCarry c42 (a4 * b2);
  const ResultCarry d2 (c42.result + c33.result + c24.result + d3.carry);

  const ResultCarry c14 (a1 * b4);
  const ResultCarry c23 (a2 * b3);
  const ResultCarry c32 (a3 * b2);
  const ResultCarry c41 (a4 * b1);
  const ResultCarry d1 (c41.result + c32.result + c23.result + c14.result + d2.carry);
  //d0 and other terms are overflowed and ignored
  lsb = (d3.result << 32) | d4.result;
  msb = (d1.result << 32) | d2.result;
  return *this;
}

void ConcreteValue128Bits::divide (
    ConcreteValue128Bits dividend, const ConcreteValue128Bits divisor,
    ConcreteValue128Bits &quotient, ConcreteValue128Bits &remainder) const {
  if (divisor.isZero ()) {
    throw std::runtime_error ("ConcreteValue128Bits::divide(...) method: "
                              "Division by zero!");
  } else if (divisor > dividend) {
    quotient = 0;
    remainder = dividend;
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

ConcreteValue &ConcreteValue128Bits::operator/= (const ConcreteValue &divisor) {
  ConcreteValue128Bits remainder;
  divide (*this, divisor, *this, remainder);
  return *this;
}

ConcreteValue &ConcreteValue128Bits::operator%= (const ConcreteValue &divisor) {
  ConcreteValue128Bits quotient;
  divide (*this, divisor, quotient, *this);
  return *this;
}

ConcreteValue &ConcreteValue128Bits::operator^= (const ConcreteValue &pattern) {
  const ConcreteValue128Bits tmp (pattern);
  lsb ^= tmp.lsb;
  msb ^= tmp.msb;
  return *this;
}

ConcreteValue128Bits &ConcreteValue128Bits::operator>>= (const ConcreteValue &bits) {
  if (bits > 127) {
    msb = lsb = 0;
  } else {
    const int n = (ConcreteValue64Bits (bits)).getValue ();
    if (n > 63) {
      lsb = (msb >> (n - 64));
      msb = 0;
    } else if (n > 0) {
      lsb = (msb << (64 - n)) | (lsb >> n);
      msb >>= n;
    }
  }
  return *this;
}

ConcreteValue128Bits &ConcreteValue128Bits::operator<<= (const ConcreteValue &bits) {
  if (bits > 127) {
    msb = lsb = 0;
  } else {
    const int n = (ConcreteValue64Bits (bits)).getValue ();
    if (n > 63) {
      msb = (lsb << (n - 64));
      lsb = 0;
    } else if (n > 0) {
      msb = (msb << n) | (lsb >> (64 - n));
      lsb <<= n;
    }
  }
  return *this;
}

}
}
}
}
