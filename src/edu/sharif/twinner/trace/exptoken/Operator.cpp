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

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

#include "edu/sharif/twinner/util/Logger.h"

#include <fstream>
#include <stdexcept>

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

bool Operator::doesSupportSimplification () const {
  return false;
}

bool Operator::apply (edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::cv::ConcreteValue *operand) {
  edu::sharif::twinner::util::Logger::error ()
      << "Operator::apply(Expression *, ConcreteValue *): "
      "Unknown OperatorIdentifier: " << std::dec << oi << '\n';
  throw std::runtime_error ("Operator::apply(Expression *, ConcreteValue *):"
                            " Non-handled operator identifier");
}

void Operator::apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
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
