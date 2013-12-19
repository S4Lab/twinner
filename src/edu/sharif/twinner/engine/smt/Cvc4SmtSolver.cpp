//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "Cvc4SmtSolver.h"

#if 1 // inclusion of CVC4 header
/*
 * Of course, Twinner is not supposed to fix warnings of CVC4. Also it does not like
 * to disable -Werror. So we need to disable warnings in CVC4 temporarily. CVC4 is
 * using deprecated code which defining following macro, will prevent its related warning
 * to be raised while compiling following included header file.
 */
#define _BACKWARD_BACKWARD_WARNING_H 1
#include "inc/cvc4/cvc4.h"
#undef _BACKWARD_BACKWARD_WARNING_H
#endif // end of inclusion of CVC4 header

#include "edu/sharif/twinner/trace/Constant.h"
#include "edu/sharif/twinner/trace/Symbol.h"
#include "edu/sharif/twinner/trace/Operator.h"
#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/Constraint.h"

#include "edu/sharif/twinner/util/Logger.h"

using namespace CVC4;

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

inline const edu::sharif::twinner::util::Logger &operator<< (
    const edu::sharif::twinner::util::Logger &log, const Expr &exp);
inline const edu::sharif::twinner::util::Logger &operator<< (
    const edu::sharif::twinner::util::Logger &log, const Result &res);
}
namespace engine {
namespace smt {

Expr convertConstraintToCvc4Expr (ExprManager &em, Type &type,
    std::map<std::string, Expr> &symbols, std::map<UINT64, Expr> &constants,
    Expr &zero, const edu::sharif::twinner::trace::Constraint *constraint);

Expr convertExpressionToCvc4Expr (ExprManager &em, Type &type,
    std::map<std::string, Expr> &symbols, std::map<UINT64, Expr> &constants,
    const edu::sharif::twinner::trace::Expression *exp);
Expr convertExpressionToCvc4Expr (ExprManager &em, Type &type,
    std::map<std::string, Expr> &symbols, std::map<UINT64, Expr> &constants,
    std::list < edu::sharif::twinner::trace::ExpressionToken * >::const_iterator &top);

Kind convertOperatorIdentifierToCvc4Kind (
    edu::sharif::twinner::trace::Operator::OperatorIdentifier oi);

void fillSatSolution (SmtEngine &smt, std::map<std::string, Expr> &symbols,
    std::set < const edu::sharif::twinner::trace::MemoryEmergedSymbol * > &satSolution);

Cvc4SmtSolver::Cvc4SmtSolver () :
SmtSolver () {
}

Cvc4SmtSolver::~Cvc4SmtSolver () {
}

void Cvc4SmtSolver::solveConstraints (
    const std::list < const edu::sharif::twinner::trace::Constraint * > &constraints,
    std::set < const edu::sharif::twinner::trace::MemoryEmergedSymbol * > &satSolution)
throw (UnsatisfiableConstraintsException) {
  edu::sharif::twinner::util::Logger::loquacious ()
      << "Cvc4SmtSolver::solveConstraints (...)\n";
  ExprManager em;
  SmtEngine smt (&em);
  /*
   * QF_ means disable quantifiers
   * BV means enable bit-vectors
   */
  smt.setLogic ("QF_BV");
  smt.setOption ("produce-models", true);
  //  smt.setOption ("trace", "smt");

  Type bitvector64 = em.mkBitVectorType (64);
  std::map<std::string, Expr> symbols;
  std::map<UINT64, Expr> constants;
  Expr zero = em.mkConst (BitVector (64, UINT64 (0)));
  constants.insert (std::make_pair (0, zero));
  std::list < const edu::sharif::twinner::trace::Constraint * >::const_iterator it =
      constraints.begin ();
  Expr totalConstraint = convertConstraintToCvc4Expr (em, bitvector64,
                                                      symbols, constants, zero, *it);
  while (++it != constraints.end ()) {
    Expr exp = convertConstraintToCvc4Expr (em, bitvector64,
                                            symbols, constants, zero, *it);
    totalConstraint = totalConstraint.andExpr (exp);
  }
  try {
    edu::sharif::twinner::util::Logger::loquacious ()
        << "starting SmtEngine::checkSat(" << totalConstraint << ")\n";
    Result res = smt.checkSat (totalConstraint);
    edu::sharif::twinner::util::Logger::loquacious ()
        << "checkSat(...) returned: " << res << '\n';
    if (res.isSat ()) {
      fillSatSolution (smt, symbols, satSolution);
      return;
    }
  } catch (const Exception &e) {
    edu::sharif::twinner::util::Logger::warning () << e.what () << '\n';
  }
  edu::sharif::twinner::util::Logger::loquacious ()
      << "throwing UnsatisfiableConstraintsException...\n";
  throw UnsatisfiableConstraintsException ();
}

void fillSatSolution (SmtEngine &smt, std::map<std::string, Expr> &symbols,
    std::set < const edu::sharif::twinner::trace::MemoryEmergedSymbol * > &satSolution) {
  edu::sharif::twinner::util::Logger log =
      edu::sharif::twinner::util::Logger::loquacious ();
  log << "Constraints are satisfiable. Filling satSolution with memory symbols...";
  for (std::map<std::string, Expr>::const_iterator it = symbols.begin ();
      it != symbols.end (); ++it) {
    if (it->first.at (0) == 'm') { // memory symbol
      Expr exp = smt.getValue (it->second); // assigned value by SMT solver
      const BitVector &bv = exp.getConst< BitVector > ();
      const Integer &val = bv.getValue ();
      const UINT64 high = UINT32 (val.divByPow2 (32).getUnsignedLong ());
      const UINT64 low = UINT32 (val.modByPow2 (32).getUnsignedLong ());
      UINT64 value = (high << 32) | low;
      log << "\tsetting " << it->first << " -> 0x" << std::hex << value << "...";
      satSolution.insert
          (edu::sharif::twinner::trace::MemoryEmergedSymbol::fromNameAndValue
           (it->first, value));
    }
  }
  log << '\n';
}

Expr convertConstraintToCvc4Expr (ExprManager &em, Type &type,
    std::map<std::string, Expr> &symbols, std::map<UINT64, Expr> &constants,
    Expr &zero, const edu::sharif::twinner::trace::Constraint *constraint) {
  Expr exp = convertExpressionToCvc4Expr (em, type, symbols, constants,
                                          constraint->getExpression ());
  switch (constraint->getComparisonType ()) {
  case edu::sharif::twinner::trace::Constraint::NON_POSITIVE:
    return em.mkExpr (kind::BITVECTOR_SLE, exp, zero);
  case edu::sharif::twinner::trace::Constraint::NON_NEGATIVE:
    return em.mkExpr (kind::BITVECTOR_SGE, exp, zero);
  case edu::sharif::twinner::trace::Constraint::POSITIVE:
    return em.mkExpr (kind::BITVECTOR_SGT, exp, zero);
  case edu::sharif::twinner::trace::Constraint::NEGATIVE:
    return em.mkExpr (kind::BITVECTOR_SLT, exp, zero);
  case edu::sharif::twinner::trace::Constraint::ZERO:
    return em.mkExpr (kind::EQUAL, exp, zero);
  case edu::sharif::twinner::trace::Constraint::NON_ZERO:
    return em.mkExpr (kind::DISTINCT, exp, zero);
  default:
    throw std::runtime_error ("Unknown Comparison Type");
  }
}

Expr convertExpressionToCvc4Expr (ExprManager &em, Type &type,
    std::map<std::string, Expr> &symbols, std::map<UINT64, Expr> &constants,
    const edu::sharif::twinner::trace::Expression *exp) {
  const std::list < edu::sharif::twinner::trace::ExpressionToken * > &stack
      = exp->getStack ();
  std::list < edu::sharif::twinner::trace::ExpressionToken * >::const_iterator top
      = stack.end ();
  return convertExpressionToCvc4Expr (em, type, symbols, constants, --top);
}

Expr convertExpressionToCvc4Expr (ExprManager &em, Type &type,
    std::map<std::string, Expr> &symbols, std::map<UINT64, Expr> &constants,
    std::list < edu::sharif::twinner::trace::ExpressionToken * >::const_iterator &top) {
  const edu::sharif::twinner::trace::ExpressionToken *token = *top;
  const edu::sharif::twinner::trace::Operator *op =
      dynamic_cast<const edu::sharif::twinner::trace::Operator *> (token);
  if (op) {
    switch (op->getType ()) {
    case edu::sharif::twinner::trace::Operator::Unary:
    {
      Expr operand = convertExpressionToCvc4Expr (em, type, symbols, constants, --top);
      return em.mkExpr
          (convertOperatorIdentifierToCvc4Kind (op->getIdentifier ()), operand);
    }
    case edu::sharif::twinner::trace::Operator::Binary:
    {
      Expr rightOperand =
          convertExpressionToCvc4Expr (em, type, symbols, constants, --top);
      Expr leftOperand =
          convertExpressionToCvc4Expr (em, type, symbols, constants, --top);
      return em.mkExpr
          (convertOperatorIdentifierToCvc4Kind (op->getIdentifier ()),
           leftOperand, rightOperand);
    }
    default:
      throw std::runtime_error ("Unknown operator type");
    }

  } else if (dynamic_cast<const edu::sharif::twinner::trace::Symbol *> (token)) {
    std::string name =
        static_cast<const edu::sharif::twinner::trace::Symbol *> (token)->toString ();
    std::map<std::string, Expr>::const_iterator it = symbols.find (name);
    if (it != symbols.end ()) {
      return it->second;
    } else {
      Expr sym = em.mkVar (name, type);
      symbols.insert (std::make_pair (name, sym));
      return sym;
    }

  } else if (dynamic_cast<const edu::sharif::twinner::trace::Constant *> (token)) {
    UINT64 value =
        static_cast<const edu::sharif::twinner::trace::Constant *> (token)->getValue ();
    std::map<UINT64, Expr>::const_iterator it = constants.find (value);
    if (it != constants.end ()) {
      return it->second;
    } else {
      Expr cc = em.mkConst (BitVector (64, value));
      constants.insert (std::make_pair (value, cc));
      return cc;
    }

  } else {
    throw std::runtime_error ("Missing expression token");
  }
}

Kind convertOperatorIdentifierToCvc4Kind (
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
    return kind::BITVECTOR_ASHR;
  default:
    throw std::runtime_error ("Unknown Operator Identifier");
  }
}

}
}
namespace util {

const edu::sharif::twinner::util::Logger &operator<< (
    const edu::sharif::twinner::util::Logger &log, const Expr &exp) {
  return log.actualWrite (exp);
}

const edu::sharif::twinner::util::Logger &operator<< (
    const edu::sharif::twinner::util::Logger &log, const Result &res) {
  return log.actualWrite (res);
}

}
}
}
}
