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

#ifndef BITWISE_AND_OPERATOR_H
#define BITWISE_AND_OPERATOR_H

#include "Operator.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

class BitwiseAndOperator : public Operator {

public:
  BitwiseAndOperator ();
  BitwiseAndOperator (const BitwiseAndOperator &ao);
  virtual ~BitwiseAndOperator ();

  virtual BitwiseAndOperator *clone () const;

  virtual bool doesSupportSimplification () const;
  virtual void apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
      const edu::sharif::twinner::trace::cv::ConcreteValue &src) const;

  virtual std::string toString () const;
  virtual bool operator== (const ExpressionToken &token) const;

protected:
  virtual SimplificationStatus deepSimplify (edu::sharif::twinner::trace::Expression *exp,
      edu::sharif::twinner::trace::cv::ConcreteValue *operand);

private:
  void initializeSimplificationRules ();

  bool isTruncatingMask (edu::sharif::twinner::trace::cv::ConcreteValue *cv) const;
  int numberOfBits (edu::sharif::twinner::trace::cv::ConcreteValue *cv) const;
};

}
}
}
}
}

#endif	/* BitwiseAndOperator.h */
