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

#include "pin.H"

#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Expression;
class Constraint;
class Syscall;
class ExecutionTraceSegment;

class Trace {
private:
  std::list < ExecutionTraceSegment * > segments;

public:
  Trace ();
  ~Trace ();

  /**
   * The getter searches segments backwards and may allocate new symbols (side effects) if required.
   * Returned expression is owned by Trace object.
   */
  Expression getSymbolicExpressionByRegister (REG reg);
  Expression getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa);

  /**
   * The setters, clone input expression and take ownership of the newly allocated one.
   */
  void setSymbolicExpressionByRegister (REG reg, Expression exp);
  void setSymbolicExpressionByMemoryAddress (ADDRINT memoryEa, Expression exp);

  void addPathConstraint (Constraint c);

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
