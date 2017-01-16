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

#ifndef CONCRETE_VALUE_64_BITS_H
#define CONCRETE_VALUE_64_BITS_H

#include "ConcreteValueAbstractImp.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace cv {

class ConcreteValue64Bits : public ConcreteValueAbstractImp<64, UINT64> {
public:
  ConcreteValue64Bits (UINT64 value);
  ConcreteValue64Bits (const ConcreteValue &cv);
  virtual ~ConcreteValue64Bits ();

  virtual bool writeToRegister (CONTEXT *context, LEVEL_BASE::REG reg) const;

  virtual ConcreteValue64Bits *twosComplement () const;
  virtual ConcreteValue64Bits *bitwiseNegated () const;

  virtual ConcreteValue *signExtended (int length) const;

  static ConcreteValue64Bits *loadFromBinaryStream (std::ifstream &in);

protected:
  virtual ConcreteValue *realClone (int length) const;
};

}
}
}
}
}

#endif /* ConcreteValue64Bits.h */
