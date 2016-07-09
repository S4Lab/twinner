//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2016 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "BitwiseAndOperator.h"

#include "Constant.h"
#include "Symbol.h"

#include "edu/sharif/twinner/trace/ExpressionImp.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

#include "edu/sharif/twinner/util/max.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

BitwiseAndOperator::BitwiseAndOperator () :
    Operator (Operator::BITWISE_AND) {
  initializeSimplificationRules ();
}

BitwiseAndOperator::BitwiseAndOperator (const BitwiseAndOperator &ao) :
    Operator (ao) {
  initializeSimplificationRules ();
}

BitwiseAndOperator::~BitwiseAndOperator () {
}

BitwiseAndOperator *BitwiseAndOperator::clone () const {
  return new BitwiseAndOperator (*this);
}

bool BitwiseAndOperator::doesSupportSimplification () const {
  return true;
}

bool BitwiseAndOperator::isCommutable () const {
  return true;
}

void BitwiseAndOperator::initializeSimplificationRules () {
  simplificationRules.push_back
      (SimplificationRule (Operator::BITWISE_AND, Operator::BITWISE_AND));
}

bool BitwiseAndOperator::apply (edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  if (operand->isZero ()) {
    delete operand;
    (*exp) = edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
    return true;
  }
  edu::sharif::twinner::trace::Expression::Stack &stack = exp->getStack ();
  if (stack.size () == 1) {
    const Symbol *symbol = dynamic_cast<Symbol *> (stack.back ());
    if (symbol) {
      const int bitSize = symbol->getValue ().getSize ();
      if (isTruncatingMask (operand->clone ())
          && numberOfBits (operand->clone ()) >= bitSize) {
        delete operand;
        return true;
      }
    }
  }
  return Operator::apply (exp, operand);
}

bool BitwiseAndOperator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  dst &= src;
  return dst.getCarryBit ();
}

