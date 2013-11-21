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
#include <fstream>
#include <sstream>

#include "edu/sharif/twinner/util/Logger.h"

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

std::string Expression::toString () const {
  std::list < ExpressionToken * > st = std::list < ExpressionToken * > (stack);
  std::stringstream ss;
  convertToInfixExpression (st, ss);
  ss << "/*0x" << std::hex << lastConcreteValue << "*/";
  return ss.str ();
}

void Expression::convertToInfixExpression (std::list < ExpressionToken * > &st,
    std::stringstream &ss) const {
  ss << "not-implemented";
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
  lastConcreteValue = op->apply (lastConcreteValue, exp->lastConcreteValue);
}

void Expression::truncate (int bits) {
  UINT64 mask;
  if (bits == 64) {
    mask = (UINT64) (-1);
  } else {
    mask = 1;
    mask <<= bits;
    mask--;
  }
  stack.push_back (new Constant (mask));
  stack.push_back (new Operator (Operator::BITWISE_AND));
  lastConcreteValue &= mask;
}

void Expression::shiftToRight (int bits) {
  UINT64 val = 1;
  val <<= bits; // shift-to-right by n bits is equivalent to division by 2^n
  stack.push_back (new Constant (val));
  stack.push_back (new Operator (Operator::DIVIDE));
  lastConcreteValue >>= bits;
}

void Expression::shiftToLeft (int bits) {
  UINT64 val = 1;
  val <<= bits; // shift-to-left by n bits is equivalent to multiplication by 2^n
  stack.push_back (new Constant (val));
  stack.push_back (new Operator (Operator::MULTIPLY));
  lastConcreteValue <<= bits;
}

void Expression::shiftToLeft (const Expression *bits) {
  throw std::runtime_error
      ("Expression::shiftToLeft: symbolic-shift: Not yet implemented!");
}

void Expression::minus (UINT64 immediate) {
  stack.push_back (new Constant (immediate));
  stack.push_back (new Operator (Operator::MINUS));
  lastConcreteValue -= immediate;
}

void Expression::add (UINT64 immediate) {
  stack.push_back (new Constant (immediate));
  stack.push_back (new Operator (Operator::ADD));
  lastConcreteValue += immediate;
}

void Expression::makeLeastSignificantBitsZero (int bits) {
  UINT64 mask = 1;
  mask <<= bits;
  mask = ~(mask - 1);
  stack.push_back (new Constant (mask));
  stack.push_back (new Operator (Operator::BITWISE_AND));
  lastConcreteValue &= mask;
}

Expression *Expression::clone () const {
  return new Expression (*this);
}

void Expression::saveToBinaryStream (std::ofstream &out) const {
  saveListToBinaryStream (out, "EXP", stack);
  out.write ((const char *) &lastConcreteValue, sizeof (lastConcreteValue));
}

}
}
}
}
