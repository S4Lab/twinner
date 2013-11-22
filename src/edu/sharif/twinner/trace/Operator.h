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

#include "pin.H"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Operator : public ExpressionToken {

public:

  enum OperatorIdentifier {

    ADD, MINUS,
    MULTIPLY, DIVIDE,
    REMAINDER,
    NEGATE,
    XOR,
    BITWISE_AND, BITWISE_OR,
    SHIFT_LEFT, SHIFT_RIGHT,
  };

private:
  OperatorIdentifier oi;

  Operator (const Operator &op);

public:
  Operator (OperatorIdentifier);

  virtual Operator *clone () const;

  virtual void saveToBinaryStream (std::ofstream &out) const;

  UINT64 apply (UINT64 a, UINT64 b) const;
};

}
}
}
}

#endif /* Operator.h */
