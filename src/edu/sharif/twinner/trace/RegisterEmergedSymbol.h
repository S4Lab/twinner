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

#ifndef REGISTEREMERGEDSYMBOL_H_
#define REGISTEREMERGEDSYMBOL_H_

#include "Symbol.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class RegisterEmergedSymbol : public Symbol {

private:
  REG address;

  RegisterEmergedSymbol (const RegisterEmergedSymbol &s);

  RegisterEmergedSymbol (REG address);

public:
  RegisterEmergedSymbol (REG address, const ConcreteValue &concreteValue,
      int generationIndex);

  virtual RegisterEmergedSymbol *clone () const;
  virtual std::pair < int, SymbolRecord > toSymbolRecord () const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static RegisterEmergedSymbol *loadFromBinaryStream (std::ifstream &in);
  static RegisterEmergedSymbol *fromNameAndValue (const std::string &name, UINT64 value);

  virtual std::string toString () const;
  virtual bool operator== (const ExpressionToken &token) const;

private:
  const char *getRegisterName () const;
  static REG getRegisterFromName (const std::string &name);
};

}
}
}
}

#endif /* REGISTEREMERGEDSYMBOL_H_ */
