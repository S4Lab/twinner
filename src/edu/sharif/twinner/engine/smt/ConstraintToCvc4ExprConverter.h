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

#ifndef CONSTRAINT_TO_CVC4_EXPR_CONVERTER_H
#define CONSTRAINT_TO_CVC4_EXPR_CONVERTER_H

// MUST NOT BE INCLUDED FROM ANYWHERE ELSE OF Cvc4SmtSolver.cpp and ConstraintToCvc4ExprConverter.cpp files.

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

using namespace CVC4;

#include <list>

#include "edu/sharif/twinner/trace/Operator.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Expression;
class ExpressionToken;
class Constraint;
}
namespace util {

class Logger;

const edu::sharif::twinner::util::Logger &operator<< (
    const edu::sharif::twinner::util::Logger &log, const Expr &exp);
const edu::sharif::twinner::util::Logger &operator<< (
    const edu::sharif::twinner::util::Logger &log, const Result &res);
}
namespace engine {
namespace smt {

class ConstraintToCvc4ExprConverter {

private:
  ExprManager &em;
  const std::list < const edu::sharif::twinner::trace::Constraint * > &constraints;

  Type type;

  std::map<std::string, Expr> constants;
  Expr zero;
  Expr max64;

  bool initialized;
  Expr totalConstraint;

public:
  ConstraintToCvc4ExprConverter (ExprManager &_em,
      const std::list < const edu::sharif::twinner::trace::Constraint * > &_constraints);

  Expr convert (std::map<std::string, Expr> &symbols);

private:
  void addConstraint (Expr constraint);

  Expr convertConstraintToCvc4Expr (std::map<std::string, Expr> &symbols,
      const edu::sharif::twinner::trace::Constraint *constraint);

  Expr convertExpressionToCvc4Expr (std::map<std::string, Expr> &symbols,
      const edu::sharif::twinner::trace::Expression *exp);
  Expr convertExpressionToCvc4Expr (std::map<std::string, Expr> &symbols,
      std::list < edu::sharif::twinner::trace::ExpressionToken * >::const_iterator &top);

  UINT64 extractConstantUint64 (
      std::list < edu::sharif::twinner::trace::ExpressionToken * >::const_iterator &top);
  Expr signExtendCvc4Expr (Expr &operand, UINT64 source, UINT64 target);

  Kind convertOperatorIdentifierToCvc4Kind (
      edu::sharif::twinner::trace::Operator::OperatorIdentifier oi);
};

}
}
}
}
}

#endif	/* ConstraintToCvc4ExprConverter.h */
