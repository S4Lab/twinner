//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "Operator.h"

#include "pin.H"

#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Expression {

private:
  /**
   * Uses push_back and pop_back instead of push and pop to implement a stack.
   */
  std::list < ExpressionToken * > stack;
  UINT64 lastConcreteValue;

  Expression (const Expression &exp);

public:
  /**
   * Instantiates an expression containing a new (yet unused) symbol,
   * initiated from a register.
   */
  Expression (REG reg, UINT64 concreteValue, int generationIndex);

  /**
   * Instantiates an expression containing a new (yet unused) symbol,
   * initiated from a memory address.
   */
  Expression (ADDRINT memoryEa, UINT64 concreteValue, int generationIndex);

  /**
   * Instantiates an expression containing a constant value (non-symbolic).
   */
  Expression (UINT64 value);

  /**
   * Deletes all hold expression tokens and destruct the expression instance.
   */
  ~Expression ();

  UINT64 getLastConcreteValue () const;

  void toString () const;

  void unaryOperation (Operator op, Expression exp);
  /**
   * The op operator is assumed to be owned by this expression object.
   * But exp will be kept untouched and its inner expression tokens will be cloned.
   */
  void binaryOperation (Operator *op, const Expression *exp);

  Expression *clone () const;
};

}
}
}
}

#endif /* Expression.h */
