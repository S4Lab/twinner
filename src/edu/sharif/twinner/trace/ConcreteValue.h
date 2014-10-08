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

#ifndef CONCRETE_VALUE_H
#define	CONCRETE_VALUE_H

#include "Savable.h"

#include <pin.H>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

/**
 * @interface ConcreteValue is responsible for keeping and applying arithmetic/logical
 * operations on a concrete value. Implementations of this interface may support different
 * length (e.g. 64 bits implementation using UINT64) values.
 */
class ConcreteValue : public Savable {

protected:
  ConcreteValue ();

public:
  virtual ~ConcreteValue ();

  virtual ConcreteValue &operator= (UINT64 value) = 0;
  virtual ConcreteValue &operator= (const ConcreteValue &v) = 0;

  virtual void writeToMemoryAddress (ADDRINT memoryEa) const = 0;
  virtual void writeToRegister (CONTEXT *context, REG reg) const = 0;

  friend std::basic_ostream<char> &operator<< (std::basic_ostream<char> &stream,
      const ConcreteValue &me);

  /**
   * Interpreting this concrete value as a signed value which is encoded using
   * two's complement rules, return true iff the value is negative.
   *
   * @return True if this value is negative and false if it is zero or positive.
   */
  virtual bool isNegative () const = 0;

  /**
   * Returns true if and only if the underlying concrete value is equal to zero.
   * @return true iff concrete value is zero.
   */
  virtual bool isZero () const = 0;

  /**
   * Instantiates a new concrete value object holding two's complement of this concrete
   * value. Caller should delete the returned object.
   * @return The two's complement of this concrete value.
   */
  virtual ConcreteValue *twosComplement () const = 0;

  /**
   * Instantiates a new concrete value object holding the bitwise negated value of this
   * concrete value. Caller should delete the returned object.
   * @return The bitwise negated of this concrete value.
   */
  virtual ConcreteValue *bitwiseNegated () const = 0;

  /**
   * Clones the concrete value object. For values of length else of -1, the concrete
   * value will be casted to hold a length-bits value (at most).
   *
   * @param length The max precision of resulting value (in bits) or -1 (for no change)
   * @return The cloned concrete value
   */
  virtual ConcreteValue *clone (int length = -1) const = 0;

  static ConcreteValue *loadFromBinaryStream (std::ifstream &in);

  /**
   * Returns maximum number of bits in this concrete value. This value is independent of
   * the currently hold concrete value and is a property of its type.
   * @return maximum number of bits which may be contained in this concrete value.
   */
  virtual int getSize () const = 0;

  /**
   * Casts the underlying concrete value to UINT64. This may cause value truncation (for
   * values with more than 64 bits, e.g. 128 bits). If underlying value has a length less
   * than 64 bits, it will be unsigned-casted (MSB will be zero).
   * @return Result of unsigned casting of the underlying concrete value to UINT64.
   */
  virtual UINT64 toUint64 () const = 0;

  bool operator< (const ConcreteValue &cv) const {
    return !((*this) >= cv);
  }

  bool lessThan (const ConcreteValue &cv) const {
    return !(this->greaterThanOrEqualTo (cv));
  }

  bool operator<= (const ConcreteValue &cv) const {
    return !((*this) > cv);
  }

  bool lessThanOrEqualTo (const ConcreteValue &cv) const {
    return !(this->greaterThan (cv));
  }

  bool operator>= (const ConcreteValue &cv) const {
    return ((*this) > cv) || ((*this) == cv);
  }

  bool greaterThanOrEqualTo (const ConcreteValue &cv) const {
    return this->greaterThan (cv) || ((*this) == cv);
  }

  // unsigned comparison
  virtual bool operator> (const ConcreteValue &cv) const = 0;
  // signed comparison
  virtual bool greaterThan (const ConcreteValue &cv) const = 0;
  virtual bool operator== (const ConcreteValue &cv) const = 0;

  bool operator!= (const ConcreteValue &cv) const {
    return !((*this) == cv);
  }

  bool operator< (UINT64 value) const;
  bool operator<= (UINT64 value) const;
  bool operator>= (UINT64 value) const;
  bool operator> (UINT64 value) const;
  bool operator== (UINT64 value) const;

  bool lessThan (UINT64 value) const;
  bool lessThanOrEqualTo (UINT64 value) const;
  bool greaterThanOrEqualTo (UINT64 value) const;
  bool greaterThan (UINT64 value) const;

  virtual ConcreteValue &operator-= (const ConcreteValue &cv) = 0;
  virtual ConcreteValue &operator+= (const ConcreteValue &cv) = 0;

  virtual ConcreteValue &operator&= (const ConcreteValue &mask) = 0;
  virtual ConcreteValue &operator|= (const ConcreteValue &mask) = 0;

  virtual ConcreteValue &operator*= (const ConcreteValue &mul) = 0;
  virtual ConcreteValue &operator/= (const ConcreteValue &value) = 0;
  virtual ConcreteValue &operator%= (const ConcreteValue &divisor) = 0;
  virtual ConcreteValue &operator^= (const ConcreteValue &pattern) = 0;

  virtual ConcreteValue &operator>>= (const ConcreteValue &bits) = 0;
  virtual ConcreteValue &operator<<= (const ConcreteValue &bits) = 0;

  virtual ConcreteValue &rotateToRight (const ConcreteValue &bits) = 0;

  ConcreteValue &operator-= (UINT64 cv);
  ConcreteValue &operator+= (UINT64 cv);
  ConcreteValue &operator&= (UINT64 mask);
  ConcreteValue &operator|= (UINT64 mask);
  ConcreteValue &operator*= (UINT64 mul);
  ConcreteValue &operator/= (UINT64 divisor);
  ConcreteValue &operator%= (UINT64 divisor);
  ConcreteValue &operator^= (UINT64 pattern);
  ConcreteValue &operator>>= (UINT64 bits);
  ConcreteValue &operator<<= (UINT64 bits);
  ConcreteValue &rotateToRight (UINT64 bits);
};

}
}
}
}

#endif	/* ConcreteValue.h */
