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

#include <stdexcept>
#include <list>

#include "OperationGroup.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/Expression.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

const OperationGroup *OperationGroup::SUB_OPGROUP = new SubtractOperationGroup ();
const OperationGroup *OperationGroup::ADD_OPGROUP = new AdditionOperationGroup ();
const OperationGroup *OperationGroup::ADD_WITH_CARRY_OPGROUP = NULL; // TODO: Implement
const OperationGroup *OperationGroup::SHIFT_LEFT_OPGROUP = NULL; // TODO: Implement
const OperationGroup *OperationGroup::SHIFT_RIGHT_OPGROUP = NULL; // TODO: Implement
const OperationGroup *OperationGroup::SHIFT_ARITHMETIC_RIGHT_OPGROUP =
    NULL; // TODO: Implement
const OperationGroup *OperationGroup::AND_OPGROUP = new BitwiseAndOperationGroup ();
const OperationGroup *OperationGroup::INCREMENT_OPGROUP = NULL; // TODO: Implement
const OperationGroup *OperationGroup::DECREMENT_OPGROUP = NULL; // TODO: Implement

OperationGroup::OperationGroup () {
}

OperationGroup::~OperationGroup () {
}

SubtractOperationGroup::SubtractOperationGroup () {
}

std::list <edu::sharif::twinner::trace::Constraint *>
SubtractOperationGroup::instantiateConstraintForZeroCase (bool &zero,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  if (!auxExp) {
    edu::sharif::twinner::util::Logger::error ()
        << "SubtractOperationGroup needs two expressions (auxExp is null)\n";
    throw std::runtime_error
        ("SubtractOperationGroup::instantiateConstraintForZeroCase (): auxExp is null");
  }
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  list.push_back (edu::sharif::twinner::trace::Constraint::instantiateEqualConstraint
                  (zero, mainExp, auxExp, instruction));
  return list;
}

std::list <edu::sharif::twinner::trace::Constraint *>
SubtractOperationGroup::instantiateConstraintForLessCase (bool &less,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  if (!auxExp) {
    edu::sharif::twinner::util::Logger::error ()
        << "SubtractOperationGroup needs two expressions (auxExp is null)\n";
    throw std::runtime_error
        ("SubtractOperationGroup::instantiateConstraintForLessCase (): auxExp is null");
  }
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  list.push_back (edu::sharif::twinner::trace::Constraint::instantiateLessConstraint
                  (less, mainExp, auxExp, instruction));
  return list;
}

std::list <edu::sharif::twinner::trace::Constraint *>
SubtractOperationGroup::instantiateConstraintForLessOrEqualCase (bool &lessOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  list.push_back (edu::sharif::twinner::trace::Constraint::instantiateLessOrEqualConstraint
                  (lessOrEqual, mainExp, auxExp, instruction));
  return list;
}

std::list <edu::sharif::twinner::trace::Constraint *>
SubtractOperationGroup::instantiateConstraintForBelowCase (bool &below,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  list.push_back (edu::sharif::twinner::trace::Constraint::instantiateBelowConstraint
                  (below, mainExp, auxExp, instruction));
  return list;
}

std::list <edu::sharif::twinner::trace::Constraint *>
SubtractOperationGroup::instantiateConstraintForBelowOrEqualCase (bool &belowOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  list.push_back
      (edu::sharif::twinner::trace::Constraint::instantiateBelowOrEqualConstraint
       (belowOrEqual, mainExp, auxExp, instruction));
  return list;
}

std::list <edu::sharif::twinner::trace::Constraint *>
SubtractOperationGroup::operationResultIsLessOrEqualWithZero (bool &lessOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  edu::sharif::twinner::trace::Expression *exp = mainExp->clone ();
  exp->minus (auxExp);
  list.push_back
      (edu::sharif::twinner::trace::Constraint::instantiateLessOrEqualConstraint
       (lessOrEqual, exp, instruction));
  delete exp;
  return list;
}

std::list <edu::sharif::twinner::trace::Constraint *>
SubtractOperationGroup::operationResultIsLessThanZero (bool &lessOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  edu::sharif::twinner::trace::Expression *exp = mainExp->clone ();
  exp->minus (auxExp);
  list.push_back
      (edu::sharif::twinner::trace::Constraint::instantiateLessConstraint
       (lessOrEqual, exp, instruction));
  delete exp;
  return list;
}

