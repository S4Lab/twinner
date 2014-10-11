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

#ifndef OPERATOR_H
#define OPERATOR_H

#include "ExpressionToken.h"

#include "pin.H"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class ConcreteValue;

class Operator : public ExpressionToken {

public:

  enum OperatorIdentifier {

    SIGN_EXTEND,
    ADD, MINUS,
    MULTIPLY, DIVIDE,
    REMAINDER,
    NEGATE,
    XOR,
    BITWISE_AND, BITWISE_OR,
    SHIFT_LEFT, SHIFT_RIGHT,
    ARITHMETIC_SHIFT_RIGHT,
    ROTATE_RIGHT, ROTATE_LEFT,
  };

  enum OperatorType {

    Unary,
    Binary,
    FunctionalBinary
  };

private:
  OperatorIdentifier oi;

  Operator (const Operator &op);

public:
  Operator (OperatorIdentifier);

  virtual Operator *clone () const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static Operator *loadFromBinaryStream (std::ifstream &in);

  void apply (ConcreteValue &dst, const ConcreteValue &src) const;

  virtual std::string toString () const;

  OperatorType getType () const;
  OperatorIdentifier getIdentifier () const;

  virtual bool operator== (const ExpressionToken &token) const;
};

}
}
}
}

#endif /* Operator.h */
