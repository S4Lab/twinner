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

#include "Expression.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

Expression::Expression (UINT64 concreteValue) {
  // TODO: Instantiate a new expression containing a new symbol
}

UINT64 Expression::getLastConcreteValue () const {
  return lastConcreteValue;
}

void Expression::toString () {
  throw "Not yet implemented";
}

void Expression::unaryOperation (Operator op, Expression exp) {
  throw "Not yet implemented";
}

void Expression::binaryOperation (Operator op, Expression exp) {
  throw "Not yet implemented";
}

Expression *Expression::clone () const {
  throw "Not yet implemented";
}

}
}
}
}
