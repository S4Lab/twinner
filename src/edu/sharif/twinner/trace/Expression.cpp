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

#include "Expression.h"

#include "edu/sharif/twinner/trace/exptoken/Constant.h"
#include "edu/sharif/twinner/trace/exptoken/Symbol.h"
#include "ExpressionImp.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue128Bits.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/memory.h"
#include "StateSummary.h"

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
  for (Stack::const_iterator it = exp.stack.begin ();
      it != exp.stack.end (); ++it) {
    const edu::sharif::twinner::trace::exptoken::ExpressionToken *et = *it;
    stack.push_back (et->clone ());
  }
}

Expression::Expression (Stack::iterator begin, Stack::iterator end,
    int bitsize) :
    stack (begin, end) {
  edu::sharif::twinner::trace::cv::ConcreteValue64Bits zero (UINT64 (0));
  lastConcreteValue = zero.clone (bitsize);
  isOverwriting = false;
}

Expression &Expression::operator= (const Expression &exp) {
  (*lastConcreteValue) = (*exp.lastConcreteValue);
  isOverwriting = false;
  Stack clonedStack;
  for (Stack::const_iterator it = exp.stack.begin ();
      it != exp.stack.end (); ++it) {
    const edu::sharif::twinner::trace::exptoken::ExpressionToken *et = *it;
    clonedStack.push_back (et->clone ());
  }
  while (!stack.empty ()) {
    delete stack.back ();
    stack.pop_back ();
  }
  stack = clonedStack;
  return *this;
}

