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

#ifndef ADDITION_OPERATION_GROUP_H
#define ADDITION_OPERATION_GROUP_H

#include "OperationGroup.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {
namespace operationgroup {

class AdditionOperationGroup : public NaryOperationGroup<2> {

public:
  AdditionOperationGroup (ConstExpressionPtr mainExp, ConstExpressionPtr auxExp);

  virtual ExpressionPtr getCarryExpression () const;

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
};

}
}
}
}
}

#endif	/* AdditionOperationGroup.h */