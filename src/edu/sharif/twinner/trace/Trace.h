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
  /**
   * Most recent segment is kept at front of list.
   * Newer segments should be push_front()ed.
   */
  std::list < ExecutionTraceSegment * > segments;

  /*
   * With an iterator, it's possible to create future segments and
   * set symbols/expressions there, without being worried that which segment
   * is representing the current execution state.
   * The past is segments.last() -> current
   * Future is current -> segments.front()
   */
  std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator;

  /**
   * This index indicates current generation starting from zero. This should not be
   * confused with an index in segments list (as segments list is in reverse order).
   * This generation index matches with values kept in
   * memoryResidentSymbolsGenerationIndices and registerResidentSymbolsGenerationIndices.
   */
  int currentSegmentIndex;

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

  Trace (const std::list < ExecutionTraceSegment * > &list);

public:
  Trace ();
  ~Trace ();

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

  /**
   * The setter, uses most recent trace segment for setting the new value.
   */
  virtual Expression *setSymbolicExpressionByRegister (REG reg, const Expression *exp);

  /**
   * The setter, uses most recent trace segment for setting the new value.
   */
  virtual Expression *setSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      const Expression *exp);

  /**
   * The constraint will be added to the most recent trace segment.
   */
  virtual void addPathConstraint (const Constraint *c);

  void syscallInvoked (Syscall s);

  bool saveToFile (const char *path) const;
  static Trace *loadFromFile (const char *path);
  static Trace *loadFromBinaryStream (std::ifstream &in);

  virtual void saveToBinaryStream (std::ofstream &out) const;

private:
  ExecutionTraceSegment *getCurrentTraceSegment () const;

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

public:
  virtual void printRegistersValues (
      const edu::sharif::twinner::util::Logger &logger) const;
  virtual void printMemoryAddressesValues (
      const edu::sharif::twinner::util::Logger &logger) const;
  virtual void printPathConstraints (
      const edu::sharif::twinner::util::Logger &logger) const;
  virtual void printCompleteState (
      const edu::sharif::twinner::util::Logger &logger) const;
};

}
}
}
}

#endif /* Trace.h */
