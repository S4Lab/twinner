//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2016 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "AdditionOperationGroup.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {
namespace operationgroup {

AdditionOperationGroup::AdditionOperationGroup (ConstExpressionPtr mainExp,
    ConstExpressionPtr auxExp) :
    NaryOperationGroup (mainExp, auxExp) {
}

OperationGroup::ExpressionPtr AdditionOperationGroup::getCarryExpression () const {
  const int size = exp[0]->getLastConcreteValue ().getSize ();
  if (size > 64) {
    edu::sharif::twinner::util::Logger::error ()
        << "AdditionOperationGroup::getCarryExpression ()"
        " [size=" << std::dec << size << "]:"
        "Too large bit-length for expression\n";
    abort ();
  }
  OperationGroup::ExpressionPtr doublePrecision = exp[0]->clone (size * 2);
  doublePrecision->add (exp[1]);
  doublePrecision->shiftToRight (size);
  OperationGroup::ExpressionPtr truncexp = doublePrecision->clone (size);
  delete doublePrecision;
  truncexp->bitwiseAnd (0x1);
  return truncexp;
}

std::list <OperationGroup::ConstraintPtr>
AdditionOperationGroup::instantiateConstraintForOverflowCase (
    bool &overflow, uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "AdditionOperationGroup::instantiateConstraintForOverflowCase"
      " (...): Not yet implemented\n";
  abort ();
}

std::list <OperationGroup::ConstraintPtr>
AdditionOperationGroup::instantiateConstraintForZeroCase (bool &zero,
    uint32_t instruction) const {
  const int precision = exp[0]->getLastConcreteValue ().getSize ();
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr negativeOfRightExp =
      exp[1]->twosComplement (precision);
  list.push_back (OperationGroup::Constraint::instantiateEqualConstraint
                  (zero, exp[0], negativeOfRightExp, instruction));
  delete negativeOfRightExp;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
AdditionOperationGroup::instantiateConstraintForLessCase (bool &less,
    uint32_t instruction) const {
  const int precision = exp[0]->getLastConcreteValue ().getSize ();
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr negativeOfRightExp =
      exp[1]->twosComplement (precision);
  list.push_back (OperationGroup::Constraint::instantiateLessConstraint
                  (less, exp[0], negativeOfRightExp, instruction));
  delete negativeOfRightExp;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
AdditionOperationGroup::instantiateConstraintForLessOrEqualCase (bool &lessOrEqual,
    uint32_t instruction) const {
  const int precision = exp[0]->getLastConcreteValue ().getSize ();
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr negativeOfRightExp =
      exp[1]->twosComplement (precision);
  list.push_back (OperationGroup::Constraint::instantiateLessOrEqualConstraint
                  (lessOrEqual, exp[0], negativeOfRightExp, instruction));
  delete negativeOfRightExp;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
AdditionOperationGroup::instantiateConstraintForBelowCase (bool &below,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list =
      instantiateConstraintForBelowOrEqualCase (below, instruction);
  if (below) {
    bool zero;
    std::list <OperationGroup::ConstraintPtr> l2 =
        instantiateConstraintForZeroCase (zero, instruction);
    list.insert (list.end (), l2.begin (), l2.end ());
    below = below && !zero;
  }
  return list;
}

std::list <OperationGroup::ConstraintPtr>
AdditionOperationGroup::instantiateConstraintForBelowOrEqualCase (bool &belowOrEqual,
    uint32_t instruction) const {
  const int precision = exp[0]->getLastConcreteValue ().getSize ();
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr negativeOfRightExp =
      exp[1]->twosComplement (precision);
  list.push_back (OperationGroup::Constraint::instantiateBelowConstraint
                  (belowOrEqual, exp[0], negativeOfRightExp, instruction));
  delete negativeOfRightExp;
  belowOrEqual = !belowOrEqual;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
AdditionOperationGroup::operationResultIsLessOrEqualWithZero (bool &lessOrEqual,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr mainExp = exp[0]->clone ();
  mainExp->add (exp[1]);
  list.push_back (OperationGroup::Constraint::instantiateLessOrEqualConstraint
                  (lessOrEqual, mainExp, instruction));
  delete mainExp;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
AdditionOperationGroup::operationResultIsLessThanZero (bool &lessOrEqual,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr mainExp = exp[0]->clone ();
  mainExp->add (exp[1]);
  list.push_back (OperationGroup::Constraint::instantiateLessConstraint
                  (lessOrEqual, mainExp, instruction));
  delete mainExp;
  return list;
}

}
}
}
}
}
