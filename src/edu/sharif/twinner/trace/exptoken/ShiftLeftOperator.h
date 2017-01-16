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

#ifndef SHIFT_LEFT_OPERATOR_H
#define SHIFT_LEFT_OPERATOR_H

#include "Operator.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

class ShiftLeftOperator : public Operator {

public:
  ShiftLeftOperator ();
  ShiftLeftOperator (const ShiftLeftOperator &ao);
  virtual ~ShiftLeftOperator ();

  virtual ShiftLeftOperator *clone () const;

  virtual bool doesSupportSimplification () const;
  virtual bool isCommutable () const;
  virtual bool apply (edu::sharif::twinner::trace::Expression *exp,
      edu::sharif::twinner::trace::cv::ConcreteValue *cv);
  virtual bool apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
      const edu::sharif::twinner::trace::cv::ConcreteValue &src) const;

  virtual std::string toString () const;
  virtual bool operator== (const ExpressionToken &token) const;

private:
  void initializeSimplificationRules ();

protected:
  virtual SimplificationStatus deepSimplify (edu::sharif::twinner::trace::Expression *exp,
      edu::sharif::twinner::trace::cv::ConcreteValue *operand);

private:
  bool skipBitwiseOrAndBitwiseAndOperators (int numberOfItems,
      std::list < ExpressionToken * >::iterator &it) const;

  struct AppliedMask {

    edu::sharif::twinner::trace::cv::ConcreteValue *mask;
    bool opIsBitwiseOr;
  };

  bool aggregateMasks (std::list < AppliedMask > &appliedMasks,
      std::list < ExpressionToken * >::iterator it,
      std::list < ExpressionToken * >::iterator end,
      edu::sharif::twinner::trace::cv::ConcreteValue &shiftAmount) const;
};

}
}
}
}
}

#endif	/* ShiftLeftOperator.h */
