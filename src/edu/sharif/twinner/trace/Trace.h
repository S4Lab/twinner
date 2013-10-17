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
#include <map>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Syscall;
class ExecutionTraceSegment;

class Trace : public ExecutionState {
private:
  // TODO: Add an iterator pointing to current segment instead of depending on the head of the list.
  // With an iterator, it's possible to create future segments and set symbols/expressions there, without
  // being worried that which segment is representing the current execution state.
  std::list < ExecutionTraceSegment * > segments;
  /**
   * The last used index (starting from one) for each symbol
   * which was created at a given memory address is kept here.
   */
  std::map < ADDRINT, int > memoryResidentSymbolsGenerationIndices;
  /**
   * The last used index (starting from one) for each symbol
   * which was created in a given register is kept here.
   */
  std::map < REG, int > registerResidentSymbolsGenerationIndices;

public:
  Trace ();
  ~Trace ();

  /**
   * Searches backwards to find queried values.
   */
  virtual const Expression *tryToGetSymbolicExpressionByRegister (REG reg,
      UINT64 regval) const throw (WrongStateException);

  /**
   * Searches backwards to find queried values.
   */
  virtual const Expression *tryToGetSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      UINT64 memval) const throw (WrongStateException);

  /**
   * The getter searches segments backwards to find queried value.
   */
  virtual const Expression *getSymbolicExpressionByRegister (REG reg, UINT64 regval,
      Expression *newExpression = 0);

  /**
   * The getter searches segments backwards to find queried value.
   */
  virtual const Expression *getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      UINT64 memval, Expression *newExpression = 0);

  /**
   * The setter, uses most recent trace segment for setting the new value.
   */
  virtual void setSymbolicExpressionByRegister (REG reg, const Expression *exp);

  /**
   * The setter, uses most recent trace segment for setting the new value.
   */
  virtual void setSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      const Expression *exp);

  /**
   * The constraint will be added to the most recent trace segment.
   */
  virtual void addPathConstraint (Constraint c);

  void syscallInvoked (Syscall s);

  void saveToFile (const char *path) const;
  static Trace *loadFromFile (const char *path);

private:
  ExecutionTraceSegment *getCurrentTraceSegment () const;

  template < typename T >
  struct TryToGetSymbolicExpressionMethod {

    typedef const Expression *(ExecutionTraceSegment::*TraceSegmentType) (T address,
        UINT64 val) const;
    typedef const Expression *(Trace::*TraceType) (T address, UINT64 val) const;
  };

  template < typename T >
  struct GetSymbolicExpressionMethod {

    typedef const Expression *(ExecutionTraceSegment::*TraceSegmentType) (T address,
        UINT64 val, Expression *newExpression);
  };

  template < typename T >
  const Expression *tryToGetSymbolicExpressionImplementation (T address, UINT64 val,
      typename TryToGetSymbolicExpressionMethod < T >::TraceSegmentType method) const
          throw (WrongStateException);

  template < typename T >
  const Expression *getSymbolicExpressionImplementation (T address, UINT64 val,
      Expression *newExpression,
      typename TryToGetSymbolicExpressionMethod < T >::TraceType tryToGetMethod,
      std::map < T, int > &generationIndices,
      typename GetSymbolicExpressionMethod < T >::TraceSegmentType getMethod);
};

}
}
}
}

#endif /* Trace.h */
