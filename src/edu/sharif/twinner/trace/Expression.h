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

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "Savable.h"

#include "ExecutionState.h"

#include "edu/sharif/twinner/trace/exptoken/Operator.h"
#include "edu/sharif/twinner/trace/exptoken/ExpressionVisitor.h"

#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace cv {

class ConcreteValue;
class ConcreteValue128Bits;
}

class Expression : public Savable {
public:
  /**
   * Uses push_back and pop_back instead of push and pop to implement a stack.
   */
  typedef std::list < edu::sharif::twinner::trace::exptoken::ExpressionToken * > Stack;
  typedef edu::sharif::twinner::trace::exptoken::Operator Operator;

protected:
  Stack stack;

private:
  edu::sharif::twinner::trace::cv::ConcreteValue *lastConcreteValue;

  bool isOverwriting;

  /**
   * Instantiates an expression with given internal stack of symbols and given last
   * concrete value. This object takes ownership of both.
   * @param stk Internal stack of symbols of the new expression.
   * @param concreteValue The last concrete value of the new expression.
   */
  Expression (const Stack &stk,
      edu::sharif::twinner::trace::cv::ConcreteValue *concreteValue);

  /**
   * Instantiates an expression based on the given exp.
   * This constructor is the same as copy-constructor with one difference: here, the
   * concrete value is casted to size-bits precision. For size == -1, no cast will happen.
   *
   * @param size The bit-precision of the concrete value of the instantiated expression.
   * @param exp The expression which is going to be cloned to create this new expression.
   */
  Expression (int size, const Expression &exp);

protected:
  /**
   * Instantiates an expression with given concrete value and leaves its internal stack
   * of symbols uninitialized to be filled by a child implementation. It also takes
   * ownership of the concrete value object.
   * @param lastConcreteValue The last concrete value of the new expression.
   * @param isOverwriting If true, on first read, it writes concrete value to real memory.
   */
  Expression (edu::sharif::twinner::trace::cv::ConcreteValue *lastConcreteValue,
      bool isOverwriting);

public:
  Expression (const Expression &exp);
  Expression &operator= (const Expression &exp);

  /**
   * Instantiates an expression by filling its stack from [begin, end) range.
   * The concrete value is not important in this case.
   * And the size of the concrete value (precision of the expression) is
   * indicated by the bitsize parameter.
   *
   * @param begin The inclusive begin iterator for filling the stack.
   * @param end The exclusive end iterator for filling the stack.
   * @param bitsize The size of the concrete value in bits.
   */
  Expression (Stack::iterator begin, Stack::iterator end, int bitsize);

  /**
   * Deletes all hold expression tokens and destructs the expression instance.
   */
  ~Expression ();

  edu::sharif::twinner::trace::cv::ConcreteValue &getLastConcreteValue ();
  const edu::sharif::twinner::trace::cv::ConcreteValue &getLastConcreteValue () const;

  /**
   * Set last concrete value to given object and take ownership of it.
   *
   * @param value The new concrete value which is assumed to be owned by this object.
   */
  void setLastConcreteValue (edu::sharif::twinner::trace::cv::ConcreteValue *value);

  void setOverwriting (bool overwriting);
  bool isOverwritingExpression () const;

  std::string toString () const;
  std::string toDetailedString () const;

  template <typename Result>
  Result visit (edu::sharif::twinner::trace::exptoken
      ::ExpressionVisitor<Result> &visitor) const {
    Stack::const_iterator it = getStack ().end ();
    return visit (visitor, --it);
  }

private:

