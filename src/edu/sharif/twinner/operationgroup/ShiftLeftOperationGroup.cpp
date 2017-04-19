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

#include "ShiftLeftOperationGroup.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace operationgroup {

ShiftLeftOperationGroup::ShiftLeftOperationGroup (
    ConstExpressionPtr mainExp, ConstExpressionPtr auxExp) :
    NaryOperationGroup (mainExp, auxExp) {
}

OperationGroup::ExpressionPtr
ShiftLeftOperationGroup::getCarryExpression () const {
  const int size = exp[0]->getLastConcreteValue ().getSize ();
  ExpressionPtr dstexp = exp[0]->clone ();
  ExpressionPtr shiftBits = exp[1]->clone ();
  shiftBits->minus (1);
  dstexp->shiftToLeft (shiftBits);
  delete shiftBits;
  dstexp->shiftToRight (size - 1);
  dstexp->bitwiseAnd (0x1);
  return dstexp;
}

std::list <OperationGroup::ConstraintPtr>
ShiftLeftOperationGroup::instantiateConstraintForOverflowCase (
    bool &overflow, uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "ShiftLeftOperationGroup::instantiateConstraintForOverflowCase"
      " (...): Not yet implemented\n";
  abort ();
}

std::list <OperationGroup::ConstraintPtr>
ShiftLeftOperationGroup::instantiateConstraintForZeroCase (bool &zero,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  const int size = exp[0]->getLastConcreteValue ().getSize ();
  ExpressionPtr dstexp = exp[0]->clone ();
  dstexp->shiftToLeft (exp[1]);
  dstexp->truncate (size);
  list.push_back (OperationGroup::Constraint::instantiateEqualConstraint
                  (zero, dstexp, instruction));
  delete dstexp;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
ShiftLeftOperationGroup::instantiateConstraintForLessCase (bool &less,
    uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "ShiftLeftOperationGroup"
      "::instantiateConstraintForLessCase (...): Not implemented yet.\n";
  abort ();
}

std::list <OperationGroup::ConstraintPtr>
ShiftLeftOperationGroup::instantiateConstraintForLessOrEqualCase (
    bool &lessOrEqual, uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "ShiftLeftOperationGroup"
      "::instantiateConstraintForLessOrEqualCase (...): Not implemented yet.\n";
  abort ();
}

std::list <OperationGroup::ConstraintPtr>
ShiftLeftOperationGroup::instantiateConstraintForBelowCase (bool &below,
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
ShiftLeftOperationGroup::instantiateConstraintForBelowOrEqualCase (
    bool &belowOrEqual, uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list =
      instantiateConstraintForZeroCase (belowOrEqual, instruction);
  if (belowOrEqual) {
    return list;
  }
  std::list <OperationGroup::ConstraintPtr> tmp =
      instantiateConstraintForBelowCase (belowOrEqual, instruction);
  list.insert (list.end (), tmp.begin (), tmp.end ());
  return list;
}

std::list <OperationGroup::ConstraintPtr>
ShiftLeftOperationGroup::operationResultIsLessOrEqualWithZero (
    bool &lessOrEqual, uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  edu::sharif::twinner::trace::Expression *dstexp = exp[0]->clone ();
  dstexp->shiftToLeft (exp[1]);
  list.push_back (OperationGroup::Constraint::instantiateLessOrEqualConstraint
                  (lessOrEqual, dstexp, instruction));
  delete dstexp;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
ShiftLeftOperationGroup::operationResultIsLessThanZero (bool &less,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  edu::sharif::twinner::trace::Expression *dstexp = exp[0]->clone ();
  dstexp->shiftToLeft (exp[1]);
  list.push_back (OperationGroup::Constraint::instantiateLessConstraint
                  (less, dstexp, instruction));
  delete dstexp;
  return list;
}

OperationGroup::ExpressionPtr ShiftLeftOperationGroup::getOperationResult () const {
  edu::sharif::twinner::trace::Expression *dstexp = exp[0]->clone ();
  dstexp->shiftToLeft (exp[1]);
  return dstexp;
}

}
}
}
}
