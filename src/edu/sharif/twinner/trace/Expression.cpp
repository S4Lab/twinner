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

#include "edu/sharif/twinner/trace/exptoken/Constant.h"
#include "edu/sharif/twinner/trace/exptoken/Symbol.h"
#include "WrongStateException.h"
#include "ExpressionImp.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue128Bits.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/memory.h"

#include <stdexcept>
#include <fstream>
#include <sstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

Expression::Expression (const Stack &stk,
    edu::sharif::twinner::trace::cv::ConcreteValue *concreteValue) :
    stack (stk), lastConcreteValue (concreteValue), isOverwriting (false) {
}

Expression::Expression (const Expression &exp) :
    lastConcreteValue (exp.lastConcreteValue->clone ()), isOverwriting (false) {
  for (typename Stack::const_iterator it = exp.stack.begin ();
      it != exp.stack.end (); ++it) {
    const edu::sharif::twinner::trace::exptoken::ExpressionToken *et = *it;
    stack.push_back (et->clone ());
  }
}

Expression::Expression (int size, const Expression &exp) :
    lastConcreteValue (exp.lastConcreteValue->clone (size)), isOverwriting (false) {
  for (typename Stack::const_iterator it = exp.stack.begin ();
      it != exp.stack.end (); ++it) {
    const edu::sharif::twinner::trace::exptoken::ExpressionToken *et = *it;
    stack.push_back (et->clone ());
  }
}

Expression::Expression (edu::sharif::twinner::trace::cv::ConcreteValue *concreteValue,
    bool _isOverwriting) :
    lastConcreteValue (concreteValue), isOverwriting (_isOverwriting) {
}

Expression::~Expression () {
  while (!stack.empty ()) {
    delete stack.back ();
    stack.pop_back ();
  }
  delete lastConcreteValue;
}

const edu::sharif::twinner::trace::cv::ConcreteValue &
Expression::getLastConcreteValue () const {
  return *lastConcreteValue;
}

void Expression::setLastConcreteValue (
    edu::sharif::twinner::trace::cv::ConcreteValue *value) {
  delete lastConcreteValue;
  lastConcreteValue = value;
}

void Expression::setOverwriting (bool overwriting) {
  isOverwriting = overwriting;
}

std::string Expression::toString () const {
  Stack st = Stack (stack);
  std::stringstream ss;
  convertToInfixExpression (st, ss);
  ss << " /*" << *lastConcreteValue << "*/";
  return ss.str ();
}

