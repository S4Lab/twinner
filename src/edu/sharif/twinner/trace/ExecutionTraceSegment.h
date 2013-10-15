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

#ifndef EXECUTION_TRACE_SEGMENT_H
#define EXECUTION_TRACE_SEGMENT_H

#include "ExecutionState.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class ExecutionTraceSegment : public ExecutionState {
public:
  virtual const Expression *tryToGetSymbolicExpressionByRegister (REG reg) const;
  virtual const Expression *tryToGetSymbolicExpressionByMemoryAddress (
      ADDRINT memoryEa) const;

  virtual const Expression *getSymbolicExpressionByRegister (REG reg);
  virtual const Expression *getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa);

  virtual void setSymbolicExpressionByRegister (REG reg, const Expression *exp);
  virtual void setSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      const Expression *exp);

  virtual void addPathConstraint (Constraint c);
};

}
}
}
}

#endif /* ExecutionTraceSegment.h */
