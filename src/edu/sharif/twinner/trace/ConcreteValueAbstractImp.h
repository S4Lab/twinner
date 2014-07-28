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

#ifndef CONCRETE_VALUE_ABSTRACT_IMP_H
#define CONCRETE_VALUE_ABSTRACT_IMP_H

#include "ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

template<unsigned int bits> // bits must be a multiple of 8
class ConcreteValueAbstractImp : public ConcreteValue {

public:

  ConcreteValueAbstractImp () : ConcreteValue () {
  }

  virtual ~ConcreteValueAbstractImp () {
  }

  virtual bool isNegative () const {
    // ASSERT: bits <= 64 OTHERWISE: specialize this method and override it in child class
    return (toUint64 () >= (1ull << (bits - 1)));
  }

  virtual bool isZero () const {
    // ASSERT: bits <= 64 OTHERWISE: specialize this method and override it in child class
    return (toUint64 () == 0);
  }

  virtual int getSize () const {
    return bits;
  }
};

template <>
bool ConcreteValueAbstractImp<128>::isNegative () const;
template <>
bool ConcreteValueAbstractImp<128>::isZero () const;

}
}
}
}

#endif	/* ConcreteValueAbstractImp.h */
