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

#include "OperationGroup.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

const OperationGroup *OperationGroup::SUB_OPGROUP = new SubtractOperationGroup ();
const OperationGroup *OperationGroup::ADD_OPGROUP = new AdditionOperationGroup ();
const OperationGroup *OperationGroup::AND_OPGROUP = new BitwiseAndOperationGroup ();

OperationGroup::OperationGroup () {
}

OperationGroup::~OperationGroup () {
}

SubtractOperationGroup::SubtractOperationGroup () {
}

edu::sharif::twinner::trace::Constraint *
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
  return edu::sharif::twinner::trace::Constraint::instantiateEqualConstraint
      (zero, mainExp, auxExp, instruction);
}

edu::sharif::twinner::trace::Constraint *
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
  return edu::sharif::twinner::trace::Constraint::instantiateLessConstraint
      (less, mainExp, auxExp, instruction);
}

edu::sharif::twinner::trace::Constraint *
SubtractOperationGroup::instantiateConstraintForLessOrEqualCase (bool &lessOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  return edu::sharif::twinner::trace::Constraint::instantiateLessOrEqualConstraint
      (lessOrEqual, mainExp, auxExp, instruction);
}

edu::sharif::twinner::trace::Constraint *
SubtractOperationGroup::instantiateConstraintForBelowCase (bool &below,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  return edu::sharif::twinner::trace::Constraint::instantiateBelowConstraint
      (below, mainExp, auxExp, instruction);
}

edu::sharif::twinner::trace::Constraint *
SubtractOperationGroup::operationResultIsLessOrEqualWithZero (bool &lessOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  edu::sharif::twinner::trace::Expression *exp = mainExp->clone ();
  exp->minus (auxExp);
  edu::sharif::twinner::trace::Constraint *res =
      edu::sharif::twinner::trace::Constraint::instantiateLessOrEqualConstraint
      (lessOrEqual, exp, instruction);
  delete exp;
  return res;
}

edu::sharif::twinner::trace::Constraint *
SubtractOperationGroup::operationResultIsLessThanZero (bool &lessOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  edu::sharif::twinner::trace::Expression *exp = mainExp->clone ();
  exp->minus (auxExp);
  edu::sharif::twinner::trace::Constraint *res =
      edu::sharif::twinner::trace::Constraint::instantiateLessConstraint
      (lessOrEqual, exp, instruction);
  delete exp;
  return res;
}

AdditionOperationGroup::AdditionOperationGroup () {
}

edu::sharif::twinner::trace::Constraint *
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
  edu::sharif::twinner::trace::Expression *negativeOfRightExp = auxExp->twosComplement ();
  edu::sharif::twinner::trace::Constraint *res =
      edu::sharif::twinner::trace::Constraint::instantiateEqualConstraint
      (zero, mainExp, negativeOfRightExp, instruction);
  delete negativeOfRightExp;
  return res;
}

edu::sharif::twinner::trace::Constraint *
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
  edu::sharif::twinner::trace::Expression *negativeOfRightExp = auxExp->twosComplement ();
  edu::sharif::twinner::trace::Constraint *res =
      edu::sharif::twinner::trace::Constraint::instantiateLessConstraint
      (less, mainExp, negativeOfRightExp, instruction);
  delete negativeOfRightExp;
  return res;
}

edu::sharif::twinner::trace::Constraint *
AdditionOperationGroup::instantiateConstraintForLessOrEqualCase (bool &lessOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  edu::sharif::twinner::trace::Expression *negativeOfRightExp =
      auxExp->twosComplement ();
  edu::sharif::twinner::trace::Constraint *res =
      edu::sharif::twinner::trace::Constraint::instantiateLessOrEqualConstraint
      (lessOrEqual, mainExp, negativeOfRightExp, instruction);
  delete negativeOfRightExp;
  return res;
}

edu::sharif::twinner::trace::Constraint *
AdditionOperationGroup::instantiateConstraintForBelowCase (bool &below,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  if (!auxExp) {
    edu::sharif::twinner::util::Logger::error ()
        << "AdditionOperationGroup needs two expressions (auxExp is null)\n";
    throw std::runtime_error
        ("AdditionOperationGroup::instantiateConstraintForBelowCase (): auxExp is null");
  }
  edu::sharif::twinner::trace::Expression *negativeOfRightExp = auxExp->twosComplement ();
  edu::sharif::twinner::trace::Constraint *res =
      edu::sharif::twinner::trace::Constraint::instantiateLessConstraint
      (below, mainExp, negativeOfRightExp, instruction);
  delete negativeOfRightExp;
  below = !below;
  return res;
}

edu::sharif::twinner::trace::Constraint *
AdditionOperationGroup::operationResultIsLessOrEqualWithZero (bool &lessOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  edu::sharif::twinner::trace::Expression *exp = mainExp->clone ();
  exp->add (auxExp);
  edu::sharif::twinner::trace::Constraint *res =
      edu::sharif::twinner::trace::Constraint::instantiateLessOrEqualConstraint
      (lessOrEqual, exp, instruction);
  delete exp;
  return res;
}

edu::sharif::twinner::trace::Constraint *
AdditionOperationGroup::operationResultIsLessThanZero (bool &lessOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  edu::sharif::twinner::trace::Expression *exp = mainExp->clone ();
  exp->add (auxExp);
  edu::sharif::twinner::trace::Constraint *res =
      edu::sharif::twinner::trace::Constraint::instantiateLessConstraint
      (lessOrEqual, exp, instruction);
  delete exp;
  return res;
}

BitwiseAndOperationGroup::BitwiseAndOperationGroup () {
}

edu::sharif::twinner::trace::Constraint *
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
  return edu::sharif::twinner::trace::Constraint::instantiateEqualConstraint
      (zero, mainExp, instruction);
}

edu::sharif::twinner::trace::Constraint *
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

edu::sharif::twinner::trace::Constraint *
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

edu::sharif::twinner::trace::Constraint *
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

edu::sharif::twinner::trace::Constraint *
BitwiseAndOperationGroup::operationResultIsLessOrEqualWithZero (bool &lessOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  edu::sharif::twinner::trace::Constraint *res =
      edu::sharif::twinner::trace::Constraint::instantiateLessOrEqualConstraint
      (lessOrEqual, mainExp, instruction);
  return res;
}

edu::sharif::twinner::trace::Constraint *
BitwiseAndOperationGroup::operationResultIsLessThanZero (bool &lessOrEqual,
    const edu::sharif::twinner::trace::Expression *mainExp,
    const edu::sharif::twinner::trace::Expression *auxExp,
    uint32_t instruction) const {
  edu::sharif::twinner::trace::Constraint *res =
      edu::sharif::twinner::trace::Constraint::instantiateLessConstraint
      (lessOrEqual, mainExp, instruction);
  return res;
}

}
}
}
}
