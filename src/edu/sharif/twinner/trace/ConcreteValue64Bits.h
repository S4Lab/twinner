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

#ifndef CONCRETE_VALUE_64_BITS_H
#define CONCRETE_VALUE_64_BITS_H

#include "ConcreteValueAbstractImp.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class ConcreteValue64Bits : public ConcreteValueAbstractImp<64> {

private:
  UINT64 value;

public:
  ConcreteValue64Bits (UINT64 value);
  ConcreteValue64Bits (const ConcreteValue &cv);
  virtual ~ConcreteValue64Bits ();

  virtual ConcreteValue64Bits &operator= (UINT64 value);
  ConcreteValue64Bits &operator= (const ConcreteValue &value);

  virtual void saveToBinaryStream (std::ofstream &out) const;

  virtual void writeToMemoryAddress (ADDRINT memoryEa) const;
  virtual void writeToRegister (CONTEXT *context, REG reg) const;

  friend std::basic_ostream<char> &operator<< (std::basic_ostream<char> &stream,
      const ConcreteValue64Bits &me);

  virtual ConcreteValue64Bits *twosComplement () const;

  virtual ConcreteValue64Bits *clone () const;

  UINT64 getValue () const;

  static ConcreteValue64Bits *loadFromBinaryStream (std::ifstream &in);

  virtual UINT64 toUint64 () const {
    return getValue ();
  }

  virtual bool operator> (const ConcreteValue &cv) const;
  virtual bool greaterThan (const ConcreteValue &cv) const;
  virtual bool operator== (const ConcreteValue &cv) const;

  virtual ConcreteValue64Bits &operator-= (const ConcreteValue &cv);
  virtual ConcreteValue64Bits &operator+= (const ConcreteValue &cv);

  virtual ConcreteValue64Bits &operator&= (const ConcreteValue &mask);
  virtual ConcreteValue64Bits &operator|= (const ConcreteValue &mask);

  virtual ConcreteValue64Bits &operator*= (const ConcreteValue &mul);
  virtual ConcreteValue64Bits &operator/= (const ConcreteValue &divisor);
  virtual ConcreteValue64Bits &operator%= (const ConcreteValue &divisor);
  virtual ConcreteValue64Bits &operator^= (const ConcreteValue &pattern);

  virtual ConcreteValue64Bits &operator>>= (const ConcreteValue &bits);
  virtual ConcreteValue64Bits &operator<<= (const ConcreteValue &bits);
};

}
}
}
}

#endif	/* ConcreteValue64Bits.h */
