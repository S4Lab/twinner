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

#include "Expression.h"
#include "Constant.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

#include <fstream>
#include <stdexcept>

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

bool Operator::doesSupportSimplification () const {
  switch (oi) {
  case ADD:
  case MINUS:
  case MULTIPLY:
  case DIVIDE:
    //  case REMAINDER:
  case XOR:
  case BITWISE_AND:
  case BITWISE_OR:
  case SHIFT_LEFT:
  case SHIFT_RIGHT:
  case ARITHMETIC_SHIFT_RIGHT:
  case ROTATE_RIGHT:
    //  case ROTATE_LEFT:
    return true;
  default:
    return false;
  }
}

bool Operator::apply (Expression *exp,
    edu::sharif::twinner::trace::ConcreteValue *operand) {
  switch (oi) {
  case ADD:
  {
    Constant *lastConstant = 0;
    if (!exp->stack.empty () && dynamic_cast<Constant *> (exp->stack.back ())) {
      lastConstant = static_cast<Constant *> (exp->stack.back ());

    } else if (exp->stack.size () > 2 && dynamic_cast<Operator *> (exp->stack.back ())) {
      std::list < ExpressionToken * >::iterator it = exp->stack.end ();
      if (static_cast<Operator *> (*--it)->getIdentifier () == Operator::ADD) {
        lastConstant = dynamic_cast<Constant *> (*--it);
      }
    }
    if (lastConstant) {
      (*exp->lastConcreteValue) += *operand;
      (*operand) += lastConstant->getValue ();
      lastConstant->setValue (*operand);
      delete operand;
      return true;
    } else {
      exp->stack.push_back (new Constant (operand));
      exp->stack.push_back (this);
      (*exp->lastConcreteValue) += *operand;
      return false;
    }
  }
  case MINUS:
  {
    Constant *lastConstant = 0;
    if (!exp->stack.empty () && dynamic_cast<Constant *> (exp->stack.back ())) {
      lastConstant = static_cast<Constant *> (exp->stack.back ());

    } else if (exp->stack.size () > 2 && dynamic_cast<Operator *> (exp->stack.back ())) {
      std::list < ExpressionToken * >::iterator it = exp->stack.end ();
      if (static_cast<Operator *> (*--it)->getIdentifier () == Operator::ADD) {
        lastConstant = dynamic_cast<Constant *> (*--it);
      }
    }
    if (lastConstant) {
      (*exp->lastConcreteValue) -= *operand;
      edu::sharif::twinner::trace::ConcreteValue *cv = lastConstant->getValue ().clone ();
      (*cv) -= (*operand);
      lastConstant->setValue (*cv);
      delete operand;
      delete cv;
      return true;
    } else {
      exp->stack.push_back (new Constant (operand));
      exp->stack.push_back (this);
      (*exp->lastConcreteValue) -= *operand;
      return false;
    }
  }
  case MULTIPLY:
  {
    Constant *lastConstant = 0;
    if (!exp->stack.empty () && dynamic_cast<Constant *> (exp->stack.back ())) {
      lastConstant = static_cast<Constant *> (exp->stack.back ());

    } else if (exp->stack.size () > 2 && dynamic_cast<Operator *> (exp->stack.back ())) {
      std::list < ExpressionToken * >::iterator it = exp->stack.end ();
      if (static_cast<Operator *> (*--it)->getIdentifier () == Operator::MULTIPLY) {
        lastConstant = dynamic_cast<Constant *> (*--it);
      }
    }
    if (lastConstant) {
      (*exp->lastConcreteValue) *= *operand;
      (*operand) *= lastConstant->getValue ();
      lastConstant->setValue (*operand);
      delete operand;
      return true;
    } else {
      exp->stack.push_back (new Constant (operand));
      exp->stack.push_back (this);
      (*exp->lastConcreteValue) *= *operand;
      return false;
    }
  }
  case DIVIDE:
  {
    Constant *lastConstant = 0;
    if (!exp->stack.empty () && dynamic_cast<Constant *> (exp->stack.back ())) {
      lastConstant = static_cast<Constant *> (exp->stack.back ());

    } else if (exp->stack.size () > 2 && dynamic_cast<Operator *> (exp->stack.back ())) {
      std::list < ExpressionToken * >::iterator it = exp->stack.end ();
      if (static_cast<Operator *> (*--it)->getIdentifier () == Operator::MULTIPLY) {
        lastConstant = dynamic_cast<Constant *> (*--it);
      }
    }
    if (lastConstant) {
      (*exp->lastConcreteValue) /= *operand;
      edu::sharif::twinner::trace::ConcreteValue *cv = lastConstant->getValue ().clone ();
      (*cv) /= (*operand);
      lastConstant->setValue (*cv);
      delete operand;
      delete cv;
      return true;
    } else {
      exp->stack.push_back (new Constant (operand));
      exp->stack.push_back (this);
      (*exp->lastConcreteValue) /= *operand;
      return false;
    }
  }
    //  case REMAINDER:
  case XOR:
  {
    Constant *lastConstantMask = 0;
    if (!exp->stack.empty () && dynamic_cast<Constant *> (exp->stack.back ())) {
      lastConstantMask = static_cast<Constant *> (exp->stack.back ());

    } else if (exp->stack.size () > 2 && dynamic_cast<Operator *> (exp->stack.back ())) {
      std::list < ExpressionToken * >::iterator it = exp->stack.end ();
      if (static_cast<Operator *> (*--it)->getIdentifier () == Operator::XOR) {
        lastConstantMask = dynamic_cast<Constant *> (*--it);
      }
    }
    if (lastConstantMask) {
      (*exp->lastConcreteValue) ^= *operand;
      (*operand) ^= lastConstantMask->getValue ();
      lastConstantMask->setValue (*operand);
      delete operand;
      return true;
    } else {
      exp->stack.push_back (new Constant (operand));
      exp->stack.push_back (this);
      (*exp->lastConcreteValue) ^= *operand;
      return false;
    }
  }
  case BITWISE_AND:
  {
    Constant *lastConstantMask = 0;
    if (!exp->stack.empty () && dynamic_cast<Constant *> (exp->stack.back ())) {
      lastConstantMask = static_cast<Constant *> (exp->stack.back ());

    } else if (exp->stack.size () > 2 && dynamic_cast<Operator *> (exp->stack.back ())) {
      std::list < ExpressionToken * >::iterator it = exp->stack.end ();
      if (static_cast<Operator *> (*--it)->getIdentifier () == Operator::BITWISE_AND) {
        lastConstantMask = dynamic_cast<Constant *> (*--it);
      }
    }
    if (lastConstantMask) {
      (*exp->lastConcreteValue) &= *operand;
      (*operand) &= lastConstantMask->getValue ();
      lastConstantMask->setValue (*operand);
      delete operand;
      return true;
    } else {
      exp->stack.push_back (new Constant (operand));
      exp->stack.push_back (this);
      (*exp->lastConcreteValue) &= *operand;
      return false;
    }
  }
  case BITWISE_OR:
  {
    Constant *lastConstantMask = 0;
    if (!exp->stack.empty () && dynamic_cast<Constant *> (exp->stack.back ())) {
      lastConstantMask = static_cast<Constant *> (exp->stack.back ());

    } else if (exp->stack.size () > 2 && dynamic_cast<Operator *> (exp->stack.back ())) {
      std::list < ExpressionToken * >::iterator it = exp->stack.end ();
      if (static_cast<Operator *> (*--it)->getIdentifier () == Operator::BITWISE_OR) {
        lastConstantMask = dynamic_cast<Constant *> (*--it);
      }
    }
    if (lastConstantMask) {
      (*exp->lastConcreteValue) |= *operand;
      (*operand) |= lastConstantMask->getValue ();
      lastConstantMask->setValue (*operand);
      delete operand;
      return true;
    } else {
      exp->stack.push_back (new Constant (operand));
      exp->stack.push_back (this);
      (*exp->lastConcreteValue) |= *operand;
      return false;
    }
  }
  case SHIFT_LEFT:
  {
    if ((*operand) >= 64) {
      exp->stack.push_back (new Constant (operand));
      exp->stack.push_back (this);
      (*exp->lastConcreteValue) <<= *operand;
      return false;
    } else {
      // shift-to-left by n bits is equivalent to multiplication by 2^n
      UINT64 val = (1ull << operand->toUint64 ());
      delete operand;
      if (val > 1) {
        exp->multiply (val);
      }
      return true;
    }
  }
  case SHIFT_RIGHT:
  {
    if ((*operand) >= 64) {
      (*exp->lastConcreteValue) >>= *operand;
      if (!exp->stack.empty () && dynamic_cast<Constant *> (exp->stack.back ())) {
        Constant *lastConstant = static_cast<Constant *> (exp->stack.back ());
        edu::sharif::twinner::trace::ConcreteValue *cv = lastConstant->getValue ().clone ();
        (*cv) >>= (*operand);
        lastConstant->setValue (*cv);
        delete operand;
        delete cv;
        return true; // means that this operator is not used and can be deleted.
      } else {
        exp->stack.push_back (new Constant (operand));
        exp->stack.push_back (this);
        return false; // means that this operator is owned by exp since now
      }
    } else {
      // shift-to-right by n bits is equivalent to division by 2^n
      const UINT64 val = (1ull << operand->toUint64 ());
      delete operand;
      if (val > 1) {
        exp->divide (val);
      }
      return true; // means that this operator is not used and can be deleted.
    }
  }
  case ARITHMETIC_SHIFT_RIGHT:
  {
    exp->lastConcreteValue->arithmeticShiftToRight (*operand);
    if (!exp->stack.empty () && dynamic_cast<Constant *> (exp->stack.back ())) {
      Constant *lastConstant = static_cast<Constant *> (exp->stack.back ());
      edu::sharif::twinner::trace::ConcreteValue *cv = lastConstant->getValue ().clone ();
      cv->arithmeticShiftToRight (*operand);
      lastConstant->setValue (*cv);
      delete operand;
      delete cv;
      return true;
    } else {
      exp->stack.push_back (new Constant (operand));
      exp->stack.push_back (this);
      return false;
    }
  }
  case ROTATE_RIGHT:
  {
    exp->stack.push_back (new Constant (operand));
    exp->stack.push_back (this);
    exp->lastConcreteValue->rotateToRight (*operand);
    return false;
  }
    //  case ROTATE_LEFT:
  default:
    throw std::runtime_error ("Operator::apply(Expression *, ConcreteValue *):"
                              " Non-handled operator identifier");
  }
}

void Operator::apply (edu::sharif::twinner::trace::ConcreteValue &dst,
    const edu::sharif::twinner::trace::ConcreteValue &src) const {
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
    dst.arithmeticShiftToRight (src);
    break;
  case ROTATE_RIGHT:
    dst.rotateToRight (src);
    break;
  case ROTATE_LEFT:
    // TODO: Implement
  default:
    throw std::runtime_error ("Operator::apply(ConcreteValue &, const ConcreteValue &):"
                              " Non-handled operator identifier");
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
