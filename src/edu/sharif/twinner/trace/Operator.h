//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef OPERATOR_H
#define OPERATOR_H

#include "ExpressionToken.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Operator : public ExpressionToken {

public:

  enum OperatorIdentifier {

    ADD, MINUS, MULTIPLY, DIVIDE, NEGATE, XOR, BITWISE_AND
  };

private:
  OperatorIdentifier oi;

  Operator (const Operator &op);

public:
  Operator (OperatorIdentifier);

  virtual Operator *clone () const;
};

}
}
}
}

#endif /* Operator.h */
