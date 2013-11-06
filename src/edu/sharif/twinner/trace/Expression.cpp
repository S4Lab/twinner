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

#include "Expression.h"

#include <stdexcept>

#include "RegisterEmergedSymbol.h"
#include "MemoryEmergedSymbol.h"
#include "Constant.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

Expression::Expression (REG reg, UINT64 concreteValue, int generationIndex) :
lastConcreteValue (concreteValue) {
  stack.push_back (new RegisterEmergedSymbol (reg, concreteValue, generationIndex));
}

Expression::Expression (ADDRINT memoryEa, UINT64 concreteValue, int generationIndex) :
lastConcreteValue (concreteValue) {
  stack.push_back (new MemoryEmergedSymbol (memoryEa, concreteValue, generationIndex));
}

Expression::Expression (const Expression &exp) :
lastConcreteValue (exp.lastConcreteValue) {
  for (std::list < ExpressionToken * >::const_iterator it = exp.stack.begin ();
      it != exp.stack.end (); ++it) {
    const ExpressionToken *et = *it;
    stack.push_back (et->clone ());
  }
}

Expression::Expression (UINT64 value) :
lastConcreteValue (value) {
  stack.push_back (new Constant (value));
}

Expression::~Expression () {
  while (!stack.empty ()) {
    delete stack.back ();
    stack.pop_back ();
  }
}

UINT64 Expression::getLastConcreteValue () const {
  return lastConcreteValue;
}

void Expression::toString () const {
  throw std::runtime_error ("Expression::toString: Not yet implemented");
}

void Expression::unaryOperation (Operator op, Expression exp) {
  throw std::runtime_error ("Expression::unaryOperation: Not yet implemented");
}

void Expression::binaryOperation (Operator *op, const Expression *exp) {
  /**
   * It's possible that this object and given constant expression object be the same.
   * In that case changing this object while searching the given expression can
   * change the constant expression unexpectedly. To avoid such situations, it's important
   * to take required non constant clone of the given constant expression atomically
   * before applying any change to this object.
   */
  Expression *copy = exp->clone ();
  for (std::list < ExpressionToken * >::iterator it = copy->stack.begin ();
      it != copy->stack.end (); ++it) {
    /**
     * The token's ownership is taken by this object. So the copy temporary expression
     * must not be deleted (to avoid deleting tokens which are currently owned by this
     * object).
     */
    ExpressionToken *token = *it;
    stack.push_back (token);
  }
  stack.push_back (op);
}

Expression *Expression::clone () const {
  return new Expression (*this);
}

}
}
}
}
