//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2017 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "ShiftRightOperationGroup.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace operationgroup {

ShiftRightOperationGroup::ShiftRightOperationGroup (
    ConstExpressionPtr mainExp, ConstExpressionPtr auxExp) :
    NaryOperationGroup (mainExp, auxExp) {
}

OperationGroup::ExpressionPtr
ShiftRightOperationGroup::getCarryExpression () const {
  ExpressionPtr dstexp = exp[0]->clone ();
  ExpressionPtr shiftBits = exp[1]->clone ();
  shiftBits->minus (1);
  dstexp->shiftToRight (shiftBits);
  delete shiftBits;
  dstexp->bitwiseAnd (0x1);
  return dstexp;
}

std::list <OperationGroup::ConstraintPtr>
ShiftRightOperationGroup::instantiateConstraintForOverflowCase (
    bool &overflow, uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "ShiftRightOperationGroup::instantiateConstraintForOverflowCase"
      " (...): Not yet implemented\n";
  abort ();
}

std::list <OperationGroup::ConstraintPtr>
ShiftRightOperationGroup::instantiateConstraintForZeroCase (bool &zero,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  ExpressionPtr dstexp = exp[0]->clone ();
  dstexp->shiftToRight (exp[1]);
  list.push_back (OperationGroup::Constraint::instantiateEqualConstraint
                  (zero, dstexp, instruction));
  delete dstexp;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
ShiftRightOperationGroup::instantiateConstraintForLessCase (bool &less,
    uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "ShiftRightOperationGroup"
      "::instantiateConstraintForLessCase (...): Not implemented yet.\n";
  abort ();
}

std::list <OperationGroup::ConstraintPtr>
ShiftRightOperationGroup::instantiateConstraintForLessOrEqualCase (
    bool &lessOrEqual, uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "ShiftRightOperationGroup"
      "::instantiateConstraintForLessOrEqualCase (...): Not implemented yet.\n";
  abort ();
}

std::list <OperationGroup::ConstraintPtr>
ShiftRightOperationGroup::instantiateConstraintForBelowCase (bool &below,
    uint32_t instruction) const {
  ConstExpressionPtr carry = getCarryExpression ();
  std::list <OperationGroup::ConstraintPtr> list;
  list.push_back (OperationGroup::Constraint::instantiateEqualConstraint
                  (below, carry, instruction));
  delete carry;
  below = !below;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
ShiftRightOperationGroup::instantiateConstraintForBelowOrEqualCase (
    bool &belowOrEqual, uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "ShiftRightOperationGroup"
      "::instantiateConstraintForBelowOrEqualCase (...):"
      " Not implemented yet.\n";
  abort ();
}

std::list <OperationGroup::ConstraintPtr>
ShiftRightOperationGroup::operationResultIsLessOrEqualWithZero (
    bool &lessOrEqual, uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  edu::sharif::twinner::trace::Expression *dstexp = exp[0]->clone ();
  dstexp->shiftToRight (exp[1]);
  list.push_back (OperationGroup::Constraint::instantiateLessOrEqualConstraint
                  (lessOrEqual, dstexp, instruction));
  delete dstexp;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
ShiftRightOperationGroup::operationResultIsLessThanZero (bool &less,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  edu::sharif::twinner::trace::Expression *dstexp = exp[0]->clone ();
  dstexp->shiftToRight (exp[1]);
  list.push_back (OperationGroup::Constraint::instantiateLessConstraint
                  (less, dstexp, instruction));
  delete dstexp;
  return list;
}

OperationGroup::ExpressionPtr ShiftRightOperationGroup::getOperationResult () const {
  edu::sharif::twinner::trace::Expression *dstexp = exp[0]->clone ();
  dstexp->shiftToRight (exp[1]);
  return dstexp;
}

}
}
}
}
