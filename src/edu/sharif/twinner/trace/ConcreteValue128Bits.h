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
#define	CONCRETE_VALUE_128_BITS_H

#include "ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class ConcreteValue128Bits : public ConcreteValue {

private:
  UINT64 msb, lsb; // most/least significant byte

public:
  ConcreteValue128Bits ();
  ConcreteValue128Bits (UINT64 msb, UINT64 lsb);
  ConcreteValue128Bits (const PIN_REGISTER &value);
  ConcreteValue128Bits (const ConcreteValue &cv);
  virtual ~ConcreteValue128Bits ();

  ConcreteValue128Bits &operator= (UINT64 value);
  ConcreteValue128Bits &operator= (const ConcreteValue &value);

  virtual void saveToBinaryStream (std::ofstream &out) const;

  virtual void writeToMemoryAddress (ADDRINT memoryEa) const;
  virtual void writeToRegister (CONTEXT *context, REG reg) const;

  friend std::basic_ostream<char> &operator<< (std::basic_ostream<char> &stream,
      const ConcreteValue128Bits &me);

  virtual bool isNegative (int size) const;
  virtual bool isZero () const;

  virtual ConcreteValue128Bits *twosComplement () const;

  virtual ConcreteValue128Bits *clone () const;

  UINT64 getLsb () const;
  UINT64 getMsb () const;

  static ConcreteValue128Bits *loadFromBinaryStream (std::ifstream &in);

  virtual int getSize () const;

  virtual bool operator> (const ConcreteValue &cv) const;
  virtual bool operator== (const ConcreteValue &cv) const;

  virtual ConcreteValue128Bits &operator-= (const ConcreteValue &cv);
  virtual ConcreteValue128Bits &operator+= (const ConcreteValue &cv);

  virtual ConcreteValue128Bits &operator&= (const ConcreteValue &mask);
  virtual ConcreteValue128Bits &operator|= (const ConcreteValue &mask);

  virtual ConcreteValue &operator*= (const ConcreteValue &mul);
  virtual ConcreteValue &operator/= (const ConcreteValue &divisor);
  virtual ConcreteValue &operator%= (const ConcreteValue &divisor);
  virtual ConcreteValue &operator^= (const ConcreteValue &pattern);

  virtual ConcreteValue128Bits &operator>>= (const ConcreteValue &bits);
  virtual ConcreteValue128Bits &operator<<= (const ConcreteValue &bits);

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
