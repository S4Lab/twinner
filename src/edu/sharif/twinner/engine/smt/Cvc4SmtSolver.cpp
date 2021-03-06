//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2018 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "edu/sharif/twinner/pin-wrapper.h"

#include "Cvc4SmtSolver.h"

#include "Cvc4SmtSolverState.h"
#include "ConstraintToCvc4ExprConverter.h"

#include "edu/sharif/twinner/trace/exptoken/MemoryEmergedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/NamedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/RegisterEmergedSymbol.h"
#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace smt {

void fillSatSolution (SmtEngine &smt, std::map<std::string, Expr> &symbols,
    std::set < const edu::sharif::twinner::trace::exptoken::Symbol * > &satSolution);

Cvc4SmtSolver::Cvc4SmtSolver (int _timeoutMilliseconds) :
    SmtSolver (),
    state (new Cvc4SmtSolverState ()),
    timeoutMilliseconds (_timeoutMilliseconds) {
}

Cvc4SmtSolver::~Cvc4SmtSolver () {
  delete state;
}

bool Cvc4SmtSolver::solveConstraints (
    const std::list < const edu::sharif::twinner::trace::Constraint * > &constraints,
    std::set < const edu::sharif::twinner::trace::exptoken::Symbol * > &satSolution) const {
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
  if (timeoutMilliseconds) {
    smt.setOption ("tlimit", new SExpr (timeoutMilliseconds));
  }
  //  smt.setOption ("trace", "smt");

  std::map<std::string, Expr> symbols;
  // FIXME: all operations that are supposed to overflow, must use explicit bitwise and
  bool ok;
  Expr totalConstraint = ConstraintToCvc4ExprConverter
      (em, true, constraints).convert (ok, symbols);
  if (!ok) {
    return false;
  }
  try {
    edu::sharif::twinner::util::Logger::loquacious ()
        << "starting SmtEngine::checkSat(...)\n";
    //  << "starting SmtEngine::checkSat(" << totalConstraint << ")\n";
    Result res = smt.checkSat (totalConstraint);
    edu::sharif::twinner::util::Logger::loquacious ()
        << "checkSat(...) returned: " << res << '\n';
    if (res.isSat ()) {
      fillSatSolution (smt, symbols, satSolution);
      return true;
    }
  } catch (const Exception &e) {
    edu::sharif::twinner::util::Logger::warning ()
        << "CVC4 throws an exception: " << e.what () << '\n';
  }
  edu::sharif::twinner::util::Logger::error ()
      << "Unsatisfiable Constraints ...\n";
  return false;
}

void aggregate_expression_symbol_values (
    std::map<std::string, const edu::sharif::twinner::trace::cv::ConcreteValue *> &vals,
    edu::sharif::twinner::trace::exptoken::ExpressionToken * const &token) {
  if (const edu::sharif::twinner::trace::exptoken::Symbol * symbol =
      dynamic_cast<edu::sharif::twinner::trace::exptoken::Symbol *> (token)) {
    vals.insert (make_pair (symbol->getTechnicalName (), &(symbol->getValue ())));
  }
}

void aggregate_symbol_values (
    std::map<std::string, const edu::sharif::twinner::trace::cv::ConcreteValue *> &vals,
    const edu::sharif::twinner::trace::Constraint * const &constraint) {
  edu::sharif::twinner::util::foreach
      (constraint->getMainExpression ()->getStack (),
       aggregate_expression_symbol_values, vals);
  if (constraint->getAuxExpression ()) {
    edu::sharif::twinner::util::foreach
        (constraint->getAuxExpression ()->getStack (),
         aggregate_expression_symbol_values, vals);
  }
}

std::list < const edu::sharif::twinner::trace::Constraint * >
Cvc4SmtSolver::simplifyConstraint (bool &ok,
    const edu::sharif::twinner::trace::Constraint *constraint) const {
  std::list < const edu::sharif::twinner::trace::Constraint * > constraints;
  constraints.push_back (constraint);
  return simplifyConstraints (ok, constraints);
}

