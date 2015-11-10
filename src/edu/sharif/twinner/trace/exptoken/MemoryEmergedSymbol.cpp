//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2015  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include <sstream>

#include "MemoryEmergedSymbol.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue128Bits.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

MemoryEmergedSymbol::MemoryEmergedSymbol (const MemoryEmergedSymbol &s) :
    Symbol (s), address (s.address) {
}

MemoryEmergedSymbol::MemoryEmergedSymbol (ADDRINT addr) :
    Symbol (), address (addr) {
}

MemoryEmergedSymbol::MemoryEmergedSymbol (ADDRINT _address,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue,
    int generationIndex) :
    Symbol (concreteValue, generationIndex), address (_address) {
}

MemoryEmergedSymbol *MemoryEmergedSymbol::clone () const {
  return new MemoryEmergedSymbol (*this);
}

std::pair < int, SymbolRecord >
MemoryEmergedSymbol::toSymbolRecord () const {
  SymbolRecord record;
  record.address = address;
  record.type = SymbolType (1 << (concreteValue->getSize () / 8));
  record.concreteValueLsb = concreteValue->toUint64 ();
  if (concreteValue->getSize () == 128) {
    record.concreteValueMsb =
        static_cast<const edu::sharif::twinner::trace::cv::ConcreteValue128Bits *>
        (concreteValue)->getMsb ();
  } else {
    record.concreteValueMsb = 0;
  }
  return make_pair (generationIndex, record);
}

void MemoryEmergedSymbol::saveToBinaryStream (std::ofstream &out) const {
  out.write ("M", 1);
  out.write ((const char *) &address, sizeof (address));
  Symbol::saveToBinaryStream (out);
}

MemoryEmergedSymbol *MemoryEmergedSymbol::loadFromBinaryStream (std::ifstream &in) {
  ADDRINT address;
  in.read ((char *) &address, sizeof (address));
  MemoryEmergedSymbol *symbol = new MemoryEmergedSymbol (address);
  symbol->Symbol::loadFromBinaryStream (in);
  return symbol;
}

MemoryEmergedSymbol *MemoryEmergedSymbol::fromNameAndValue (const std::string &name,
    UINT32 v4, UINT32 v3, UINT32 v2, UINT32 v1) {
  if (v4 != 0 || v3 != 0) {
    throw std::runtime_error ("MemoryEmergedSymbol::fromNameAndValue (...): "
                              "Illegal value: Currently, all memory symbols"
                              " have at most 64 bits.");
  }
  const UINT64 value = (UINT64 (v2) << 32) | v1;
  return MemoryEmergedSymbol::fromNameAndValue
      (name, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (value));
}

MemoryEmergedSymbol *MemoryEmergedSymbol::fromNameAndValue (const std::string &name,
    const edu::sharif::twinner::trace::cv::ConcreteValue &value) {
  std::stringstream ss (name);
  char dummy;
  ADDRINT address;
  int generationIndex;
  int size;
  ss >> dummy >> std::hex >> address >> dummy >> generationIndex
      >> dummy >> std::dec >> size;
  const edu::sharif::twinner::trace::cv::ConcreteValue *cv = value.clone (size);
  MemoryEmergedSymbol *symbol =
      new MemoryEmergedSymbol (address, *cv, generationIndex);
  delete cv;
  return symbol;
}

std::string MemoryEmergedSymbol::toString () const {
  std::stringstream ss;
  ss << 'm' << std::hex << address << '_' << generationIndex
      << '_' << std::dec << concreteValue->getSize ();
  return ss.str ();
}

bool MemoryEmergedSymbol::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const MemoryEmergedSymbol *> (&token)
      && static_cast<const MemoryEmergedSymbol *> (&token)->generationIndex ==
      generationIndex
      && static_cast<const MemoryEmergedSymbol *> (&token)->address == address;
}

ADDRINT MemoryEmergedSymbol::getAddress () const {
  return address;
}

}
}
}
}
}
