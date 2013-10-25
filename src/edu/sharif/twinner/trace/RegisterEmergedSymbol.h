//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013  Behnam Momeni
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

public:
  RegisterEmergedSymbol (REG address, UINT64 concreteValue, int generationIndex);

  virtual RegisterEmergedSymbol *clone () const;
};

}
}
}
}

#endif /* REGISTEREMERGEDSYMBOL_H_ */
