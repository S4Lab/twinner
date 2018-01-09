//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2018 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "SymbolRepresentation.h"

#include <sstream>

#include "Snapshot.h"

#include "edu/sharif/twinner/trace/exptoken/RegisterEmergedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/MemoryEmergedSymbol.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"
#include "Expression.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

SymbolRepresentation::SymbolRepresentation (
    const edu::sharif::twinner::trace::exptoken::Symbol *sym) :
    generationIndex (sym->getGenerationIndex ()),
    snapshotIndex (sym->getSnapshotIndex ()),
    cv (0) {
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

SymbolRepresentation::SymbolRepresentation (
    const edu::sharif::twinner::trace::exptoken::Symbol *sym,
    const edu::sharif::twinner::trace::cv::ConcreteValue &_cv) :
    generationIndex (sym->getGenerationIndex ()),
    snapshotIndex (sym->getSnapshotIndex ()),
    cv (_cv.toUint64 ()) {
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
      if (memSize > 64) {
        edu::sharif::twinner::util::Logger::error ()
            << "SymbolRepresentation constructor:"
            " memory size is larger than 64 bits\n";
        abort ();
      }
    } else {
      edu::sharif::twinner::util::Logger::error ()
          << "SymbolRepresentation constructor: symbol should be reg or mem\n";
      abort ();
    }
  }
}

Expression *SymbolRepresentation::resolve (const Snapshot *sna) const {
  Expression *exp;
  if (isReg) {
    exp = sna->resolveRegister (regAddress);
  } else {
    exp = sna->resolveMemory (memSize, memAddress);
    if (exp && cv) {
      exp->bitwiseAnd (cv);
    }
  }
  return exp;
}

bool SymbolRepresentation::operator== (const SymbolRepresentation &sr) const {
  return isReg == sr.isReg
      && generationIndex == sr.generationIndex
      && snapshotIndex == sr.snapshotIndex
      && ((isReg && regAddress == sr.regAddress)
          || (!isReg
              && memAddress == sr.memAddress
              && memSize == sr.memSize
              && cv == sr.cv));
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
        || (memAddress == sr.memAddress && memSize < sr.memSize)
        || (memAddress == sr.memAddress && memSize == sr.memSize
            && cv < sr.cv);
  }
}

std::string SymbolRepresentation::toString () const {
  std::stringstream ss;
  if (isReg) {
    ss << "Type=Reg, Addr=" << regAddress;
  } else {
    ss << "Type=Mem, Addr=0x" << std::hex << memAddress
        << ", Size=" << std::dec << memSize;
    if (cv) {
      ss << ", Mask=0x" << std::hex << cv;
    } else {
      ss << ", No Mask";
    }
  }
  ss << std::hex << ", SegmentIndex=0x" << generationIndex
      << ", SnapshotIndex=0x" << snapshotIndex;
  return ss.str ();
}


}
}
}
}
