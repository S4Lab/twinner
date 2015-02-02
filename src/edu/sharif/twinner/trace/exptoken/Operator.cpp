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

#include "Operator.h"

#include "Constant.h"
#include "AddOperator.h"
#include "MinusOperator.h"
#include "MultiplyOperator.h"
#include "DivideOperator.h"
#include "XorOperator.h"
#include "BitwiseAndOperator.h"
#include "BitwiseOrOperator.h"
#include "ShiftLeftOperator.h"
#include "ShiftRightOperator.h"
#include "ArithmeticShiftRightOperator.h"
#include "RotateRightOperator.h"

#include "edu/sharif/twinner/trace/ExpressionImp.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

#include "edu/sharif/twinner/util/Logger.h"

#include <fstream>
#include <stdexcept>
#include <algorithm>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

Operator::Operator (OperatorIdentifier _oi) :
    ExpressionToken (), oi (_oi) {
}

Operator::Operator (const Operator &op) :
    ExpressionToken (op), oi (op.oi) {
}

Operator::~Operator () {
}

Operator *Operator::clone () const {
  return new Operator (*this);
}

void Operator::saveToBinaryStream (std::ofstream &out) const {
  out.write ("O", 1);
  out.write ((const char *) &oi, sizeof (oi));
}

Operator *Operator::loadFromBinaryStream (std::ifstream &in) {
  OperatorIdentifier oi;
  in.read ((char *) &oi, sizeof (oi));
  return instantiateOperator (oi);
}

Operator *Operator::instantiateOperator (OperatorIdentifier oi) {
  switch (oi) {
  case ADD:
    return new AddOperator ();
  case MINUS:
    return new MinusOperator ();
  case MULTIPLY:
    return new MultiplyOperator ();
  case DIVIDE:
    return new DivideOperator ();
  case XOR:
    return new XorOperator ();
  case BITWISE_AND:
    return new BitwiseAndOperator ();
  case BITWISE_OR:
    return new BitwiseOrOperator ();
  case SHIFT_LEFT:
    return new ShiftLeftOperator ();
  case SHIFT_RIGHT:
    return new ShiftRightOperator ();
  case ARITHMETIC_SHIFT_RIGHT:
    return new ArithmeticShiftRightOperator ();
  case ROTATE_RIGHT:
    return new RotateRightOperator ();
  default:
    return new Operator (oi);
  }
}

Operator *Operator::instantiateNegatedOperator () const {
  switch (oi) {
  case ADD:
    return new MinusOperator ();
  case MINUS:
    return new AddOperator ();
  case SHIFT_LEFT:
    return new ShiftRightOperator ();
  case SHIFT_RIGHT:
    return new ShiftLeftOperator ();
  default:
    throw std::runtime_error ("Operator::instantiateNegatedOperator (): "
                              "This operator is not negatable");
  }
}

bool Operator::doesSupportSimplification () const {
  return false;
}

