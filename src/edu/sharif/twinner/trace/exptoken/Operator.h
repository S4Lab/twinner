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
    BITWISE_NEGATE,

    MAX_UNARY_OPERATOR_IDENTIFIER,

    ADD, MINUS,
    MULTIPLY, DIVIDE,
    REMAINDER,
    XOR,
    BITWISE_AND, BITWISE_OR,
    SHIFT_LEFT,

    MAX_BINARY_OPERATOR_IDENTIFIER,

    SIGNED_DIVIDE, SIGNED_REMAINDER,
    SHIFT_RIGHT,

    MAX_FUNCTIONAL_BINARY_OPERATOR_IDENTIFIER,

    SIGN_EXTEND,
    ARITHMETIC_SHIFT_RIGHT,
    ROTATE_RIGHT, ROTATE_LEFT,
  };

  enum OperatorType {
    Unary,
    Binary,
    FunctionalBinary,
    Trinary,
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

  virtual bool apply (edu::sharif::twinner::trace::Expression *exp,
      edu::sharif::twinner::trace::cv::ConcreteValue *middle,
      edu::sharif::twinner::trace::cv::ConcreteValue *right);

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

  virtual bool apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
      const edu::sharif::twinner::trace::cv::ConcreteValue &mid,
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

  /**
   * Tries to simplify the {exp} {this operator} {operand} expression.
   * It may update exp, has ownership of the operand, and return one of
   * three possible outcomes:
   * 1. can not simplify; in this case the {exp} may be simplified/modified,
   *    but {operand} is not used,
   * 2. need restart; in this case operand is not used too, but exp is
   *    simplified in a way that restarting the {apply} operation can gain
   *    more simplifications,
   * 3. complete simplification; in this case operand is used (caller should
   *    not touch it) and {this operator} is not used (caller should delete it).
   *
   * ASSUMPTION: The exp stack contains more than two elements and consequently,
   *             an operator is placed on top of its stack (with at least
   *             two operands below it).
   *
   * @param exp The expression which this operator is applied upon it
   * @param operand The right-hand operand
   * @return The final status of the performed simplification
   */
  virtual SimplificationStatus deepSimplify (edu::sharif::twinner::trace::Expression *exp,
      edu::sharif::twinner::trace::cv::ConcreteValue *operand);

  /**
   * Finds an iterator pointing to the first exp token of next operand.
   *
   * @param it An iterator pointing to the right exclusive search boundry.
   * @return An iterator pointing to the left/first token of next operand.
   */
  std::list < ExpressionToken * >::iterator findNextOperand (
      std::list < ExpressionToken * >::iterator it) const;
};

}
}
}
}
}

#endif /* Operator.h */
