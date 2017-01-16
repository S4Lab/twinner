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

#ifndef SYMBOL_H
#define SYMBOL_H

#include "Operand.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

enum SymbolType {
  MEMORY_8_BITS_SYMBOL_TYPE = (1 << 1), // 0x0000 0002
  MEMORY_16_BITS_SYMBOL_TYPE = (1 << 2), // 0x0000 0004
  MEMORY_32_BITS_SYMBOL_TYPE = (1 << 4), // 0x0000 0010
  MEMORY_64_BITS_SYMBOL_TYPE = (1 << 8), // 0x0000 0100
  MEMORY_128_BITS_SYMBOL_TYPE = (1 << 16), // 0x0001 0000
  REGISTER_32_BITS_SYMBOL_TYPE = 0x00020003,
  REGISTER_64_BITS_SYMBOL_TYPE = 0x00020001,
  REGISTER_128_BITS_SYMBOL_TYPE = 0x00020002,
  MAIN_ARGV_I_TYPE = 0x00040001,
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

  /**
   * This index, starting from zero, indicates a temporary symbol which is used
   * to track changes at intermediate snapshot points between syscall invocations.
   */
  int snapshotIndex;

  /// Creates a temporary symbol iff snapshotIndex != -1
  Symbol (const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue,
      int generationIndex, int snapshotIndex = -1);
  Symbol (const Symbol &s);
  Symbol ();

public:
  virtual Symbol *clone () const = 0;

protected:
  virtual void saveToBinaryStream (std::ofstream &out) const;
  void loadFromBinaryStream (std::ifstream &in);

public:
  int getGenerationIndex () const;
  int getSnapshotIndex () const;

  virtual std::pair < int, SymbolRecord > toSymbolRecord () const = 0;

  virtual bool isConstant () const;

  bool isTemporary () const;

  virtual bool operator< (const Symbol &symbol) const = 0;
};

}
}
}
}
}

#endif /* Symbol.h */