  template <typename Result>
  Result visit (edu::sharif::twinner::trace::exptoken
      ::ExpressionVisitor<Result> &visitor, Stack::const_iterator &it) const {
    const edu::sharif::twinner::trace::exptoken::ExpressionToken *token = *it--;
    const Operator *op = dynamic_cast<const Operator *> (token);
    if (op) {
      switch (op->getType ()) {
        case Operator::Trinary:
        {
          Result right = visit (visitor, it);
          Result middle = visit (visitor, it);
          Result left = visit (visitor, it);
          return visitor.visitTrinary (op, left, middle, right);
        }
        case Operator::Unary:
        {
          Result main = visit (visitor, it);
          return visitor.visitUnary (op, main);
        }
        case Operator::FunctionalBinary:
        {
          Result right = visit (visitor, it);
          Result left = visit (visitor, it);
          return visitor.visitFunctionalBinary (op, left, right);
        }
        case Operator::Binary:
        {
          Result right = visit (visitor, it);
          Result left = visit (visitor, it);
          return visitor.visitBinary (op, left, right);
        }
        default:
          edu::sharif::twinner::util::Logger::error ()
              << "Expression::visit (): Unknown operator type\n";
          abort ();
      }
    } else {
      const edu::sharif::twinner::trace::exptoken::Operand *operand =
          static_cast<const edu::sharif::twinner::trace::exptoken::Operand *> (token);
      return visitor.visitOperand (operand);
    }
  }

protected:
  void unaryOperation (Operator *op, const Expression *exp);

public:
  /**
   * The op operator is assumed to be owned by this expression object.
   * But exp will be kept untouched and its inner expression tokens will be cloned.
   * The exp may or may not be the same as this object.
   * If exp is a trivial expression, this method tries to simplify and apply it directly.
   *
   * @param op The owned operator which mediates between this and exp expression objects.
   * @param exp The given constant expression which its clone should be applied by op.
   */
  void binaryOperation (Operator *op, const Expression *exp);

protected:
  /**
   * Same as `binaryOperation (Operator *op, const Expression *exp)` but tries to
   * simplify the concrete operand and apply it directly. If it isn't possible, it
   * applies the operation without simplification.
   * The cv is assumed to be owned by this expression object.
   *
   * @param op The owned operator which mediates between this and cv objects.
   * @param cv The owned concrete value which should be applied by op.
   */
  void binaryOperation (Operator *op, edu::sharif::twinner::trace::cv::ConcreteValue *cv);

  /**
   * Same as `binaryOperation (Operator *op, ConcreteValue *cv)` method.
   * It converts given cv to ConcreteValue and calls the main binaryOperation method.
   *
   * @param op The owned operator which mediates between this and cv.
   * @param cv The value that should be converted to ConcreteValue to be used.
   */
  void binaryOperation (Operator *op, UINT64 cv);

  /**
   * The op operator is assumed to be owned by this expression object.
   * But exp will be kept untouched and its inner expression tokens will
   * be cloned. The exp may or may not be the same as this object.
   * If exp is a trivial expression, this method tries to simplify and apply
   * it directly.
   * This method is similar to binaryOperation but it uses the size of this
   * expression as an implicit operand.
   *
   * @param op The owned operator which mediates between this and the exp
   * expression objects. The size of this is assumed as an implicit operand.
   * @param exp The given constant expression which its clone should be applied by op.
   */
  void trinaryOperation (Operator *op, const Expression *exp);

  /**
   * Same as `trinaryOperation (Operator *op, const Expression *exp)` but tries
   * to simplify the concrete operand and apply it directly.
   * If it isn't possible, it applies the operation without simplification.
   * The cv is assumed to be owned by this expression object.
   * This method is similar to binaryOperation but it uses the size of this
   * expression as an implicit operand.
   *
   * @param op The owned operator which mediates between this and cv objects.
   * The size of this is assumed as an implicit operand.
   * @param cv The owned concrete value which should be applied by op.
   */
  void trinaryOperation (Operator *op, edu::sharif::twinner::trace::cv::ConcreteValue *cv);

  /**
   * Same as `trinaryOperation (Operator *op, ConcreteValue *cv)` method.
   * It converts given cv to ConcreteValue and calls the main
   * trinaryOperation method.
   * This method is similar to binaryOperation but it uses the size of this
   * expression as an implicit operand.
   *
   * @param op The owned operator which mediates between this and cv.
   * @param cv The value that should be converted to ConcreteValue to be used.
   */
  void trinaryOperation (Operator *op, UINT64 cv);

private:
  bool checkForTrivialExpression (Operator *op, const Expression *exp);
  bool checkForCancelingOperation (Operator *op, const Expression *exp);
  bool checkForNonTrivialAddition (Operator *op, const Expression *exp);
  UINT64 checkForNonTrivialAddition (Stack::const_iterator me, Stack::const_iterator that,
      Stack::const_iterator myEnd, Stack::const_iterator thatEnd);

public:
  /**
   * Truncates the expression to occupy at most a given count of bits.
   * @param bits Count of bits that this expression should be truncated to be fitted in.
   */
  void truncate (int bits);

