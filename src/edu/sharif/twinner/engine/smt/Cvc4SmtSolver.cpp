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

#include "Cvc4SmtSolver.h"

#include "ConstraintToCvc4ExprConverter.h"
#include "UnsatisfiableConstraintsException.h"

#include "edu/sharif/twinner/trace/MemoryEmergedSymbol.h"
#include "edu/sharif/twinner/trace/RegisterEmergedSymbol.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace smt {

void fillSatSolution (SmtEngine &smt, std::map<std::string, Expr> &symbols,
    std::set < const edu::sharif::twinner::trace::Symbol * > &satSolution);

Cvc4SmtSolver::Cvc4SmtSolver () :
    SmtSolver () {
}

Cvc4SmtSolver::~Cvc4SmtSolver () {
}

void Cvc4SmtSolver::solveConstraints (
    const std::list < const edu::sharif::twinner::trace::Constraint * > &constraints,
    std::set < const edu::sharif::twinner::trace::Symbol * > &satSolution) const
/* @throw (UnsatisfiableConstraintsException) */ {
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

  std::map<std::string, Expr> symbols;
  // FIXME: all operations that are supposed to overflow, must use explicit bitwise and
  Expr totalConstraint =
      ConstraintToCvc4ExprConverter (em, true, constraints).convert (symbols);
  try {
    edu::sharif::twinner::util::Logger::loquacious ()
        << "starting SmtEngine::checkSat(...)\n";
    //  << "starting SmtEngine::checkSat(" << totalConstraint << ")\n";
    Result res = smt.checkSat (totalConstraint);
    edu::sharif::twinner::util::Logger::loquacious ()
        << "checkSat(...) returned: " << res << '\n';
    if (res.isSat ()) {
      fillSatSolution (smt, symbols, satSolution);
      return;
    }
  } catch (const Exception &e) {
    edu::sharif::twinner::util::Logger::warning ()
        << "CVC4 throws an exception: " << e.what () << '\n';
  }
  edu::sharif::twinner::util::Logger::loquacious ()
      << "throwing UnsatisfiableConstraintsException...\n";
  throw UnsatisfiableConstraintsException ();
}

const edu::sharif::twinner::trace::Constraint *Cvc4SmtSolver::simplifyConstraint (
    const edu::sharif::twinner::trace::Constraint *constraint) const {
  return constraint;
}

void fillSatSolution (SmtEngine &smt, std::map<std::string, Expr> &symbols,
    std::set < const edu::sharif::twinner::trace::Symbol * > &satSolution) {
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
          (edu::sharif::twinner::trace::MemoryEmergedSymbol::fromNameAndValue
           (it->first, v4, v3, v2, v1));
    } else { // register symbol
      satSolution.insert
          (edu::sharif::twinner::trace::RegisterEmergedSymbol::fromNameAndValue
           (it->first, v4, v3, v2, v1));
    }
  }
  log << '\n';
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
