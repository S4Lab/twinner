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

#include "Trace.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

void getCurrentTraceSegment () {
  throw "Not yet implemented";
}

edu::sharif::twinner::trace::Expression getSymbolicExpression (int address) {
  throw "Not yet implemented";
}

void addPathConstraint (edu::sharif::twinner::trace::Constraint c) {
  throw "Not yet implemented";
}

void setSymbolicExpression (int address, edu::sharif::twinner::trace::Expression exp) {
  throw "Not yet implemented";
}

void syscallInvoked (edu::sharif::twinner::trace::Syscall s) {
  throw "Not yet implemented";
}

void saveToFile () {
  throw "Not yet implemented";
}

void loadFromFile () {
  throw "Not yet implemented";
}

}
}
}
}
