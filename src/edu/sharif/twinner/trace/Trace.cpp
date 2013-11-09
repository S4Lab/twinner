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

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

Trace::Trace () {
  segments.push_front (new ExecutionTraceSegment ());
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
  for (std::list < ExecutionTraceSegment * >::iterator it = segments.begin ();
      it != segments.end (); ++it) {
    // searches segments starting from the most recent one
    ExecutionTraceSegment *seg = *it;
    Expression *exp = (seg->*method) (address, val);
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

Expression *Trace::getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa, UINT64 memval,
    Expression *newExpression) {
  return getSymbolicExpressionImplementation
      (memoryEa, memval, newExpression,
       &Trace::tryToGetSymbolicExpressionByMemoryAddress,
       memoryResidentSymbolsGenerationIndices,
       &ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress);
}

template < typename T >
Expression *Trace::getSymbolicExpressionImplementation (T address, UINT64 val,
    Expression *newExpression,
    typename TryToGetSymbolicExpressionMethod < T >::TraceType tryToGetMethod,
    std::map < T, int > &generationIndices,
    typename GetSymbolicExpressionMethod < T >::TraceSegmentType getMethod) {
  try {
    Expression *exp = (this->*tryToGetMethod) (address, val);
    if (exp) {
      return exp;
    }
  } catch (const WrongStateException &e) {
  }
  // instantiate and set a new expression in the most recent segment
  int index = ++generationIndices[address];
  if (!newExpression) {
    newExpression = new Expression (address, val, index);
  }
  return (getCurrentTraceSegment ()->*getMethod) (address, val, newExpression);
}

void Trace::setSymbolicExpressionByRegister (REG reg, const Expression *exp) {
  getCurrentTraceSegment ()->setSymbolicExpressionByRegister (reg, exp);
}

void Trace::setSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
    const Expression *exp) {
  getCurrentTraceSegment ()->setSymbolicExpressionByMemoryAddress (memoryEa, exp);
}

void Trace::addPathConstraint (const Constraint *c) {
  getCurrentTraceSegment ()->addPathConstraint (c);
}

void Trace::syscallInvoked (Syscall s) {
  throw std::runtime_error ("Trace::syscallInvoked: Not yet implemented");
}

void Trace::saveToFile (const char *path) const {
  std::ofstream out;
  out.open (path, ios_base::out | ios_base::trunc | ios_base::binary);
  if (!out.is_open ()) {
    edu::sharif::twinner::util::Logger::error () << "Can not write trace info:"
        " (error in open function)\n";
    return;
  }
  //TODO: Implement...
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
  return segments.front ();
}

}
}
}
}