Operator::SimplificationStatus BitwiseAndOperator::deepSimplify (
    edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  std::list < ExpressionToken * > &stack = exp->getStack ();
  std::list < ExpressionToken * >::iterator it = stack.end ();
  Operator *secondOp = static_cast<Operator *> (*--it);
  if (secondOp->getIdentifier () == Operator::BITWISE_OR) {
    if (propagateDeepSimplificationToSubExpressions
        (stack, *operand, exp->getLastConcreteValue ().getSize ())) {
      return RESTART_SIMPLIFICATION;
    }
    it = stack.end ();
    secondOp = static_cast<Operator *> (*--it);
  }
  if (secondOp->getIdentifier () == Operator::ADD
      || secondOp->getIdentifier () == Operator::MINUS
      || secondOp->getIdentifier () == Operator::BITWISE_OR
      || secondOp->getIdentifier () == Operator::MULTIPLY
      || secondOp->getIdentifier () == Operator::SHIFT_LEFT
      || secondOp->getIdentifier () == Operator::SHIFT_RIGHT) {
    Constant *second = dynamic_cast<Constant *> (*--it);
    if (second) {
      if (secondOp->getIdentifier () == Operator::BITWISE_OR) {
        edu::sharif::twinner::trace::cv::ConcreteValue *secondCv =
            second->getValue ().clone (operand->getSize ());
        (*secondCv) &= (*operand);
        if ((*secondCv) == (*operand)) {
          delete secondCv;
          (*exp) = edu::sharif::twinner::trace::ExpressionImp (operand);
          return COMPLETED;
        }
        delete secondCv;
      } else if (secondOp->getIdentifier () == Operator::SHIFT_LEFT) {
        // exp == (...) << second
        if (isTruncatingMask (operand->clone ())
            && second->getValue () >= numberOfBits (operand->clone ())) {
          delete operand;
          (*exp) = edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
          return COMPLETED;
        }
      }
      if (stack.size () <= 4) {
        return CAN_NOT_SIMPLIFY;
      }
      if (secondOp->getIdentifier () == Operator::SHIFT_LEFT
          || secondOp->getIdentifier () == Operator::SHIFT_RIGHT) {
        // exp == X [<<>>] second
        stack.pop_back (); // removes secondOp
        stack.pop_back (); // removes second
        if (secondOp->getIdentifier () == Operator::SHIFT_LEFT
            || second->getValue () <=
            operand->getSize () - numberOfBits (operand->clone ())) {
          edu::sharif::twinner::trace::cv::ConcreteValue *operandCopy =
              operand->clone ();
          Operator *negOp = secondOp->instantiateNegatedOperator ();
          negOp->apply (*operandCopy, second->getValue ());
          delete negOp;
          exp->bitwiseAnd (operandCopy);
        }
        it = stack.end ();
        Operator *op = dynamic_cast<Operator *> (*--it);
        if (op && op->getIdentifier () == Operator::BITWISE_AND) {
          Constant *mask = dynamic_cast<Constant *> (*--it);
          if (mask) {
            const int size = edu::sharif::twinner::util::max
                (mask->getValue ().getSize (), operand->getSize ());
            edu::sharif::twinner::trace::cv::ConcreteValue *cv =
                mask->getValue ().clone (size);
            stack.pop_back (); // removes op
            stack.pop_back (); // removes mask
            delete op;
            delete mask;
            secondOp->apply (*cv, second->getValue ());
            (*operand) &= (*cv);
            delete cv;
          }
        }
        if (secondOp->getIdentifier () == Operator::SHIFT_LEFT) {
          exp->shiftToLeft (second->getValue ().clone ());
        } else {
          exp->shiftToRight (second->getValue ().clone ());
        }
        delete secondOp;
        delete second;
        return CAN_NOT_SIMPLIFY;
      }
      Operator *firstOp = dynamic_cast<Operator *> (*--it);
      if (firstOp->getIdentifier () == Operator::BITWISE_AND) {
        Constant *first = dynamic_cast<Constant *> (*--it);
        if (first) {
          // exp == (...) & first [+-|*] second
          if (secondOp->getIdentifier () == Operator::BITWISE_OR) {
            const int size = operand->getSize ();
            edu::sharif::twinner::trace::cv::ConcreteValue *secondCv =
                second->getValue ().clone (size);
            (*secondCv) &= (*operand);
            edu::sharif::twinner::trace::cv::ConcreteValue *firstCv =
                first->getValue ().clone (size);
            (*firstCv) |= (*secondCv);
            (*operand) &= (*firstCv);
            delete firstCv;
            stack.pop_back (); // removes addOrMinusOrBitwiseOrOp
            stack.pop_back (); // removes second
            stack.pop_back (); // removes andOp
            stack.pop_back (); // removes first
            exp->bitwiseOr (secondCv);
            delete secondOp;
            delete second;
            delete firstOp;
            delete first;
            return RESTART_SIMPLIFICATION;
          } else if (isTruncatingMask (first->getValue ().clone ())
              && isEquallyOrMoreLimitedThan (*operand, first->getValue ())) {
            // first is similar to 0x00001111
            stack.pop_back (); // removes secondOp
            stack.pop_back (); // removes second
            stack.pop_back (); // removes firstOp
            stack.pop_back (); // removes first
            if (secondOp->getIdentifier () == Operator::ADD) {
              exp->add (second->getValue ().clone ());
            } else if (secondOp->getIdentifier () == Operator::MINUS) {
              exp->minus (second->getValue ().clone ());
            } else {
              exp->multiply (second->getValue ().clone ());
            }
            delete secondOp;
            delete second;
            delete firstOp;
            delete first;
            return RESTART_SIMPLIFICATION;
          } else if (secondOp->getIdentifier () == Operator::MULTIPLY) {
            // exp == (...) & first * second
            if (isTruncatingMask (operand->clone ())) {
              const int maxNumberOfBitsOfResult =
                  numberOfBits (first->getValue ().clone ()) +
                  numberOfBits (second->getValue ().clone ());
              if (maxNumberOfBitsOfResult <= numberOfBits (operand->clone ())) {
                delete operand;
                return COMPLETED;
              }
            }
          }
        }
      } else if (firstOp->getIdentifier () == Operator::ADD
          || firstOp->getIdentifier () == Operator::MINUS) {
        if (secondOp->getIdentifier () == Operator::ADD
            || secondOp->getIdentifier () == Operator::MINUS) {
          Constant *first = dynamic_cast<Constant *> (*--it);
          if (first && isTruncatingMask (operand->clone ())) {
            // exp == (...) [+-] first [+-] second
            edu::sharif::twinner::trace::cv::ConcreteValue *firstCv =
                first->getValue ().clone (operand->getSize ());
            if ((*firstOp) == (*secondOp)) {
              // exp == (...) [+-] (first + second)
              (*firstCv) += second->getValue ();
            } else {
              (*firstCv) -= second->getValue ();
            }
            first->setValue (firstCv);
            stack.pop_back (); // removes addOrMinusOrBitwiseOrOp
            stack.pop_back (); // removes second
            delete secondOp;
            delete second;
            return RESTART_SIMPLIFICATION;
          }
        }
      }
    }
  }
  return CAN_NOT_SIMPLIFY;
}

