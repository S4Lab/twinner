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

#include "ConstraintToCvc4ExprConverter.h"

#include "edu/sharif/twinner/trace/ConcreteValue.h"
#include "edu/sharif/twinner/trace/Constant.h"
#include "edu/sharif/twinner/trace/Symbol.h"
#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/Constraint.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace smt {

ConstraintToCvc4ExprConverter::ConstraintToCvc4ExprConverter (ExprManager &_em,
    const std::list < const edu::sharif::twinner::trace::Constraint * > &_constraints) :
    em (_em), constraints (_constraints), initialized (false) {
  type = em.mkBitVectorType (128);

  zero = em.mkConst (BitVector (128, UINT64 (0)));
  max64 = em.mkConst (BitVector (128, UINT64 (0)).setBit (64));

  constants.insert (std::make_pair ("00000000000000000", zero));
  constants.insert (std::make_pair ("10000000000000000", max64));
}

Expr ConstraintToCvc4ExprConverter::convert (std::map<std::string, Expr> &symbols) {
  std::list < const edu::sharif::twinner::trace::Constraint * >::const_iterator it =
      constraints.begin ();
  addConstraint (convertConstraintToCvc4Expr (symbols, *it));
  while (++it != constraints.end ()) {
    addConstraint (convertConstraintToCvc4Expr (symbols, *it));
  }
  return totalConstraint;
}

void ConstraintToCvc4ExprConverter::addConstraint (Expr constraint) {
  if (initialized) {
    totalConstraint = totalConstraint.andExpr (constraint);
  } else {
    initialized = true;
    totalConstraint = constraint;
  }
}

Expr ConstraintToCvc4ExprConverter::convertConstraintToCvc4Expr (
    std::map<std::string, Expr> &symbols,
    const edu::sharif::twinner::trace::Constraint *constraint) {
  Expr leftExp = convertExpressionToCvc4Expr (symbols, constraint->getMainExpression ());
  if (constraint->getAuxExpression ()) {
    Expr rightExp = convertExpressionToCvc4Expr (symbols, constraint->getAuxExpression ());
    switch (constraint->getComparisonType ()) {
    case edu::sharif::twinner::trace::Constraint::BELOW_OR_EQUAL:
      return em.mkExpr (kind::BITVECTOR_ULE, leftExp, rightExp);
    case edu::sharif::twinner::trace::Constraint::ABOVE_OR_EQUAL:
      return em.mkExpr (kind::BITVECTOR_UGE, leftExp, rightExp);
    case edu::sharif::twinner::trace::Constraint::NON_EQUAL:
      return em.mkExpr (kind::DISTINCT, leftExp, rightExp);
    case edu::sharif::twinner::trace::Constraint::ABOVE:
      return em.mkExpr (kind::BITVECTOR_UGT, leftExp, rightExp);
    case edu::sharif::twinner::trace::Constraint::BELOW:
      return em.mkExpr (kind::BITVECTOR_ULT, leftExp, rightExp);
    case edu::sharif::twinner::trace::Constraint::EQUAL:
      return em.mkExpr (kind::EQUAL, leftExp, rightExp);
    case edu::sharif::twinner::trace::Constraint::LESS_OR_EQUAL:
      return em.mkExpr (kind::BITVECTOR_SLE, leftExp, rightExp);
    case edu::sharif::twinner::trace::Constraint::GREATER_OR_EQUAL:
      return em.mkExpr (kind::BITVECTOR_SGE, leftExp, rightExp);
    case edu::sharif::twinner::trace::Constraint::GREATER:
      return em.mkExpr (kind::BITVECTOR_SGT, leftExp, rightExp);
    case edu::sharif::twinner::trace::Constraint::LESS:
      return em.mkExpr (kind::BITVECTOR_SLT, leftExp, rightExp);
    default:
      throw std::runtime_error ("Unknown Comparison Type");
    }
  } else {
    switch (constraint->getComparisonType ()) {
    case edu::sharif::twinner::trace::Constraint::NON_POSITIVE:
      return em.mkExpr (kind::BITVECTOR_SLE, leftExp, zero);
    case edu::sharif::twinner::trace::Constraint::NON_NEGATIVE:
      return em.mkExpr (kind::BITVECTOR_SGE, leftExp, zero);
    case edu::sharif::twinner::trace::Constraint::NON_ZERO:
      return em.mkExpr (kind::DISTINCT, leftExp, zero);
    case edu::sharif::twinner::trace::Constraint::POSITIVE:
      return em.mkExpr (kind::BITVECTOR_SGT, leftExp, zero);
    case edu::sharif::twinner::trace::Constraint::NEGATIVE:
      return em.mkExpr (kind::BITVECTOR_SLT, leftExp, zero);
    case edu::sharif::twinner::trace::Constraint::ZERO:
      return em.mkExpr (kind::EQUAL, leftExp, zero);
    default:
      throw std::runtime_error ("Unknown Comparison Type");
    }
  }
}

Expr ConstraintToCvc4ExprConverter::convertExpressionToCvc4Expr (
    std::map<std::string, Expr> &symbols,
    const edu::sharif::twinner::trace::Expression *exp) {
  const std::list < edu::sharif::twinner::trace::ExpressionToken * > &stack =
      exp->getStack ();
  std::list < edu::sharif::twinner::trace::ExpressionToken * >::const_iterator top =
      stack.end ();
  return convertExpressionToCvc4Expr (symbols, --top);
}

