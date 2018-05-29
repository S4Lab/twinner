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

#include "SubtractWithBorrowOperationGroup.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace operationgroup {

SubtractWithBorrowOperationGroup::SubtractWithBorrowOperationGroup (
    ConstExpressionPtr mainExp,
    ConstExpressionPtr auxExp, ConstExpressionPtr carryExp) :
    NaryOperationGroup (mainExp, auxExp, carryExp) {
}

OperationGroup::ExpressionPtr SubtractWithBorrowOperationGroup::getCarryExpression () const {
  const int size = exp[0]->getLastConcreteValue ().getSize ();
  if (size > 64) {
    edu::sharif::twinner::util::Logger::error ()
        << "SubtractWithBorrowOperationGroup::getCarryExpression ()"
        " [size=" << std::dec << size << "]:"
        " Too large bit-length for expression\n";
    abort ();
  }
  OperationGroup::ExpressionPtr doublePrecision = exp[0]->clone (size * 2);
  doublePrecision->minus (exp[1]);
  doublePrecision->minus (exp[2]);
  doublePrecision->shiftToRight (size);
  OperationGroup::ExpressionPtr truncexp = doublePrecision->clone (size);
  delete doublePrecision;
  truncexp->bitwiseAnd (0x1);
  return truncexp;
}

std::list <OperationGroup::ConstraintPtr>
SubtractWithBorrowOperationGroup::instantiateConstraintForOverflowCase (
    bool &overflow, uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "SubtractWithBorrowOperationGroup::instantiateConstraintForOverflowCase"
      " (...): Not yet implemented\n";
  abort ();
}

std::list <OperationGroup::ConstraintPtr>
SubtractWithBorrowOperationGroup::instantiateConstraintForZeroCase (bool &zero,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr rightExp = exp[1]->clone ();
  rightExp->add (exp[2]);
  list.push_back (OperationGroup::Constraint::instantiateEqualConstraint
                  (zero, exp[0], rightExp, instruction));
  delete rightExp;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
SubtractWithBorrowOperationGroup::instantiateConstraintForLessCase (bool &less,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr rightExp = exp[1]->clone ();
  rightExp->add (exp[2]);
  list.push_back (OperationGroup::Constraint::instantiateLessConstraint
                  (less, exp[0], rightExp, instruction));
  delete rightExp;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
SubtractWithBorrowOperationGroup::instantiateConstraintForLessOrEqualCase (bool &lessOrEqual,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr rightExp = exp[1]->clone ();
  rightExp->add (exp[2]);
  list.push_back (OperationGroup::Constraint::instantiateLessOrEqualConstraint
                  (lessOrEqual, exp[0], rightExp, instruction));
  delete rightExp;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
SubtractWithBorrowOperationGroup::instantiateConstraintForBelowCase (bool &below,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr rightExp = exp[1]->clone ();
  rightExp->add (exp[2]);
  list.push_back (OperationGroup::Constraint::instantiateBelowConstraint
                  (below, exp[0], rightExp, instruction));
  delete rightExp;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
SubtractWithBorrowOperationGroup::instantiateConstraintForBelowOrEqualCase (bool &belowOrEqual,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr rightExp = exp[1]->clone ();
  rightExp->add (exp[2]);
  list.push_back (OperationGroup::Constraint::instantiateBelowOrEqualConstraint
                  (belowOrEqual, exp[0], rightExp, instruction));
  delete rightExp;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
SubtractWithBorrowOperationGroup::operationResultIsLessOrEqualWithZero (bool &lessOrEqual,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr mainExp = getOperationResult ();
  list.push_back (OperationGroup::Constraint::instantiateLessOrEqualConstraint
                  (lessOrEqual, mainExp, instruction));
  delete mainExp;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
SubtractWithBorrowOperationGroup::operationResultIsLessThanZero (bool &less,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr mainExp = getOperationResult ();
  list.push_back (OperationGroup::Constraint::instantiateLessConstraint
                  (less, mainExp, instruction));
  delete mainExp;
  return list;
}

OperationGroup::ExpressionPtr SubtractWithBorrowOperationGroup::getOperationResult () const {
  OperationGroup::ExpressionPtr mainExp = exp[0]->clone ();
  mainExp->minus (exp[1]);
  mainExp->minus (exp[2]);
  return mainExp;
}

}
}
}
}
