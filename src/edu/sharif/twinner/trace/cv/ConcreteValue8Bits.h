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

#ifndef CONCRETE_VALUE_8_BITS_H
#define CONCRETE_VALUE_8_BITS_H

#include "ConcreteValueAbstractImp.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace cv {

class ConcreteValue8Bits : public ConcreteValueAbstractImp<8, UINT8> {
public:
  ConcreteValue8Bits (UINT8 value);
  ConcreteValue8Bits (const ConcreteValue &cv);
  virtual ~ConcreteValue8Bits ();

  virtual bool writeToRegister (CONTEXT *context, LEVEL_BASE::REG reg) const;

  virtual ConcreteValue8Bits *twosComplement () const;
  virtual ConcreteValue8Bits *bitwiseNegated () const;

  static ConcreteValue8Bits *loadFromBinaryStream (std::ifstream &in);

protected:
  virtual ConcreteValue *realClone (int length) const;
};

}
}
}
}
}

#endif /* ConcreteValue8Bits.h */
