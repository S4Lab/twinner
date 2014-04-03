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
  MemoryEmergedSymbol (ADDRINT address, const ConcreteValue &concreteValue,
      int generationIndex);

  virtual MemoryEmergedSymbol *clone () const;
  virtual std::pair < int, SymbolRecord > toSymbolRecord () const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static MemoryEmergedSymbol *loadFromBinaryStream (std::ifstream &in);
  static MemoryEmergedSymbol *fromNameAndValue (const std::string &name, UINT64 value);

  virtual std::string toString () const;
  virtual bool operator== (const ExpressionToken &token) const;

  ADDRINT getAddress () const;
};

}
}
}
}

#endif /* MEMORYEMERGEDSYMBOL_H_ */
