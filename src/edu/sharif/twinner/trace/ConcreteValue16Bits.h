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

#ifndef CONCRETE_VALUE_16_BITS_H
#define CONCRETE_VALUE_16_BITS_H

#include "ConcreteValueAbstractImp.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class ConcreteValue16Bits : public ConcreteValueAbstractImp<16, UINT16> {

public:
  ConcreteValue16Bits (UINT16 value);
  ConcreteValue16Bits (const ConcreteValue &cv);
  virtual ~ConcreteValue16Bits ();

  virtual void writeToRegister (CONTEXT *context, REG reg) const;

  virtual ConcreteValue16Bits *twosComplement () const;

  virtual ConcreteValue *clone (int length = -1) const;

  static ConcreteValue16Bits *loadFromBinaryStream (std::ifstream &in);
};

}
}
}
}

#endif	/* ConcreteValue16Bits.h */