edu::sharif::twinner::trace::Expression *SubtractOperationGroup::getCarryExpression (
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp) const {
  const int size = mainExp->getLastConcreteValue ().getSize ();
  if (size > 64) {
    throw std::runtime_error ("SubtractOperationGroup::getCarryExpression (...):"
                              " Too large bit-length for expression");
  }
  edu::sharif::twinner::trace::Expression *doublePrecision = mainExp->clone (size * 2);
  doublePrecision->minus (auxExp);
  doublePrecision->shiftToRight (size);
  edu::sharif::twinner::trace::Expression *truncexp = doublePrecision->clone (size);
  delete doublePrecision;
  truncexp->bitwiseAnd (0x1);
  return truncexp;
}

AdditionOperationGroup::AdditionOperationGroup () {
}

std::list <edu::sharif::twinner::trace::Constraint *>
AdditionOperationGroup::instantiateConstraintForZeroCase (bool &zero,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  if (!auxExp) {
    edu::sharif::twinner::util::Logger::error ()
        << "AdditionOperationGroup needs two expressions (auxExp is null)\n";
    throw std::runtime_error
        ("AdditionOperationGroup::instantiateConstraintForZeroCase (): auxExp is null");
  }
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  edu::sharif::twinner::trace::Expression *negativeOfRightExp = auxExp->twosComplement ();
  list.push_back
      (edu::sharif::twinner::trace::Constraint::instantiateEqualConstraint
       (zero, mainExp, negativeOfRightExp, instruction));
  delete negativeOfRightExp;
  return list;
}

std::list <edu::sharif::twinner::trace::Constraint *>
AdditionOperationGroup::instantiateConstraintForLessCase (bool &less,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  if (!auxExp) {
    edu::sharif::twinner::util::Logger::error ()
        << "AdditionOperationGroup needs two expressions (auxExp is null)\n";
    throw std::runtime_error
        ("AdditionOperationGroup::instantiateConstraintForLessCase (): auxExp is null");
  }
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  edu::sharif::twinner::trace::Expression *negativeOfRightExp = auxExp->twosComplement ();
  list.push_back
      (edu::sharif::twinner::trace::Constraint::instantiateLessConstraint
       (less, mainExp, negativeOfRightExp, instruction));
  delete negativeOfRightExp;
  return list;
}

std::list <edu::sharif::twinner::trace::Constraint *>
AdditionOperationGroup::instantiateConstraintForLessOrEqualCase (bool &lessOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  edu::sharif::twinner::trace::Expression *negativeOfRightExp =
      auxExp->twosComplement ();
  list.push_back
      (edu::sharif::twinner::trace::Constraint::instantiateLessOrEqualConstraint
       (lessOrEqual, mainExp, negativeOfRightExp, instruction));
  delete negativeOfRightExp;
  return list;
}

std::list <edu::sharif::twinner::trace::Constraint *>
AdditionOperationGroup::instantiateConstraintForBelowCase (bool &below,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list =
      instantiateConstraintForBelowOrEqualCase (below, mainExp, auxExp, instruction);
  if (below) {
    bool zero;
    list.push_back (edu::sharif::twinner::trace::Constraint::instantiateEqualConstraint
                    (zero, mainExp, instruction));
    below = below && !zero;
  }
  return list;
}

std::list <edu::sharif::twinner::trace::Constraint *>
AdditionOperationGroup::instantiateConstraintForBelowOrEqualCase (bool &belowOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  if (!auxExp) {
    edu::sharif::twinner::util::Logger::error ()
        << "AdditionOperationGroup needs two expressions (auxExp is null)\n";
    throw std::runtime_error
        ("AdditionOperationGroup::instantiateConstraintForBelowCase (): auxExp is null");
  }
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  edu::sharif::twinner::trace::Expression *negativeOfRightExp = auxExp->twosComplement ();
  list.push_back
      (edu::sharif::twinner::trace::Constraint::instantiateBelowConstraint
       (belowOrEqual, mainExp, negativeOfRightExp, instruction));
  delete negativeOfRightExp;
  belowOrEqual = !belowOrEqual;
  return list;
}

std::list <edu::sharif::twinner::trace::Constraint *>
AdditionOperationGroup::operationResultIsLessOrEqualWithZero (bool &lessOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  edu::sharif::twinner::trace::Expression *exp = mainExp->clone ();
  exp->add (auxExp);
  list.push_back
      (edu::sharif::twinner::trace::Constraint::instantiateLessOrEqualConstraint
       (lessOrEqual, exp, instruction));
  delete exp;
  return list;
}