  /**
   * Shifts the expression to right as much as the given symbolic/concrete
   * expression/value.
   *
   * @param operand Count of bits (symbolically or concretely) that this expression should
   * be shifted upon to right.
   * @type ValueOrExp type can be UINT64, ConcreteValue *, or const Expression *
   */
  template <typename ValueOrExp>
  void shiftToRight (ValueOrExp operand) {
    binaryOperation (Operator::instantiateOperator (Operator::SHIFT_RIGHT), operand);
  }

  /**
   * Arithmetic shifts the expression to right as much as the given symbolic/concrete
   * count of bits.
   *
   * @param operand Count of bits (symbolically or concretely) that this expression should
   * be arithmetically shifted upon to right.
   * @type ValueOrExp type can be UINT64, ConcreteValue *, or const Expression *
   */
  template <typename ValueOrExp>
  void arithmeticShiftToRight (ValueOrExp operand) {
    trinaryOperation
        (Operator::instantiateOperator (Operator::ARITHMETIC_SHIFT_RIGHT),
        operand);
  }

  /**
   * Shifts the expression to left as much as the given symbolic/concrete count of bits.
   *
   * @param operand Count of bits (symbolically or concretely) that this expression should
   * be shifted upon to left.
   * @type ValueOrExp type can be UINT64, ConcreteValue *, or const Expression *
   */
  template <typename ValueOrExp>
  void shiftToLeft (ValueOrExp operand) {
    binaryOperation (Operator::instantiateOperator (Operator::SHIFT_LEFT), operand);
  }

  /**
   * Rotates the expression to right as much as the given symbolic/concrete count of bits.
   *
   * @param operand Count of bits (symbolically or concretely) that this expression should
   * be rotated upon towards right.
   * @type ValueOrExp type can be UINT64, ConcreteValue *, or const Expression *
   */
  template <typename ValueOrExp>
  void rotateToRight (ValueOrExp operand) {
    trinaryOperation
        (Operator::instantiateOperator (Operator::ROTATE_RIGHT), operand);
  }

  /**
   * Rotates the expression to left as much as the given symbolic/concrete count of bits.
   *
   * @param operand Count of bits (symbolically or concretely) that this expression should
   * be rotated upon towards left.
   * @type ValueOrExp type can be UINT64, ConcreteValue *, or const Expression *
   */
  template <typename ValueOrExp>
  void rotateToLeft (ValueOrExp operand) {
    trinaryOperation
        (Operator::instantiateOperator (Operator::ROTATE_LEFT), operand);
  }

  /**
   * Decrement this expression as much as the given symbolic/concrete value.
   *
   * @param operand The value (symbolically or concretely) which its value will be
   * decremented from this expression.
   * @type ValueOrExp type can be UINT64, ConcreteValue *, or const Expression *
   */
  template <typename ValueOrExp>
  void minus (ValueOrExp operand) {
    binaryOperation (Operator::instantiateOperator (Operator::MINUS), operand);
  }

  /**
   * Instantiates an expression indicating the two's complement of this expression.
   * Caller should delete the returned expression.
   *
   * @param precision The maximum size (in bits) of resulting expression or -1 (for no change).
   * @return The two's complement of this expression.
   */
  Expression *twosComplement (int precision = -1) const;

  /**
   * Increment this expression as much as the given symbolic/concrete value.
   *
   * @param operand The value (symbolically or concretely) which its value will be
   * added to this expression.
   * @type ValueOrExp type can be UINT64, ConcreteValue *, or const Expression *
   */
  template <typename ValueOrExp>
  void add (ValueOrExp operand) {
    binaryOperation (Operator::instantiateOperator (Operator::ADD), operand);
  }

  /**
   * Multiply this expression with the given symbolic/concrete value.
   *
   * @param operand The value (symbolically or concretely) which will be
   * multiplied by this expression.
   * @type ValueOrExp type can be UINT64, ConcreteValue *, or const Expression *
   */
  template <typename ValueOrExp>
  void multiply (ValueOrExp operand) {
    binaryOperation (Operator::instantiateOperator (Operator::MULTIPLY), operand);
  }

  /**
   * Divides this expression by the given symbolic/concrete value.
   *
   * @param operand The value (symbolically or concretely) that this expression will be
   * divided by it.
   * @type ValueOrExp type can be UINT64, ConcreteValue *, or const Expression *
   */
  template <typename ValueOrExp>
  void divide (ValueOrExp operand) {
    binaryOperation (Operator::instantiateOperator (Operator::DIVIDE), operand);
  }

