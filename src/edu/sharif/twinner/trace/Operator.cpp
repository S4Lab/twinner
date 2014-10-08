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
#include "ConcreteValue.h"

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

void Operator::apply (ConcreteValue &dst, const ConcreteValue &src) const {
  switch (oi) {
  case ADD:
    dst += src;
    break;
  case MINUS:
    dst -= src;
    break;
  case MULTIPLY:
    dst *= src;
    break;
  case DIVIDE:
    dst /= src;
    break;
  case REMAINDER:
    dst %= src;
    break;
  case XOR:
    dst ^= src;
    break;
  case BITWISE_AND:
    dst &= src;
    break;
  case BITWISE_OR:
    dst |= src;
    break;
  case SHIFT_LEFT:
    dst <<= src;
    break;
  case SHIFT_RIGHT:
    dst >>= src;
    break;
  case ARITHMETIC_SHIFT_RIGHT:
  {
    ConcreteValue *tmp = dst.clone ();
    (*tmp) >>= dst.getSize () - 1;
    dst >>= src;
    if ((*tmp) == 1) {
      (*tmp) <<= src;
      (*tmp) -= 1;
      ConcreteValue *size = src.twosComplement ();
      (*size) += dst.getSize ();
      (*tmp) <<= (*size);
      delete size;
      dst |= (*tmp);
    }
    delete tmp;
    break;
  }
  case ROTATE_RIGHT:
    dst.rotateToRight (src);
    break;
  case ROTATE_LEFT:
    // TODO: Implement
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
    return "logicalShiftToLeft";
  case SHIFT_RIGHT:
    return ">>";
  case ARITHMETIC_SHIFT_RIGHT:
    return "arithmeticShiftToRight";
  case ROTATE_RIGHT:
    return "rotateToRight";
  case ROTATE_LEFT:
    return "rotateToLeft";
  default:
    throw std::runtime_error ("Operator::toString(...): Non-handled operator identifier");
  }
}

Operator::OperatorType Operator::getType () const {
  if (oi == NEGATE) {
    return Unary;
  } else if (oi == ARITHMETIC_SHIFT_RIGHT || oi == SHIFT_LEFT) {
    return FunctionalBinary;
  } else {
    return Binary;
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
