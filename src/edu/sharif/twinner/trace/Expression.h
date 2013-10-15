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

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "Operator.h"

#include "pin.H"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Expression {
private:
  int stack; // TODO: change to a stack of symbols and operators
  UINT64 lastConcreteValue;

public:
  /**
   * Instantiates an expression containing a new (yet unused) symbol.
   */
  Expression ();

  void toString ();

  void unaryOperation (Operator op, Expression exp);

  void binaryOperation (Operator op, Expression exp);

  Expression *clone () const;
};

}
}
}
}

#endif /* Expression.h */
