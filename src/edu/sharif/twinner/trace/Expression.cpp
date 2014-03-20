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

#include "Expression.h"

#include <stdexcept>
#include <fstream>
#include <sstream>

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/memory.h"

#include "Constant.h"
#include "edu/sharif/twinner/trace/ConcreteValue64Bits.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

Expression::Expression (const std::list < ExpressionToken * > &stk,
    ConcreteValue *concreteValue) :
stack (stk), lastConcreteValue (concreteValue), isOverwriting (false) {
}

Expression::Expression (const Expression &exp) :
lastConcreteValue (exp.lastConcreteValue->clone ()), isOverwriting (false) {
  for (std::list < ExpressionToken * >::const_iterator it = exp.stack.begin ();
      it != exp.stack.end (); ++it) {
    const ExpressionToken *et = *it;
    stack.push_back (et->clone ());
  }
}

Expression::Expression (ConcreteValue *concreteValue, bool _isOverwriting) :
lastConcreteValue (concreteValue), isOverwriting (_isOverwriting) {
}

Expression::~Expression () {
  while (!stack.empty ()) {
    delete stack.back ();
    stack.pop_back ();
  }
  delete lastConcreteValue;
}

const ConcreteValue &Expression::getLastConcreteValue () const {
  return *lastConcreteValue;
}

void Expression::setLastConcreteValue (ConcreteValue *value) {
  delete lastConcreteValue;
  lastConcreteValue = value;
}

std::string Expression::toString () const {
  std::list < ExpressionToken * > st = std::list < ExpressionToken * > (stack);
  std::stringstream ss;
  convertToInfixExpression (st, ss);
  ss << " /*" << *lastConcreteValue << "*/";
  return ss.str ();
}

void Expression::convertToInfixExpression (std::list < ExpressionToken * > &st,
    std::stringstream &ss) const {
  const ExpressionToken *token = st.back ();
  st.pop_back ();
  const Operator *op = dynamic_cast<const Operator *> (token);
  if (op) {
    std::stringstream operand;
    switch (op->getType ()) {
    case Operator::Unary:
      convertToInfixExpression (st, operand);
      ss << op->toString () << operand.str ();
      break;
    case Operator::Binary:
      convertToInfixExpression (st, operand); // right operand
      ss << '(';
      convertToInfixExpression (st, ss); // left operand
      ss << ' ' << op->toString () << ' ' << operand.str () << ')';
      break;
    default:
      edu::sharif::twinner::util::Logger::error () << "Unknown operator type\n";
      break;
    }
  } else if (token) {
    ss << token->toString (); // single operand
  } else {
    edu::sharif::twinner::util::Logger::error () << "Missing expression token\n";
  }
}

void Expression::unaryOperation (Operator op, const Expression *exp) {
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
  op->apply (*lastConcreteValue, *(exp->lastConcreteValue));
}

void Expression::truncate (int bits) {
  //TODO: Detect when truncation has no effect (value is already truncated) and ignore it
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
  (*lastConcreteValue) &= mask;
}

void Expression::shiftToRight (ConcreteValue *bits) {
  if ((*bits) >= 64) {
    stack.push_back (new Constant (bits));
    stack.push_back (new Operator (Operator::SHIFT_RIGHT));
    (*lastConcreteValue) >>= *bits;
  } else {
    ConcreteValue64Bits tmp = *bits;
    UINT64 val = 1;
    // shift-to-right by n bits is equivalent to division by 2^n
    val <<= tmp.getValue ();
    stack.push_back (new Constant (val));
    stack.push_back (new Operator (Operator::DIVIDE));
    (*lastConcreteValue) >>= *bits;
    delete bits;
  }
}

void Expression::shiftToRight (int bits) {
  shiftToRight (new ConcreteValue64Bits (bits));
}

void Expression::shiftToRight (const Expression *bits) {
  binaryOperation (new Operator (Operator::SHIFT_RIGHT), bits);
}

void Expression::shiftToLeft (ConcreteValue *bits) {
  if ((*bits) >= 64) {
    stack.push_back (new Constant (bits));
    stack.push_back (new Operator (Operator::SHIFT_LEFT));
    (*lastConcreteValue) <<= *bits;
  } else {
    ConcreteValue64Bits tmp = *bits;
    UINT64 val = 1;
    // shift-to-left by n bits is equivalent to multiplication by 2^n
    val <<= tmp.getValue ();
    stack.push_back (new Constant (val));
    stack.push_back (new Operator (Operator::MULTIPLY));
    (*lastConcreteValue) <<= *bits;
    delete bits;
  }
}

