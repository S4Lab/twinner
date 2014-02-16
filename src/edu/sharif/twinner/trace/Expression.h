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

#include "pin.H"
#include "MemoryEmergedSymbol.h"
#include "ExecutionState.h"

#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

class Logger;
}
namespace trace {

class Expression : public Savable {

protected:
  /**
   * Uses push_back and pop_back instead of push and pop to implement a stack.
   */
  std::list < ExpressionToken * > stack;

private:
  UINT64 lastConcreteValue;

  bool isOverwriting;

  Expression (const std::list < ExpressionToken * > &stk, UINT64 concreteValue);

protected:
  Expression (const Expression &exp);

  Expression (UINT64 lastConcreteValue, bool isOverwriting);

public:
  /**
   * Deletes all hold expression tokens and destruct the expression instance.
   */
  ~Expression ();

  UINT64 getLastConcreteValue () const;
  void setLastConcreteValue (UINT64 value);

  std::string toString () const;
  void convertToInfixExpression (std::list < ExpressionToken * > &st,
      std::stringstream &ss) const;

  void unaryOperation (Operator op, Expression exp);

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
  void shiftToRight (int bits);

  /**
   * Shifts the expression to right as much as the given symbolic value/expression.
   * @param bits Count of bits (symbolically) that this expression should be
   * shifted upon to right.
   */
  void shiftToRight (const Expression *bits);

  /**
   * Shifts the expression to left as much as the given count of bits.
   * @param bits Count of bits that this expression should be shifted upon to left.
   */
  void shiftToLeft (int bits);

  /**
   * Shifts the expression to left as much as the given symbolic value/expression.
   * @param bits Count of bits (symbolically) that this expression should be
   * shifted upon to left.
   */
  void shiftToLeft (const Expression *bits);

  /**
   * Decrement this expression as much as the given immediate value.
   * @param immediate The value which its value will be decremented from this expression.
   */
  void minus (UINT64 immediate);

  /**
   * Increment this expression as much as the given immediate value.
   * @param immediate The value which its value will be added to this expression.
   */
  void add (UINT64 immediate);

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

  virtual Expression *clone () const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static Expression *loadFromBinaryStream (std::ifstream &in);

  const std::list < ExpressionToken * > &getStack () const;

  void checkConcreteValueReg (REG reg,
      UINT64 concreteVal) const throw (WrongStateException);
  void checkConcreteValueMemory (ADDRINT memoryEa,
      UINT64 concreteVal) throw (WrongStateException);
};

}
}
}
}

#endif /* Expression.h */
