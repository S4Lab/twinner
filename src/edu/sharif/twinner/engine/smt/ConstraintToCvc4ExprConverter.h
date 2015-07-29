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

#ifndef CONSTRAINT_TO_CVC4_EXPR_CONVERTER_H
#define CONSTRAINT_TO_CVC4_EXPR_CONVERTER_H

// MUST NOT BE INCLUDED FROM ANYWHERE ELSE OF CVC4 specific .cpp files.

#include "Cvc4Header.h"

#include <list>

#include "edu/sharif/twinner/trace/exptoken/Operator.h"
#include "edu/sharif/twinner/trace/Constraint.h"

#include "pin.H"

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

  bool limitSymbols;

  UINT64 bitLength;

public:
  ConstraintToCvc4ExprConverter (ExprManager &_em, bool _limitSymbols,
      const std::list < const edu::sharif::twinner::trace::Constraint * > &_constraints);

  /**
   * Converts given constraints to CVC4 Expr instances, takes care of symbols by placing
   * them in the given map to report them as name -> Expr mapping. Also same symbol is
   * mapped to same Expr. And for 64-bits symbols, a 128-bits Expr will be created which
   * is afterwards limited to 64-bits by adding a constraint saying exp < 2^64 and so
   * becomes a 64-bits Expr in practice. This behavior may be changed whenever multiple
   * bitvector lengths were supported by this converter.
   *
   * @param symbols The output map of symbol name to its Expr instance.
   * @return An Expr representing all given constraints (specified by the constructor).
   */
  Expr convert (std::map<std::string, Expr> &symbols);

  /**
   * Converts back CVC4 Expr to Twinner's format constraint.
   *
   * @param exp The CVC4 Expr expression to converted back to Twinner's format.
   * @param vals Concrete values of the variables which are used in the exp.
   * @return The Twinner's format constraint of the given expression.
   */
  std::list < const edu::sharif::twinner::trace::Constraint * > convertBack (Expr exp,
      const std::map<std::string, const edu::sharif::twinner::trace::cv::ConcreteValue *> &vals);

private:
  void addConstraint (Expr constraint);

  template <typename Combiner>
  edu::sharif::twinner::trace::Expression *convertByFoldingList (Expr &exp,
      const std::map<std::string, const edu::sharif::twinner::trace::cv::ConcreteValue *> &vals,
      Combiner &combiner, const char *name);
  template <typename Combiner, typename BitLengthTracker>
  edu::sharif::twinner::trace::Expression *convertByFoldingList (Expr &exp,
      const std::map<std::string, const edu::sharif::twinner::trace::cv::ConcreteValue *> &vals,
      Combiner &combiner, BitLengthTracker &tracker, const char *name);

  Expr convertConstraintToCvc4Expr (std::map<std::string, Expr> &symbols,
      const edu::sharif::twinner::trace::Constraint *constraint);
  edu::sharif::twinner::trace::Constraint *convertCvc4ExprToConstraint (Expr &exp,
      const std::map<std::string, const edu::sharif::twinner::trace::cv::ConcreteValue *> &vals);
  edu::sharif::twinner::trace::Constraint *convertCvc4ExprToConstraint (
      edu::sharif::twinner::trace::Constraint::ComparisonType type,
      Expr &mainExp,
      const std::map<std::string, const edu::sharif::twinner::trace::cv::ConcreteValue *> &vals);
  edu::sharif::twinner::trace::Constraint *convertCvc4ExprToConstraint (
      edu::sharif::twinner::trace::Constraint::ComparisonType type,
      Expr &mainExp, Expr &auxExp,
      const std::map<std::string, const edu::sharif::twinner::trace::cv::ConcreteValue *> &vals);

  edu::sharif::twinner::trace::Constraint::ComparisonType
  exprVsZeroKindToComparisonType (Kind kind) const;
  edu::sharif::twinner::trace::Constraint::ComparisonType
  exprVsZeroKindToReversedComparisonType (Kind kind) const;
  edu::sharif::twinner::trace::Constraint::ComparisonType
  exprVsExprKindToComparisonType (Kind kind) const;

  Expr convertExpressionToCvc4Expr (std::map<std::string, Expr> &symbols,
      const edu::sharif::twinner::trace::Expression *exp);
  Expr convertExpressionToCvc4Expr (std::map<std::string, Expr> &symbols,
      std::list < edu::sharif::twinner::trace::exptoken::ExpressionToken * >::const_iterator &top);
  edu::sharif::twinner::trace::Expression *convertCvc4ExprToExpression (Expr &exp,
      const std::map<std::string, const edu::sharif::twinner::trace::cv::ConcreteValue *> &vals);

  UINT64 extractConstantUint64 (
      std::list < edu::sharif::twinner::trace::exptoken::ExpressionToken * >::const_iterator &top);
  Expr signExtendCvc4Expr (Expr &operand, UINT64 source, UINT64 target);

  Kind convertOperatorIdentifierToCvc4Kind (
      edu::sharif::twinner::trace::exptoken::Operator::OperatorIdentifier oi);
};

}
}
}
}
}

#endif	/* ConstraintToCvc4ExprConverter.h */