std::list <edu::sharif::twinner::trace::Constraint *>
AdditionOperationGroup::operationResultIsLessThanZero (bool &lessOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  edu::sharif::twinner::trace::Expression *exp = mainExp->clone ();
  exp->add (auxExp);
  list.push_back
      (edu::sharif::twinner::trace::Constraint::instantiateLessConstraint
       (lessOrEqual, exp, instruction));
  delete exp;
  return list;
}

edu::sharif::twinner::trace::Expression *AdditionOperationGroup::getCarryExpression (
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp) const {
  const int size = mainExp->getLastConcreteValue ().getSize ();
  if (size > 64) {
    throw std::runtime_error ("AdditionOperationGroup::getCarryExpression (...):"
                              " Too large bit-length for expression");
  }
  edu::sharif::twinner::trace::Expression *doublePrecision = mainExp->clone (size * 2);
  doublePrecision->add (auxExp);
  doublePrecision->shiftToRight (size);
  edu::sharif::twinner::trace::Expression *truncexp = doublePrecision->clone (size);
  delete doublePrecision;
  return truncexp;
}

BitwiseAndOperationGroup::BitwiseAndOperationGroup () {
}

std::list <edu::sharif::twinner::trace::Constraint *>
BitwiseAndOperationGroup::instantiateConstraintForZeroCase (bool &zero,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  if (auxExp) {
    edu::sharif::twinner::util::Logger::error ()
        << "BitwiseAndOperationGroup needs one expression (auxExp is not null)\n";
    throw std::runtime_error
        ("BitwiseAndOperationGroup::instantiateConstraintForZeroCase ():"
         " auxExp is not null");
  }
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  list.push_back
      (edu::sharif::twinner::trace::Constraint::instantiateEqualConstraint
       (zero, mainExp, instruction));
  return list;
}

std::list <edu::sharif::twinner::trace::Constraint *>
BitwiseAndOperationGroup::instantiateConstraintForLessCase (bool &less,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "BitwiseAndOperationGroup always clears OF and so this code is unreachable!\n";
  throw std::runtime_error
      ("BitwiseAndOperationGroup::instantiateConstraintForLessCase ():"
       " unreachable code");
}

std::list <edu::sharif::twinner::trace::Constraint *>
BitwiseAndOperationGroup::instantiateConstraintForLessOrEqualCase (bool &lessOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "BitwiseAndOperationGroup always clears OF and so this code is unreachable!\n";
  throw std::runtime_error
      ("BitwiseAndOperationGroup::instantiateConstraintForLessOrEqualCase ():"
       " unreachable code");
}

std::list <edu::sharif::twinner::trace::Constraint *>
BitwiseAndOperationGroup::instantiateConstraintForBelowCase (bool &below,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "BitwiseAndOperationGroup always clears CF and so this code is unreachable!\n";
  throw std::runtime_error
      ("BitwiseAndOperationGroup::instantiateConstraintForBelowCase ():"
       " unreachable code");
}

std::list <edu::sharif::twinner::trace::Constraint *>
BitwiseAndOperationGroup::instantiateConstraintForBelowOrEqualCase (bool &belowOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  // CF is clear, so below-or-equal should just check for ZF=1
  return instantiateConstraintForZeroCase (belowOrEqual, mainExp, auxExp, instruction);
}

std::list <edu::sharif::twinner::trace::Constraint *>
BitwiseAndOperationGroup::operationResultIsLessOrEqualWithZero (bool &lessOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  list.push_back
      (edu::sharif::twinner::trace::Constraint::instantiateLessOrEqualConstraint
       (lessOrEqual, mainExp, instruction));
  return list;
}

std::list <edu::sharif::twinner::trace::Constraint *>
BitwiseAndOperationGroup::operationResultIsLessThanZero (bool &lessOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  list.push_back
      (edu::sharif::twinner::trace::Constraint::instantiateLessConstraint
       (lessOrEqual, mainExp, instruction));
  return list;
}

edu::sharif::twinner::trace::Expression *BitwiseAndOperationGroup::getCarryExpression (
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp) const {
  edu::sharif::twinner::util::Logger::error ()
      << "BitwiseAndOperationGroup always clears CF and so this code is unreachable!\n";
  throw std::runtime_error
      ("BitwiseAndOperationGroup::getCarryExpression (): unreachable code");
}

}
}
}
}
