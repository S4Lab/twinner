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

#ifndef SYMBOL_H
#define SYMBOL_H

#include "Operand.h"

#include "pin.H"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

/**
 * @abstract
 */
class Symbol : public Operand {
protected:
  UINT64 concreteValue;
  /**
   * Different values can be stored at the same memory address (as they can be changed by syscalls).
   * This index, starting from zero, indicates which symbol is stored there.
   */
  int generationIndex;

  Symbol (UINT64 concreteValue, int generationIndex);

  Symbol (const Symbol &s);
};

}
}
}
}

#endif /* Symbol.h */
