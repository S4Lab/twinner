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

ExecutionTraceSegment::ExecutionTraceSegment () {
}

ExecutionTraceSegment::~ExecutionTraceSegment () {
  for (std::map < REG, Expression * >::iterator it = registerToExpression.begin ();
      it != registerToExpression.end (); ++it) {
    Expression *exp = it->second;
    delete exp;
  }
  registerToExpression.clear ();
  for (std::map < ADDRINT, Expression * >::iterator it =
      memoryAddressToExpression.begin (); it != memoryAddressToExpression.end (); ++it) {
    Expression *exp = it->second;
    delete exp;
  }
  memoryAddressToExpression.clear ();
}

const Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionByRegister (
    REG reg) const {
  return tryToGetSymbolicExpressionImplementation (registerToExpression, reg);
}

const Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionByMemoryAddress (
    ADDRINT memoryEa) const {
  return tryToGetSymbolicExpressionImplementation (memoryAddressToExpression, memoryEa);
}

template < typename KEY >
const Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionImplementation (
    const std::map < KEY, Expression * > &map, KEY key) const {
  typename std::map < KEY, Expression * >::const_iterator it = map.find (key);
  if (it == map.end ()) { // not found!
    return 0;
  } else {
    return it->second;
  }
}

const Expression *ExecutionTraceSegment::getSymbolicExpressionByRegister (REG reg) {
  return getSymbolicExpressionImplementation (registerToExpression, reg);
}

const Expression *ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress (
    ADDRINT memoryEa) {
  return getSymbolicExpressionImplementation (memoryAddressToExpression, memoryEa);
}

template < typename KEY >
const Expression *ExecutionTraceSegment::getSymbolicExpressionImplementation (
    std::map < KEY, Expression * > &map, KEY key) {
  const Expression *exp = tryToGetSymbolicExpressionImplementation (map, key);
  if (exp == 0) { // does not exist! instantiate a new one...
    Expression *exp2 = new Expression ();
    map.insert (make_pair (key, exp2));
    return exp2;

  } else {
    return exp;
  }
}

void ExecutionTraceSegment::setSymbolicExpressionByRegister (REG reg,
    const Expression *exp) {
  setSymbolicExpressionImplementation (registerToExpression, reg, exp);
}

void ExecutionTraceSegment::setSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
    const Expression *exp) {
  setSymbolicExpressionImplementation (memoryAddressToExpression, memoryEa, exp);
}

template < typename KEY >
void ExecutionTraceSegment::setSymbolicExpressionImplementation (
    std::map < KEY, Expression * > &map, KEY key, const Expression *exp) {
  // The exp is owned by caller. We must clone it and take ownership of the cloned object.
  map.insert (make_pair (key, exp->clone ()));
}

void ExecutionTraceSegment::addPathConstraint (Constraint c) {
  throw "Not yet implemented";
}

}
}
}
}
