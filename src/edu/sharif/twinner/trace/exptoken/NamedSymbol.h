//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2015  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef NAMED_SYMBOL_H_
#define NAMED_SYMBOL_H_

#include "Symbol.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

class NamedSymbol : public Symbol {
private:
  std::string name;
  bool constant;

  NamedSymbol (const NamedSymbol &symbol);
  NamedSymbol (std::string name, bool constant);

public:
  NamedSymbol (std::string name, bool constant,
      const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue,
      int generationIndex);

  virtual NamedSymbol *clone () const;
  virtual std::pair < int, SymbolRecord > toSymbolRecord () const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static NamedSymbol *loadFromBinaryStream (std::ifstream &in);

  virtual std::string toString () const;
  virtual std::string technicalName () const;
  virtual bool operator== (const ExpressionToken &token) const;

  virtual bool isConstant () const;
};

}
}
}
}
}

#endif /* NamedSymbol.h */
