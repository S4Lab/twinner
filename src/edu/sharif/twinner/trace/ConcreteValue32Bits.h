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

#ifndef CONCRETE_VALUE_32_BITS_H
#define CONCRETE_VALUE_32_BITS_H

#include "ConcreteValueAbstractImp.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class ConcreteValue32Bits : public ConcreteValueAbstractImp<32, UINT32> {

public:
  ConcreteValue32Bits (UINT32 value);
  ConcreteValue32Bits (const ConcreteValue &cv);
  virtual ~ConcreteValue32Bits ();

  virtual void writeToRegister (CONTEXT *context, REG reg) const;

  virtual ConcreteValue32Bits *twosComplement () const;

  virtual ConcreteValue *clone (int length = -1) const;

  static ConcreteValue32Bits *loadFromBinaryStream (std::ifstream &in);
};

}
}
}
}

#endif	/* ConcreteValue32Bits.h */
