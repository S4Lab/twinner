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

#include <sstream>

#include "MemoryEmergedSymbol.h"

#include "RegisterEmergedSymbol.h"

#include "edu/sharif/twinner/trace/ExpressionImp.h"

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
    int generationIndex, int snapshotIndex) :
    Symbol (concreteValue, generationIndex, snapshotIndex), address (_address) {
}

MemoryEmergedSymbol *MemoryEmergedSymbol::clone () const {
  return new MemoryEmergedSymbol (*this);
}

std::pair < int, SymbolRecord >
MemoryEmergedSymbol::toSymbolRecord () const {
  // ASSERT: Symbol is not temporary
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
  // ASSERT: Name is for a non-temporary symbol
  if (v4 != 0 || v3 != 0) {
    edu::sharif::twinner::util::Logger::error ()
        << "MemoryEmergedSymbol::fromNameAndValue (...): Illegal value:"
        " Currently, all memory symbols have at most 64 bits.\n";
    abort ();
  }
  const UINT64 value = (UINT64 (v2) << 32) | v1;
  return MemoryEmergedSymbol::fromNameAndValue
      (name, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (value));
}

MemoryEmergedSymbol *MemoryEmergedSymbol::fromNameAndValue (const std::string &name,
    const edu::sharif::twinner::trace::cv::ConcreteValue &value) {
  // ASSERT: Name is for a non-temporary symbol
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
  if (snapshotIndex != -1) {
    ss << "_tmp_" << std::hex << snapshotIndex;
  }
  return ss.str ();
}

bool MemoryEmergedSymbol::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const MemoryEmergedSymbol *> (&token)
      && static_cast<const MemoryEmergedSymbol *> (&token)->generationIndex ==
      generationIndex
      && static_cast<const MemoryEmergedSymbol *> (&token)->snapshotIndex ==
      snapshotIndex
      && static_cast<const MemoryEmergedSymbol *> (&token)->address == address;
}

bool MemoryEmergedSymbol::operator< (const Symbol &symbol) const {
  if (getGenerationIndex () < symbol.getGenerationIndex ()
      || (getGenerationIndex () == symbol.getGenerationIndex ()
          && getSnapshotIndex () < symbol.getSnapshotIndex ())) {
    return true;
  }
  if (getGenerationIndex () > symbol.getGenerationIndex ()
      || getSnapshotIndex () > symbol.getSnapshotIndex ()) {
    return false;
  }
  if (dynamic_cast<const RegisterEmergedSymbol *> (&symbol)) {
    return true;
  }
  const MemoryEmergedSymbol *mem =
      dynamic_cast<const MemoryEmergedSymbol *> (&symbol);
  if (mem == 0) {
    return false;
  }
  return address < mem->address;
}

ADDRINT MemoryEmergedSymbol::getAddress () const {
  return address;
}

std::map < ADDRINT, Expression * > MemoryEmergedSymbol::instantiateTemporarySymbols (
    const std::map < ADDRINT, Expression * > * const *memoryToExpressionMaps,
    int segmentIndex, int snapshotIndex) {
  typedef std::map < ADDRINT, Expression * > AddrToExp;
  AddrToExp tempExpressions;
  int size = 16; // in bytes
  for (AddrToExp::const_iterator it = memoryToExpressionMaps[0]->begin ();
      it != memoryToExpressionMaps[0]->end (); ++it) {
    const ADDRINT address = it->first;
    const Expression *exp = it->second;
    if (exp == 0) {
      instantiateTemporarySymbols
          (tempExpressions, address, size / 2, 1,
           memoryToExpressionMaps, segmentIndex, snapshotIndex);
      instantiateTemporarySymbols
          (tempExpressions, address + size / 2, size / 2, 1,
           memoryToExpressionMaps, segmentIndex, snapshotIndex);
    } else {
      Expression *tmpExp = new ExpressionImp
          (address, exp->getLastConcreteValue (), segmentIndex,
           exp->isOverwritingExpression (), snapshotIndex);
      tempExpressions.insert (make_pair (address, tmpExp));
    }
  }
  return tempExpressions;
}

void MemoryEmergedSymbol::instantiateTemporarySymbols (
    std::map < ADDRINT, Expression * > &tempExpressions,
    ADDRINT address, int size, int level,
    const std::map < ADDRINT, Expression * > * const *memoryToExpressionMaps,
    int segmentIndex, int snapshotIndex) {
  std::map < ADDRINT, Expression * >::const_iterator it =
      memoryToExpressionMaps[level]->find (address);
  if (it == memoryToExpressionMaps[level]->end ()) {
    return;
  }
  const Expression *exp = it->second;
  if (exp == 0) {
    instantiateTemporarySymbols
        (tempExpressions, address, size / 2, level + 1,
         memoryToExpressionMaps, segmentIndex, snapshotIndex);
    instantiateTemporarySymbols
        (tempExpressions, address + size / 2, size / 2, level + 1,
         memoryToExpressionMaps, segmentIndex, snapshotIndex);
  } else {
    Expression *tmpExp = new ExpressionImp
        (address, exp->getLastConcreteValue (), segmentIndex,
         exp->isOverwritingExpression (), snapshotIndex);
    tempExpressions.insert (make_pair (address, tmpExp));
  }
}

}
}
}
}
}
