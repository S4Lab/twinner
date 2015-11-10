//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2015  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "ConstraintToCvc4ExprConverter.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue128Bits.h"
#include "edu/sharif/twinner/trace/ExpressionImp.h"
#include "edu/sharif/twinner/trace/exptoken/NamedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/MemoryEmergedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/RegisterEmergedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/Constant.h"
#include "edu/sharif/twinner/trace/exptoken/Symbol.h"

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
  maxLimits.insert
      (make_pair (64, em.mkConst (BitVector (128, UINT64 (0)).setBit (64))));
  maxLimits.insert
      (make_pair (32, em.mkConst (BitVector (128, UINT64 (0)).setBit (32))));
  maxLimits.insert
      (make_pair (16, em.mkConst (BitVector (128, UINT64 (0)).setBit (16))));
  maxLimits.insert
      (make_pair (8, em.mkConst (BitVector (128, UINT64 (0)).setBit (8))));

  constants.insert (std::make_pair ("00000000000000000", zero));
  constants.insert (std::make_pair ("10000000000000000", maxLimits.at (64)));
  constants.insert (std::make_pair ("100000000", maxLimits.at (32)));
  constants.insert (std::make_pair ("10000", maxLimits.at (16)));
  constants.insert (std::make_pair ("100", maxLimits.at (8)));
}

Expr ConstraintToCvc4ExprConverter::convert (std::map<std::string, Expr> &symbols) {
  for (std::list < const edu::sharif::twinner::trace::Constraint * >::const_iterator it =
      constraints.begin (); it != constraints.end (); ++it) {
    addConstraint (convertConstraintToCvc4Expr (symbols, *it));
  }
  return totalConstraint;
}

