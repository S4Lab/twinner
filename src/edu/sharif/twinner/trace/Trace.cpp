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

#include "Expression.h"
#include "Constraint.h"
#include "Syscall.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

void Trace::getCurrentTraceSegment () {
  throw "Not yet implemented";
}

Expression Trace::getSymbolicExpression (int address) {
  throw "Not yet implemented";
}

void Trace::addPathConstraint (Constraint c) {
  throw "Not yet implemented";
}

void Trace::setSymbolicExpression (int address, Expression exp) {
  throw "Not yet implemented";
}

void Trace::syscallInvoked (Syscall s) {
  throw "Not yet implemented";
}

void Trace::saveToFile (const char *path) const {
  throw "Not yet implemented";
}

/**
 * Check for existence of path and loads a Trace from it. The Trace is newed
 * and the caller must delete it. If path does not exist, an exception will be raised.
 * @return A newed Trace object loaded from file "path".
 */
Trace *Trace::loadFromFile (const char *path) {
  throw "Not yet implemented";
}

}
}
}
}
