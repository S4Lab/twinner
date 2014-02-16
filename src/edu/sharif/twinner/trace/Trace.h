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

protected:
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
   * memoryResidentSymbolsGenerationIndices and registerResidentSymbolsGenerationIndices
   * in TraceImp class.
   */
  int currentSegmentIndex;

  /**
   * Dummy constructor: Allowing child class to do all initializations
   */
  Trace (int) {
  }

private:
  Trace (const std::list < ExecutionTraceSegment * > &list);

public:
  Trace ();
  virtual ~Trace ();

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
  static bool saveAddressToValueMapToFile (const std::map < ADDRINT, UINT64 > &map,
      const char *path);
  static map < ADDRINT, UINT64 > loadAddressToValueMapFromFile (const char *path);

  const std::list < ExecutionTraceSegment * > &getTraceSegments () const;

private:
  void saveToBinaryStream (std::ofstream &out) const;
  static Trace *loadFromBinaryStream (std::ifstream &in);
  static void saveAddressToValueMapToBinaryStream (
      const std::map < ADDRINT, UINT64 > &map, std::ofstream &out);
  static map < ADDRINT, UINT64 > loadAddressToValueMapFromBinaryStream (
      std::ifstream &in);

protected:

  inline ExecutionTraceSegment *getCurrentTraceSegment () const {
    return *currentSegmentIterator;
  }

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
