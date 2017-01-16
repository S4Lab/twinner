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

#ifndef CONCRETE_VALUE_128_BITS_H
#define CONCRETE_VALUE_128_BITS_H

#include "ConcreteValueAbstractImp.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace cv {

class ConcreteValue128Bits : public ConcreteValueAbstractImp<128> {
private:
  UINT64 msb, lsb; // most/least significant byte
  bool cf;

public:
  ConcreteValue128Bits ();
  ConcreteValue128Bits (UINT64 msb, UINT64 lsb);
  ConcreteValue128Bits (const PIN_REGISTER &value);
  ConcreteValue128Bits (const ConcreteValue &cv);
  virtual ~ConcreteValue128Bits ();

  virtual ConcreteValue128Bits &operator= (UINT64 value);
  virtual ConcreteValue128Bits &operator= (const ConcreteValue &value);

  virtual void saveToBinaryStream (std::ofstream &out) const;

  virtual bool writeToMemoryAddress (ADDRINT memoryEa) const;
  virtual bool writeToRegister (CONTEXT *context, LEVEL_BASE::REG reg) const;

  friend std::basic_ostream<char> &operator<< (std::basic_ostream<char> &stream,
      const ConcreteValue128Bits &me);
  virtual std::string toHexString () const;

  virtual bool getCarryBit () const;
  virtual void setCarryBit (bool cf);

  virtual bool isNegative () const;
  virtual bool isZero () const;
  virtual bool isCompletePowerOfTwo (int *n) const;

  virtual ConcreteValue128Bits *twosComplement () const;
  virtual ConcreteValue128Bits *bitwiseNegated () const;

  virtual ConcreteValue *signExtended (int length) const;

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

  virtual ConcreteValue128Bits &signedDivide (const ConcreteValue &divisor);
  virtual ConcreteValue128Bits &signedRemainder (const ConcreteValue &divisor);

  virtual ConcreteValue128Bits &operator>>= (const ConcreteValue &bits);
  virtual ConcreteValue128Bits &operator<<= (const ConcreteValue &bits);

  virtual ConcreteValue128Bits &arithmeticShiftToRight (const ConcreteValue &cv);
  virtual ConcreteValue128Bits &rotateToRight (const ConcreteValue &bits);
  virtual ConcreteValue128Bits &rotateToLeft (const ConcreteValue &bits);

protected:
  virtual ConcreteValue *realClone (int length) const;

private:
  bool wouldSummationHaveCarry (UINT64 a, UINT64 b) const;
  bool getNthBit (UINT64 value, int n) const;

  void divide (ConcreteValue128Bits dividend, ConcreteValue128Bits divisor,
      ConcreteValue128Bits &quotient, ConcreteValue128Bits &remainder,
      bool signedArithmetic = false) const;

  void doubleIt ();

  void twosComplement (ConcreteValue128Bits &num) const;
  bool absolute (ConcreteValue128Bits &num) const;
};

}
}
}
}
}

#endif /* ConcreteValue128Bits.h */
