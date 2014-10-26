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

#include "edu/sharif/twinner/trace/ConcreteValue128Bits.h"
#include "edu/sharif/twinner/trace/ExpressionImp.h"
#include "edu/sharif/twinner/trace/MemoryEmergedSymbol.h"
#include "edu/sharif/twinner/trace/RegisterEmergedSymbol.h"
#include "edu/sharif/twinner/trace/Constant.h"
#include "edu/sharif/twinner/trace/Symbol.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

inline const Logger &operator<< (const Logger &log, Kind kind) {
  return log.actualWrite (kind);
}

}
namespace engine {
namespace smt {

ConstraintToCvc4ExprConverter::ConstraintToCvc4ExprConverter (ExprManager &_em,
    bool _limitSymbols,
    const std::list < const edu::sharif::twinner::trace::Constraint * > &_constraints) :
    em (_em), constraints (_constraints), initialized (false),
    limitSymbols (_limitSymbols) {
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

std::list < const edu::sharif::twinner::trace::Constraint * >
ConstraintToCvc4ExprConverter::convertBack (
    Expr exp) {
  std::list < const edu::sharif::twinner::trace::Constraint * > lst;
  if (exp.getKind () == kind::CONST_BOOLEAN) {
    const bool state = exp.getConst <bool> ();
    const edu::sharif::twinner::trace::Expression *zero =
        new edu::sharif::twinner::trace::ExpressionImp ();
    edu::sharif::twinner::trace::Constraint *res =
        new edu::sharif::twinner::trace::Constraint
        (zero, state ? edu::sharif::twinner::trace::Constraint::ZERO
         : edu::sharif::twinner::trace::Constraint::NON_ZERO, 0, false);
    delete zero;
    lst.push_back (res);
  } else {
    lst.push_back (convertCvc4ExprToConstraint (exp));
  }
  return lst;
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

edu::sharif::twinner::trace::Constraint *
ConstraintToCvc4ExprConverter::convertCvc4ExprToConstraint (Expr &exp) {
  if (exp.getNumChildren () == 1 && exp.getKind () == kind::NOT) {
    Expr con = *exp.begin ();
    edu::sharif::twinner::trace::Constraint *constraint =
        convertCvc4ExprToConstraint (con);
    edu::sharif::twinner::trace::Constraint *notConstraint =
        constraint->instantiateNegatedConstraint ();
    delete constraint;
    return notConstraint;
  }
  if (exp.getNumChildren () != 2) {
    edu::sharif::twinner::util::Logger::error ()
        << "ConstraintToCvc4ExprConverter::convertCvc4ExprToConstraint (" << exp
        << "): CVC4 Expr must have exactly two children\n";
    throw std::runtime_error ("CVC4 Expr must have exactly two children");
  }
  Expr::const_iterator it = exp.begin ();
  Expr mainExp = *it++;
  Expr auxExp = *it;
  if (auxExp == zero) {
    return convertCvc4ExprToConstraint
        (exprVsZeroKindToComparisonType (exp.getKind ()), mainExp);
  } else if (mainExp == zero) {
    return convertCvc4ExprToConstraint
        (exprVsZeroKindToReversedComparisonType (exp.getKind ()), auxExp);
  } else {
    return convertCvc4ExprToConstraint
        (exprVsExprKindToComparisonType (exp.getKind ()), mainExp, auxExp);
  }
}

edu::sharif::twinner::trace::Constraint *
ConstraintToCvc4ExprConverter::convertCvc4ExprToConstraint (
    edu::sharif::twinner::trace::Constraint::ComparisonType type,
    Expr &mainExp) {
  edu::sharif::twinner::trace::Expression *exp = convertCvc4ExprToExpression (mainExp);
  edu::sharif::twinner::trace::Constraint *constraint =
      new edu::sharif::twinner::trace::Constraint (exp, type, 0, false);
  delete exp;
  return constraint; // mainExp <op-type> zero
}

edu::sharif::twinner::trace::Constraint *
ConstraintToCvc4ExprConverter::convertCvc4ExprToConstraint (
    edu::sharif::twinner::trace::Constraint::ComparisonType type,
    Expr &mainExp, Expr &auxExp) {
  edu::sharif::twinner::trace::Expression *exp1 = convertCvc4ExprToExpression (mainExp);
  edu::sharif::twinner::trace::Expression *exp2 = convertCvc4ExprToExpression (auxExp);
  edu::sharif::twinner::trace::Constraint *constraint =
      new edu::sharif::twinner::trace::Constraint (exp1, exp2, type, 0, false);
  delete exp1;
  delete exp2;
  return constraint; // mainExp <op-type> auxExp
}

edu::sharif::twinner::trace::Constraint::ComparisonType
ConstraintToCvc4ExprConverter::exprVsZeroKindToComparisonType (Kind kind) const {
  switch (kind) {
  case kind::BITVECTOR_SGE:
    return edu::sharif::twinner::trace::Constraint::NON_NEGATIVE;
  case kind::BITVECTOR_SGT:
    return edu::sharif::twinner::trace::Constraint::POSITIVE;
  case kind::BITVECTOR_SLE:
    return edu::sharif::twinner::trace::Constraint::NON_POSITIVE;
  case kind::BITVECTOR_SLT:
    return edu::sharif::twinner::trace::Constraint::NEGATIVE;
  case kind::DISTINCT:
    return edu::sharif::twinner::trace::Constraint::NON_ZERO;
  case kind::EQUAL:
    return edu::sharif::twinner::trace::Constraint::ZERO;
  default:
    edu::sharif::twinner::util::Logger::error () << "Got " << kind
        << " expression Kind unexpectedly.\n";
    throw std::runtime_error ("Unexpected CVC4 Expr Kind");
  }
}

edu::sharif::twinner::trace::Constraint::ComparisonType
ConstraintToCvc4ExprConverter::exprVsZeroKindToReversedComparisonType (Kind kind) const {
  switch (kind) {
  case kind::BITVECTOR_SGE:
    return edu::sharif::twinner::trace::Constraint::NEGATIVE;
  case kind::BITVECTOR_SGT:
    return edu::sharif::twinner::trace::Constraint::NON_POSITIVE;
  case kind::BITVECTOR_SLE:
    return edu::sharif::twinner::trace::Constraint::POSITIVE;
  case kind::BITVECTOR_SLT:
    return edu::sharif::twinner::trace::Constraint::NON_NEGATIVE;
  case kind::DISTINCT:
    return edu::sharif::twinner::trace::Constraint::ZERO;
  case kind::EQUAL:
    return edu::sharif::twinner::trace::Constraint::NON_ZERO;
  default:
    edu::sharif::twinner::util::Logger::error () << "Got " << kind
        << " expression Kind unexpectedly.\n";
    throw std::runtime_error ("Unexpected CVC4 Expr Kind");
  }
}

edu::sharif::twinner::trace::Constraint::ComparisonType
ConstraintToCvc4ExprConverter::exprVsExprKindToComparisonType (Kind kind) const {
  switch (kind) {
  case kind::BITVECTOR_UGE:
    return edu::sharif::twinner::trace::Constraint::ABOVE_OR_EQUAL;
  case kind::BITVECTOR_UGT:
    return edu::sharif::twinner::trace::Constraint::ABOVE;
  case kind::BITVECTOR_ULE:
    return edu::sharif::twinner::trace::Constraint::BELOW_OR_EQUAL;
  case kind::BITVECTOR_ULT:
    return edu::sharif::twinner::trace::Constraint::BELOW;
  case kind::BITVECTOR_SGE:
    return edu::sharif::twinner::trace::Constraint::GREATER_OR_EQUAL;
  case kind::BITVECTOR_SGT:
    return edu::sharif::twinner::trace::Constraint::GREATER;
  case kind::BITVECTOR_SLE:
    return edu::sharif::twinner::trace::Constraint::LESS_OR_EQUAL;
  case kind::BITVECTOR_SLT:
    return edu::sharif::twinner::trace::Constraint::LESS;
  case kind::DISTINCT:
    return edu::sharif::twinner::trace::Constraint::NON_EQUAL;
  case kind::EQUAL:
    return edu::sharif::twinner::trace::Constraint::EQUAL;
  default:
    edu::sharif::twinner::util::Logger::error () << "Got " << kind
        << " expression Kind unexpectedly.\n";
    throw std::runtime_error ("Unknown CVC4 Expr Kind");
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
    case edu::sharif::twinner::trace::Operator::SignExtension:
    {
      const UINT64 target = extractConstantUint64 (--top);
      const UINT64 source = extractConstantUint64 (--top);
      Expr operand = convertExpressionToCvc4Expr (symbols, --top);
      return signExtendCvc4Expr (operand, source, target);
    }
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
      const edu::sharif::twinner::trace::Operator::OperatorIdentifier opid =
          op->getIdentifier ();
      if (opid == edu::sharif::twinner::trace::Operator::REMAINDER
          || opid == edu::sharif::twinner::trace::Operator::DIVIDE) {
        // Adding (divisor!=zero) constraint
        addConstraint (em.mkExpr (kind::DISTINCT, rightOperand, zero));
      }
      return em.mkExpr (convertOperatorIdentifierToCvc4Kind (opid),
                        leftOperand, rightOperand);
    }
    default:
      throw std::runtime_error ("Unknown operator type");
    }

  } else if (dynamic_cast<const edu::sharif::twinner::trace::Symbol *> (token)) {
    const edu::sharif::twinner::trace::Symbol *symbolToken =
        static_cast<const edu::sharif::twinner::trace::Symbol *> (token);
    const std::string name = symbolToken->toString ();
    std::map<std::string, Expr>::const_iterator it = symbols.find (name);
    if (it != symbols.end ()) {
      return it->second;
    } else {
      Expr sym = em.mkVar (name, type);
      if (symbolToken->getValue ().getSize () == 64 && limitSymbols) {
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
    std::string valstr = value.toHexString ();
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

edu::sharif::twinner::trace::Expression *
ConstraintToCvc4ExprConverter::convertCvc4ExprToExpression (Expr &exp) {
  if (exp.isConst ()) {
    bitLength = BitVectorType (exp.getType (false)).getSize ();
    // as size of Constant is <= 128-bits, following code does not lose precision
    const Integer &val = exp.getConst <BitVector> ().getValue ();
    const UINT32 v1 = UINT32 (val.modByPow2 (32).getUnsignedLong ()); // least-significant
    const UINT32 v2 = UINT32 (val.modByPow2 (64).divByPow2 (32).getUnsignedLong ());
    const UINT32 v3 = UINT32 (val.modByPow2 (96).divByPow2 (64).getUnsignedLong ());
    const UINT32 v4 = UINT32 (val.divByPow2 (96).getUnsignedLong ()); // most-significant
    const UINT64 high = (UINT64 (v4) << 32) | v3;
    const UINT64 low = (UINT64 (v2) << 32) | v1;
    return new edu::sharif::twinner::trace::ExpressionImp
        (new edu::sharif::twinner::trace::ConcreteValue128Bits (high, low));
  } else if (exp.isVariable ()) {
    bitLength = 128;
    std::string variableName = exp.toString ();
    if (variableName.at (0) == 'm') { // memory symbol
      return new edu::sharif::twinner::trace::ExpressionImp
          (edu::sharif::twinner::trace::MemoryEmergedSymbol::fromNameAndValue
           (variableName, 0, 0, 0, 0)); // TODO: Handle value
    } else { // register symbol
      return new edu::sharif::twinner::trace::ExpressionImp
          (edu::sharif::twinner::trace::RegisterEmergedSymbol::fromNameAndValue
           (variableName, 0, 0, 0, 0));
    }
  } else switch (exp.getKind ()) {
    case kind::BITVECTOR_EXTRACT:
    {
      const Expr extractionInterval = exp.getOperator ();
      const int low = extractionInterval.getConst <BitVectorExtract> ().low;
      int high = extractionInterval.getConst <BitVectorExtract> ().high;
      Expr child = *exp.begin ();
      edu::sharif::twinner::trace::Expression *operand =
          convertCvc4ExprToExpression (child);
      if (low > 0) {
        edu::sharif::twinner::trace::Expression *lowBits =
            new edu::sharif::twinner::trace::ExpressionImp (low);
        operand->shiftToRight (lowBits);
        delete lowBits;
        high -= low; // now we should extract [high,0]
      }
      bitLength = high + 1;
      return operand;
    }
    case kind::BITVECTOR_SIGN_EXTEND:
    {
      const Expr numberOfExtensionBits = exp.getOperator ();
      const int extensionAmount = numberOfExtensionBits.getConst <BitVectorSignExtend> ();
      Expr child = *exp.begin ();
      edu::sharif::twinner::trace::Expression *operand =
          convertCvc4ExprToExpression (child);
      const int sourceBits = bitLength;
      const int targetBits = sourceBits + extensionAmount;
      edu::sharif::twinner::trace::Expression *source = operand->clone (sourceBits);
      delete operand;
      edu::sharif::twinner::trace::Expression *target = source->signExtended (128);
      delete source;
      if (targetBits < 128) {
        target->truncate (targetBits);
      }
      bitLength = targetBits;
      return target;
    }
    case kind::BITVECTOR_CONCAT:
    {
      const int numberOfChildren = exp.getNumChildren ();
      if (numberOfChildren < 2) {
        edu::sharif::twinner::util::Logger::error ()
            << "ConstraintToCvc4ExprConverter::convertCvc4ExprToExpression (" << exp
            << "): CVC4 BITVECTOR_CONCAT needs at least two children\n";
        throw std::runtime_error ("CVC4 Expr must have at least two children");
      }
      Expr::const_iterator it = exp.begin ();
      Expr left = *it++;
      edu::sharif::twinner::trace::Expression *leftExp =
          convertCvc4ExprToExpression (left);
      int totalLength = bitLength;
      do {
        Expr right = *it++;
        edu::sharif::twinner::trace::Expression *rightExp =
            convertCvc4ExprToExpression (right);
        if (!leftExp->isTrivial () || !leftExp->getLastConcreteValue ().isZero ()) {
          edu::sharif::twinner::trace::Expression *length =
              new edu::sharif::twinner::trace::ExpressionImp (bitLength);
          leftExp->shiftToLeft (length);
          delete length;
          if (!rightExp->isTrivial () || !rightExp->getLastConcreteValue ().isZero ()) {
            leftExp->bitwiseOr (rightExp);
          }
          delete rightExp;
        } else {
          delete leftExp;
          leftExp = rightExp;
        }
        totalLength += bitLength;
      } while (it != exp.end ());
      bitLength = totalLength;
      return leftExp;
    }
    case kind::BITVECTOR_PLUS:
    {
      if (exp.getNumChildren () != 2) {
        edu::sharif::twinner::util::Logger::error ()
            << "ConstraintToCvc4ExprConverter::convertCvc4ExprToExpression (" << exp
            << "): CVC4 BITVECTOR_PLUS needs exactly two children\n";
        throw std::runtime_error ("CVC4 Expr must have exactly two children");
      }
      Expr::const_iterator it = exp.begin ();
      Expr left = *it++;
      Expr right = *it;
      edu::sharif::twinner::trace::Expression *leftExp =
          convertCvc4ExprToExpression (left);
      edu::sharif::twinner::trace::Expression *rightExp =
          convertCvc4ExprToExpression (right);
      leftExp->add (rightExp);
      delete rightExp;
      return leftExp;
    }
    case kind::BITVECTOR_NEG:
    {
      if (exp.getNumChildren () != 1) {
        edu::sharif::twinner::util::Logger::error ()
            << "ConstraintToCvc4ExprConverter::convertCvc4ExprToExpression (" << exp
            << "): CVC4 BITVECTOR_NEG needs exactly one child\n";
        throw std::runtime_error ("CVC4 Expr must have exactly one children");
      }
      Expr child = *exp.begin ();
      edu::sharif::twinner::trace::Expression *resExp =
          convertCvc4ExprToExpression (child);
      resExp->negate ();
      return resExp;
    }
      // TODO: Implement following operator types: MINUS, MULTIPLY, DIVIDE, REMAINDER, XOR, BITWISE_AND, BITWISE_OR, SHIFT_RIGHT, SHIFT_LEFT, ARITHMETIC_SHIFT_RIGHT, ROTATE_RIGHT, ROTATE_LEFT
    default:
      edu::sharif::twinner::util::Logger::error ()
          << "ConstraintToCvc4ExprConverter::convertCvc4ExprToExpression (" << exp
          << "): Unexpected Expr Kind. It is " << exp.getKind () << '\n';
      throw std::runtime_error ("Unexpected Expr Kind");
    }
  return 0;
}

UINT64 ConstraintToCvc4ExprConverter::extractConstantUint64 (
    std::list < edu::sharif::twinner::trace::ExpressionToken * >::const_iterator &top) {
  const edu::sharif::twinner::trace::ExpressionToken *token = *top;
  if (dynamic_cast<const edu::sharif::twinner::trace::Constant *> (token)) {
    const edu::sharif::twinner::trace::Constant *constantToken =
        static_cast<const edu::sharif::twinner::trace::Constant *> (token);
    const edu::sharif::twinner::trace::ConcreteValue &value =
        constantToken->getValue ();
    return value.toUint64 ();
  } else {
    throw std::runtime_error ("No constant UINT64 found");
  }
}

Expr ConstraintToCvc4ExprConverter::signExtendCvc4Expr (Expr &operand,
    UINT64 source, UINT64 target) {
  // TODO: Change BV-types instead of truncating expressions and eliminate below EXTRACT
  Expr srcExp = em.mkExpr (kind::BITVECTOR_EXTRACT,
                           em.mkConst (BitVectorExtract (source - 1, 0)),
                           operand);
  Expr singExtended = em.mkExpr (kind::BITVECTOR_SIGN_EXTEND,
                                 em.mkConst (BitVectorSignExtend (target - source)),
                                 srcExp);
  if (target == 128) {
    return singExtended;
  } else {
    return em.mkExpr (kind::BITVECTOR_ZERO_EXTEND,
                      em.mkConst (BitVectorZeroExtend (128 - target)),
                      singExtended);
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
