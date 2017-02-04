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

#ifndef TIMED_TRACE_H
#define TIMED_TRACE_H

#include "ExecutionState.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Trace;
class ExecutionTraceSegment;

class TimedTrace : public ExecutionState {
private:
  Trace *trace;
  std::list < ExecutionTraceSegment * >::iterator it;

public:
  TimedTrace ();
  TimedTrace (Trace *trace, std::list < ExecutionTraceSegment * >::iterator it);
  virtual ~TimedTrace ();

  std::list < ExecutionTraceSegment * >::iterator getSegmentIterator () const {
    return it;
  }

  virtual Expression *tryToGetSymbolicExpressionByRegister (int size,
      REG reg,
      const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
      StateSummary &state);
  virtual Expression *tryToGetSymbolicExpressionByRegister (int size,
      REG reg);

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

  virtual Expression *getSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa,
      const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
      Expression *newExpression, StateSummary &state);
  virtual Expression *getSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa,
      Expression *newExpression);

  bool doesLastGetterCallNeedPropagation () const;

  virtual Expression *setSymbolicExpressionByRegister (int regsize, REG reg,
      const Expression *exp);
  virtual Expression *setSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa,
      const Expression *exp);

  virtual void addPathConstraints (
      const std::list <edu::sharif::twinner::trace::Constraint *> &c,
      const edu::sharif::twinner::trace::Constraint *lastConstraint);

  virtual void printRegistersValues (
      const edu::sharif::twinner::util::Logger &logger) const;
  virtual void printMemoryAddressesValues (
      const edu::sharif::twinner::util::Logger &logger) const;
  virtual void printPathConstraints (
      const edu::sharif::twinner::util::Logger &logger) const;
  virtual void printCompleteState (
      const edu::sharif::twinner::util::Logger &logger) const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
};

}
}
}
}

#endif /* TimedTrace.h */
