//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2017 Behnam Momeni
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

#include "Snapshot.h"
#include "TimedTrace.h"

#include <list>
#include <set>
#include <map>

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

class MemoryManager;
}
namespace trace {

class SymbolRepresentation;
class Syscall;
class ExecutionTraceSegment;
class TraceSegmentTerminator;

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
   * The current iterator is wrapped in a TimedTrace object.
   */
  TimedTrace current;

  /**
   * This index indicates current generation starting from zero. This should not be
   * confused with an index in segments list (as segments list is in reverse order).
   */
  int currentSegmentIndex;

  /**
   * Indicates that a getter method has created a new symbol and
   * stored it in the requested address.
   * It should be propagated downwards to ensure that no two created symbols
   * may have a containing-enclosing relationship.
   * For memory addresses, it should be propagated lazily upwards too
   * in order to avoid reading unmapped memory.
   */
  bool needsPropagation;

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
      const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
      StateSummary &state);
  virtual Expression *tryToGetSymbolicExpressionByRegister (int size, REG reg,
      const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
      StateSummary &state,
      std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator);

  /**
   * Searches backwards to find queried values.
   */
  virtual Expression *tryToGetSymbolicExpressionByRegister (int size, REG reg);
  virtual Expression *tryToGetSymbolicExpressionByRegister (int size, REG reg,
      std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator);

  /**
   * Searches backwards to find queried values.
   * ASSERT: The precision of memval must match with precision of memoryEa location
   */
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa,
      const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
      StateSummary &state);
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa,
      const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
      StateSummary &state,
      std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator);

  /**
   * Searches backwards to find queried values.
   */
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa);
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa,
      std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator);

  /**
   * The getter searches segments backwards to find queried value.
   * ASSERT: The precision of regval must match with precision of reg
   */
  virtual Expression *getSymbolicExpressionByRegister (int size, REG reg,
      const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
      Expression *newExpression, StateSummary &state);
  virtual Expression *getSymbolicExpressionByRegister (int size, REG reg,
      const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
      Expression *newExpression, StateSummary &state,
      std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator);

  /**
   * The getter searches segments backwards to find queried value.
   */
  virtual Expression *getSymbolicExpressionByRegister (int size, REG reg,
      Expression *newExpression = 0);
  virtual Expression *getSymbolicExpressionByRegister (int size, REG reg,
      Expression *newExpression,
      std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator);

  /**
   * The getter searches segments backwards to find queried value.
   * ASSERT: The precision of memval must match with precision of memoryEa location
   */
  virtual Expression *getSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa,
      const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
      Expression *newExpression, StateSummary &state);
  virtual Expression *getSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa,
      const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
      Expression *newExpression, StateSummary &state,
      std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator);

  bool doesLastGetterCallNeedPropagation () const;

  /**
   * The getter searches segments backwards to find queried value.
   */
  virtual Expression *getSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
      Expression *newExpression = 0);
  virtual Expression *getSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
      Expression *newExpression,
      std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator);

  /**
   * The setter, uses most recent trace segment for setting the new value.
   * The exp will be casted to fit in reg.
   */
  virtual Expression *setSymbolicExpressionByRegister (int regsize, REG reg,
      const Expression *exp);
  virtual Expression *setSymbolicExpressionByRegister (int regsize, REG reg,
      const Expression *exp,
      std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator);

  /**
   * The setter, uses most recent trace segment for setting the new value.
   * The exp will be casted to fit in memoryEa location.
   */
  virtual Expression *setSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa, const Expression *exp);
  virtual Expression *setSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa,
      const Expression *exp,
      std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator);

  /**
   * The constraint will be added to the most recent trace segment.
   */
  virtual void addPathConstraints (
      const std::list <edu::sharif::twinner::trace::Constraint *> &c,
      const edu::sharif::twinner::trace::Constraint *lastConstraint = 0);
  virtual void addPathConstraints (
      const std::list <edu::sharif::twinner::trace::Constraint *> &c,
      const edu::sharif::twinner::trace::Constraint *lastConstraint,
      std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator);

  void terminateTraceSegment (TraceSegmentTerminator *tst);
  void initializeNewTraceSegment (CONTEXT *context) const;

  bool saveToFile (const char *path, const char *memoryPath) const;
  static Trace *loadFromFile (std::string path, std::string memoryPath);
  static bool saveAddressToValueMapToFile (
      const std::map < std::pair < ADDRINT, int >, UINT64 > &map,
      const char *path);
  static std::map < std::pair < ADDRINT, int >, UINT64 >
  loadAddressToValueMapFromFile (std::string path);

  const std::list < ExecutionTraceSegment * > &getTraceSegments () const;
  std::list < ExecutionTraceSegment * > &getTraceSegments ();

  Snapshot::snapshot_iterator begin () const;
  Snapshot::snapshot_iterator end () const;
  Snapshot::snapshot_reverse_iterator rbegin () const;
  Snapshot::snapshot_reverse_iterator rend () const;

  virtual void saveToBinaryStream (std::ofstream &out) const;

private:
  static Trace *loadFromBinaryStream (std::ifstream &in, const char *memoryPath);
  static void saveAddressToValueMapToBinaryStream (
      const std::map < std::pair < ADDRINT, int >, UINT64 > &map,
      std::ofstream &out);
  static std::map < std::pair < ADDRINT, int >, UINT64 >
  loadAddressToValueMapFromBinaryStream (std::ifstream &in);

public:
  virtual void printRegistersValues (
      const edu::sharif::twinner::util::Logger &logger) const;
  virtual void printRegistersValues (
      const edu::sharif::twinner::util::Logger &logger,
      std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) const;

  virtual void printMemoryAddressesValues (
      const edu::sharif::twinner::util::Logger &logger) const;
  virtual void printMemoryAddressesValues (
      const edu::sharif::twinner::util::Logger &logger,
      std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) const;

  virtual void printPathConstraints (
      const edu::sharif::twinner::util::Logger &logger) const;
  virtual void printPathConstraints (
      const edu::sharif::twinner::util::Logger &logger,
      std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) const;

  virtual void printCompleteState (
      const edu::sharif::twinner::util::Logger &logger) const;
  virtual void printCompleteState (
      const edu::sharif::twinner::util::Logger &logger,
      std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) const;

  int getCurrentSegmentIndex () const;

  edu::sharif::twinner::util::MemoryManager *getMemoryManager ();
  const edu::sharif::twinner::util::MemoryManager *getMemoryManager () const;
  void markCriticalAddresses ();
  void replaceTemporarySymbols ();

protected:
  void addNewSegment (ExecutionTraceSegment *segment);

private:
  std::set<SymbolRepresentation> aggregateTemporarySymbols (
      const std::list<const Expression *> &exps) const;
};

}
}
}
}

#endif /* Trace.h */
