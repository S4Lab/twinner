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

#ifndef BITWISE_OR_OPERATOR_H
#define BITWISE_OR_OPERATOR_H

#include "Operator.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

class BitwiseOrOperator : public Operator {

public:
  BitwiseOrOperator ();
  BitwiseOrOperator (const BitwiseOrOperator &ao);
  virtual ~BitwiseOrOperator ();

  virtual BitwiseOrOperator *clone () const;

  virtual bool doesSupportSimplification () const;
  virtual void apply (edu::sharif::twinner::trace::cv::ConcreteValue &dst,
      const edu::sharif::twinner::trace::cv::ConcreteValue &src) const;

  virtual std::string toString () const;
  virtual bool operator== (const ExpressionToken &token) const;

protected:
  virtual void initializeSimplificationRules ();
};

}
}
}
}
}

#endif	/* BitwiseOrOperator.h */
