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
#include <string>

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

  typedef edu::sharif::twinner::trace::Expression Expression;
  typedef Expression *ExpressionPtr;
  typedef const Expression *ConstExpressionPtr;

  typedef edu::sharif::twinner::trace::Constraint Constraint;
  typedef Constraint *ConstraintPtr;

  virtual ExpressionPtr getCarryExpression () const = 0;

  virtual std::list <ConstraintPtr> instantiateConstraintForZeroCase (bool &zero,
      uint32_t instruction) const = 0;
  virtual std::list <ConstraintPtr> instantiateConstraintForLessCase (bool &less,
      uint32_t instruction) const = 0;
  virtual std::list <ConstraintPtr> instantiateConstraintForLessOrEqualCase (
      bool &lessOrEqual, uint32_t instruction) const = 0;
  virtual std::list <ConstraintPtr> instantiateConstraintForBelowCase (bool &below,
      uint32_t instruction) const = 0;
  virtual std::list <ConstraintPtr> instantiateConstraintForBelowOrEqualCase (
      bool &belowOrEqual, uint32_t instruction) const = 0;

  virtual std::list <ConstraintPtr> operationResultIsLessOrEqualWithZero (
      bool &lessOrEqual, uint32_t instruction) const = 0;
  virtual std::list <ConstraintPtr> operationResultIsLessThanZero (bool &lessOrEqual,
      uint32_t instruction) const = 0;
};

template <int N>
class NaryOperationGroup : public OperationGroup {

protected:
  ConstExpressionPtr exp[N];

  NaryOperationGroup () :
      OperationGroup () {
  }

  NaryOperationGroup (ConstExpressionPtr exp1) :
      OperationGroup () {
    exp[0] = exp1;
  }

  NaryOperationGroup (ConstExpressionPtr exp1, ConstExpressionPtr exp2) :
      OperationGroup () {
    exp[0] = exp1;
    exp[1] = exp2;
  }

  NaryOperationGroup (ConstExpressionPtr exp1, ConstExpressionPtr exp2,
      ConstExpressionPtr exp3) :
      OperationGroup () {
    exp[0] = exp1;
    exp[1] = exp2;
    exp[2] = exp3;
  }

public:

  virtual ~NaryOperationGroup () {
    for (int i = 0; i < N; ++i) {
      delete exp[i];
    }
  }
};

class DummyOperationGroup : public NaryOperationGroup<0> {

private:
  const std::string name;

public:
  DummyOperationGroup (const char *name);

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

class SubtractOperationGroup : public NaryOperationGroup<2> {

public:
  SubtractOperationGroup (ConstExpressionPtr mainExp, ConstExpressionPtr auxExp);

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

class BitwiseAndOperationGroup : public NaryOperationGroup<1> {

public:
  BitwiseAndOperationGroup (ConstExpressionPtr mainExp);

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

#endif	/* OperationGroup.h */
