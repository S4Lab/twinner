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

#include "SymbolRepresentation.h"

#include "Snapshot.h"

#include "edu/sharif/twinner/trace/exptoken/RegisterEmergedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/MemoryEmergedSymbol.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

SymbolRepresentation::SymbolRepresentation (const edu::sharif::twinner::trace::exptoken::Symbol *sym) :
    generationIndex (sym->getGenerationIndex ()),
    snapshotIndex (sym->getSnapshotIndex ()) {
  const edu::sharif::twinner::trace::exptoken::RegisterEmergedSymbol *reg =
      dynamic_cast<const edu::sharif::twinner::trace::exptoken
      ::RegisterEmergedSymbol *> (sym);
  if (reg) {
    isReg = true;
    regAddress = reg->getAddress ();
  } else {
    const edu::sharif::twinner::trace::exptoken::MemoryEmergedSymbol *mem =
        dynamic_cast<const edu::sharif::twinner::trace::exptoken
        ::MemoryEmergedSymbol *> (sym);
    if (mem) {
      isReg = false;
      memAddress = mem->getAddress ();
      memSize = mem->getValue ().getSize ();
    } else {
      edu::sharif::twinner::util::Logger::error ()
          << "SymbolRepresentation constructor: symbol should be reg or mem\n";
      abort ();
    }
  }
}

const Expression *SymbolRepresentation::resolve (const Snapshot *sna) const {
  if (isReg) {
    return sna->resolveRegister (regAddress);
  } else {
    return sna->resolveMemory (memSize, memAddress);
  }
}

bool SymbolRepresentation::operator== (const SymbolRepresentation &sr) const {
  return isReg == sr.isReg
      && generationIndex == sr.generationIndex
      && snapshotIndex == sr.snapshotIndex
      && ((isReg && regAddress == sr.regAddress)
          || (!isReg && memAddress == sr.memAddress && memSize == sr.memSize));
}

bool SymbolRepresentation::operator< (const SymbolRepresentation &sr) const {
  if (generationIndex < sr.generationIndex
      || (generationIndex == sr.generationIndex
          && snapshotIndex < sr.snapshotIndex)) {
    return true;
  }
  if (generationIndex > sr.generationIndex
      || snapshotIndex > sr.snapshotIndex) {
    return false;
  }
  if (isReg) {
    return sr.isReg
        && regAddress < sr.regAddress;
  } else {
    return sr.isReg
        || memAddress < sr.memAddress
        || (memAddress == sr.memAddress && memSize < sr.memSize);
  }
}


}
}
}
}
