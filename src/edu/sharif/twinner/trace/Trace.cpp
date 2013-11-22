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

#include <stdexcept>
#include <fstream>

#include "Expression.h"
#include "Constraint.h"
#include "Syscall.h"
#include "ExecutionTraceSegment.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

Trace::Trace () {
  segments.push_front (new ExecutionTraceSegment ());
  currentSegmentIterator = segments.begin ();
  currentSegmentIndex = 0;
}

Trace::~Trace () {
  while (!segments.empty ()) {
    delete segments.front ();
    segments.pop_front ();
  }
}

Expression *Trace::tryToGetSymbolicExpressionByRegister (REG reg, UINT64 regval)
throw (WrongStateException) {
  return tryToGetSymbolicExpressionImplementation
      (reg, regval,
       &ExecutionTraceSegment::tryToGetSymbolicExpressionByRegister);
}

Expression *Trace::tryToGetSymbolicExpressionByRegister (REG reg) {
  return tryToGetSymbolicExpressionImplementation
      (reg, &ExecutionTraceSegment::tryToGetSymbolicExpressionByRegister);
}

Expression *Trace::tryToGetSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
    UINT64 memval) throw (WrongStateException) {
  return tryToGetSymbolicExpressionImplementation
      (memoryEa, memval,
       &ExecutionTraceSegment::tryToGetSymbolicExpressionByMemoryAddress);
}

template < typename T >
Expression *Trace::tryToGetSymbolicExpressionImplementation (T address, UINT64 val,
    typename TryToGetSymbolicExpressionMethod < T >::TraceSegmentType method)
throw (WrongStateException) {
  for (std::list < ExecutionTraceSegment * >::iterator it = currentSegmentIterator;
      it != segments.end (); ++it) {
    // searches segments starting from the current towards the oldest one
    ExecutionTraceSegment *seg = *it;
    Expression *exp = (seg->*method) (address, val);
    if (exp) {
      return exp;
    }
  }
  return 0;
}

template < typename T >
Expression *Trace::tryToGetSymbolicExpressionImplementation (T address,
    typename TryToGetSymbolicExpressionMethod < T >::
    TraceSegmentTypeWithoutConcreteValue method) {
  for (std::list < ExecutionTraceSegment * >::iterator it = currentSegmentIterator;
      it != segments.end (); ++it) {
    // searches segments starting from the current towards the oldest one
    ExecutionTraceSegment *seg = *it;
    Expression *exp = (seg->*method) (address);
    if (exp) {
      return exp;
    }
  }
  return 0;
}

Expression *Trace::getSymbolicExpressionByRegister (REG reg, UINT64 regval,
    Expression *newExpression) {
  return getSymbolicExpressionImplementation
      (reg, regval, newExpression,
       &Trace::tryToGetSymbolicExpressionByRegister,
       registerResidentSymbolsGenerationIndices,
       &ExecutionTraceSegment::getSymbolicExpressionByRegister);
}

Expression *Trace::getSymbolicExpressionByRegister (REG reg, Expression *newExpression) {
  return getSymbolicExpressionImplementation
      (reg, newExpression,
       &Trace::tryToGetSymbolicExpressionByRegister,
       registerResidentSymbolsGenerationIndices,
       &ExecutionTraceSegment::getSymbolicExpressionByRegister);
}

Expression *Trace::getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa, UINT64 memval,
    Expression *newExpression) {
  return getSymbolicExpressionImplementation
      (memoryEa, memval, newExpression,
       &Trace::tryToGetSymbolicExpressionByMemoryAddress,
       memoryResidentSymbolsGenerationIndices,
       &ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress);
}

extern void throw_exception_about_unexpected_change_in_memory_or_register_address
(REG reg, UINT64 expectedVal, UINT64 currentVal);
extern void throw_exception_about_unexpected_change_in_memory_or_register_address
(ADDRINT address, UINT64 expectedVal, UINT64 currentVal);

template < typename T >
Expression *Trace::getSymbolicExpressionImplementation (T address, UINT64 val,
    Expression *newExpression,
    typename TryToGetSymbolicExpressionMethod < T >::TraceType tryToGetMethod,
    std::map < T, int > &generationIndices,
    typename GetSymbolicExpressionMethod < T >::TraceSegmentType getMethod) {
  UINT64 currentValue = -1;
  try {
    Expression *exp = (this->*tryToGetMethod) (address, val);
    if (exp) {
      return exp;
    }
  } catch (const WrongStateException &e) {
    currentValue = e.getCurrentStateValue ();
    edu::sharif::twinner::util::Logger::debug () << "Unexpected value (0x"
        << std::hex << currentValue
        << ") was found (instead of 0x" << val << "). "
        "Probably, a new symbol is required to describe it.\n";
  }
  // instantiate and set a new expression in the current segment
  typename std::map < T, int >::iterator it = generationIndices.find (address);
  if (it == generationIndices.end () || it->second < currentSegmentIndex) {
    generationIndices[address] = currentSegmentIndex;

  } else {
    getCurrentTraceSegment ()->printRegistersValues
        (edu::sharif::twinner::util::Logger::loquacious ());
    throw_exception_about_unexpected_change_in_memory_or_register_address
        (address, val, currentValue);
  }
  if (!newExpression) {
    newExpression = new Expression (address, val, currentSegmentIndex);
  }
  return (getCurrentTraceSegment ()->*getMethod) (address, val, newExpression);
}