Expr ConstraintToCvc4ExprConverter::convertExpressionToCvc4Expr (
    std::map<std::string, Expr> &symbols,
    std::list < edu::sharif::twinner::trace::ExpressionToken * >::const_iterator &top) {
  const edu::sharif::twinner::trace::ExpressionToken *token = *top;
  const edu::sharif::twinner::trace::Operator *op =
      dynamic_cast<const edu::sharif::twinner::trace::Operator *> (token);
  if (op) {
    switch (op->getType ()) {
    case edu::sharif::twinner::trace::Operator::Unary:
    {
      Expr operand = convertExpressionToCvc4Expr (symbols, --top);
      return em.mkExpr
          (convertOperatorIdentifierToCvc4Kind (op->getIdentifier ()), operand);
    }
    case edu::sharif::twinner::trace::Operator::Binary:
    case edu::sharif::twinner::trace::Operator::FunctionalBinary:
    {
      Expr rightOperand = convertExpressionToCvc4Expr (symbols, --top);
      Expr leftOperand = convertExpressionToCvc4Expr (symbols, --top);
      return em.mkExpr (convertOperatorIdentifierToCvc4Kind (op->getIdentifier ()),
                        leftOperand, rightOperand);
    }
    default:
      throw std::runtime_error ("Unknown operator type");
    }

  } else if (dynamic_cast<const edu::sharif::twinner::trace::Symbol *> (token)) {
    const edu::sharif::twinner::trace::Symbol *symbolToken =
        static_cast<const edu::sharif::twinner::trace::Symbol *> (token);
    std::string name = symbolToken->toString ();
    std::map<std::string, Expr>::const_iterator it = symbols.find (name);
    if (it != symbols.end ()) {
      return it->second;
    } else {
      Expr sym = em.mkVar (name, type);
      if (symbolToken->getValue ().getSize () == 64) {
        addConstraint (em.mkExpr (kind::BITVECTOR_ULT, sym, max64));
      }
      symbols.insert (std::make_pair (name, sym));
      return sym;
    }

  } else if (dynamic_cast<const edu::sharif::twinner::trace::Constant *> (token)) {
    const edu::sharif::twinner::trace::Constant *constantToken =
        static_cast<const edu::sharif::twinner::trace::Constant *> (token);
    const edu::sharif::twinner::trace::ConcreteValue &value =
        constantToken->getValue ();
    std::string valstr;
    {
      std::stringstream ss;
      ss << value;
      ss.ignore (2);
      ss >> valstr;
    }
    std::map<std::string, Expr>::const_iterator it = constants.find (valstr);
    if (it != constants.end ()) {
      return it->second;
    } else {
      edu::sharif::twinner::util::Logger logger =
          edu::sharif::twinner::util::Logger::loquacious ();
      logger << "Creating CVC4 Integer from hex string: " << valstr;
      Expr cc = em.mkConst (BitVector (128, Integer (valstr, 16)));
      logger << " ... done.\n";
      constants.insert (std::make_pair (valstr, cc));
      return cc;
    }

  } else {
    throw std::runtime_error ("Missing expression token");
  }
}

Kind ConstraintToCvc4ExprConverter::convertOperatorIdentifierToCvc4Kind (
    edu::sharif::twinner::trace::Operator::OperatorIdentifier oi) {
  switch (oi) {
  case edu::sharif::twinner::trace::Operator::ADD:
    return kind::BITVECTOR_PLUS;
  case edu::sharif::twinner::trace::Operator::MINUS:
    return kind::BITVECTOR_SUB;
  case edu::sharif::twinner::trace::Operator::MULTIPLY:
    return kind::BITVECTOR_MULT;
  case edu::sharif::twinner::trace::Operator::DIVIDE:
    return kind::BITVECTOR_UDIV;
  case edu::sharif::twinner::trace::Operator::REMAINDER:
    return kind::BITVECTOR_UREM;
  case edu::sharif::twinner::trace::Operator::NEGATE:
    return kind::BITVECTOR_NEG;
  case edu::sharif::twinner::trace::Operator::XOR:
    return kind::BITVECTOR_XOR;
  case edu::sharif::twinner::trace::Operator::BITWISE_AND:
    return kind::BITVECTOR_AND;
  case edu::sharif::twinner::trace::Operator::BITWISE_OR:
    return kind::BITVECTOR_OR;
  case edu::sharif::twinner::trace::Operator::SHIFT_LEFT:
    return kind::BITVECTOR_SHL;
  case edu::sharif::twinner::trace::Operator::SHIFT_RIGHT:
    return kind::BITVECTOR_LSHR;
  case edu::sharif::twinner::trace::Operator::ARITHMETIC_SHIFT_RIGHT:
    return kind::BITVECTOR_ASHR;
  case edu::sharif::twinner::trace::Operator::ROTATE_RIGHT:
    return kind::BITVECTOR_ROTATE_RIGHT;
  default:
    throw std::runtime_error ("Unknown Operator Identifier");
  }
}

}
}
}
}
}
