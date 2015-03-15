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

#include <stdexcept>
#include <list>

#include "BitwiseAndOperationGroup.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/Expression.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {
namespace operationgroup {

BitwiseAndOperationGroup::BitwiseAndOperationGroup (ConstExpressionPtr mainExp) :
    NaryOperationGroup (mainExp) {
}

OperationGroup::ExpressionPtr BitwiseAndOperationGroup::getCarryExpression () const {
  edu::sharif::twinner::util::Logger::error ()
      << "BitwiseAndOperationGroup always clears CF and so this code is unreachable!\n";
  throw std::runtime_error
      ("BitwiseAndOperationGroup::getCarryExpression (): unreachable code");
}

std::list <OperationGroup::ConstraintPtr>
BitwiseAndOperationGroup::instantiateConstraintForZeroCase (bool &zero,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  list.push_back (OperationGroup::Constraint::instantiateEqualConstraint
                  (zero, exp[0], instruction));
  return list;
}

std::list <OperationGroup::ConstraintPtr>
BitwiseAndOperationGroup::instantiateConstraintForLessCase (bool &less,
    uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "BitwiseAndOperationGroup always clears OF and so this code is unreachable!\n";
  throw std::runtime_error
      ("BitwiseAndOperationGroup::instantiateConstraintForLessCase ():"
       " unreachable code");
}

std::list <OperationGroup::ConstraintPtr>
BitwiseAndOperationGroup::instantiateConstraintForLessOrEqualCase (bool &lessOrEqual,
    uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "BitwiseAndOperationGroup always clears OF and so this code is unreachable!\n";
  throw std::runtime_error
      ("BitwiseAndOperationGroup::instantiateConstraintForLessOrEqualCase ():"
       " unreachable code");
}

std::list <OperationGroup::ConstraintPtr>
BitwiseAndOperationGroup::instantiateConstraintForBelowCase (bool &below,
    uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "BitwiseAndOperationGroup always clears CF and so this code is unreachable!\n";
  throw std::runtime_error
      ("BitwiseAndOperationGroup::instantiateConstraintForBelowCase ():"
       " unreachable code");
}

std::list <OperationGroup::ConstraintPtr>
BitwiseAndOperationGroup::instantiateConstraintForBelowOrEqualCase (bool &belowOrEqual,
    uint32_t instruction) const {
  // CF is clear, so below-or-equal should just check for ZF=1
  return instantiateConstraintForZeroCase (belowOrEqual, instruction);
}

std::list <OperationGroup::ConstraintPtr>
BitwiseAndOperationGroup::operationResultIsLessOrEqualWithZero (bool &lessOrEqual,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  list.push_back (OperationGroup::Constraint::instantiateLessOrEqualConstraint
                  (lessOrEqual, exp[0], instruction));
  return list;
}

std::list <OperationGroup::ConstraintPtr>
BitwiseAndOperationGroup::operationResultIsLessThanZero (bool &lessOrEqual,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  list.push_back (OperationGroup::Constraint::instantiateLessConstraint
                  (lessOrEqual, exp[0], instruction));
  return list;
}

}
}
}
}
}