Expression::Expression (int size, const Expression &exp) :
    lastConcreteValue (exp.lastConcreteValue->clone (size)), isOverwriting (false) {
  for (Stack::const_iterator it = exp.stack.begin ();
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

edu::sharif::twinner::trace::cv::ConcreteValue &Expression::getLastConcreteValue () {
  return *lastConcreteValue;
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

bool Expression::isOverwritingExpression () const {
  return isOverwriting;
}

class ConvertToInfixExpressionStringVisitor :
public edu::sharif::twinner::trace::exptoken::ExpressionVisitor<std::string> {
public:

  virtual std::string visitTrinary (const Expression::Operator *op,
      std::string &left, std::string &mid, std::string &right) {
    std::stringstream ss;
    if (op->getIdentifier () == Expression::Operator::SIGN_EXTEND) {
      ss << op->toString () << '_' << right /* target */
          << '_' << mid /* source */ << " (" << left << ')';
    } else {
      ss << op->toString () /*function name*/ << '_' << mid /* size */
          << " (" << left << ", " << right /* amount */ << ')';
    }
    return ss.str ();
  }

  virtual std::string visitFunctionalBinary (const Expression::Operator *op,
      std::string &left, std::string &right) {
    std::stringstream ss;
    ss << op->toString () /*function name*/ << " (" << left << ", "
        << right << ')';
    return ss.str ();
  }

  virtual std::string visitBinary (const Expression::Operator *op,
      std::string &left, std::string &right) {
    std::stringstream ss;
    ss << '(' << left << ' ' << op->toString () << ' ' << right << ')';
    return ss.str ();
  }

  virtual std::string visitUnary (const Expression::Operator *op,
      std::string &main) {
    std::stringstream ss;
    ss << op->toString () << main;
    return ss.str ();
  }

  virtual std::string visitOperand (
      const edu::sharif::twinner::trace::exptoken::Operand *operand) {
    return operand->getTechnicalName ();
  }
};

std::string Expression::toString () const {
  if (isTrivial (true)) {
    std::stringstream ss;
    ss << *lastConcreteValue;
    return ss.str ();
  }
  return toDetailedString ();
}

std::string Expression::toDetailedString () const {
  std::stringstream ss;
  ConvertToInfixExpressionStringVisitor visitor;
  std::string str = visit (visitor);
  ss << str << " /*" << *lastConcreteValue << "*/";
  return ss.str ();
}

void Expression::unaryOperation (Operator *op, const Expression *exp) {
  edu::sharif::twinner::util::Logger::error ()
      << "Expression::unaryOperation: Not yet implemented\n";
  abort ();
}

void Expression::binaryOperation (Operator *op, const Expression *exp) {
  if (!(checkForTrivialExpression (op, exp)
        || checkForCancelingOperation (op, exp)
        || checkForNonTrivialAddition (op, exp))) {
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
}

bool Expression::checkForTrivialExpression (Operator *op, const Expression *exp) {
  if (op->doesSupportSimplification ()) {
    if (exp->isTrivial (true)) {
      binaryOperation (op, exp->getLastConcreteValue ().clone ());
      return true;
    } else if (op->isCommutable () && isTrivial (true)) {
      edu::sharif::twinner::trace::cv::ConcreteValue *cv = lastConcreteValue->clone ();
      (*this) = (*exp);
      binaryOperation (op, cv);
      return true;
    }
  }
  return false;
}

bool Expression::checkForCancelingOperation (Operator *op, const Expression *exp) {
  if ((op->getIdentifier () == Operator::DIVIDE
       || op->getIdentifier () == Operator::MINUS
       || op->getIdentifier () == Operator::XOR)
      && (*this) == (*exp)) {
    while (!stack.empty ()) {
      delete stack.back ();
      stack.pop_back ();
    }
    const int v = ((op->getIdentifier () == Operator::DIVIDE) ? 1 : 0);
    stack.push_back (new edu::sharif::twinner::trace::exptoken::Constant (v));
    *lastConcreteValue = v;
    return true;
  }
  return false;
}

bool Expression::checkForNonTrivialAddition (Operator *op, const Expression *exp) {
  if (op->getIdentifier () == Operator::ADD) {
    std::pair<Stack::const_iterator, Stack::const_iterator> its = compareTo (*exp);
    Stack::const_iterator me = its.first, that = its.second;
    if (me == stack.end () && that == exp->stack.end ()) {
      delete op;
      multiply (2);
      return true;
    }
    UINT64 c = checkForNonTrivialAddition (me, that, stack.end (), exp->stack.end ());
    if (c == 0) {
      c = checkForNonTrivialAddition (that, me, exp->stack.end (), stack.end ());
      if (c == 0) {
        return false;
      }
      (*this) = (*exp);
    }
    delete op;
    multiply (c);
    return true;
  }
  return false;
}

UINT64 Expression::checkForNonTrivialAddition (
    Stack::const_iterator me, Stack::const_iterator that,
    Stack::const_iterator myEnd, Stack::const_iterator thatEnd) {
  typedef edu::sharif::twinner::trace::exptoken::Constant Constant;
  typedef edu::sharif::twinner::trace::cv::ConcreteValue ConcreteValue;
  // Checking for `me == Y` and `that == Y c <<` case
  if (me == myEnd) {
    Constant *c = dynamic_cast<Constant *> (*that++);
    if (c && that != thatEnd) {
      Operator *shiftLeft = dynamic_cast<Operator *> (*that++);
      if (shiftLeft && shiftLeft->getIdentifier () == Operator::SHIFT_LEFT) {
        if (that == thatEnd) {
          const ConcreteValue &cv = c->getValue ();
          if (cv.getSize () <= 64) {
            UINT64 val = cv.toUint64 ();
            if (val < 64) {
              return 1ull + (1ull << val);
            }
          }
        }
      }
    }
  } else if (that != thatEnd) {
    // Checking for `me == Z m &` and `that == Z c << k &` case where `k == m c <<`
    Constant *m = dynamic_cast<Constant *> (*me++);
    Constant *c = dynamic_cast<Constant *> (*that++);
    if (m && c && me != myEnd && that != thatEnd) {
      Operator *bitwiseAnd = dynamic_cast<Operator *> (*me++);
      Operator *shiftLeft = dynamic_cast<Operator *> (*that++);
      if (bitwiseAnd && bitwiseAnd->getIdentifier () == Operator::BITWISE_AND
          && shiftLeft && shiftLeft->getIdentifier () == Operator::SHIFT_LEFT
          && me == myEnd && that != thatEnd) {
        Constant *k = dynamic_cast<Constant *> (*that++);
        if (k && that != thatEnd) {
          bitwiseAnd = dynamic_cast<Operator *> (*that++);
          if (bitwiseAnd && bitwiseAnd->getIdentifier () == Operator::BITWISE_AND
              && that == thatEnd) {
            ConcreteValue *mVal = m->getValue ().clone ();
            (*mVal) <<= c->getValue ();
            const bool kIsValid = (*mVal) == k->getValue ();
            delete mVal;
            if (kIsValid) {
              const ConcreteValue &cv = c->getValue ();
              if (cv.getSize () <= 64) {
                UINT64 val = cv.toUint64 ();
                if (val < 64) {
                  return 1ull + (1ull << val);
                }
              }
            }
          }
        }
      }
    }
  }
  return 0;
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

void Expression::trinaryOperation (Operator *op, const Expression *exp) {
  if (!(checkForTrivialExpression (op, exp)
        || checkForCancelingOperation (op, exp)
        || checkForNonTrivialAddition (op, exp))) {
    const UINT64 mySize = lastConcreteValue->getSize ();
    stack.push_back (new edu::sharif::twinner::trace::exptoken::Constant (mySize));
    /**
     * It's possible that this object and given constant expression object be
     * the same. In that case changing this object while searching the given
     * expression can change the constant expression unexpectedly.
     * To avoid such situations, it's required to take a non-constant clone
     * of the given constant expression atomically before applying any change
     * to this object.
     */
    Expression *copy = exp->clone ();
    stack.insert (stack.end (), copy->stack.begin (), copy->stack.end ());
    copy->stack.clear ();
    delete copy;
    stack.push_back (op);
    op->apply (*lastConcreteValue, *(exp->lastConcreteValue));
  }
}

void Expression::trinaryOperation (Operator *op,
    edu::sharif::twinner::trace::cv::ConcreteValue *cv) {
  binaryOperation (op, cv); // op is aware of the implicit size operand
}

void Expression::trinaryOperation (Operator *op, UINT64 cv) {
  trinaryOperation (op, new edu::sharif::twinner::trace::cv::ConcreteValue64Bits (cv));
}

void Expression::truncate (int bits) {
  //TODO: Detect when truncation has no effect (value is already truncated) and ignore it
  edu::sharif::twinner::trace::cv::ConcreteValue *mask;
  if (bits > 128) {
    edu::sharif::twinner::util::Logger::error ()
        << "Expression::truncate(bits=" << std::dec << bits << "): "
        "number of bits is too large\n";
    abort ();
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

Expression *Expression::twosComplement (int precision) const {
  edu::sharif::twinner::trace::cv::ConcreteValue *cv =
      lastConcreteValue->clone (precision);
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
        << "Expression::makeLeastSignificantBitsZero(bits="
        << std::dec << bits << "): number of bits is too large\n";
    abort ();
  } else if (bits == 128) {
    mask = new edu::sharif::twinner::trace::cv::ConcreteValue128Bits
        (UINT64 (0), UINT64 (0));
  } else if (bits > 64) {
    mask = new edu::sharif::twinner::trace::cv::ConcreteValue128Bits
        (~((1ull << (bits - 64)) - 1), UINT64 (0));
  } else if (bits == 64) {
    mask = new edu::sharif::twinner::trace::cv::ConcreteValue128Bits
        (UINT64 (-1), UINT64 (0));
  } else { // < 64
    mask = new edu::sharif::twinner::trace::cv::ConcreteValue128Bits
        (UINT64 (-1), ~((1ull << bits) - 1));
  }
  if (size != 128) {
    edu::sharif::twinner::trace::cv::ConcreteValue *temp = mask->clone (size);
    delete mask;
    mask = temp;
  }
  bitwiseAnd (mask);
}

void Expression::bitwiseNegate () {
  Operator *op = dynamic_cast<Operator *> (stack.back ());
  if (op && op->getIdentifier () == Operator::BITWISE_NEGATE) {
    stack.pop_back ();
    delete op;
  } else {
    stack.push_back (Operator::instantiateOperator (Operator::BITWISE_NEGATE));
  }
  edu::sharif::twinner::trace::cv::ConcreteValue *neg =
      lastConcreteValue->bitwiseNegated ();
  delete lastConcreteValue;
  lastConcreteValue = neg;
}

Expression *Expression::signExtended (int size) const {
  const int mySize = lastConcreteValue->getSize ();
  if (size < mySize) {
    edu::sharif::twinner::util::Logger::error ()
        << "Expression::signExtended (size=" << std::dec << size << ") "
        "[current size=" << mySize << "]:"
        " Sign-extension cannot truncate the expression\n";
    abort ();
  } else if (size == mySize) { // no change is required
    return clone (size);
  } else {
    if (isTrivial (true)) {
      return new ExpressionImp (lastConcreteValue->signExtended (size));
    } else {
      Expression *exp = clone (size);
      exp->stack.push_back (new edu::sharif::twinner::trace::exptoken::Constant (mySize));
      exp->stack.push_back (new edu::sharif::twinner::trace::exptoken::Constant (size));
      exp->stack.push_back (Operator::instantiateOperator (Operator::SIGN_EXTEND));
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

Expression::Stack &Expression::getStack () {
  return stack;
}

const Expression::Stack &Expression::getStack () const {
  return stack;
}

void Expression::checkConcreteValueReg (REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal,
    StateSummary &state) const {
  if (*lastConcreteValue == concreteVal) {
    return;
  }
  // for reg case, isOverwriting is set to false after the syscall instruction
  state.initWrongState (concreteVal, *lastConcreteValue);
}

void Expression::checkConcreteValueMemory (ADDRINT memoryEa,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal,
    StateSummary &state) {
  if (*lastConcreteValue == concreteVal) {
    isOverwriting = false; // overwriting just works at first getting/synchronization
    return;
  }
  if (!isOverwriting) {
    state.initWrongState (concreteVal, *lastConcreteValue);
    return;
  }
  isOverwriting = false;
  if (!lastConcreteValue->writeToMemoryAddress (memoryEa)) {
    edu::sharif::twinner::util::Logger::error ()
        << "Expression::checkConcreteValueMemory"
        " (memoryEa=0x" << std::hex << memoryEa << ", ...):"
        " cannot overwrite memory value\n";
    abort ();
  }
}

std::pair<Expression::Stack::const_iterator, Expression::Stack::const_iterator>
Expression::compareTo (const Expression &exp) const {
  Stack::const_iterator it1 = stack.begin (), end1 = stack.end (),
      it2 = exp.stack.begin (), end2 = exp.stack.end ();
  for (; it1 != end1 && it2 != end2 && (**it1) == (**it2); ++it1, ++it2)
    continue;
  return make_pair (it1, it2);
}

bool Expression::operator== (const Expression &exp) const {
  Stack::const_iterator it1 = stack.begin (), end1 = stack.end (),
      it2 = exp.stack.begin (), end2 = exp.stack.end ();
  for (; it1 != end1 && it2 != end2 && (**it1) == (**it2); ++it1, ++it2)
    continue;
  return it1 == end1 && it2 == end2;
}

bool Expression::isTrivial (bool requiresValidConcreteValue) const {
  for (Stack::const_iterator it = stack.begin (); it != stack.end (); ++it) {
    const edu::sharif::twinner::trace::exptoken::ExpressionToken *token = *it;
    if (dynamic_cast<const edu::sharif::twinner::trace::exptoken::Operand *> (token)) {
      const edu::sharif::twinner::trace::exptoken::Operand *operand =
          static_cast<const edu::sharif::twinner::trace::exptoken::Operand *> (token);
      if (!operand->isConstant ()) {
        return false;
      }
    }
  }
  return !requiresValidConcreteValue || makeConcreteValueValid ();
}

class CalculateConcreteValueVisitor :
public edu::sharif::twinner::trace::exptoken::ExpressionVisitor
<edu::sharif::twinner::trace::cv::ConcreteValue128Bits> {
private:
  bool overflow;

public:

  CalculateConcreteValueVisitor () :
      overflow (false) {
  }

  virtual ResultType visitTrinary (
      const edu::sharif::twinner::trace::exptoken::Operator *op,
      ResultType &left,
      ResultType &mid,
      ResultType &right) {
    overflow = overflow || op->apply (left, mid, right);
    return left;
  }

  virtual ResultType visitFunctionalBinary (
      const edu::sharif::twinner::trace::exptoken::Operator *op,
      ResultType &left, ResultType &right) {
    return visitBinary (op, left, right);
  }

  virtual ResultType visitBinary (
      const edu::sharif::twinner::trace::exptoken::Operator *op,
      ResultType &left, ResultType &right) {
    overflow = overflow || op->apply (left, right);
    return left;
  }

  virtual ResultType visitUnary (
      const edu::sharif::twinner::trace::exptoken::Operator *op,
      ResultType &cv) {
    // The only supported unary operation is bitwise negation
    edu::sharif::twinner::trace::cv::ConcreteValue *negated =
        cv.bitwiseNegated ();
    cv = *negated;
    delete negated;
    return cv;

  }

  virtual ResultType visitOperand (
      const edu::sharif::twinner::trace::exptoken::Operand *operand) {
    // ASSERT: operand is constant
    edu::sharif::twinner::trace::cv::ConcreteValue128Bits cv =
        operand->getValue ();
    return cv;
  }

  bool hasOverflow () const {
    return overflow;
  }
};

bool Expression::makeConcreteValueValid () const {
  CalculateConcreteValueVisitor visitor;
  edu::sharif::twinner::trace::cv::ConcreteValue128Bits cv = visit (visitor);
  if (visitor.hasOverflow ()) {
    return false;
  }
  (*lastConcreteValue) = cv;
  return cv == (*lastConcreteValue);
}


}
}
}
}
