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

#ifndef ADD_WITH_CARRY_OPERATION_GROUP_H
#define ADD_WITH_CARRY_OPERATION_GROUP_H

#include "OperationGroup.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace operationgroup {

class AddWithCarryOperationGroup : public NaryOperationGroup<3> {
public:
  AddWithCarryOperationGroup (ConstExpressionPtr mainExp, ConstExpressionPtr auxExp,
      ConstExpressionPtr carryExp);

  virtual ExpressionPtr getCarryExpression () const;

  virtual std::list <ConstraintPtr> instantiateConstraintForOverflowCase (
      bool &overflow, uint32_t instruction) const;
  virtual std::list <ConstraintPtr> instantiateConstraintForZeroCase (bool &zero,
      uint32_t instruction) const;
  virtual std::list <ConstraintPtr> instantiateConstraintForLessCase (bool &less,
      uint32_t instruction) const;
  virtual std::list <ConstraintPtr> instantiateConstraintForLessOrEqualCase (
      bool &lessOrEqual, uint32_t instruction) const;
  virtual std::list <ConstraintPtr> instantiateConstraintForBelowCase (bool &below,
      uint32_t instruction) const;
  virtual std::list <ConstraintPtr> instantiateConstraintForBelowOrEqualCase (
      bool &belowOrEqual, uint32_t instruction) const;

  virtual std::list <ConstraintPtr> operationResultIsLessOrEqualWithZero (
      bool &lessOrEqual, uint32_t instruction) const;
  virtual std::list <ConstraintPtr> operationResultIsLessThanZero (bool &lessOrEqual,
      uint32_t instruction) const;
  virtual ExpressionPtr getOperationResult () const;
};

}
}
}
}

#endif /* AddWithCarryOperationGroup.h */
