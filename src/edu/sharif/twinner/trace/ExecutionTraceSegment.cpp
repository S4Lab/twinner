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

const Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionByRegister (
    REG reg) const {
  throw "Not yet implemented";
}

const Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionByMemoryAddress (
    ADDRINT memoryEa) const {
  throw "Not yet implemented";
}

const Expression *ExecutionTraceSegment::getSymbolicExpressionByRegister (REG reg) {
  throw "Not yet implemented";
}

const Expression *ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress (
    ADDRINT memoryEa) {
  throw "Not yet implemented";
}

void ExecutionTraceSegment::setSymbolicExpressionByRegister (REG reg,
    const Expression *exp) {
  throw "Not yet implemented";
}

void ExecutionTraceSegment::setSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
    const Expression *exp) {
  throw "Not yet implemented";
}

void ExecutionTraceSegment::addPathConstraint (Constraint c) {
  throw "Not yet implemented";
}

}
}
}
}
