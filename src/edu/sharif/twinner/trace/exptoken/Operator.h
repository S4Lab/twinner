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

#include <vector>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace cv {

class ConcreteValue;
}
class Expression;

namespace exptoken {

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

protected:

  /**
   * Indicates a simplification rule as a mapping between lastOperator
   * and simplificationOperator. An expression which is similar to
   * Y <lastOperator> Z <thisOperator> operand
   * will be simplified as
   * Y <lastOperator> (Z <simplificationOperator> operand)
   */
  struct SimplificationRule {

    OperatorIdentifier lastOperator;
    OperatorIdentifier simplificationOperator;

    SimplificationRule (OperatorIdentifier last, OperatorIdentifier simplified) :
        lastOperator (last), simplificationOperator (simplified) {
    }
  };

  std::vector<SimplificationRule> simplificationRules;

  OperatorIdentifier oi;

  Operator (OperatorIdentifier);
  Operator (const Operator &op);

public:
  virtual ~Operator ();

  virtual Operator *clone () const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static Operator *loadFromBinaryStream (std::ifstream &in);
  static Operator *instantiateOperator (OperatorIdentifier oi);

  Operator *instantiateNegatedOperator () const;

  /**
   * Indicates whether this operator supports expression simplification and direct
   * application for constant operands.
   *
   * @return true iff the `apply (Expression *, ConcreteValue *)` method has a chance
   * to simplify the expression.
   */
  virtual bool doesSupportSimplification () const;

  /**
   * Returns true if and only if this operator is commutable. That is, `a <op> b` is
   * equal to `b <op> a` expression.
   *
   * @return true iff this operator is commutable.
   */
  virtual bool isCommutable () const;

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
  virtual bool apply (edu::sharif::twinner::trace::Expression *exp,
      edu::sharif::twinner::trace::cv::ConcreteValue *cv);

  /**
   * Applies this operator on given concrete values. The dst will be updated to indicate
   * value of `dst <this> src`.
   *
   * @param dst The destination concrete value which will be updated according to this op.
   * @param src The source concrete value which is used as second operand.
   * @return true if and only if there is an unsigned overflow in the operation.
   */
  virtual bool apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
      const edu::sharif::twinner::trace::cv::ConcreteValue &src) const;

  virtual std::string toString () const;

  OperatorType getType () const;
  OperatorIdentifier getIdentifier () const;

  virtual bool operator== (const ExpressionToken &token) const;

protected:

  enum SimplificationStatus {

    CAN_NOT_SIMPLIFY,
    RESTART_SIMPLIFICATION,
    COMPLETED // operator is not used and can be deleted
  };

  virtual SimplificationStatus deepSimplify (edu::sharif::twinner::trace::Expression *exp,
      edu::sharif::twinner::trace::cv::ConcreteValue *operand);
};

}
}
}
}
}

#endif /* Operator.h */
