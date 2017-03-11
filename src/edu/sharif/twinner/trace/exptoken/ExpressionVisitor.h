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

#ifndef EXPRESSION_VISITOR_H
#define EXPRESSION_VISITOR_H

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

class Operator;
class Operand;

template <typename Result>
class ExpressionVisitor {
protected:
  typedef Result ResultType;

  ExpressionVisitor () {
  }

public:

  virtual ~ExpressionVisitor () {
  }

  virtual ResultType visitTrinary (const Operator *op,
      ResultType &left, ResultType &middle, ResultType &right) = 0;
  virtual ResultType visitFunctionalBinary (const Operator *op,
      ResultType &left, ResultType &right) = 0;
  virtual ResultType visitBinary (const Operator *op,
      ResultType &left, ResultType &right) = 0;
  virtual ResultType visitUnary (const Operator *op, ResultType &main) = 0;
  virtual ResultType visitOperand (const Operand *operand) = 0;
};

}
}
}
}
}

#endif /* ExpressionVisitor.h */
