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

#ifndef SYMBOL_REF_H
#define SYMBOL_REF_H

#include "edu/sharif/twinner/pin-wrapper.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace cv {

class ConcreteValue;
}
namespace exptoken {

class Symbol;
}

class Snapshot;
class Expression;

class SymbolRepresentation {
private:
  int generationIndex;
  int snapshotIndex;
  bool isReg; // otherwise, it is Mem
  REG regAddress;
  ADDRINT memAddress;
  int memSize;
  UINT64 cv;

public:
  SymbolRepresentation (
      const edu::sharif::twinner::trace::exptoken::Symbol *symbol);
  SymbolRepresentation (
      const edu::sharif::twinner::trace::exptoken::Symbol *symbol,
      const edu::sharif::twinner::trace::cv::ConcreteValue &cv);

  /**
   * Caller takes ownership of the returned expression.
   */
  Expression *resolve (const Snapshot *sna) const;
  bool operator== (const SymbolRepresentation &sr) const;
  bool operator< (const SymbolRepresentation &sr) const;

  std::string toString () const;
};

}
}
}
}

#endif /* SymbolRepresentation.h */
