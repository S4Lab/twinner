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

#include <utility>
#include <stdexcept>

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

const Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionByRegister (REG reg,
    ADDRINT regval) const throw (WrongStateException) {
  return tryToGetSymbolicExpressionImplementation (registerToExpression, reg, regval);
}

const Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionByMemoryAddress (
    ADDRINT memoryEa, UINT64 memval) const throw (WrongStateException) {
  return tryToGetSymbolicExpressionImplementation (memoryAddressToExpression, memoryEa,
      memval);
}

template < typename KEY >
const Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionImplementation (
    const std::map < KEY, Expression * > &map, const KEY key, UINT64 concreteVal) const
        throw (WrongStateException) {
  typename std::map < KEY, Expression * >::const_iterator it = map.find (key);
  if (it == map.end ()) { // not found!
    return 0;
  } else {
    const Expression *exp = it->second;
    if (exp->getLastConcreteValue () != concreteVal) {
      throw WrongStateException ();
    }
    return exp;
  }
}

const Expression *ExecutionTraceSegment::getSymbolicExpressionByRegister (REG reg,
    UINT64 regval, Expression *newExpression) {
  return getSymbolicExpressionImplementation (registerToExpression, reg, regval,
      newExpression);
}

const Expression *ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress (
    ADDRINT memoryEa, UINT64 memval, Expression *newExpression) {
  return getSymbolicExpressionImplementation (memoryAddressToExpression, memoryEa, memval,
      newExpression);
}

template < typename KEY >
const Expression *ExecutionTraceSegment::getSymbolicExpressionImplementation (
    std::map < KEY, Expression * > &map, const KEY key, UINT64 currentConcreteValue,
    Expression *newExpression) {
  typedef typename std::map < KEY, Expression * >::iterator MapIterator;
  try {
    const Expression *exp = tryToGetSymbolicExpressionImplementation (map, key,
        currentConcreteValue);
    if (exp) {
      return exp;
    }
  } catch (const WrongStateException &e) {
  }
  if (!newExpression) {
    throw std::runtime_error (
        "ExecutionTraceSegment::getSymbolicExpressionImplementation method: newExpression is null!");
  }
  std::pair < MapIterator, bool > res = map.insert (make_pair (key, newExpression));
  if (!res.second) { // another expression already exists. overwriting...
    MapIterator it = res.first;
    delete it->second;
    it->second = newExpression;
  }
  return newExpression;
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
    std::map < KEY, Expression * > &map, const KEY key,
    const Expression *nonOwnedExpression) {
  typedef typename std::map < KEY, Expression * >::iterator MapIterator;
  // The nonOwnedExpression is owned by caller. We must clone it and take ownership of the cloned object.
  Expression *exp = nonOwnedExpression->clone ();
  std::pair < MapIterator, bool > res = map.insert (make_pair (key, exp));
  if (!res.second) { // another expression already exists. overwriting...
    // old expression is owned by us; so it should be deleted before loosing its pointer!
    MapIterator it = res.first;
    delete it->second;
    it->second = exp;
  }
}

void ExecutionTraceSegment::addPathConstraint (Constraint c) {
  throw std::runtime_error (
      "ExecutionTraceSegment::addPathConstraint: Not yet implemented");
}

}
}
}
}