std::list < const edu::sharif::twinner::trace::Constraint * >
Cvc4SmtSolver::simplifyConstraints (bool &ok,
    std::list < const edu::sharif::twinner::trace::Constraint * > constraints) const {
  ok = true;
  //  edu::sharif::twinner::util::Logger::loquacious ()
  //      << "Cvc4SmtSolver::simplifyConstraint (...)\n";
  ExprManager em;
  SmtEngine smt (&em);
  /*
   * QF_ means disable quantifiers
   * BV means enable bit-vectors
   */
  smt.setLogic ("QF_BV");
  smt.setOption ("produce-models", true);
  //  smt.setOption ("trace", "smt");

  if (constraints.empty ()) {
    std::list < const edu::sharif::twinner::trace::Constraint * > emptyList;
    return emptyList;
  }
  ConstraintToCvc4ExprConverter converter (em, false, constraints);
  std::map<std::string, Expr> symbols;
  Expr cvc4Constraint = converter.convert (ok, symbols);
  if (!ok) {
    std::list < const edu::sharif::twinner::trace::Constraint * > emptyList;
    return emptyList;
  }
  Expr simple = smt.simplify (cvc4Constraint);
  std::map<std::string, const edu::sharif::twinner::trace::cv::ConcreteValue *> vals;
  edu::sharif::twinner::util::foreach
      (constraints, aggregate_symbol_values, vals);
  return converter.convertBack (ok, simple, vals);
}

void Cvc4SmtSolver::clearState () {
  delete state;
  state = new Cvc4SmtSolverState ();
}

void Cvc4SmtSolver::assertConstraint (
    const edu::sharif::twinner::trace::Constraint *constraint) {
  std::list < const edu::sharif::twinner::trace::Constraint * > constraints;
  constraints.push_back (constraint);
  state->assertConstraints (constraints);
}

void Cvc4SmtSolver::popLastAssertion () {
  state->popLastAssertion ();
}

bool Cvc4SmtSolver::checkValidity (
    const edu::sharif::twinner::trace::Constraint *constraint) {
  std::list < const edu::sharif::twinner::trace::Constraint * > constraints;
  constraints.push_back (constraint);
  return state->checkValidity (constraints);
}

void fillSatSolution (SmtEngine &smt, std::map<std::string, Expr> &symbols,
    std::set < const edu::sharif::twinner::trace::exptoken::Symbol * > &satSolution) {
  edu::sharif::twinner::util::Logger log =
      edu::sharif::twinner::util::Logger::loquacious ();
  log << "Constraints are satisfiable. Filling satSolution with memory symbols...";
  for (std::map<std::string, Expr>::const_iterator it = symbols.begin ();
      it != symbols.end (); ++it) {
    Expr exp = smt.getValue (it->second); // assigned value by SMT solver
    const BitVector &bv = exp.getConst< BitVector > ();
    const Integer &val = bv.getValue ();
    const UINT32 v1 = UINT32 (val.modByPow2 (32).getUnsignedLong ()); // least-significant
    const UINT32 v2 = UINT32 (val.modByPow2 (64).divByPow2 (32).getUnsignedLong ());
    const UINT32 v3 = UINT32 (val.modByPow2 (96).divByPow2 (64).getUnsignedLong ());
    const UINT32 v4 = UINT32 (val.divByPow2 (96).getUnsignedLong ()); // most-significant
    std::stringstream ss;
    ss << "0x" << std::hex << std::setw (8) << std::setfill ('0') << v4
        << std::setw (8) << std::setfill ('0') << v3
        << std::setw (8) << std::setfill ('0') << v2
        << std::setw (8) << std::setfill ('0') << v1;
    const std::string valstr = ss.str ();
    log << "\tsetting " << it->first << " -> " << valstr << "...";
    if (it->first.at (0) == 'm') { // memory symbol
      satSolution.insert
          (edu::sharif::twinner::trace::exptoken::MemoryEmergedSymbol::fromNameAndValue
           (it->first, v4, v3, v2, v1));
    } else if (it->first.at (0) == 'n') { // named symbol
      satSolution.insert
          (edu::sharif::twinner::trace::exptoken::NamedSymbol::fromTechnicalName
           (it->first, v4, v3, v2, v1));
    } else { // register symbol
      satSolution.insert
          (edu::sharif::twinner::trace::exptoken::RegisterEmergedSymbol::fromNameAndValue
           (it->first, v4, v3, v2, v1));
    }
  }
  log << '\n';
}

}
}
namespace util {

const edu::sharif::twinner::util::Logger &
operator<< (const edu::sharif::twinner::util::Logger &log, const Expr &exp) {
  return log.actualWrite (exp);
}

const edu::sharif::twinner::util::Logger &
operator<< (const edu::sharif::twinner::util::Logger &log, const Result &res) {
  return log.actualWrite (res);
}

}
}
}
}
