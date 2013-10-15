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

  virtual const Expression *tryToGetSymbolicExpressionByRegister (REG reg,
      UINT64 regval) const throw (WrongStateException);
  virtual const Expression *tryToGetSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      UINT64 memval) const throw (WrongStateException);

  virtual const Expression *getSymbolicExpressionByRegister (REG reg, UINT64 regval);
  virtual const Expression *getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      UINT64 memval);

  virtual void setSymbolicExpressionByRegister (REG reg, const Expression *exp);
  virtual void setSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      const Expression *exp);

  virtual void addPathConstraint (Constraint c);

private:
  template < typename KEY >
  const Expression *tryToGetSymbolicExpressionImplementation (
      const std::map < KEY, Expression * > &map, const KEY key, UINT64 concreteVal) const
          throw (WrongStateException);
  template < typename KEY >
  const Expression *getSymbolicExpressionImplementation (
      std::map < KEY, Expression * > &map, const KEY key, UINT64 currentConcreteValue);
  template < typename KEY >
  void setSymbolicExpressionImplementation (std::map < KEY, Expression * > &map,
      const KEY key, const Expression *exp);
};

}
}
}
}

#endif /* ExecutionTraceSegment.h */
