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

#ifndef EXECUTION_TRACE_SEGMENT_H
#define EXECUTION_TRACE_SEGMENT_H

#include "ExecutionState.h"

#include "TimedTrace.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class TraceSegmentTerminator;
class Snapshot;

class ExecutionTraceSegment : public ExecutionState {
private:
  std::list < Snapshot * > snapshots;
  TraceSegmentTerminator *terminator;

  int segmentIndex;

  TimedTrace timedTrace;

  ExecutionTraceSegment (int index,
      const std::list < Snapshot * > &snas,
      TraceSegmentTerminator *terminator);

public:
  ExecutionTraceSegment (int index);
  ExecutionTraceSegment (int index,
      const std::map < REG, Expression * > &regMap,
      const std::map < ADDRINT, Expression * > &memMap);
  virtual ~ExecutionTraceSegment ();

  void setTimedTrace (TimedTrace timedTrace);

  virtual Expression *tryToGetSymbolicExpressionByRegister (int size, REG reg,
      const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
      StateSummary &state);
  virtual Expression *tryToGetSymbolicExpressionByRegister (int size, REG reg);
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa,
      const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
      StateSummary &state);
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa);

  virtual Expression *getSymbolicExpressionByRegister (int size, REG reg,
      const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
      Expression *newExpression, StateSummary &state);
  virtual Expression *getSymbolicExpressionByRegister (int size, REG reg,
      Expression *newExpression);
  virtual Expression *getSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
      const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
      Expression *newExpression, StateSummary &state);
  virtual Expression *getSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
      Expression *newExpression);

  virtual Expression *setSymbolicExpressionByRegister (int regsize, REG reg,
      const Expression *exp);
  virtual Expression *setSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
      const Expression *exp);

  virtual void addPathConstraints (
      const std::list <edu::sharif::twinner::trace::Constraint *> &c,
      const edu::sharif::twinner::trace::Constraint *lastConstraint);

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static ExecutionTraceSegment *loadFromBinaryStream (std::ifstream &in);

  int getSegmentIndex () const;

  virtual void printRegistersValues (
      const edu::sharif::twinner::util::Logger &logger) const;
  virtual void printMemoryAddressesValues (
      const edu::sharif::twinner::util::Logger &logger) const;
  virtual void printPathConstraints (
      const edu::sharif::twinner::util::Logger &logger) const;
  virtual void printCompleteState (
      const edu::sharif::twinner::util::Logger &logger) const;

  const std::map < REG, Expression * > &getFirstRegisterToExpression () const;
  const std::map < REG, Expression * > &getRegisterToExpression () const;
  const std::map < ADDRINT, Expression * > &getMemoryAddressTo64BitsExpression () const;
  const std::map < ADDRINT, Expression * > &getMemoryAddressTo8BitsExpression () const;
  std::list < Constraint * > getPathConstraints () const;
  const Constraint *getLastPathConstraint () const;

  const std::list < Snapshot * > &getSnapshots () const;

  void setTerminator (TraceSegmentTerminator *terminator);
  const TraceSegmentTerminator *getTerminator () const;

  void addTemporaryExpressions (const ExecutionTraceSegment *seg,
      REG fullReg, int size);
  void addTemporaryExpressions (const ExecutionTraceSegment *seg,
      ADDRINT address, int size);

private:
  void addNewSnapshot (Snapshot *snapshot);
};

}
}
}
}

#endif /* ExecutionTraceSegment.h */
