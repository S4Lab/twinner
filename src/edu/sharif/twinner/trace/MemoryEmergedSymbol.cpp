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

#include "MemoryEmergedSymbol.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

MemoryEmergedSymbol::MemoryEmergedSymbol (const MemoryEmergedSymbol &s) :
Symbol (s), address (s.address) {
}

MemoryEmergedSymbol::MemoryEmergedSymbol (ADDRINT _address, UINT64 concreteValue,
    int generationIndex) :
Symbol (concreteValue, generationIndex), address (_address) {
}

MemoryEmergedSymbol *MemoryEmergedSymbol::clone () const {
  return new MemoryEmergedSymbol (*this);
}

}
}
}
}