bool Operator::apply (edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  Constant *lastConstant = 0;
  const Operator *lop = 0;
  const Operator *sop = 0;
  // i.e. simplifying last concrete value of exp to zero, zeroes whole of exp
  bool isZeroNatural = false; // e.g. Z * 0 == 0
  // i.e. using zero as the last operand on this exp has no effect
  bool isZeroNeutral = false; //  e.g. Z + 0 == Z
  edu::sharif::twinner::trace::Expression::Stack &stack = exp->getStack ();
  if (!stack.empty () && dynamic_cast<Constant *> (stack.back ())) {
    lastConstant = static_cast<Constant *> (stack.back ());
    lop = this;

  } else if (stack.size () > 2 && dynamic_cast<Operator *> (stack.back ())) {
    std::list < ExpressionToken * >::iterator it = stack.end ();
    lop = static_cast<Operator *> (*--it);
    isZeroNatural = (lop->getIdentifier () == Operator::BITWISE_AND
        || lop->getIdentifier () == Operator::MULTIPLY);
    isZeroNeutral = (lop->getIdentifier () == Operator::BITWISE_OR
        || lop->getIdentifier () == Operator::SHIFT_LEFT
        || lop->getIdentifier () == Operator::SHIFT_RIGHT
        || lop->getIdentifier () == Operator::ADD
        || lop->getIdentifier () == Operator::MINUS);
    bool mayNeedDeepSimplification = true;
    for (std::vector<SimplificationRule>::iterator rule = simplificationRules.begin ();
        rule != simplificationRules.end (); ++rule) {
      if (lop->getIdentifier () == rule->lastOperator) {
        mayNeedDeepSimplification = false;
        lastConstant = dynamic_cast<Constant *> (*--it);
        sop = Operator::instantiateOperator (rule->simplificationOperator);
        break;
      }
    }
    if (mayNeedDeepSimplification) {
      switch (deepSimplify (exp, operand)) {
      case CAN_NOT_SIMPLIFY:
        break;
      case RESTART_SIMPLIFICATION:
        return apply (exp, operand);
      case COMPLETED:
        return true;
      }
    }
  }
  apply (exp->getLastConcreteValue (), *operand);
  if (lastConstant) {
    const int size = std::max (std::max (exp->getLastConcreteValue ().getSize (),
                                         lastConstant->getValue ().getSize ()),
                               operand->getSize ());
    edu::sharif::twinner::trace::cv::ConcreteValue *cv =
        lastConstant->getValue ().clone (size);
    bool overflow;
    bool alternatingNegatableOperators = false; // e.g. Z + x - y or Z >> x << y
    if (sop) {
      overflow = sop->apply (*cv, *operand);
      alternatingNegatableOperators = (lop->getIdentifier () == Operator::ADD
          && sop->getIdentifier () == Operator::MINUS)
          || (lop->getIdentifier () == Operator::MINUS
          && sop->getIdentifier () == Operator::ADD)
          || (lop->getIdentifier () == Operator::SHIFT_LEFT
          && sop->getIdentifier () == Operator::SHIFT_RIGHT)
          || (lop->getIdentifier () == Operator::SHIFT_RIGHT
          && sop->getIdentifier () == Operator::SHIFT_LEFT);
      delete sop;
    } else {
      overflow = lop->apply (*cv, *operand);
    }
    if (overflow) {
      if (alternatingNegatableOperators) {
        edu::sharif::twinner::trace::cv::ConcreteValue *negativeOfCv =
            cv->twosComplement ();
        delete cv;
        cv = negativeOfCv;
        stack.pop_back ();
        stack.push_back (lop->instantiateNegatedOperator ());
        delete lop;
      } else {
        delete cv;
        cv = 0;
      }
    }
    if (cv) {
      delete operand;
      lastConstant->setValue (cv);
      if (isZeroNatural && lastConstant->getValue ().isZero ()) {
        (*exp) = edu::sharif::twinner::trace::ExpressionImp (UINT64 (0));
      } else if (isZeroNeutral && lastConstant->getValue ().isZero ()) {
        delete stack.back ();
        stack.pop_back (); // removes operator
        delete stack.back ();
        stack.pop_back (); // removes operand
      }
      return true;
    }
  }
  stack.push_back (new Constant (operand));
  stack.push_back (this);
  return false;
}

Operator::SimplificationStatus Operator::deepSimplify (
    edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  return CAN_NOT_SIMPLIFY;
}

bool Operator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
    const edu::sharif::twinner::trace::cv::ConcreteValue &src) const {
  switch (oi) {
  case REMAINDER:
    dst %= src;
    break;
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Operator::apply(ConcreteValue &, const ConcreteValue &):"
        "Unknown OperatorIdentifier: " << std::dec << oi << '\n';
    throw std::runtime_error ("Operator::apply(ConcreteValue &, const ConcreteValue &):"
                              " Non-handled operator identifier");
  }
  return dst.getCarryBit ();
}

std::string Operator::toString () const {
  switch (oi) {
  case REMAINDER:
    return "%";
  case NEGATE:
    return "-";
  case ROTATE_LEFT:
    return "rotateToLeft";
  case SIGN_EXTEND:
    return "signExtend";
  default:
    throw std::runtime_error ("Operator::toString(...): Non-handled operator identifier");
  }
}

Operator::OperatorType Operator::getType () const {
  if (oi == SIGN_EXTEND) {
    return SignExtension;
  } else if (oi < MAX_UNARY_OPERATOR_IDENTIFIER) {
    return Unary;
  } else if (oi < MAX_BINARY_OPERATOR_IDENTIFIER) {
    return Binary;
  } else {
    return FunctionalBinary;
  }
}

Operator::OperatorIdentifier Operator::getIdentifier () const {
  return oi;
}

bool Operator::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const Operator *> (&token)
      && static_cast<const Operator *> (&token)->oi == oi;
}

}
}
}
}
}