  /**
   * Signed divides this expression by the given symbolic/concrete value.
   *
   * @param operand The value (symbolically or concretely) that this expression will be
   * signed divided by it.
   * @type ValueOrExp type can be UINT64, ConcreteValue *, or const Expression *
   */
  template <typename ValueOrExp>
  void signedDivide (ValueOrExp operand) {
    binaryOperation (Operator::instantiateOperator (Operator::SIGNED_DIVIDE), operand);
  }

  /**
   * Calculates the remainder of division of this expression by the given
   * symbolic/concrete value.
   *
   * @param operand The value (symbolically or concretely) that this expression will be
   * divided by it and its resulting remainder will be kept by this expression.
   * @type ValueOrExp type can be UINT64, ConcreteValue *, or const Expression *
   */
  template <typename ValueOrExp>
  void remainder (ValueOrExp operand) {
    binaryOperation (Operator::instantiateOperator (Operator::REMAINDER), operand);
  }

  /**
   * Calculates the remainder of signed division of this expression by the given
   * symbolic/concrete value.
   *
   * @param operand The value (symbolically or concretely) that this expression will be
   * signed divided by it and its resulting remainder will be kept by this expression.
   * @type ValueOrExp type can be UINT64, ConcreteValue *, or const Expression *
   */
  template <typename ValueOrExp>
  void signedRemainder (ValueOrExp operand) {
    binaryOperation (Operator::instantiateOperator (Operator::SIGNED_REMAINDER), operand);
  }

  /**
   * Bitwise AND of this expression and given symbolic/concrete mask value.
   *
   * @param operand The value (symbolically or concretely) that this expression will be
   * bitwise-and-ed by it.
   * @type ValueOrExp type can be UINT64, ConcreteValue *, or const Expression *
   */
  template <typename ValueOrExp>
  void bitwiseAnd (ValueOrExp operand) {
    binaryOperation (Operator::instantiateOperator (Operator::BITWISE_AND), operand);
  }

  /**
   * Bitwise OR of this expression and given symbolic/concrete mask value.
   *
   * @param operand The value (symbolically or concretely) that this expression will be
   * bitwise-or-ed by it.
   * @type ValueOrExp type can be UINT64, ConcreteValue *, or const Expression *
   */
  template <typename ValueOrExp>
  void bitwiseOr (ValueOrExp operand) {
    binaryOperation (Operator::instantiateOperator (Operator::BITWISE_OR), operand);
  }

  /**
   * Bitwise XOR of this expression and given symbolic/concrete mask value.
   *
   * @param operand The value (symbolically or concretely) that this expression will be
   * xor-ed by it.
   * @type ValueOrExp type can be UINT64, ConcreteValue *, or const Expression *
   */
  template <typename ValueOrExp>
  void bitwiseXor (ValueOrExp operand) {
    binaryOperation (Operator::instantiateOperator (Operator::XOR), operand);
  }

  /**
   * Sets the given count of bits, from least significant bits, to zero.
   * @param bits Count of least significant bits which should be set to zero.
   */
  void makeLeastSignificantBitsZero (int bits);

  /**
   * Negates this expression. If this expression has a negation operator on top of its
   * stack, it will be removed and redundant negation operators won't be pushed onto
   * the stack.
   */
  void bitwiseNegate ();

  /**
   * Similar to the clone() method, but it needs to clone to a larger "size" and also
   * sign extends the expression (while the clone method zero extends it).
   *
   * @param size The maximum size (in bits) that resulting sign-extended exp can hold.
   * @return The sign-extend(this) which can hold size-bits values.
   */
  virtual Expression *signExtended (int size) const;
  virtual Expression *clone (int size = -1) const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static Expression *loadFromBinaryStream (std::ifstream &in);

  Stack &getStack ();
  const Stack &getStack () const;

  void checkConcreteValueReg (REG reg,
      const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal,
      StateSummary &state) const;
  void checkConcreteValueMemory (ADDRINT memoryEa,
      const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal,
      StateSummary &state);

  bool operator== (const Expression &exp) const;

  bool isTrivial (bool requiresValidConcreteValue) const;

private:
  bool makeConcreteValueValid () const;
  std::pair<Stack::const_iterator, Stack::const_iterator> compareTo (
      const Expression &exp) const;
};

}
}
}
}

#endif /* Expression.h */
