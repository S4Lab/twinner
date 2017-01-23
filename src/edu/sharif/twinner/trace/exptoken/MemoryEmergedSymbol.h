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

#ifndef MEMORYEMERGEDSYMBOL_H_
#define MEMORYEMERGEDSYMBOL_H_

#include "Symbol.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

class MemoryEmergedSymbol : public Symbol {
private:
  ADDRINT address;

  MemoryEmergedSymbol (const MemoryEmergedSymbol &s);

  MemoryEmergedSymbol (ADDRINT address);

public:
  /// Creates a temporary memory symbol iff snapshotIndex != -1
  MemoryEmergedSymbol (ADDRINT address,
      const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue,
      int generationIndex, int snapshotIndex = -1);

  virtual MemoryEmergedSymbol *clone () const;
  virtual std::pair < int, SymbolRecord > toSymbolRecord () const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static MemoryEmergedSymbol *loadFromBinaryStream (std::ifstream &in);
  static MemoryEmergedSymbol *fromNameAndValue (const std::string &name,
      UINT32 v4, UINT32 v3, UINT32 v2, UINT32 v1);
  static MemoryEmergedSymbol *fromNameAndValue (const std::string &name,
      const edu::sharif::twinner::trace::cv::ConcreteValue &value);

  virtual std::string toString () const;
  virtual bool operator== (const ExpressionToken &token) const;
  virtual bool operator< (const Symbol &symbol) const;

  ADDRINT getAddress () const;

  /**
   * Iterating on all most-high-level memory contents, i.e. those which do not
   * need lazy loading and do not have an upper containing memory content
   * which does not need lazy loading too, of the given memory contents maps,
   * instantiates a new temporary symbol for them with the same concrete value,
   * the requested segment and snapshot indices, and the same overwriting mode.
   *
   * @param memoryToExpressionMaps The map of mem exps to be converted to temp exps.
   * @param segmentIndex The segment index of newly instantiated temp symbols.
   * @param snapshotIndex The snapshot index of newly instantiated temp symbols.
   * @return A map containing a temp exp for each most-high-level mem in the initial map.
   */
  static std::map < ADDRINT, Expression * > instantiateTemporarySymbols (
      const std::map < ADDRINT, Expression * > * const *memoryToExpressionMaps,
      int segmentIndex, int snapshotIndex);
  static void instantiateTemporarySymbols (
      std::map < ADDRINT, Expression * > &tempExpressions,
      ADDRINT address, int size, int level,
      const std::map < ADDRINT, Expression * > * const *memoryToExpressionMaps,
      int segmentIndex, int snapshotIndex);
};

}
}
}
}
}

#endif /* MEMORYEMERGEDSYMBOL_H_ */
