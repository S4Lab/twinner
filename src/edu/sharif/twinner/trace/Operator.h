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

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class ConcreteValue;
class Expression;

class Operator : public ExpressionToken {

public:

  enum OperatorIdentifier {

    SIGN_EXTEND,
    NEGATE,

    MAX_UNARY_OPERATOR_IDENTIFIER,

    ADD, MINUS,
    MULTIPLY, DIVIDE,
    REMAINDER,
    XOR,
    BITWISE_AND, BITWISE_OR,
    SHIFT_RIGHT,

    MAX_BINARY_OPERATOR_IDENTIFIER,

    SHIFT_LEFT,
    ARITHMETIC_SHIFT_RIGHT,
    ROTATE_RIGHT, ROTATE_LEFT,
  };

  enum OperatorType {

    SignExtension,
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

  /**
   * Indicates whether this operator supports expression simplification and direct
   * application for constant operands.
   *
   * @return true iff the `apply (Expression *, ConcreteValue *)` method has a chance
   * to simplify the expression.
   */
  bool doesSupportSimplification () const;

  /**
   * Applies this operator on given expression and concrete value objects.
   * This modifies internal state of exp to make it equal with `exp <this> cv`.
   * Also, if it's possible to simplify the operation and avoid applying it symbolically,
   * this method applies the operation directly and returns true. Otherwise it returns
   * false to inform caller that this operator is saved in internal state of exp and
   * is owned by the exp since then.
   *
   * @param exp The expression which its state will be changed by applying this operator.
   * @param cv The owned concrete value that is used as operand of this operation.
   * @return true iff the operator is not used and operation is applied directly.
   */
  bool apply (Expression *exp, edu::sharif::twinner::trace::ConcreteValue *cv);
  void apply (edu::sharif::twinner::trace::ConcreteValue &dst,
      const edu::sharif::twinner::trace::ConcreteValue &src) const;

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
