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

void Expression::binaryOperation (Operator op, Expression exp) {
  throw std::runtime_error ("Expression::binaryOperation: Not yet implemented");
}

Expression *Expression::clone () const {
  return new Expression (*this);
}

}
}
}
}
