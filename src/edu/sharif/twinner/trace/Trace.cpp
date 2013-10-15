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

const Expression *Trace::tryToGetSymbolicExpressionByRegister (REG reg,
    UINT64 regval) const throw (WrongStateException) {
  for (std::list < ExecutionTraceSegment * >::const_iterator it = segments.begin ();
      it != segments.end (); ++it) { // searches segments starting from the most recent one
    const ExecutionTraceSegment *seg = *it;
    const Expression *exp = seg->tryToGetSymbolicExpressionByRegister (reg, regval);
    if (exp) {
      return exp;
    }
  }
  return 0;
}

const Expression *Trace::tryToGetSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
    UINT64 memval) const throw (WrongStateException) {
  for (std::list < ExecutionTraceSegment * >::const_iterator it = segments.begin ();
      it != segments.end (); ++it) { // searches segments starting from the most recent one
    const ExecutionTraceSegment *seg = *it;
    const Expression *exp = seg->tryToGetSymbolicExpressionByMemoryAddress (memoryEa,
        memval);
    if (exp) {
      return exp;
    }
  }
  return 0;
}

const Expression *Trace::getSymbolicExpressionByRegister (REG reg, UINT64 regval) {
  try {
    const Expression *exp = tryToGetSymbolicExpressionByRegister (reg, regval);
    if (exp) {
      return exp;
    }
  } catch (const WrongStateException &e) {
  }
  // instantiate and set a new expression in the most recent segment
  return getCurrentTraceSegment ()->getSymbolicExpressionByRegister (reg, regval);
}

const Expression *Trace::getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
    UINT64 memval) {
  try {
    const Expression *exp = tryToGetSymbolicExpressionByMemoryAddress (memoryEa, memval);
    if (exp) {
      return exp;
    }
  } catch (const WrongStateException &e) {
  }
  // instantiate and set a new expression in the most recent segment
  return getCurrentTraceSegment ()->getSymbolicExpressionByMemoryAddress (memoryEa,
      memval);
}

void Trace::setSymbolicExpressionByRegister (REG reg, const Expression *exp) {
  getCurrentTraceSegment ()->setSymbolicExpressionByRegister (reg, exp);
}

void Trace::setSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
    const Expression *exp) {
  getCurrentTraceSegment ()->setSymbolicExpressionByMemoryAddress (memoryEa, exp);
}

void Trace::addPathConstraint (Constraint c) {
  getCurrentTraceSegment ()->addPathConstraint (c);
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

ExecutionTraceSegment *Trace::getCurrentTraceSegment () const {
  return segments.front ();
}

}
}
}
}
