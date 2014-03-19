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

ConcreteValue128Bits *ConcreteValue128Bits::twosComplement () const {
  ConcreteValue128Bits *tmp = new ConcreteValue128Bits (~msb, ~lsb);
  (*tmp) += UINT64 (1);
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

}
}
}
}
