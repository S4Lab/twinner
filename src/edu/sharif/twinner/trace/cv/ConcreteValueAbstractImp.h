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

#ifndef CONCRETE_VALUE_ABSTRACT_IMP_H
#define CONCRETE_VALUE_ABSTRACT_IMP_H

#include "ConcreteValue.h"

#include "edu/sharif/twinner/util/memory.h"

#include <sstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace cv {

struct ResultCarry {
  UINT64 result;
  UINT64 carry;

  ResultCarry (UINT64 v) :
  result (v & 0xFFFFFFFF), carry (v >> 32) {
  }

  operator bool () const {
    return result != 0 || carry != 0;
  }
};

template<unsigned int bits, /* bits must be a multiple of 8 */
typename ValueType = UINT64 /* bits-length type */>
class ConcreteValueAbstractImp : public ConcreteValue {
protected:
  ValueType value;
  bool cf;

public:

  ConcreteValueAbstractImp (ValueType _val = 0) : ConcreteValue (), value (_val) {
  }

  virtual ~ConcreteValueAbstractImp () {
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &operator= (UINT64 v) {
    value = v;
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &operator= (const ConcreteValue &v) {
    value = v.toUint64 ();
    cf = v.getCarryBit ();
    return *this;
  }

  virtual void saveToBinaryStream (std::ofstream &out) const {
    const UINT32 size = bits;
    out.write ((const char *) &size, sizeof (size));
    out.write ((const char *) &value, sizeof (value));
  }

  virtual bool writeToMemoryAddress (ADDRINT memoryEa) const {
    return edu::sharif::twinner::util::writeMemoryContent
        (memoryEa, (const UINT8 *) &value, sizeof (value));
  }

  friend std::basic_ostream<char> &operator<< (std::basic_ostream<char> &stream,
      const ConcreteValueAbstractImp<bits, ValueType> &me) {
    // ASSERT: bits <= 64
    return stream << "0x" << std::hex << UINT64 (me.value);
  }

  virtual std::string toHexString () const {
    std::stringstream ss;
    ss << std::hex << UINT64 (value);
    return ss.str ();
  }

  virtual bool getCarryBit () const {
    return cf;
  }

  virtual void setCarryBit (bool cf) {
    this->cf = cf;
  }

  virtual bool isNegative () const {
    return (toUint64 () >= (1ull << (bits - 1)));
  }

  virtual bool isZero () const {
    return (toUint64 () == 0);
  }

  virtual bool isCompletePowerOfTwo (int *n) const {
    ValueType v = value;
    int b = 0;
    while (v > 0) {
      v >>= 1;
      b++;
    }
    if (b == 0) {
      return false;
    } else if (b == 1 || value == (1ull << (b - 1))) {
      if (n) {
        *n = b - 1;
      }
      return true;
    } else {
      return false;
    }
  }

  virtual ValueType getValue () const {
    return value;
  }

  ConcreteValue *signExtended (int length) const {
    if (isNegative ()) {
      ConcreteValue *val = clone (length);
      (*val) -= (1ull << bits);
      return val;
    } else { // sign/zero extension are the same for positive concrete values
      return clone (length);
    }
  }

  virtual int getSize () const {
    return bits;
  }

  virtual UINT64 toUint64 () const {
    return getValue ();
  }

  virtual bool operator> (const ConcreteValue &cv) const {
    if (getSize () < cv.getSize ()) {
      return (cv < (*this));
    }
    const UINT64 cvValue = cv.toUint64 ();
    return value > cvValue;
  }

  virtual bool greaterThan (const ConcreteValue &cv) const {
    if (getSize () < cv.getSize ()) {
      return cv.lessThan (*this);
    }
    const UINT64 cvValue = cv.toUint64 ();
    if (isNegative ()) {
      if (cv.isNegative ()) {
        return value > cvValue;
      } else {
        return false;
      }
    } else {
      if (cv.isNegative ()) {
        return true;
      } else {
        return value > cvValue;
      }
    }
  }

  virtual bool operator== (const ConcreteValue &cv) const {
    if (getSize () < cv.getSize ()) {
      return (cv == (*this));
    }
    const UINT64 cvValue = cv.toUint64 ();
    return value == cvValue;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &operator-= (const ConcreteValue &cv) {
    const UINT64 cvValue = cv.toUint64 ();
    cf = (value < cvValue);
    value -= cvValue;
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &operator+= (const ConcreteValue &cv) {
    const ValueType cvValue = cv.toUint64 ();
    const ValueType maxValue = ValueType (-1);
    cf = (value > maxValue - cvValue);
    value += cvValue;
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &operator&= (const ConcreteValue &cv) {
    const ValueType cvValue = cv.toUint64 ();
    cf = false;
    value &= cvValue;
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &operator|= (const ConcreteValue &cv) {
    const ValueType cvValue = cv.toUint64 ();
    cf = false;
    value |= cvValue;
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &operator*= (const ConcreteValue &cv) {
    // bits <= 32
    const UINT64 cvValue = cv.toUint64 ();
    UINT64 result = value;
    result *= cvValue;
    value = result;
    cf = (value != result);
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &operator/= (const ConcreteValue &cv) {
    const UINT64 cvValue = cv.toUint64 ();
    if (cvValue == 0) {
      edu::sharif::twinner::util::Logger::error () << "division by zero\n";
      abort ();
    }
    value /= cvValue;
    cf = false;
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &signedDivide (
      const ConcreteValue &divisor) {
    const INT64 cvValue = INT64 (ValueType (divisor.toUint64 ()));
    if (cvValue == 0) {
      edu::sharif::twinner::util::Logger::error () << "division by zero\n";
      abort ();
    }
    INT64 res = INT64 (value);
    res /= cvValue;
    value = res;
    cf = false;
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &operator%= (const ConcreteValue &cv) {
    const UINT64 cvValue = cv.toUint64 ();
    value %= cvValue;
    cf = false;
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &signedRemainder (
      const ConcreteValue &divisor) {
    const INT64 cvValue = INT64 (ValueType (divisor.toUint64 ()));
    if (cvValue == 0) {
      edu::sharif::twinner::util::Logger::error () << "remainder by zero\n";
      abort ();
    }
    INT64 res = INT64 (value);
    res %= cvValue;
    value = res;
    cf = false;
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &operator^= (const ConcreteValue &cv) {
    const ValueType cvValue = cv.toUint64 ();
    cf = false;
    value ^= cvValue;
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &operator>>= (const ConcreteValue &cv) {
    const UINT64 cvValue = cv.toUint64 ();
    if (cvValue == 1) {
      cf = value & 0x1;
    } else if (cvValue > bits) {
      cf = 0;
    } else {
      cf = (value >> (cvValue - 1)) & 0x1;
    }
    if (cvValue >= bits) {
      value = 0; // CPU truncates cvValue; so it must be considered by caller
    } else {
      value >>= cvValue;
    }
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &operator<<= (const ConcreteValue &cv) {
    const UINT64 cvValue = cv.toUint64 ();
    if (cvValue >= bits) {
      cf = (cvValue == bits ? value & 0x1 : 0);
      value = 0;
      return *this; // CPU truncates cvValue; so it must be considered by caller
    }
    cf = (value >> (bits - cvValue)) & 0x1;
    value <<= cvValue;
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &arithmeticShiftToRight (
      const ConcreteValue &cv) {
    const bool signBit = value >> (bits - 1);
    (*this) >>= cv;
    if (signBit) {
      const UINT64 cvValue = cv.toUint64 ();
      if (cvValue > bits) {
        cf = 1;
      }
      if (cvValue >= bits) {
        value = ValueType (-1);
        return *this;
      }
      ValueType mask = 1;
      mask <<= cvValue;
      mask -= 1;
      mask <<= bits - cvValue;
      value |= mask;
    }
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &rotateToRight (const ConcreteValue &cv) {
    //TODO: handle CF
    const UINT64 cvValue = cv.toUint64 ();
    value = (value << (bits - cvValue)) | (value >> cvValue);
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &rotateToLeft (const ConcreteValue &cv) {
    //TODO: handle CF
    const UINT64 cvValue = cv.toUint64 ();
    value = (value << cvValue) | (value >> (bits - cvValue));
    return *this;
  }
};

template<> inline
ConcreteValue *ConcreteValueAbstractImp<64, UINT64>::signExtended (int length) const {
  edu::sharif::twinner::util::Logger::error ()
      << "ConcreteValueAbstractImp<64, UINT64>::signExtended (length="
      << length << "): 64-bits sign-extension is not supported"
      " by ConcreteValueAbstractImp\n";
  abort ();
}

template<> inline
ConcreteValueAbstractImp<64, UINT64> &ConcreteValueAbstractImp<64, UINT64>::operator*= (
    const ConcreteValue &cv) {
  const UINT64 cvValue = cv.toUint64 ();
  const UINT64 b2 = cvValue & 0xFFFFFFFF;
  const UINT64 b1 = cvValue >> 32;
  const UINT64 a2 = value & 0xFFFFFFFF;
  const UINT64 a1 = value >> 32;
  //   b1 b2
  // * a1 a2
  const ResultCarry d2 (a2 * b2);

  const ResultCarry c21 (a2 * b1);
  const ResultCarry c12 (a1 * b2);
  const ResultCarry d1 (c21.result + c12.result + d2.carry);

  const ResultCarry c11 (a1 * b1);
  const ResultCarry d0 (c11.result + d1.carry);
  // d0 is overflowed and ignored
  value = (d1.result << 32) | d2.result;
  cf = d0;
  return *this;
}

template<>
class ConcreteValueAbstractImp<128, UINT64> : public ConcreteValue {
public:

  ConcreteValueAbstractImp () : ConcreteValue () {
  }

  virtual ~ConcreteValueAbstractImp () {
  }

  virtual int getSize () const {
    return 128;
  }
};

}
}
}
}
}

#endif /* ConcreteValueAbstractImp.h */
