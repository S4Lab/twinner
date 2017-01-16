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

#ifndef CONCRETE_VALUE_32_BITS_H
#define CONCRETE_VALUE_32_BITS_H

#include "ConcreteValueAbstractImp.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace cv {

class ConcreteValue32Bits : public ConcreteValueAbstractImp<32, UINT32> {
public:
  ConcreteValue32Bits (UINT32 value);
  ConcreteValue32Bits (const ConcreteValue &cv);
  virtual ~ConcreteValue32Bits ();

  virtual bool writeToRegister (CONTEXT *context, LEVEL_BASE::REG reg) const;

  virtual ConcreteValue32Bits *twosComplement () const;
  virtual ConcreteValue32Bits *bitwiseNegated () const;

  static ConcreteValue32Bits *loadFromBinaryStream (std::ifstream &in);

protected:
  virtual ConcreteValue *realClone (int length) const;
};

}
}
}
}
}

#endif /* ConcreteValue32Bits.h */