std::list < const edu::sharif::twinner::trace::Constraint * >
ConstraintToCvc4ExprConverter::convertBack (Expr exp,
    const std::map<std::string, const edu::sharif::twinner::trace::cv::ConcreteValue *> &vals) {
  std::list < const edu::sharif::twinner::trace::Constraint * > lst;
  if (exp.getKind () == kind::AND) {
    for (Expr::const_iterator it = exp.begin (); it != exp.end (); ++it) {
      Expr child = *it;
      std::list < const edu::sharif::twinner::trace::Constraint * > constraints =
          convertBack (child, vals);
      lst.insert (lst.end (), constraints.begin (), constraints.end ());
    }
  } else if (exp.getKind () == kind::CONST_BOOLEAN) {
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
    lst.push_back (convertCvc4ExprToConstraint (exp, vals));
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
ConstraintToCvc4ExprConverter::convertCvc4ExprToConstraint (Expr &exp,
    const std::map<std::string, const edu::sharif::twinner::trace::cv::ConcreteValue *> &vals) {
  if (exp.getNumChildren () == 1 && exp.getKind () == kind::NOT) {
    Expr con = *exp.begin ();
    edu::sharif::twinner::trace::Constraint *constraint =
        convertCvc4ExprToConstraint (con, vals);
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
        (exprVsZeroKindToComparisonType (exp.getKind ()), mainExp, vals);
  } else if (mainExp == zero) {
    return convertCvc4ExprToConstraint
        (exprVsZeroKindToReversedComparisonType (exp.getKind ()), auxExp, vals);
  } else {
    return convertCvc4ExprToConstraint
        (exprVsExprKindToComparisonType (exp.getKind ()), mainExp, auxExp, vals);
  }
}

edu::sharif::twinner::trace::Constraint *
ConstraintToCvc4ExprConverter::convertCvc4ExprToConstraint (
    edu::sharif::twinner::trace::Constraint::ComparisonType type,
    Expr &mainExp,
    const std::map<std::string, const edu::sharif::twinner::trace::cv::ConcreteValue *> &vals) {
  edu::sharif::twinner::trace::Expression *exp = convertCvc4ExprToExpression (mainExp, vals);
  edu::sharif::twinner::trace::Constraint *constraint =
      new edu::sharif::twinner::trace::Constraint (exp, type, 0, false);
  delete exp;
  return constraint; // mainExp <op-type> zero
}

edu::sharif::twinner::trace::Constraint *
ConstraintToCvc4ExprConverter::convertCvc4ExprToConstraint (
    edu::sharif::twinner::trace::Constraint::ComparisonType type,
    Expr &mainExp, Expr &auxExp,
    const std::map<std::string, const edu::sharif::twinner::trace::cv::ConcreteValue *> &vals) {
  edu::sharif::twinner::trace::Expression *exp1 = convertCvc4ExprToExpression (mainExp, vals);
  edu::sharif::twinner::trace::Expression *exp2 = convertCvc4ExprToExpression (auxExp, vals);
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
  const std::list < edu::sharif::twinner::trace::exptoken::ExpressionToken * > &stack =
      exp->getStack ();
  std::list < edu::sharif::twinner::trace::exptoken::ExpressionToken * >::const_iterator top =
      stack.end ();
  return convertExpressionToCvc4Expr (symbols, --top);
}

Expr ConstraintToCvc4ExprConverter::convertExpressionToCvc4Expr (
    std::map<std::string, Expr> &symbols,
    std::list < edu::sharif::twinner::trace::exptoken::ExpressionToken * >::const_iterator &top) {
  const edu::sharif::twinner::trace::exptoken::ExpressionToken *token = *top;
  const edu::sharif::twinner::trace::exptoken::Operator *op =
      dynamic_cast<const edu::sharif::twinner::trace::exptoken::Operator *> (token);
  if (op) {
    switch (op->getType ()) {
    case edu::sharif::twinner::trace::exptoken::Operator::SignExtension:
    {
      const UINT64 target = extractConstantUint64 (--top);
      const UINT64 source = extractConstantUint64 (--top);
      Expr operand = convertExpressionToCvc4Expr (symbols, --top);
      return signExtendCvc4Expr (operand, source, target);
    }
    case edu::sharif::twinner::trace::exptoken::Operator::Unary:
    {
      Expr operand = convertExpressionToCvc4Expr (symbols, --top);
      return em.mkExpr
          (convertOperatorIdentifierToCvc4Kind (op->getIdentifier ()), operand);
    }
    case edu::sharif::twinner::trace::exptoken::Operator::Binary:
    case edu::sharif::twinner::trace::exptoken::Operator::FunctionalBinary:
    {
      Expr rightOperand = convertExpressionToCvc4Expr (symbols, --top);
      Expr leftOperand = convertExpressionToCvc4Expr (symbols, --top);
      const edu::sharif::twinner::trace::exptoken::Operator::OperatorIdentifier opid =
          op->getIdentifier ();
      if (opid == edu::sharif::twinner::trace::exptoken::Operator::REMAINDER
          || opid == edu::sharif::twinner::trace::exptoken::Operator::DIVIDE) {
        // Adding (divisor!=zero) constraint
        addConstraint (em.mkExpr (kind::DISTINCT, rightOperand, zero));
      }
      return em.mkExpr (convertOperatorIdentifierToCvc4Kind (opid),
                        leftOperand, rightOperand);
    }
    default:
      throw std::runtime_error ("Unknown operator type");
    }

  } else {
    const edu::sharif::twinner::trace::exptoken::Operand *operandToken =
        static_cast<const edu::sharif::twinner::trace::exptoken::Operand *> (token);
    if (!operandToken->isConstant ()) {
      const edu::sharif::twinner::trace::exptoken::Symbol *symbolToken =
          static_cast<const edu::sharif::twinner::trace::exptoken::Symbol *> (token);
      const std::string name = symbolToken->getTechnicalName ();
      std::map<std::string, Expr>::const_iterator it = symbols.find (name);
      if (it != symbols.end ()) {
        return it->second;
      } else {
        Expr sym = em.mkVar (name, type);
        if (limitSymbols) {
          const int size = symbolToken->getValue ().getSize ();
          if (size < 128) {
            addConstraint
                (em.mkExpr (kind::BITVECTOR_ULT, sym, maxLimits.at (size)));
          }
        }
        symbols.insert (std::make_pair (name, sym));
        return sym;
      }

    } else {
      const edu::sharif::twinner::trace::cv::ConcreteValue &value =
          operandToken->getValue ();
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
    }
  }
}

namespace {

struct NoOpTracker {

  void operator<< (UINT64 bitLength) const {
  }

  void operator>> (UINT64 bitLength) const {
  }
};

void bitvectorPlusConverter (
    edu::sharif::twinner::trace::Expression *exp,
    const edu::sharif::twinner::trace::Expression *operand) {
  exp->add (operand);
}

void bitvectorOrConverter (
    edu::sharif::twinner::trace::Expression *exp,
    const edu::sharif::twinner::trace::Expression *operand) {
  exp->bitwiseOr (operand);
}

void bitvectorAndConverter (
    edu::sharif::twinner::trace::Expression *exp,
    const edu::sharif::twinner::trace::Expression *operand) {
  exp->bitwiseAnd (operand);
}

void bitvectorXorConverter (
    edu::sharif::twinner::trace::Expression *exp,
    const edu::sharif::twinner::trace::Expression *operand) {
  exp->bitwiseXor (operand);
}

void bitvectorMultConverter (
    edu::sharif::twinner::trace::Expression *exp,
    const edu::sharif::twinner::trace::Expression *operand) {
  exp->multiply (operand);
}

class BitvectorConcatConverter {
private:
  int totalLength;
  UINT64 bitLength;

public:

  BitvectorConcatConverter () : totalLength (0) {
  }

  void operator<< (UINT64 _bitLength) {
    bitLength = _bitLength;
    totalLength += _bitLength;
  }

  void operator() (edu::sharif::twinner::trace::Expression *&exp,
                   edu::sharif::twinner::trace::Expression *&operand) const {
    if (!exp->isTrivial () || !exp->getLastConcreteValue ().isZero ()) {
      edu::sharif::twinner::trace::Expression *length =
          new edu::sharif::twinner::trace::ExpressionImp (bitLength);
      exp->shiftToLeft (length);
      delete length;
      if (!operand->isTrivial () || !operand->getLastConcreteValue ().isZero ()) {
        exp->bitwiseOr (operand);
      }
    } else {
      std::swap (exp, operand);
    }
  }

  void operator>> (UINT64 &_bitLength) const {
    _bitLength = totalLength;
  }
};

}

template <typename Combiner>
edu::sharif::twinner::trace::Expression *
ConstraintToCvc4ExprConverter::convertByFoldingList (Expr &exp,
    const std::map<std::string, const edu::sharif::twinner::trace::cv::ConcreteValue *> &vals,
    Combiner &combiner, const char *name) {
  NoOpTracker tracker;
  return convertByFoldingList (exp, vals, combiner, tracker, name);
}

template <typename Combiner, typename BitLengthTracker>
edu::sharif::twinner::trace::Expression *
ConstraintToCvc4ExprConverter::convertByFoldingList (Expr &exp,
    const std::map<std::string, const edu::sharif::twinner::trace::cv::ConcreteValue *> &vals,
    Combiner &combiner, BitLengthTracker &tracker, const char *name) {
  const int numberOfChildren = exp.getNumChildren ();
  if (numberOfChildren < 2) {
    edu::sharif::twinner::util::Logger::error ()
        << "ConstraintToCvc4ExprConverter::convertCvc4ExprToExpression (" << exp
        << "): CVC4 " << name << " needs at least two children\n";
    throw std::runtime_error ("CVC4 Expr must have at least two children");
  }
  edu::sharif::twinner::trace::Expression *result = 0;
  for (Expr::const_iterator it = exp.begin (); it != exp.end (); ++it) {
    Expr child = *it;
    edu::sharif::twinner::trace::Expression *operand =
        convertCvc4ExprToExpression (child, vals);
    if (result == 0) {
      tracker << bitLength;
      result = operand;
    } else {
      tracker << bitLength;
      combiner (result, operand);
      delete operand;
    }
  }
  tracker >> bitLength;
  return result;
}

edu::sharif::twinner::trace::Expression *
ConstraintToCvc4ExprConverter::convertCvc4ExprToExpression (Expr &exp,
    const std::map<std::string, const edu::sharif::twinner::trace::cv::ConcreteValue *> &vals) {
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
        (new edu::sharif::twinner::trace::cv::ConcreteValue128Bits (high, low));
  } else if (exp.isVariable ()) {
    bitLength = 128;
    std::string variableName = exp.toString ();
    typedef std::map<std::string, const edu::sharif::twinner::trace::cv::ConcreteValue *> ValuesMap;
    typename ValuesMap::const_iterator it = vals.find (variableName);
    if (it == vals.end ()) {
      throw std::runtime_error ("convertCvc4ExprToExpression:"
                                " vals does not contain variable name");
    }
    const edu::sharif::twinner::trace::cv::ConcreteValue &val = *(it->second);
    if (variableName.at (0) == 'm') { // memory symbol
      return new edu::sharif::twinner::trace::ExpressionImp
          (edu::sharif::twinner::trace::exptoken::MemoryEmergedSymbol::fromNameAndValue
           (variableName, val));
    } else if (variableName.at (0) == 'n') { // named symbol
      return new edu::sharif::twinner::trace::ExpressionImp
          (edu::sharif::twinner::trace::exptoken::NamedSymbol::fromTechnicalName
           (variableName, val));
    } else { // register symbol
      return new edu::sharif::twinner::trace::ExpressionImp
          (edu::sharif::twinner::trace::exptoken::RegisterEmergedSymbol::fromNameAndValue
           (variableName, val));
    }
  } else switch (exp.getKind ()) {
    case kind::BITVECTOR_EXTRACT:
    {
      const Expr extractionInterval = exp.getOperator ();
      const int low = extractionInterval.getConst <BitVectorExtract> ().low;
      int high = extractionInterval.getConst <BitVectorExtract> ().high;
      Expr child = *exp.begin ();
      edu::sharif::twinner::trace::Expression *operand =
          convertCvc4ExprToExpression (child, vals);
      if (low > 0) {
        edu::sharif::twinner::trace::Expression *lowBits =
            new edu::sharif::twinner::trace::ExpressionImp (low);
        operand->shiftToRight (lowBits);
        delete lowBits;
        high -= low; // now we should extract [high,0]
      }
      bitLength = high + 1;
      operand->truncate (bitLength);
      return operand;
    }
    case kind::BITVECTOR_SIGN_EXTEND:
    {
      const Expr numberOfExtensionBits = exp.getOperator ();
      const int extensionAmount = numberOfExtensionBits.getConst <BitVectorSignExtend> ();
      Expr child = *exp.begin ();
      edu::sharif::twinner::trace::Expression *operand =
          convertCvc4ExprToExpression (child, vals);
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
      BitvectorConcatConverter converter;
      return convertByFoldingList
          (exp, vals, converter, converter, "BITVECTOR_CONCAT");
    }
    case kind::BITVECTOR_PLUS:
      return convertByFoldingList
          (exp, vals, bitvectorPlusConverter, "BITVECTOR_PLUS");
    case kind::BITVECTOR_NEG:
    {
      if (exp.getNumChildren () != 1) {
        edu::sharif::twinner::util::Logger::error ()
            << "ConstraintToCvc4ExprConverter::convertCvc4ExprToExpression (" << exp
            << "): CVC4 BITVECTOR_NEG needs exactly one child\n";
        throw std::runtime_error ("CVC4 Expr must have exactly one children");
      }
      Expr child = *exp.begin ();
      edu::sharif::twinner::trace::Expression *childExp =
          convertCvc4ExprToExpression (child, vals);
      edu::sharif::twinner::trace::Expression *resExp =
          childExp->twosComplement ();
      delete childExp;
      return resExp;
    }
    case kind::BITVECTOR_NOT:
    {
      if (exp.getNumChildren () != 1) {
        edu::sharif::twinner::util::Logger::error ()
            << "ConstraintToCvc4ExprConverter::convertCvc4ExprToExpression (" << exp
            << "): CVC4 BITVECTOR_NOT needs exactly one child\n";
        throw std::runtime_error ("CVC4 Expr must have exactly one children");
      }
      Expr child = *exp.begin ();
      edu::sharif::twinner::trace::Expression *resExp =
          convertCvc4ExprToExpression (child, vals);
      resExp->bitwiseNegate ();
      return resExp;
    }
    case kind::BITVECTOR_UREM_TOTAL:
    {
      if (exp.getNumChildren () != 2) {
        edu::sharif::twinner::util::Logger::error ()
            << "ConstraintToCvc4ExprConverter::convertCvc4ExprToExpression (" << exp
            << "): CVC4 BITVECTOR_UREM_TOTAL needs exactly two children\n";
        throw std::runtime_error ("CVC4 Expr must have exactly two children");
      }
      Expr::const_iterator it = exp.begin ();
      Expr left = *it++;
      Expr right = *it;
      edu::sharif::twinner::trace::Expression *leftExp =
          convertCvc4ExprToExpression (left, vals);
      edu::sharif::twinner::trace::Expression *rightExp =
          convertCvc4ExprToExpression (right, vals);
      leftExp->remainder (rightExp);
      delete rightExp;
      return leftExp;
    }
    case kind::BITVECTOR_OR:
      return convertByFoldingList
          (exp, vals, bitvectorOrConverter, "BITVECTOR_OR");
    case kind::BITVECTOR_AND:
      return convertByFoldingList
          (exp, vals, bitvectorAndConverter, "BITVECTOR_AND");
    case kind::BITVECTOR_LSHR:
    {
      if (exp.getNumChildren () != 2) {
        edu::sharif::twinner::util::Logger::error ()
            << "ConstraintToCvc4ExprConverter::convertCvc4ExprToExpression ("
            << exp << "): CVC4 BITVECTOR_LSHR needs exactly two children\n";
        throw std::runtime_error ("CVC4 Expr must have exactly two children");
      }
      Expr::const_iterator it = exp.begin ();
      Expr shifteeExpr = *it;
      ++it;
      Expr bitsExpr = *it;
      edu::sharif::twinner::trace::Expression *shiftee =
          convertCvc4ExprToExpression (shifteeExpr, vals);
      edu::sharif::twinner::trace::Expression *bits =
          convertCvc4ExprToExpression (bitsExpr, vals);
      shiftee->shiftToRight (bits);
      delete bits;
      return shiftee;
    }
    case kind::BITVECTOR_XOR:
      return convertByFoldingList
          (exp, vals, bitvectorXorConverter, "BITVECTOR_XOR");
    case kind::BITVECTOR_MULT:
      return convertByFoldingList
          (exp, vals, bitvectorMultConverter, "BITVECTOR_MULT");
      // TODO: Implement following operator types: MINUS, DIVIDE, SHIFT_LEFT, ARITHMETIC_SHIFT_RIGHT, ROTATE_RIGHT, ROTATE_LEFT
    default:
      edu::sharif::twinner::util::Logger::error ()
          << "ConstraintToCvc4ExprConverter::convertCvc4ExprToExpression (" << exp
          << "): Unexpected Expr Kind. It is " << exp.getKind () << '\n';
      throw std::runtime_error ("Unexpected Expr Kind");
    }
  return 0;
}

UINT64 ConstraintToCvc4ExprConverter::extractConstantUint64 (
    std::list < edu::sharif::twinner::trace::exptoken::ExpressionToken * >::const_iterator &top) {
  const edu::sharif::twinner::trace::exptoken::ExpressionToken *token = *top;
  if (dynamic_cast<const edu::sharif::twinner::trace::exptoken::Constant *> (token)) {
    const edu::sharif::twinner::trace::exptoken::Constant *constantToken =
        static_cast<const edu::sharif::twinner::trace::exptoken::Constant *> (token);
    const edu::sharif::twinner::trace::cv::ConcreteValue &value =
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
    edu::sharif::twinner::trace::exptoken::Operator::OperatorIdentifier oi) {
  switch (oi) {
  case edu::sharif::twinner::trace::exptoken::Operator::ADD:
    return kind::BITVECTOR_PLUS;
  case edu::sharif::twinner::trace::exptoken::Operator::MINUS:
    return kind::BITVECTOR_SUB;
  case edu::sharif::twinner::trace::exptoken::Operator::MULTIPLY:
    return kind::BITVECTOR_MULT;
  case edu::sharif::twinner::trace::exptoken::Operator::DIVIDE:
    return kind::BITVECTOR_UDIV;
  case edu::sharif::twinner::trace::exptoken::Operator::REMAINDER:
    return kind::BITVECTOR_UREM_TOTAL;
  case edu::sharif::twinner::trace::exptoken::Operator::BITWISE_NEGATE:
    return kind::BITVECTOR_NOT;
  case edu::sharif::twinner::trace::exptoken::Operator::XOR:
    return kind::BITVECTOR_XOR;
  case edu::sharif::twinner::trace::exptoken::Operator::BITWISE_AND:
    return kind::BITVECTOR_AND;
  case edu::sharif::twinner::trace::exptoken::Operator::BITWISE_OR:
    return kind::BITVECTOR_OR;
  case edu::sharif::twinner::trace::exptoken::Operator::SHIFT_LEFT:
    return kind::BITVECTOR_SHL;
  case edu::sharif::twinner::trace::exptoken::Operator::SHIFT_RIGHT:
    return kind::BITVECTOR_LSHR;
  case edu::sharif::twinner::trace::exptoken::Operator::ARITHMETIC_SHIFT_RIGHT:
    return kind::BITVECTOR_ASHR;
  case edu::sharif::twinner::trace::exptoken::Operator::ROTATE_RIGHT:
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
