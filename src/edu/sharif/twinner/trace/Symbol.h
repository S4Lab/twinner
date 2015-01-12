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

#ifndef SYMBOL_H
#define SYMBOL_H

#include "Operand.h"

#include "pin.H"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

enum SymbolType {

  REGISTER_64_BITS_SYMBOL_TYPE = 16,
  REGISTER_128_BITS_SYMBOL_TYPE = 32,
  MEMORY_64_BITS_SYMBOL_TYPE = 64,
  MEMORY_128_BITS_SYMBOL_TYPE = 128,
};

struct SymbolRecord {

  UINT64 address;
  UINT32 type; // SymbolType
  UINT64 concreteValueLsb;
  UINT64 concreteValueMsb;
};

/**
 * @abstract
 */
class Symbol : public Operand {

protected:
  /**
   * Different values can be stored at the same memory address (as they can be changed
   * by syscalls). This index, starting from zero, indicates which symbol is stored there.
   * Value zero, means that symbol was generated before first syscall. Value one, means
   * after first syscal but before second syscall.
   * Generating more than one symbol, at the same address, without any intermediary
   * syscall, is an error (indicating that some user space assembly instructions are not
   * instrumented).
   */
  int generationIndex;

  Symbol (const edu::sharif::twinner::trace::ConcreteValue &concreteValue,
      int generationIndex);
  Symbol (const Symbol &s);
  Symbol ();

protected:
  virtual void saveToBinaryStream (std::ofstream &out) const;
  void loadFromBinaryStream (std::ifstream &in);

public:
  int getGenerationIndex () const;

  virtual std::pair < int, SymbolRecord > toSymbolRecord () const = 0;
};

}
}
}
}

#endif /* Symbol.h */