void Expression::convertToInfixExpression (Stack &st, std::stringstream &ss) const {
  const edu::sharif::twinner::trace::exptoken::ExpressionToken *token = st.back ();
  st.pop_back ();
  const Operator *op = dynamic_cast<const Operator *> (token);
  if (op) {
    std::stringstream operand;
    switch (op->getType ()) {
    case Operator::SignExtension:
      ss << op->toString () << '_';
      convertToInfixExpression (st, ss); // target size
      ss << '_';
      convertToInfixExpression (st, ss); // source size
      ss << " (";
      convertToInfixExpression (st, ss); // main operand (to be sign extended)
      ss << ')';
      break;
    case Operator::Unary:
      convertToInfixExpression (st, operand);
      ss << op->toString () << operand.str ();
      break;
    case Operator::FunctionalBinary:
      convertToInfixExpression (st, operand); // right operand
      ss << op->toString () /*function name*/ << " (";
      convertToInfixExpression (st, ss); // left operand
      ss << ", " << operand.str () << ')';
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
  if (op->doesSupportSimplification () && exp->isTrivial ()) {
    // FIXME: Make sure that last concrete value is always valid at this point
    binaryOperation (op, exp->getLastConcreteValue ().clone ());
    return;
  }
  /**
   * It's possible that this object and given constant expression object be the same.
   * In that case changing this object while searching the given expression can
   * change the constant expression unexpectedly. To avoid such situations, it's important
   * to take required non constant clone of the given constant expression atomically
   * before applying any change to this object.
   */
  Expression *copy = exp->clone ();
  stack.insert (stack.end (), copy->stack.begin (), copy->stack.end ());
  copy->stack.clear ();
  delete copy;
  stack.push_back (op);
  op->apply (*lastConcreteValue, *(exp->lastConcreteValue));
}

void Expression::binaryOperation (Operator *op,
    edu::sharif::twinner::trace::cv::ConcreteValue *cv) {
  if (op->apply (this, cv)) {
    delete op; // in this case operation is simplified and op is not used.
  }
}

void Expression::binaryOperation (Operator *op, UINT64 cv) {
  binaryOperation (op, new edu::sharif::twinner::trace::cv::ConcreteValue64Bits (cv));
}

void Expression::truncate (int bits) {
  //TODO: Detect when truncation has no effect (value is already truncated) and ignore it
  edu::sharif::twinner::trace::cv::ConcreteValue *mask;
  if (bits > 128) {
    edu::sharif::twinner::util::Logger::error ()
        << "Truncating to " << std::dec << bits << " bits.\n";
    throw std::runtime_error ("Expression::truncate(...): number of bits is too large");
  } else if (bits == 128) {
    mask = new edu::sharif::twinner::trace::cv::ConcreteValue128Bits
        (UINT64 (-1), UINT64 (-1));
  } else if (bits > 64) {
    mask = new edu::sharif::twinner::trace::cv::ConcreteValue128Bits
        ((1ull << (bits - 64)) - 1, UINT64 (-1));
  } else if (bits == 64) {
    mask = new edu::sharif::twinner::trace::cv::ConcreteValue64Bits (UINT64 (-1));
  } else { // < 64
    mask = new edu::sharif::twinner::trace::cv::ConcreteValue64Bits ((1ull << bits) - 1);
  }
  bitwiseAnd (mask);
}

Expression *Expression::twosComplement () const {
  edu::sharif::twinner::trace::cv::ConcreteValue *cv = lastConcreteValue->clone ();
  (*cv) = 0;
  Expression *zero = new ExpressionImp (cv);
  zero->minus (this);
  return zero;
}

void Expression::makeLeastSignificantBitsZero (int bits) {
  edu::sharif::twinner::trace::cv::ConcreteValue *mask;
  const int size = lastConcreteValue->getSize ();
  if (bits > size) {
    edu::sharif::twinner::util::Logger::error ()
        << "Cannot make " << std::dec << bits << " bits zero.\n";
    throw std::runtime_error ("Expression::makeLeastSignificantBitsZero(...): "
                              "number of bits is too large");
  } else if (bits == 128) {
    mask = new edu::sharif::twinner::trace::cv::ConcreteValue128Bits
        (UINT64 (0), UINT64 (0));
  } else if (bits > 64) {
    mask = new edu::sharif::twinner::trace::cv::ConcreteValue128Bits
        (~((1ull << (bits - 64)) - 1), UINT64 (0));
  } else if (bits == 64) {
    mask = new edu::sharif::twinner::trace::cv::ConcreteValue128Bits
        (UINT64 (1), UINT64 (0));
  } else { // < 64
    mask = new edu::sharif::twinner::trace::cv::ConcreteValue128Bits
        (UINT64 (1), ~((1ull << bits) - 1));
  }
  if (size != 128) {
    edu::sharif::twinner::trace::cv::ConcreteValue *temp = mask->clone (size);
    delete mask;
    mask = temp;
  }
  bitwiseAnd (mask);
}

void Expression::negate () {
  Operator *op = dynamic_cast<Operator *> (stack.back ());
  if (op && op->getIdentifier () == Operator::NEGATE) {
    stack.pop_back ();
  } else {
    stack.push_back (new Operator (Operator::NEGATE));
  }
  edu::sharif::twinner::trace::cv::ConcreteValue *neg =
      lastConcreteValue->bitwiseNegated ();
  delete lastConcreteValue;
  lastConcreteValue = neg;
}

Expression *Expression::signExtended (int size) const {
  const int mySize = lastConcreteValue->getSize ();
  if (size < mySize) {
    throw std::runtime_error ("Sign-extension cannot truncate the expression");
  } else if (size == mySize) { // no change is required
    return clone (size);
  } else {
    if (isTrivial ()) {
      // FIXME: Make sure that last concrete value is always valid at this point
      return new ExpressionImp (lastConcreteValue->signExtended (size));
    } else {
      Expression *exp = clone (size);
      exp->stack.push_back (new edu::sharif::twinner::trace::exptoken::Constant (mySize));
      exp->stack.push_back (new edu::sharif::twinner::trace::exptoken::Constant (size));
      exp->stack.push_back (new Operator (Operator::SIGN_EXTEND));
      exp->setLastConcreteValue (lastConcreteValue->signExtended (size));
      return exp;
    }
  }
}

Expression *Expression::clone (int size) const {
  return new Expression (size, *this);
}

void Expression::saveToBinaryStream (std::ofstream &out) const {
  saveListToBinaryStream (out, "EXP", stack);
  lastConcreteValue->saveToBinaryStream (out);
}

Expression *Expression::loadFromBinaryStream (std::ifstream &in) {
  Stack stack;
  loadListFromBinaryStream (in, "EXP", stack);
  return new Expression
      (stack, edu::sharif::twinner::trace::cv::ConcreteValue::loadFromBinaryStream (in));
}

const Expression::Stack &Expression::getStack () const {
  return stack;
}

void Expression::checkConcreteValueReg (REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal) const
/* @throw (WrongStateException) */ {
  if (*lastConcreteValue == concreteVal) {
    return;
  }
  // for reg case, isOverwriting is false for sure
  // overwriting is done after syscall instructions instead
  throw WrongStateException (concreteVal, *lastConcreteValue);
}

void Expression::checkConcreteValueMemory (ADDRINT memoryEa,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal)
/* @throw (WrongStateException) */ {
  if (*lastConcreteValue == concreteVal) {
    isOverwriting = false; // overwriting just works at first getting/synchronization
    return;
  }
  if (!isOverwriting) {
    throw WrongStateException (concreteVal, *lastConcreteValue);
  }
  isOverwriting = false;
  lastConcreteValue->writeToMemoryAddress (memoryEa);
}

bool Expression::operator== (const Expression &exp) const {
  Stack::const_iterator it1 = stack.begin (), end1 = stack.end (),
      it2 = exp.stack.begin (), end2 = exp.stack.end ();
  for (; it1 != end1 && it2 != end2 && (**it1) == (**it2); ++it1, ++it2)
    continue;
  return it1 == end1 && it2 == end2;
}

bool Expression::isTrivial () const {
  for (Stack::const_iterator it = stack.begin (); it != stack.end (); ++it) {
    const edu::sharif::twinner::trace::exptoken::ExpressionToken *token = *it;
    if (dynamic_cast<const edu::sharif::twinner::trace::exptoken::Symbol *> (token)) {
      return false;
    }
  }
  return true;
}

}
}
}
}
