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

#include <map>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class ExecutionTraceSegment : public ExecutionState {
private:
  std::map < REG, Expression * > registerToExpression;
  std::map < ADDRINT, Expression * > memoryAddressToExpression;

public:
  ExecutionTraceSegment ();
  virtual ~ExecutionTraceSegment ();

  virtual const Expression *tryToGetSymbolicExpressionByRegister (REG reg) const;
  virtual const Expression *tryToGetSymbolicExpressionByMemoryAddress (
      ADDRINT memoryEa) const;

  virtual const Expression *getSymbolicExpressionByRegister (REG reg);
  virtual const Expression *getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa);

  virtual void setSymbolicExpressionByRegister (REG reg, const Expression *exp);
  virtual void setSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      const Expression *exp);

  virtual void addPathConstraint (Constraint c);

private:
  template < typename KEY >
  const Expression *tryToGetSymbolicExpressionImplementation (
      const std::map < KEY, Expression * > &map, KEY key) const;
  template < typename KEY >
  const Expression *getSymbolicExpressionImplementation (
      std::map < KEY, Expression * > &map, KEY key);
  template < typename KEY >
  void setSymbolicExpressionImplementation (std::map < KEY, Expression * > &map, KEY key,
      const Expression *exp);
};

}
}
}
}

#endif /* ExecutionTraceSegment.h */
