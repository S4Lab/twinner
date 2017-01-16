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

#ifndef CONCRETE_VALUE_16_BITS_H
#define CONCRETE_VALUE_16_BITS_H

#include "ConcreteValueAbstractImp.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace cv {

class ConcreteValue16Bits : public ConcreteValueAbstractImp<16, UINT16> {
public:
  ConcreteValue16Bits (UINT16 value);
  ConcreteValue16Bits (const ConcreteValue &cv);
  virtual ~ConcreteValue16Bits ();

  virtual bool writeToRegister (CONTEXT *context, LEVEL_BASE::REG reg) const;

  virtual ConcreteValue16Bits *twosComplement () const;
  virtual ConcreteValue16Bits *bitwiseNegated () const;

  static ConcreteValue16Bits *loadFromBinaryStream (std::ifstream &in);

protected:
  virtual ConcreteValue *realClone (int length) const;
};

}
}
}
}
}

#endif /* ConcreteValue16Bits.h */
