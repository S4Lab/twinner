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

#ifndef TRACE_H
#define TRACE_H

#include "ExecutionState.h"

#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Syscall;
class ExecutionTraceSegment;

class Trace : public ExecutionState {
private:
  std::list < ExecutionTraceSegment * > segments;

public:
  Trace ();
  ~Trace ();

  /**
   * The getter searches segments backwards to find queries value.
   */
  virtual Expression getSymbolicExpressionByRegister (REG reg);

  /**
   * The getter searches segments backwards to find queries value.
   */
  virtual Expression getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa);

  /**
   * The setter, uses most recent trace segment for setting the new value.
   */
  virtual void setSymbolicExpressionByRegister (REG reg, Expression exp);

  /**
   * The setter, uses most recent trace segment for setting the new value.
   */
  virtual void setSymbolicExpressionByMemoryAddress (ADDRINT memoryEa, Expression exp);

  /**
   * The constraint will be added to the most recent trace segment.
   */
  virtual void addPathConstraint (Constraint c);

  void syscallInvoked (Syscall s);

  void saveToFile (const char *path) const;
  static Trace *loadFromFile (const char *path);

private:
  ExecutionTraceSegment *getCurrentTraceSegment () const;
};

}
}
}
}

#endif /* Trace.h */
