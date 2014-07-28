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

class ConcreteValue32Bits : public ConcreteValueAbstractImp<32> {

private:
  UINT32 value;

public:
  ConcreteValue32Bits (UINT32 value);
  ConcreteValue32Bits (const ConcreteValue &cv);
  virtual ~ConcreteValue32Bits ();

  virtual ConcreteValue32Bits &operator= (UINT64 value);
  ConcreteValue32Bits &operator= (const ConcreteValue &value);

  virtual void saveToBinaryStream (std::ofstream &out) const;

  virtual void writeToMemoryAddress (ADDRINT memoryEa) const;
  virtual void writeToRegister (CONTEXT *context, REG reg) const;

  friend std::basic_ostream<char> &operator<< (std::basic_ostream<char> &stream,
      const ConcreteValue32Bits &me);

  virtual ConcreteValue32Bits *twosComplement () const;

  virtual ConcreteValue32Bits *clone () const;

  UINT32 getValue () const;

  static ConcreteValue32Bits *loadFromBinaryStream (std::ifstream &in);

  virtual UINT64 toUint64 () const {
    return getValue ();
  }

  virtual bool operator> (const ConcreteValue &cv) const;
  virtual bool greaterThan (const ConcreteValue &cv) const;
  virtual bool operator== (const ConcreteValue &cv) const;

  virtual ConcreteValue32Bits &operator-= (const ConcreteValue &cv);
  virtual ConcreteValue32Bits &operator+= (const ConcreteValue &cv);

  virtual ConcreteValue32Bits &operator&= (const ConcreteValue &mask);
  virtual ConcreteValue32Bits &operator|= (const ConcreteValue &mask);

  virtual ConcreteValue32Bits &operator*= (const ConcreteValue &mul);
  virtual ConcreteValue32Bits &operator/= (const ConcreteValue &divisor);
  virtual ConcreteValue32Bits &operator%= (const ConcreteValue &divisor);
  virtual ConcreteValue32Bits &operator^= (const ConcreteValue &pattern);

  virtual ConcreteValue32Bits &operator>>= (const ConcreteValue &bits);
  virtual ConcreteValue32Bits &operator<<= (const ConcreteValue &bits);
};

}
}
}
}

#endif	/* ConcreteValue32Bits.h */
