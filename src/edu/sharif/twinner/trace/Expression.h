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

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "Savable.h"

#include "Operator.h"
#include "ExecutionState.h"

#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

class Logger;
}
namespace trace {

class ConcreteValue;

class Expression : public Savable {

protected:
  /**
   * Uses push_back and pop_back instead of push and pop to implement a stack.
   */
  std::list < ExpressionToken * > stack;

private:
  ConcreteValue *lastConcreteValue;

  bool isOverwriting;

  /**
   * Instantiates an expression with given internal stack of symbols and given last
   * concrete value. This object takes ownership of both.
   * @param stk Internal stack of symbols of the new expression.
   * @param concreteValue The last concrete value of the new expression.
   */
  Expression (const std::list < ExpressionToken * > &stk, ConcreteValue *concreteValue);

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
  Expression (ConcreteValue *lastConcreteValue, bool isOverwriting);

public:
  Expression (const Expression &exp);

  /**
   * Deletes all hold expression tokens and destruct the expression instance.
   */
  ~Expression ();

  const ConcreteValue &getLastConcreteValue () const;
  /**
   * Set last concrete value to given object and take ownership of it.
   * @param value The new concrete value which is assumed to be owned by this object.
   */
  void setLastConcreteValue (ConcreteValue *value);

  void setOverwriting (bool overwriting);

  std::string toString () const;
  void convertToInfixExpression (std::list < ExpressionToken * > &st,
      std::stringstream &ss) const;

  void unaryOperation (Operator op, const Expression *exp);

  /**
   * The op operator is assumed to be owned by this expression object.
   * But exp will be kept untouched and its inner expression tokens will be cloned.
   * The exp may or may not be the same as this object.
   * @param op The owned operator which mediates between this and exp expression objects.
   * @param exp The given constant expression which its clone should be applied by op.
   */
  void binaryOperation (Operator *op, const Expression *exp);

  /**
   * Truncates the expression to occupy at most a given count of bits.
   * @param bits Count of bits that this expression should be truncated to be fitted in.
   */
  void truncate (int bits);

  /**
   * Shifts the expression to right as much as the given count of bits.
   * @param bits Count of bits that this expression should be shifted upon to right.
   */
  void shiftToRight (ConcreteValue *bits);
  void shiftToRight (int bits);

  /**
   * Shifts the expression to right as much as the given symbolic value/expression.
   * @param bits Count of bits (symbolically) that this expression should be
   * shifted upon to right.
   */
  void shiftToRight (const Expression *bits);

  /**
   * Arithmetic shifts the expression to right as much as the given symbolic expression.
   * @param bits Count of bits (symbolically) that this expression should be
   * arithmetically shifted to right.
   */
  void arithmeticShiftToRight (const Expression *bits);

  /**
   * Shifts the expression to left as much as the given count of bits.
   * @param bits Count of bits that this expression should be shifted upon to left.
   */
  void shiftToLeft (ConcreteValue *bits);
  void shiftToLeft (int bits);

  /**
   * Shifts the expression to left as much as the given symbolic value/expression.
   * @param bits Count of bits (symbolically) that this expression should be
   * shifted upon to left.
   */
  void shiftToLeft (const Expression *bits);

  /**
   * Rotates the expression to right as much as the given count of bits.
   * @param bits Count of bits that this expression should be rotated upon towards right.
   */
  void rotateToRight (ConcreteValue *bits);
  void rotateToRight (int bits);

  /**
   * Rotates the expression to right as much as the given symbolic value/expression.
   * @param bits Count of bits (symbolically) that this expression should be
   * rotated upon towards right.
   */
  void rotateToRight (const Expression *bits);

  /**
   * Decrement this expression as much as the given immediate value.
   * @param immediate The value which its value will be decremented from this expression.
   */
  void minus (ConcreteValue *immediate);
  void minus (UINT64 immediate);

  /**
   * Decrement this expression as much as the given exp expression.
   * @param exp The expression indicating how much this expression should be decremented.
   */
  void minus (const Expression *exp);

  /**
   * Instantiates an expression indicating the two's complement of this expression.
   * Caller should delete the returned expression.
   * @return The two's complement of this expression.
   */
  Expression *twosComplement () const;

  /**
   * Increment this expression as much as the given immediate value.
   * @param immediate The value which its value will be added to this expression.
   */
  void add (ConcreteValue *immediate);
  void add (UINT64 immediate);

  /**
   * Increment this expression as much as the given exp expression.
   * @param exp The expression indicating how much this expression should be incremented.
   */
  void add (const Expression *exp);

  /**
   * Multiply this expression with the given immediate value.
   * @param immediate The value which will be multiplied by this expression.
   */
  void multiply (ConcreteValue *immediate);
  void multiply (UINT64 immediate);

  /**
   * Multiply this expression with the given exp expression.
   * @param exp The expression which will be multiplied by this expression.
   */
  void multiply (const Expression *exp);

  /**
   * Bitwise AND of this expression and given mask value.
   * @param mask The mask value which should be bitwise-and-ed by this expression.
   */
  void bitwiseAnd (ConcreteValue *mask);
  void bitwiseAnd (UINT64 mask);

  /**
   * Bitwise AND of this expression and the given symbolic value/expression.
   * @param mask The mask (symbolically) that this expression should be AND-ed with it.
   */
  void bitwiseAnd (const Expression *mask);

  /**
   * Bitwise OR of this expression and given mask value.
   * @param mask The mask value which should be bitwise-or-ed by this expression.
   */
  void bitwiseOr (ConcreteValue *mask);
  void bitwiseOr (UINT64 mask);

  /**
   * Bitwise OR of this expression and the given symbolic value/expression.
   * @param mask The mask (symbolically) that this expression should be OR-ed with it.
   */
  void bitwiseOr (const Expression *mask);

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
  void negate ();

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

  const std::list < ExpressionToken * > &getStack () const;

  void checkConcreteValueReg (REG reg,
      const ConcreteValue &concreteVal) const /* @throw (WrongStateException) */;
  void checkConcreteValueMemory (ADDRINT memoryEa,
      const ConcreteValue &concreteVal) /* @throw (WrongStateException) */;

  bool operator== (const Expression &exp) const;

  bool isTrivial () const;
};

}
}
}
}

#endif /* Expression.h */
