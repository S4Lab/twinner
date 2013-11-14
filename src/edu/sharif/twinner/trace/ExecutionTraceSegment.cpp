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

#include "edu/sharif/twinner/util/Logger.h"

#include <utility>
#include <stdexcept>
#include <fstream>
#include <string.h>

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
  while (!pathConstraint.empty ()) {
    delete pathConstraint.front ();
    pathConstraint.pop_front ();
  }
}

Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionByRegister (REG reg,
    UINT64 regval) throw (WrongStateException) {
  return tryToGetSymbolicExpressionImplementation (registerToExpression, reg, regval);
}

Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionByRegister (REG reg) {
  return tryToGetSymbolicExpressionImplementation (registerToExpression, reg);
}

Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionByMemoryAddress (
    ADDRINT memoryEa, UINT64 memval) throw (WrongStateException) {
  return tryToGetSymbolicExpressionImplementation
      (memoryAddressToExpression, memoryEa, memval);
}

template < typename KEY >
Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionImplementation (
    std::map < KEY, Expression * > &map, const KEY key, UINT64 concreteVal) const
throw (WrongStateException) {
  typename std::map < KEY, Expression * >::iterator it = map.find (key);
  if (it == map.end ()) { // not found!
    return 0;
  } else {
    Expression *exp = it->second;
    if (exp->getLastConcreteValue () != concreteVal) {
      throw WrongStateException (exp->getLastConcreteValue ());
    }
    return exp;
  }
}

template < typename KEY >
Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionImplementation (
    std::map < KEY, Expression * > &map, const KEY key) const {
  typename std::map < KEY, Expression * >::iterator it = map.find (key);
  if (it == map.end ()) { // not found!
    return 0;
  } else {
    return it->second;
  }
}

Expression *ExecutionTraceSegment::getSymbolicExpressionByRegister (REG reg,
    UINT64 regval, Expression *newExpression) {
  return getSymbolicExpressionImplementation (registerToExpression, reg, regval,
                                              newExpression);
}

Expression *ExecutionTraceSegment::getSymbolicExpressionByRegister (REG reg,
    Expression *newExpression) {
  return getSymbolicExpressionImplementation (registerToExpression, reg,
                                              newExpression);
}

Expression *ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
    UINT64 memval, Expression *newExpression) {
  return getSymbolicExpressionImplementation
      (memoryAddressToExpression, memoryEa, memval, newExpression);
}

template < typename KEY >
Expression *ExecutionTraceSegment::getSymbolicExpressionImplementation (
    std::map < KEY, Expression * > &map, const KEY key, UINT64 currentConcreteValue,
    Expression *newExpression) {
  typedef typename std::map < KEY, Expression * >::iterator MapIterator;
  try {
    Expression *exp = tryToGetSymbolicExpressionImplementation
        (map, key, currentConcreteValue);
    if (exp) {
      return exp;
    }
  } catch (const WrongStateException &e) {
  }
  if (!newExpression) {
    throw std::runtime_error
        ("ExecutionTraceSegment::getSymbolicExpressionImplementation method: "
         "newExpression is null!");
  }
  std::pair < MapIterator, bool > res = map.insert (make_pair (key, newExpression));
  if (!res.second) { // another expression already exists. overwriting...
    MapIterator it = res.first;
    delete it->second;
    it->second = newExpression;
  }
  return newExpression;
}

template < typename KEY >
Expression *ExecutionTraceSegment::getSymbolicExpressionImplementation (
    std::map < KEY, Expression * > &map, const KEY key,
    Expression *newExpression) {
  typedef typename std::map < KEY, Expression * >::iterator MapIterator;
  Expression *exp = tryToGetSymbolicExpressionImplementation
      (map, key);
  if (exp) {
    return exp;
  }
  if (!newExpression) {
    throw std::runtime_error
        ("ExecutionTraceSegment::getSymbolicExpressionImplementation method: "
         "newExpression is null!");
  }
  std::pair < MapIterator, bool > res = map.insert (make_pair (key, newExpression));
  if (!res.second) {
    // as there was no expression (regardless of concrete value), this case is impossible!
    throw std::runtime_error
        ("ExecutionTraceSegment::getSymbolicExpressionImplementation method: "
         "can not set expression, while there was no prior expression!");
  }
  return newExpression;
}

Expression *ExecutionTraceSegment::setSymbolicExpressionByRegister (REG reg,
    const Expression *exp) {
  return setSymbolicExpressionImplementation (registerToExpression, reg, exp);
}

Expression *ExecutionTraceSegment::setSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
    const Expression *exp) {
  return setSymbolicExpressionImplementation (memoryAddressToExpression, memoryEa, exp);
}

template < typename KEY >
Expression *ExecutionTraceSegment::setSymbolicExpressionImplementation (
    std::map < KEY, Expression * > &map, const KEY key,
    const Expression *nonOwnedExpression) {
  typedef typename std::map < KEY, Expression * >::iterator MapIterator;
  // The nonOwnedExpression is owned by caller.
  // We must clone it and take ownership of the cloned object.
  Expression *exp = nonOwnedExpression->clone ();
  std::pair < MapIterator, bool > res = map.insert (make_pair (key, exp));
  if (!res.second) { // another expression already exists. overwriting...
    // old expression is owned by us; so it should be deleted before loosing its pointer!
    MapIterator it = res.first;
    delete it->second;
    it->second = exp;
  }
  return exp;
}

void ExecutionTraceSegment::addPathConstraint (const Constraint *c) {
  pathConstraint.push_back (c);
}

void ExecutionTraceSegment::saveToBinaryStream (std::ofstream &out) const {
  const char *segmentMagicString = "SEG";
  out.write (segmentMagicString, 3);

  saveMapToBinaryStream (out, "REG", registerToExpression);
  saveMapToBinaryStream (out, "MEM", memoryAddressToExpression);
  saveListToBinaryStream (out, "CON", pathConstraint);
}

template <typename ADDRESS>
void ExecutionTraceSegment::saveMapToBinaryStream (std::ofstream &out,
    const char *magicString, const std::map < ADDRESS, Expression * > map) const {
  out.write (magicString, strlen (magicString));

  typename std::map < ADDRESS, Expression * >::size_type s = map.size ();
  out.write ((const char *) &s, sizeof (s));
  for (typename std::map < ADDRESS, Expression * >::const_iterator it = map.begin ();
      it != map.end (); ++it) {
    const ADDRESS a = it->first;
    out.write ((const char *) &a, sizeof (a));

    const Expression *exp = it->second;
    exp->saveToBinaryStream (out);
  }
}

void ExecutionTraceSegment::printRegistersValues (
    const edu::sharif::twinner::util::Logger &logger) const {
  logger << registerToExpression;
}

void ExecutionTraceSegment::printMemoryAddressesValues (
    const edu::sharif::twinner::util::Logger &logger) const {
  logger << memoryAddressToExpression;
}

void ExecutionTraceSegment::printPathConstraints (
    const edu::sharif::twinner::util::Logger &logger) const {
  logger << pathConstraint;
}

void ExecutionTraceSegment::printCompleteState (
    const edu::sharif::twinner::util::Logger &logger) const {
  logger << "Registers:\n";
  printRegistersValues (logger);
  logger << "Memory:\n";
  printMemoryAddressesValues (logger);
  logger << "Constraints:\n";
  printPathConstraints (logger);
}


}
}
}
}
