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

private:
  /**
   * The last used index (starting from zero) for each symbol
   * which was created at a given memory address is kept here.
   * A new expression may emerge out of an address iff either there is no stored index
   * for that address or the stored index is less than current generation/segment index.
   */
  std::map < ADDRINT, int > memoryResidentSymbolsGenerationIndices;

  /**
   * The last used index (starting from zero) for each symbol
   * which was created in a given register is kept here.
   * A new expression may emerge out of an address iff either there is no stored index
   * for that address or the stored index is less than current generation/segment index.
   */
  std::map < REG, int > registerResidentSymbolsGenerationIndices;

public:
  TraceImp (std::ifstream &symbolsFileInputStream);
  virtual ~TraceImp ();

  /**
   * Searches backwards to find queried values.
   */
  virtual Expression *tryToGetSymbolicExpressionByRegister (REG reg, UINT64 regval)
  throw (WrongStateException);

  /**
   * Searches backwards to find queried values.
   */
  virtual Expression *tryToGetSymbolicExpressionByRegister (REG reg);

  /**
   * Searches backwards to find queried values.
   */
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      UINT64 memval) throw (WrongStateException);

  /**
   * The getter searches segments backwards to find queried value.
   */
  virtual Expression *getSymbolicExpressionByRegister (REG reg, UINT64 regval,
      Expression *newExpression = 0);

  /**
   * The getter searches segments backwards to find queried value.
   */
  virtual Expression *getSymbolicExpressionByRegister (REG reg,
      Expression *newExpression = 0);

  /**
   * The getter searches segments backwards to find queried value.
   */
  virtual Expression *getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      UINT64 memval, Expression *newExpression = 0);

private:

  template < typename T >
  struct TryToGetSymbolicExpressionMethod {

    typedef Expression *(ExecutionTraceSegment::*TraceSegmentType) (T address,
        UINT64 val);
    typedef Expression *(ExecutionTraceSegment::*TraceSegmentTypeWithoutConcreteValue) (
        T address);
    typedef Expression *(Trace::*TraceType) (T address, UINT64 val);
    typedef Expression *(Trace::*TraceTypeWithoutConcreteValue) (T address);
  };

  template < typename T >
  struct GetSymbolicExpressionMethod {

    typedef Expression *(ExecutionTraceSegment::*TraceSegmentType) (T address, UINT64 val,
        Expression *newExpression);
    typedef Expression *(ExecutionTraceSegment::*TraceSegmentTypeWithoutConcreteValue) (
        T address, Expression *newExpression);
  };

  template < typename T >
  Expression *tryToGetSymbolicExpressionImplementation (T address, UINT64 val,
      typename TryToGetSymbolicExpressionMethod < T >::TraceSegmentType method)
  throw (WrongStateException);
  template < typename T >
  Expression *tryToGetSymbolicExpressionImplementation (T address,
      typename TryToGetSymbolicExpressionMethod < T >::
      TraceSegmentTypeWithoutConcreteValue method);

  template < typename T >
  Expression *getSymbolicExpressionImplementation (T address, UINT64 val,
      Expression *newExpression,
      typename TryToGetSymbolicExpressionMethod < T >::TraceType tryToGetMethod,
      std::map < T, int > &generationIndices,
      typename GetSymbolicExpressionMethod < T >::TraceSegmentType getMethod);
  template < typename T >
  Expression *getSymbolicExpressionImplementation (T address,
      Expression *newExpression,
      typename TryToGetSymbolicExpressionMethod < T >::
      TraceTypeWithoutConcreteValue tryToGetMethod,
      std::map < T, int > &generationIndices,
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