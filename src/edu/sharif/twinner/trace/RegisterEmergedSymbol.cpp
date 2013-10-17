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

#include "RegisterEmergedSymbol.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

RegisterEmergedSymbol::RegisterEmergedSymbol (const RegisterEmergedSymbol &s) :
    Symbol (s), address (s.address) {
}

RegisterEmergedSymbol::RegisterEmergedSymbol (REG _address, UINT64 concreteValue,
    int generationIndex) :
    Symbol (concreteValue, generationIndex), address (_address) {
}

RegisterEmergedSymbol *RegisterEmergedSymbol::clone () const {
  return new RegisterEmergedSymbol (*this);
}

}
}
}
}
