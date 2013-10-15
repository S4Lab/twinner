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

#include "ExecutionTraceSegment.h"

#include "Expression.h"
#include "Constraint.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

Expression ExecutionTraceSegment::getSymbolicExpressionByRegister (REG reg) {
  throw "Not yet implemented";
}

Expression ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress (
    ADDRINT memoryEa) {
  throw "Not yet implemented";
}

void ExecutionTraceSegment::setSymbolicExpressionByRegister (REG reg, Expression exp) {
  throw "Not yet implemented";
}

void ExecutionTraceSegment::setSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
    Expression exp) {
  throw "Not yet implemented";
}

void ExecutionTraceSegment::addPathConstraint (Constraint c) {
  throw "Not yet implemented";
}

}
}
}
}
