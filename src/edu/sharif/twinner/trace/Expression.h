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

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Expression {

private:
  int stack;
  int lastConcreteValue;

public:
  void toString ();

  void unaryOperation (Operator op, Expression exp);

  void binaryOperation (Operator op, Expression exp);

  Expression clone ();
};

}
}
}
}

#endif /* Expression.h */
