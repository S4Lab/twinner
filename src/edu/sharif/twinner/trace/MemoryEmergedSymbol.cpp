//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information 
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include <sstream>

#include "MemoryEmergedSymbol.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

MemoryEmergedSymbol::MemoryEmergedSymbol (const MemoryEmergedSymbol &s) :
Symbol (s), address (s.address) {
}

MemoryEmergedSymbol::MemoryEmergedSymbol (ADDRINT addr) :
Symbol (), address (addr) {
}

MemoryEmergedSymbol::MemoryEmergedSymbol (ADDRINT _address, UINT64 concreteValue,
    int generationIndex) :
Symbol (concreteValue, generationIndex), address (_address) {
}

MemoryEmergedSymbol *MemoryEmergedSymbol::clone () const {
  return new MemoryEmergedSymbol (*this);
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

std::string MemoryEmergedSymbol::toString () const {
  std::stringstream ss;
  ss << 'm' << std::hex << address << '_' << generationIndex;
  return ss.str ();
}

std::pair < int, SymbolRecord >
MemoryEmergedSymbol::toSymbolRecord () const {
  SymbolRecord record;
  record.address = address;
  record.concreteValue = concreteValue;
  return make_pair (generationIndex, record);
}

}
}
}
}
