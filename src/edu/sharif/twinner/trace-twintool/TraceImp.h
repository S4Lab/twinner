//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2016 Behnam Momeni
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
  TraceImp (std::stringstream &symbolsFileInputStream,
      edu::sharif::twinner::util::MemoryManager *_memoryManager);
  virtual ~TraceImp ();

  /**
   * Searches backwards to find queried values.
   */
  virtual Expression *tryToGetSymbolicExpressionByRegister (int size, REG reg,
      const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
      StateSummary &state);

  /**
   * Searches backwards to find queried values.
   */
  virtual Expression *tryToGetSymbolicExpressionByRegister (int size, REG reg);

  /**
   * Searches backwards to find queried values.
   */
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa,
      const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
      StateSummary &state);

  /**
   * Searches backwards to find queried values.
   */
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa);

  /**
   * The getter searches segments backwards to find queried value.
   * ASSERT: The precision of regval must match with precision of reg
   */
  virtual Expression *getSymbolicExpressionByRegister (int size, REG reg,
      const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
      Expression *newExpression, StateSummary &state);

  /**
   * The getter searches segments backwards to find queried value.
   */
  virtual Expression *getSymbolicExpressionByRegister (int size, REG reg,
      Expression *newExpression = 0);

  /**
   * The getter searches segments backwards to find queried value.
   * ASSERT: The precision of memval must match with precision of memoryEa location
   */
  virtual Expression *getSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
      const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
      Expression *newExpression, StateSummary &state);

  /**
   * The getter searches segments backwards to find queried value.
   */
  virtual Expression *getSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
      Expression *newExpression = 0);

private:

  template < typename T >
  struct TryToGetSymbolicExpressionMethod {
    typedef Expression *(ExecutionTraceSegment::*TraceSegmentType) (int size, T address,
        const edu::sharif::twinner::trace::cv::ConcreteValue &val,
        StateSummary &state);
    typedef Expression *(ExecutionTraceSegment::*TraceSegmentTypeWithoutConcreteValue) (
        int size, T address);
    typedef Expression *(TraceImp::*TraceType) (int size, T address,
        const edu::sharif::twinner::trace::cv::ConcreteValue &val,
        StateSummary &state);
    typedef Expression *(TraceImp::*TraceTypeWithoutConcreteValue) (int size,
        T address);
  };

  template < typename T >
  struct GetSymbolicExpressionMethod {
    typedef Expression *(ExecutionTraceSegment::*TraceSegmentType) (int size, T address,
        const edu::sharif::twinner::trace::cv::ConcreteValue &val,
        Expression *newExpression, StateSummary &state);
    typedef Expression *(ExecutionTraceSegment::*TraceSegmentTypeWithoutConcreteValue) (
        int size, T address, Expression *newExpression);
  };

  template < typename T >
  Expression *tryToGetSymbolicExpressionImplementation (int size, T address,
      const edu::sharif::twinner::trace::cv::ConcreteValue &val,
      typename TryToGetSymbolicExpressionMethod < T >::TraceSegmentType method,
      StateSummary &state);
  template < typename T >
  Expression *tryToGetSymbolicExpressionImplementation (int size, T address,
      typename TryToGetSymbolicExpressionMethod < T >::
      TraceSegmentTypeWithoutConcreteValue method);

  template < typename T >
  Expression *getSymbolicExpressionImplementation (int size, T address,
      const edu::sharif::twinner::trace::cv::ConcreteValue &val,
      Expression *newExpression,
      typename TryToGetSymbolicExpressionMethod < T >::TraceType tryToGetMethod,
      typename GetSymbolicExpressionMethod < T >::TraceSegmentType getMethod,
      StateSummary &state);
  template < typename T >
  Expression *getSymbolicExpressionImplementation (int size, T address,
      Expression *newExpression,
      typename TryToGetSymbolicExpressionMethod < T >::
      TraceTypeWithoutConcreteValue tryToGetMethod,
      typename GetSymbolicExpressionMethod < T >::
      TraceSegmentTypeWithoutConcreteValue getMethod);

  void loadInitializedSymbolsFromBinaryStream (std::stringstream &in);
  ExecutionTraceSegment *loadSingleSegmentSymbolsRecordsFromBinaryStream (int index,
      std::stringstream &in);

  Expression *instantiateExpression (REG addr,
      const edu::sharif::twinner::trace::cv::ConcreteValue &val, int index);
  Expression *instantiateExpression (ADDRINT addr,
      const edu::sharif::twinner::trace::cv::ConcreteValue &val, int index);
};

}
}
}
}

#endif /* TraceImp.h */
