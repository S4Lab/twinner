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

#include "edu/sharif/twinner/util/memory.h"

#include <sstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

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

  virtual void writeToMemoryAddress (ADDRINT memoryEa) const {
    edu::sharif::twinner::util::writeMemoryContent
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
    const ValueType cvValue = cv.toUint64 ();
    return value > cvValue;
  }

  virtual bool greaterThan (const ConcreteValue &cv) const {
    if (getSize () < cv.getSize ()) {
      return cv.lessThan (*this);
    }
    const ValueType cvValue = cv.toUint64 ();
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
    const ValueType cvValue = cv.toUint64 ();
    return value == cvValue;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &operator-= (const ConcreteValue &cv) {
    const ValueType cvValue = cv.toUint64 ();
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
    const ValueType cvValue = cv.toUint64 ();
    value *= cvValue;
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &operator/= (const ConcreteValue &cv) {
    const ValueType cvValue = cv.toUint64 ();
    if (cvValue == 0) {
      throw std::runtime_error ("division by zero");
    }
    value /= cvValue;
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &operator%= (const ConcreteValue &cv) {
    const ValueType cvValue = cv.toUint64 ();
    value %= cvValue;
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &operator^= (const ConcreteValue &cv) {
    const ValueType cvValue = cv.toUint64 ();
    cf = false;
    value ^= cvValue;
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &operator>>= (const ConcreteValue &cv) {
    const ValueType cvValue = cv.toUint64 ();
    if (cvValue == 1) {
      cf = value & 0x1;
    } else {
      cf = (value >> (cvValue - 1)) & 0x1;
    }
    value >>= cvValue;
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &operator<<= (const ConcreteValue &cv) {
    const ValueType cvValue = cv.toUint64 ();
    cf = (value >> (bits - cvValue)) & 0x1;
    value <<= cvValue;
    return *this;
  }

  virtual ConcreteValueAbstractImp<bits, ValueType> &rotateToRight (const ConcreteValue &cv) {
    const ValueType cvValue = cv.toUint64 ();
    value = (value << (bits - cvValue)) | (value >> cvValue);
    return *this;
  }
};

template<> inline
ConcreteValue *ConcreteValueAbstractImp<64, UINT64>::signExtended (int length) const {
  throw std::runtime_error
      ("64-bits sign-extension is not supported by ConcreteValueAbstractImp");
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

#endif	/* ConcreteValueAbstractImp.h */
