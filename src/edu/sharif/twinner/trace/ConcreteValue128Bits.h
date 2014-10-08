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

#ifndef CONCRETE_VALUE_128_BITS_H
#define CONCRETE_VALUE_128_BITS_H

#include "ConcreteValueAbstractImp.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class ConcreteValue128Bits : public ConcreteValueAbstractImp<128> {

private:
  UINT64 msb, lsb; // most/least significant byte

public:
  ConcreteValue128Bits ();
  ConcreteValue128Bits (UINT64 msb, UINT64 lsb);
  ConcreteValue128Bits (const PIN_REGISTER &value);
  ConcreteValue128Bits (const ConcreteValue &cv);
  virtual ~ConcreteValue128Bits ();

  virtual ConcreteValue128Bits &operator= (UINT64 value);
  virtual ConcreteValue128Bits &operator= (const ConcreteValue &value);

  virtual void saveToBinaryStream (std::ofstream &out) const;

  virtual void writeToMemoryAddress (ADDRINT memoryEa) const;
  virtual void writeToRegister (CONTEXT *context, REG reg) const;

  friend std::basic_ostream<char> &operator<< (std::basic_ostream<char> &stream,
      const ConcreteValue128Bits &me);

  virtual bool isNegative () const;
  virtual bool isZero () const;

  virtual ConcreteValue128Bits *twosComplement () const;
  virtual ConcreteValue128Bits *bitwiseNegated () const;

  virtual ConcreteValue *clone (int length = -1) const;

  UINT64 getLsb () const;
  UINT64 getMsb () const;

  static ConcreteValue128Bits *loadFromBinaryStream (std::ifstream &in);

  virtual UINT64 toUint64 () const {
    return getLsb ();
  }

  virtual bool operator> (const ConcreteValue &cv) const;
  virtual bool greaterThan (const ConcreteValue &cv) const;
  virtual bool operator== (const ConcreteValue &cv) const;

  virtual ConcreteValue128Bits &operator-= (const ConcreteValue &cv);
  virtual ConcreteValue128Bits &operator+= (const ConcreteValue &cv);

  virtual ConcreteValue128Bits &operator&= (const ConcreteValue &mask);
  virtual ConcreteValue128Bits &operator|= (const ConcreteValue &mask);

  virtual ConcreteValue128Bits &operator*= (const ConcreteValue &mul);
  virtual ConcreteValue128Bits &operator/= (const ConcreteValue &divisor);
  virtual ConcreteValue128Bits &operator%= (const ConcreteValue &divisor);
  virtual ConcreteValue128Bits &operator^= (const ConcreteValue &pattern);

  virtual ConcreteValue128Bits &operator>>= (const ConcreteValue &bits);
  virtual ConcreteValue128Bits &operator<<= (const ConcreteValue &bits);

  virtual ConcreteValue128Bits &rotateToRight (const ConcreteValue &bits);

private:

  struct ResultCarry {

    UINT64 result;
    UINT64 carry;

    ResultCarry (UINT64 v) :
        result (v & 0xFFFFFFFF), carry (v >> 32) {
    }
  };

  void divide (ConcreteValue128Bits dividend, ConcreteValue128Bits divisor,
      ConcreteValue128Bits &quotient, ConcreteValue128Bits &remainder) const;

  void doubleIt ();
};

}
}
}
}

#endif	/* ConcreteValue128Bits.h */
