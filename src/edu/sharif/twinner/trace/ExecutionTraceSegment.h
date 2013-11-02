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

  virtual Expression *tryToGetSymbolicExpressionByRegister (REG reg, UINT64 regval)
  throw (WrongStateException);
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      UINT64 memval) throw (WrongStateException);

  virtual Expression *getSymbolicExpressionByRegister (REG reg, UINT64 regval,
      Expression *newExpression);
  virtual Expression *getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      UINT64 memval, Expression *newExpression);

  virtual void setSymbolicExpressionByRegister (REG reg, const Expression *exp);
  virtual void setSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      const Expression *exp);

  virtual void addPathConstraint (Constraint c);

private:
  template < typename KEY >
  Expression *tryToGetSymbolicExpressionImplementation (
      std::map < KEY, Expression * > &map, const KEY key, UINT64 concreteVal) const
  throw (WrongStateException);
  template < typename KEY >
  Expression *getSymbolicExpressionImplementation (std::map < KEY, Expression * > &map,
      const KEY key, UINT64 currentConcreteValue, Expression *newExpression);
  template < typename KEY >
  void setSymbolicExpressionImplementation (std::map < KEY, Expression * > &map,
      const KEY key, const Expression *exp);
};

}
}
}
}

#endif /* ExecutionTraceSegment.h */