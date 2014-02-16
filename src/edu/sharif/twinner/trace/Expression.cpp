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

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

Expression::Expression (const std::list < ExpressionToken * > &stk,
    UINT64 concreteValue) :
stack (stk), lastConcreteValue (concreteValue), isOverwriting (false) {
}

Expression::Expression (const Expression &exp) :
lastConcreteValue (exp.lastConcreteValue), isOverwriting (false) {
  for (std::list < ExpressionToken * >::const_iterator it = exp.stack.begin ();
      it != exp.stack.end (); ++it) {
    const ExpressionToken *et = *it;
    stack.push_back (et->clone ());
  }
}

Expression::Expression (UINT64 concreteValue, bool _isOverwriting) :
lastConcreteValue (concreteValue), isOverwriting (_isOverwriting) {
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

void Expression::setLastConcreteValue (UINT64 value) {
  lastConcreteValue = value;
}

std::string Expression::toString () const {
  std::list < ExpressionToken * > st = std::list < ExpressionToken * > (stack);
  std::stringstream ss;
  convertToInfixExpression (st, ss);
  ss << " /*0x" << std::hex << lastConcreteValue << "*/";
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
  if (bits >= 64) {
    stack.push_back (new Constant (bits));
    stack.push_back (new Operator (Operator::SHIFT_RIGHT));
  } else {
    UINT64 val = 1;
    val <<= bits; // shift-to-right by n bits is equivalent to division by 2^n
    stack.push_back (new Constant (val));
    stack.push_back (new Operator (Operator::DIVIDE));
  }
  lastConcreteValue >>= bits;
}

void Expression::shiftToRight (const Expression *bits) {
  binaryOperation (new Operator (Operator::SHIFT_RIGHT), bits);
}

void Expression::shiftToLeft (int bits) {
  if (bits >= 64) {
    stack.push_back (new Constant (bits));
    stack.push_back (new Operator (Operator::SHIFT_LEFT));
  } else {
    UINT64 val = 1;
    val <<= bits; // shift-to-left by n bits is equivalent to multiplication by 2^n
    stack.push_back (new Constant (val));
    stack.push_back (new Operator (Operator::MULTIPLY));
  }
  lastConcreteValue <<= bits;
}

void Expression::shiftToLeft (const Expression *bits) {
  binaryOperation (new Operator (Operator::SHIFT_LEFT), bits);
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
  out.write ((const char *) &lastConcreteValue, sizeof (lastConcreteValue));
}

Expression *Expression::loadFromBinaryStream (std::ifstream &in) {
  std::list < ExpressionToken * > stack;
  loadListFromBinaryStream (in, "EXP", stack);
  UINT64 concreteValue;
  in.read ((char *) &concreteValue, sizeof (concreteValue));

  return new Expression (stack, concreteValue);
}

const std::list < ExpressionToken * > &Expression::getStack () const {
  return stack;
}

void Expression::checkConcreteValueReg (REG reg, UINT64 concreteVal) const
throw (WrongStateException) {
  if (lastConcreteValue == concreteVal) {

    return;
  }
  // for reg case, isOverwriting is false for sure
  throw WrongStateException (lastConcreteValue);
}

void Expression::checkConcreteValueMemory (ADDRINT memoryEa, UINT64 concreteVal)
throw (WrongStateException) {
  if (lastConcreteValue == concreteVal) {
    isOverwriting = false; // overwriting just works at first getting/synchronization
    return;
  }
  if (!isOverwriting) {
    throw WrongStateException (lastConcreteValue);
  }
  isOverwriting = false;
  edu::sharif::twinner::util::writeMemoryContent (memoryEa, lastConcreteValue);
}

}
}
}
}