template < typename T >
Expression *Trace::getSymbolicExpressionImplementation (T address,
    Expression *newExpression,
    typename TryToGetSymbolicExpressionMethod < T >
    ::TraceTypeWithoutConcreteValue tryToGetMethod,
    std::map < T, int > &generationIndices,
    typename GetSymbolicExpressionMethod < T >::
    TraceSegmentTypeWithoutConcreteValue getMethod) {
  Expression *exp = (this->*tryToGetMethod) (address);
  if (exp) {
    return exp;
  }
  // instantiate and set a new expression in the current segment
  typename std::map < T, int >::iterator it = generationIndices.find (address);
  if (it == generationIndices.end () || it->second < currentSegmentIndex) {
    generationIndices[address] = currentSegmentIndex;

  } else {
    // as there was no expression (regardless of concrete value), this case is impossible!
    throw std::runtime_error ("Trace::getSymbolicExpressionImplementation (...) method: "
                              "generation index is set while there is no expression!");
  }
  if (!newExpression) {
    /*
     * This getter, which ignores concrete value, is only called when the returned
     * expression is going to be set immediately. So previous value was random and
     * not significant. Thus we can use 0 as concrete value of instantiated expression.
     */
    newExpression = new Expression (address, 0, currentSegmentIndex);
  }
  return (getCurrentTraceSegment ()->*getMethod) (address, newExpression);
}

Expression *Trace::setSymbolicExpressionByRegister (REG reg, const Expression *exp) {
  return getCurrentTraceSegment ()->setSymbolicExpressionByRegister (reg, exp);
}

Expression *Trace::setSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
    const Expression *exp) {
  return getCurrentTraceSegment ()->setSymbolicExpressionByMemoryAddress (memoryEa, exp);
}

void Trace::addPathConstraint (const Constraint *c) {
  getCurrentTraceSegment ()->addPathConstraint (c);
}

void Trace::syscallInvoked (Syscall s) {
  throw std::runtime_error ("Trace::syscallInvoked: Not yet implemented");
}

bool Trace::saveToFile (const char *path) const {
  std::ofstream out;
  out.open (path, ios_base::out | ios_base::trunc | ios_base::binary);
  if (!out.is_open ()) {
    edu::sharif::twinner::util::Logger::error () << "Can not write trace info:"
        " Error in open function: " << path << '\n';
    return false;
  }
  if (currentSegmentIterator != segments.begin ()) {
    edu::sharif::twinner::util::Logger::warning () << "Some unvisited segments are found"
        " at front of the segments list\n";
  }
  saveToBinaryStream (out);
  out.close ();
  return true;
}

void Trace::saveToBinaryStream (std::ofstream &out) const {
  saveListToBinaryStream (out, "TRA", segments);
}

/**
 * Check for existence of path and loads a Trace from it. The Trace is newed
 * and the caller must delete it. If path does not exist, an exception will be raised.
 * @return A newed Trace object loaded from file "path".
 */
Trace *Trace::loadFromFile (const char *path) {
  throw std::runtime_error ("Trace::loadFromFile: Not yet implemented");
}

ExecutionTraceSegment *Trace::getCurrentTraceSegment () const {
  return *currentSegmentIterator;
}

void Trace::printRegistersValues (
    const edu::sharif::twinner::util::Logger &logger) const {
  int i = 0;
  for (std::list < ExecutionTraceSegment * >::const_iterator it = currentSegmentIterator;
      it != segments.end (); ++it) {
    const ExecutionTraceSegment *seg = *it;
    logger << "Segment #" << i++ << '\n';
    seg->printRegistersValues (logger);
  }
}

void Trace::printMemoryAddressesValues (
    const edu::sharif::twinner::util::Logger &logger) const {
  int i = 0;
  for (std::list < ExecutionTraceSegment * >::const_iterator it = currentSegmentIterator;
      it != segments.end (); ++it) {
    const ExecutionTraceSegment *seg = *it;
    logger << "Segment #" << i++;
    seg->printMemoryAddressesValues (logger);
  }
}

void Trace::printPathConstraints (
    const edu::sharif::twinner::util::Logger &logger) const {
  int i = 0;
  for (std::list < ExecutionTraceSegment * >::const_iterator it = currentSegmentIterator;
      it != segments.end (); ++it) {
    const ExecutionTraceSegment *seg = *it;
    logger << "Segment #" << i++;
    seg->printPathConstraints (logger);
  }
}

void Trace::printCompleteState (
    const edu::sharif::twinner::util::Logger &logger) const {
  int i = 0;
  for (std::list < ExecutionTraceSegment * >::const_iterator it = currentSegmentIterator;
      it != segments.end (); ++it) {
    const ExecutionTraceSegment *seg = *it;
    logger << "Segment #" << i++;
    seg->printCompleteState (logger);
  }
}

}
}
}
}
