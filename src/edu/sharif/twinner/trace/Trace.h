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
namespace util {

class MemoryManager;
}
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
   */
  int currentSegmentIndex;

  /**
   * Manages memory and allows allocation/deallocation from a pre-allocated part of the
   * memory. Used for keeping strings of disassembled instructions.
   */
  edu::sharif::twinner::util::MemoryManager *memoryManager;

  /**
   * Dummy constructor: Allowing child class to do all initializations
   */
  Trace (int) {
  }

private:
  Trace (const std::list < ExecutionTraceSegment * > &list,
      edu::sharif::twinner::util::MemoryManager *memoryManager);

public:
  Trace ();
  virtual ~Trace ();

  /**
   * Searches backwards to find queried values.
   * ASSERT: The precision of regval must match with precision of reg
   */
  virtual Expression *tryToGetSymbolicExpressionByRegister (int size, REG reg,
      const ConcreteValue &regval) const /* @throw (WrongStateException) */;

  /**
   * Searches backwards to find queried values.
   */
  virtual Expression *tryToGetSymbolicExpressionByRegister (int size, REG reg) const;

  /**
   * Searches backwards to find queried values.
   * ASSERT: The precision of memval must match with precision of memoryEa location
   */
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa, const ConcreteValue &memval) const
  /* @throw (WrongStateException) */;

  /**
   * Searches backwards to find queried values.
   */
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa) const;

  /**
   * The getter searches segments backwards to find queried value.
   * ASSERT: The precision of regval must match with precision of reg
   */
  virtual Expression *getSymbolicExpressionByRegister (int size, REG reg,
      const ConcreteValue &regval, Expression *newExpression = 0)
  /* @throw (UnexpectedChangeException) */;

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
      const ConcreteValue &memval, Expression *newExpression = 0)
  /* @throw (UnexpectedChangeException) */;

  /**
   * The getter searches segments backwards to find queried value.
   */
  virtual Expression *getSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
      Expression *newExpression = 0);

  /**
   * The setter, uses most recent trace segment for setting the new value.
   * The exp will be casted to fit in reg.
   */
  virtual Expression *setSymbolicExpressionByRegister (int regsize, REG reg,
      const Expression *exp);

  /**
   * The setter, uses most recent trace segment for setting the new value.
   * The exp will be casted to fit in memoryEa location.
   */
  virtual Expression *setSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa, const Expression *exp);

  /**
   * The constraint will be added to the most recent trace segment.
   */
  virtual void addPathConstraints (
      const std::list <edu::sharif::twinner::trace::Constraint *> &c);

  void syscallInvoked (Syscall s);
  void syscallReturned (CONTEXT *context) const;

  bool saveToFile (const char *path, const char *memoryPath) const;
  static Trace *loadFromFile (const char *path, const char *memoryPath);
  static bool saveAddressToValueMapToFile (const std::map < ADDRINT, UINT64 > &map,
      const char *path);
  static map < ADDRINT, UINT64 > loadAddressToValueMapFromFile (const char *path);

  const std::list < ExecutionTraceSegment * > &getTraceSegments () const;

private:
  void saveToBinaryStream (std::ofstream &out) const;
  static Trace *loadFromBinaryStream (std::ifstream &in, const char *memoryPath);
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

  int getCurrentSegmentIndex () const;

  edu::sharif::twinner::util::MemoryManager *getMemoryManager ();
  const edu::sharif::twinner::util::MemoryManager *getMemoryManager () const;
};

}
}
}
}

#endif /* Trace.h */
