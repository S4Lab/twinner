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

#include <fstream>
#include <stdexcept>

#include "Operator.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

Operator::Operator (OperatorIdentifier _oi) :
ExpressionToken (), oi (_oi) {
}

Operator::Operator (const Operator &op) :
ExpressionToken (op), oi (op.oi) {
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
  return new Operator (oi);
}

UINT64 Operator::apply (UINT64 a, UINT64 b) const {
  switch (oi) {
  case ADD:
    return a + b;
  case MINUS:
    return a - b;
  case MULTIPLY:
    return a * b;
  case DIVIDE:
    return a / b;
  case REMAINDER:
    return a % b;
  case XOR:
    return a ^ b;
  case BITWISE_AND:
    return a & b;
  case BITWISE_OR:
    return a | b;
  case SHIFT_LEFT:
    return a << b;
  case SHIFT_RIGHT:
    return a >> b;
  default:
    throw std::runtime_error ("Operator::apply(...): Non-handled operator identifier");
  }
}

std::string Operator::toString () const {
  switch (oi) {
  case ADD:
    return "+";
  case MINUS:
    return "-";
  case MULTIPLY:
    return "*";
  case DIVIDE:
    return "/";
  case REMAINDER:
    return "%";
  case NEGATE:
    return "-";
  case XOR:
    return "^";
  case BITWISE_AND:
    return "&";
  case BITWISE_OR:
    return "|";
  case SHIFT_LEFT:
    return "<<";
  case SHIFT_RIGHT:
    return ">>";
  default:
    throw std::runtime_error ("Operator::toString(...): Non-handled operator identifier");
  }
}

Operator::OperatorType Operator::getType () const {
  if (oi == NEGATE) {
    return Unary;
  } else {
    return Binary;
  }
}

Operator::OperatorIdentifier Operator::getIdentifier () const {
  return oi;
}

}
}
}
}
