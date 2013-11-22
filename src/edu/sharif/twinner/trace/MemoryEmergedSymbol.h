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

#ifndef MEMORYEMERGEDSYMBOL_H_
#define MEMORYEMERGEDSYMBOL_H_

#include "Symbol.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class MemoryEmergedSymbol : public Symbol {

private:
  ADDRINT address;

  MemoryEmergedSymbol (const MemoryEmergedSymbol &s);

  MemoryEmergedSymbol (ADDRINT address);

public:
  MemoryEmergedSymbol (ADDRINT address, UINT64 concreteValue, int generationIndex);

  virtual MemoryEmergedSymbol *clone () const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static MemoryEmergedSymbol *loadFromBinaryStream (std::ifstream &in);
};

}
}
}
}

#endif /* MEMORYEMERGEDSYMBOL_H_ */