void Expression::shiftToLeft (int bits) {
  shiftToLeft (new ConcreteValue64Bits (bits));
}

void Expression::shiftToLeft (const Expression *bits) {
  binaryOperation (new Operator (Operator::SHIFT_LEFT), bits);
}

void Expression::minus (ConcreteValue *immediate) {
  stack.push_back (new Constant (immediate));
  stack.push_back (new Operator (Operator::MINUS));
  (*lastConcreteValue) -= *immediate;
}

void Expression::minus (UINT64 immediate) {
  minus (new ConcreteValue64Bits (immediate));
}

void Expression::add (ConcreteValue *immediate) {
  stack.push_back (new Constant (immediate));
  stack.push_back (new Operator (Operator::ADD));
  (*lastConcreteValue) += *immediate;
}

void Expression::add (UINT64 immediate) {
  add (new ConcreteValue64Bits (immediate));
}

void Expression::bitwiseAnd (ConcreteValue *mask) {
  stack.push_back (new Constant (mask));
  stack.push_back (new Operator (Operator::BITWISE_AND));
  (*lastConcreteValue) &= *mask;
}

void Expression::bitwiseAnd (UINT64 mask) {
  bitwiseAnd (new ConcreteValue64Bits (mask));
}

void Expression::bitwiseAnd (const Expression *mask) {
  binaryOperation (new Operator (Operator::BITWISE_AND), mask);
}

void Expression::bitwiseOr (ConcreteValue *mask) {
  stack.push_back (new Constant (mask));
  stack.push_back (new Operator (Operator::BITWISE_OR));
  (*lastConcreteValue) |= *mask;
}

void Expression::bitwiseOr (UINT64 mask) {
  bitwiseOr (new ConcreteValue64Bits (mask));
}

void Expression::bitwiseOr (const Expression *mask) {
  binaryOperation (new Operator (Operator::BITWISE_OR), mask);
}

void Expression::makeLeastSignificantBitsZero (int bits) {
  UINT64 mask = 1;
  mask <<= bits;
  mask = ~(mask - 1);
  stack.push_back (new Constant (mask));
  stack.push_back (new Operator (Operator::BITWISE_AND));
  (*lastConcreteValue) &= mask;
}

void Expression::negate () {
  Operator *op = dynamic_cast<Operator *> (stack.back ());
  if (op) {
    if (op->getIdentifier () == Operator::NEGATE) {
      stack.pop_back ();
      return;
    }
  }
  stack.push_back (new Operator (Operator::NEGATE));
}

Expression *Expression::clone () const {
  return new Expression (*this);
}

void Expression::saveToBinaryStream (std::ofstream &out) const {
  saveListToBinaryStream (out, "EXP", stack);
  lastConcreteValue->saveToBinaryStream (out);
}

Expression *Expression::loadFromBinaryStream (std::ifstream &in) {
  std::list < ExpressionToken * > stack;
  loadListFromBinaryStream (in, "EXP", stack);
  return new Expression (stack, ConcreteValue::loadFromBinaryStream (in));
}

const std::list < ExpressionToken * > &Expression::getStack () const {
  return stack;
}

void Expression::checkConcreteValueReg (REG reg, const ConcreteValue &concreteVal) const
throw (WrongStateException) {
  if (*lastConcreteValue == concreteVal) {
    return;
  }
  // for reg case, isOverwriting is false for sure
  throw WrongStateException (*lastConcreteValue);
}

void Expression::checkConcreteValueMemory (ADDRINT memoryEa,
    const ConcreteValue &concreteVal)
throw (WrongStateException) {
  if (*lastConcreteValue == concreteVal) {
    isOverwriting = false; // overwriting just works at first getting/synchronization
    return;
  }
  if (!isOverwriting) {
    throw WrongStateException (*lastConcreteValue);
  }
  isOverwriting = false;
  lastConcreteValue->writeToMemoryAddress (memoryEa);
}

}
}
}
}
