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

#ifndef OPERATION_GROUP_H
#define OPERATION_GROUP_H

#include <stdint.h>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Constraint;
class Expression;
}
namespace twintool {

class OperationGroup {

protected:
  OperationGroup ();

public:
  virtual ~OperationGroup ();

  static const OperationGroup *SUB_OPGROUP;
  static const OperationGroup *ADD_OPGROUP;
  static const OperationGroup *ADD_WITH_CARRY_OPGROUP;
  static const OperationGroup *SHIFT_LEFT_OPGROUP;
  static const OperationGroup *SHIFT_RIGHT_OPGROUP;
  static const OperationGroup *SHIFT_ARITHMETIC_RIGHT_OPGROUP;
  static const OperationGroup *AND_OPGROUP;
  static const OperationGroup *INCREMENT_OPGROUP;
  static const OperationGroup *DECREMENT_OPGROUP;

  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForZeroCase (bool &zero,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const = 0;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForLessCase (bool &less,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const = 0;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForLessOrEqualCase (bool &lessOrEqual,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const = 0;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForBelowCase (bool &below,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const = 0;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForBelowOrEqualCase (bool &belowOrEqual,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const = 0;

  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  operationResultIsLessOrEqualWithZero (bool &lessOrEqual,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const = 0;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  operationResultIsLessThanZero (bool &lessOrEqual,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const = 0;
};

class SubtractOperationGroup : public OperationGroup {

private:
  SubtractOperationGroup ();

  friend class OperationGroup;

public:
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForZeroCase (bool &zero,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForLessCase (bool &less,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForLessOrEqualCase (bool &lessOrEqual,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForBelowCase (bool &below,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForBelowOrEqualCase (bool &belowOrEqual,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;

  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  operationResultIsLessOrEqualWithZero (bool &lessOrEqual,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  operationResultIsLessThanZero (bool &lessOrEqual,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;
};

class AdditionOperationGroup : public OperationGroup {

private:
  AdditionOperationGroup ();

  friend class OperationGroup;

public:
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForZeroCase (bool &zero,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForLessCase (bool &less,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForLessOrEqualCase (bool &lessOrEqual,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForBelowCase (bool &below,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForBelowOrEqualCase (bool &belowOrEqual,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;

  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  operationResultIsLessOrEqualWithZero (bool &lessOrEqual,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  operationResultIsLessThanZero (bool &lessOrEqual,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;
};

class BitwiseAndOperationGroup : public OperationGroup {

private:
  BitwiseAndOperationGroup ();

  friend class OperationGroup;

public:
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForZeroCase (bool &zero,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForLessCase (bool &less,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForLessOrEqualCase (bool &lessOrEqual,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForBelowCase (bool &below,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForBelowOrEqualCase (bool &belowOrEqual,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;

  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  operationResultIsLessOrEqualWithZero (bool &lessOrEqual,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;
  virtual std::list <edu::sharif::twinner::trace::Constraint *>
  operationResultIsLessThanZero (bool &lessOrEqual,
      const edu::sharif::twinner::trace::Expression *mainExp,
      const edu::sharif::twinner::trace::Expression *auxExp,
      uint32_t instruction) const;
};


}
}
}
}

#endif	/* OperationGroup.h */
