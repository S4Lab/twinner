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
  std::string technicalName;
  bool constant;

  NamedSymbol (const NamedSymbol &symbol);
  NamedSymbol (std::string name, std::string techName, bool constant);

public:
  NamedSymbol (std::string name, std::string techName, bool constant,
      const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue,
      int generationIndex);

  virtual NamedSymbol *clone () const;
  virtual std::pair < int, SymbolRecord > toSymbolRecord () const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static NamedSymbol *loadFromBinaryStream (std::ifstream &in);

  virtual std::string toString () const;
  virtual std::string getTechnicalName () const;
  virtual bool operator== (const ExpressionToken &token) const;
  virtual bool operator< (const Symbol &symbol) const;

  virtual bool isConstant () const;

  static NamedSymbol *fromTechnicalName (const std::string &name,
      const edu::sharif::twinner::trace::cv::ConcreteValue &value);
  static NamedSymbol *fromTechnicalName (const std::string &name,
      UINT32 v4, UINT32 v3, UINT32 v2, UINT32 v1);
};

}
}
}
}
}

#endif /* NamedSymbol.h */
