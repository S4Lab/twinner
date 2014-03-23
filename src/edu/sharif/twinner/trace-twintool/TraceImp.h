//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2014  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef TRACE_FOR_TWINTOOL_H
#define TRACE_FOR_TWINTOOL_H

#include "edu/sharif/twinner/trace/Trace.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class TraceImp : public Trace {

public:
  TraceImp ();
  TraceImp (std::ifstream &symbolsFileInputStream);
  virtual ~TraceImp ();

  /**
   * Searches backwards to find queried values.
   */
  virtual Expression *tryToGetSymbolicExpressionByRegister (REG reg,
      const ConcreteValue &regval) const throw (WrongStateException);

  /**
   * Searches backwards to find queried values.
   */
  virtual Expression *tryToGetSymbolicExpressionByRegister (REG reg) const;

  /**
   * Searches backwards to find queried values.
   */
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      const ConcreteValue &memval) const throw (WrongStateException);

  /**
   * Searches backwards to find queried values.
   */
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (ADDRINT memoryEa) const;

  /**
   * The getter searches segments backwards to find queried value.
   */
  virtual Expression *getSymbolicExpressionByRegister (REG reg,
      const ConcreteValue &regval, Expression *newExpression = 0);

  /**
   * The getter searches segments backwards to find queried value.
   */
  virtual Expression *getSymbolicExpressionByRegister (REG reg,
      Expression *newExpression = 0);

  /**
   * The getter searches segments backwards to find queried value.
   */
  virtual Expression *getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      const ConcreteValue &memval, Expression *newExpression = 0);

  /**
   * The getter searches segments backwards to find queried value.
   */
  virtual Expression *getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      Expression *newExpression = 0);

private:

  template < typename T >
  struct TryToGetSymbolicExpressionMethod {

    typedef Expression *(ExecutionTraceSegment::*TraceSegmentType) (T address,
        const ConcreteValue &val) const;
    typedef Expression *(ExecutionTraceSegment::*TraceSegmentTypeWithoutConcreteValue) (
        T address) const;
    typedef Expression *(TraceImp::*TraceType) (T address,
        const ConcreteValue &val) const;
    typedef Expression *(TraceImp::*TraceTypeWithoutConcreteValue) (T address) const;
  };

  template < typename T >
  struct GetSymbolicExpressionMethod {

    typedef Expression *(ExecutionTraceSegment::*TraceSegmentType) (T address,
        const ConcreteValue &val, Expression *newExpression);
    typedef Expression *(ExecutionTraceSegment::*TraceSegmentTypeWithoutConcreteValue) (
        T address, Expression *newExpression);
  };

  template < typename T >
  Expression *tryToGetSymbolicExpressionImplementation (T address,
      const ConcreteValue &val,
      typename TryToGetSymbolicExpressionMethod < T >::TraceSegmentType method) const
  throw (WrongStateException);
  template < typename T >
  Expression *tryToGetSymbolicExpressionImplementation (T address,
      typename TryToGetSymbolicExpressionMethod < T >::
      TraceSegmentTypeWithoutConcreteValue method) const;

  template < typename T >
  Expression *getSymbolicExpressionImplementation (T address, const ConcreteValue &val,
      Expression *newExpression,
      typename TryToGetSymbolicExpressionMethod < T >::TraceType tryToGetMethod,
      typename GetSymbolicExpressionMethod < T >::TraceSegmentType getMethod);
  template < typename T >
  Expression *getSymbolicExpressionImplementation (T address,
      Expression *newExpression,
      typename TryToGetSymbolicExpressionMethod < T >::
      TraceTypeWithoutConcreteValue tryToGetMethod,
      typename GetSymbolicExpressionMethod < T >::
      TraceSegmentTypeWithoutConcreteValue getMethod);

  void loadInitializedSymbolsFromBinaryStream (std::ifstream &in);
  ExecutionTraceSegment *loadSingleSegmentSymbolsRecordsFromBinaryStream (int index,
      std::ifstream &in);
};

}
}
}
}

#endif /* TraceImp.h */