bool BitwiseAndOperator::propagateDeepSimplificationToSubExpressions (
    std::list < ExpressionToken * > &stack,
    const edu::sharif::twinner::trace::cv::ConcreteValue &operand,
    int bitsize) {
  std::list < ExpressionToken * >::iterator it = stack.end ();
  Operator *op = static_cast<Operator *> (*--it);
  std::list < ExpressionToken * >::iterator rightOperand = findNextOperand (it);
  edu::sharif::twinner::trace::Expression rightExp (rightOperand, it, bitsize);
  edu::sharif::twinner::trace::Expression leftExp
      (stack.begin (), rightOperand, bitsize);
  stack.clear ();
  bool needsRestart =
      propagateDeepSimplificationToSubExpression (&rightExp, operand)
      || propagateDeepSimplificationToSubExpression (&leftExp, operand);
  std::list < ExpressionToken * > &leftStack = leftExp.getStack ();
  std::list < ExpressionToken * > &rightStack = rightExp.getStack ();
  stack.insert (stack.end (), leftStack.begin (), leftStack.end ());
  leftStack.clear ();
  stack.insert (stack.end (), rightStack.begin (), rightStack.end ());
  rightStack.clear ();
  stack.push_back (op);
  return needsRestart || (stack.size () < 3);
}

bool BitwiseAndOperator::propagateDeepSimplificationToSubExpression (
    edu::sharif::twinner::trace::Expression *exp,
    const edu::sharif::twinner::trace::cv::ConcreteValue &operand) {
  if (exp->getStack ().size () > 2) {
    edu::sharif::twinner::trace::cv::ConcreteValue *operandCopy =
        operand.clone ();
    const SimplificationStatus status = deepSimplify (exp, operandCopy);
    if (status != COMPLETED) {
      delete operandCopy;
    }
    return status == RESTART_SIMPLIFICATION;
  }
  return false;
}

std::list < ExpressionToken * >::iterator BitwiseAndOperator::findNextOperand (
    std::list < ExpressionToken * >::iterator it) {
  Operator *op = dynamic_cast<Operator *> (*--it);
  if (op) {
    switch (op->getType ()) {
    case edu::sharif::twinner::trace::exptoken::Operator::SignExtension:
      it = findNextOperand (it);
    case edu::sharif::twinner::trace::exptoken::Operator::Binary:
    case edu::sharif::twinner::trace::exptoken::Operator::FunctionalBinary:
      it = findNextOperand (it);
    case edu::sharif::twinner::trace::exptoken::Operator::Unary:
      it = findNextOperand (it);
      break;
    default:
      edu::sharif::twinner::util::Logger::error () << "Unknown operator type\n";
      abort ();
      break;
    }
  }
  return it;
}

bool BitwiseAndOperator::isTruncatingMask (
    edu::sharif::twinner::trace::cv::ConcreteValue *cv) const {
  const unsigned int s = cv->getSize ();
  for (UINT64 i = 0; i < s; ++i) {
    if ((cv->toUint64 () & 0x1) == 0) {
      if (cv->isZero ()) {
        break;
      } else {
        delete cv;
        return false;
      }
    }
    (*cv) >>= 1;
  }
  delete cv;
  return true;
}

int BitwiseAndOperator::numberOfBits (
    edu::sharif::twinner::trace::cv::ConcreteValue *cv) const {
  int bits = 0;
  while (!cv->isZero ()) {
    (*cv) >>= 1;
    bits++;
  }
  delete cv;
  return bits;
}

bool BitwiseAndOperator::isEquallyOrMoreLimitedThan (
    const edu::sharif::twinner::trace::cv::ConcreteValue &first,
    const edu::sharif::twinner::trace::cv::ConcreteValue &second) const {
  edu::sharif::twinner::trace::cv::ConcreteValue *moreLimited = first.clone ();
  (*moreLimited) &= second;
  const bool firstIsEquallyOrMoreLimited = ((*moreLimited) == first);
  delete moreLimited;
  return firstIsEquallyOrMoreLimited;
}

std::string BitwiseAndOperator::toString () const {
  return "&";
}

bool BitwiseAndOperator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const BitwiseAndOperator *> (&token);
}

}
}
}
}
}
