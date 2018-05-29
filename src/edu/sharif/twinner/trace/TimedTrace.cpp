//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2018 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "TimedTrace.h"
#include "Trace.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

TimedTrace::TimedTrace () :
    trace (0) {
}

TimedTrace::TimedTrace (Trace *_trace,
    std::list < ExecutionTraceSegment * >::iterator _it) :
    trace (_trace), it (_it) {
}

TimedTrace::~TimedTrace () {
}

TimedTrace *TimedTrace::clone () const {
  Trace *clonedTrace = trace->clone ();
  std::list < ExecutionTraceSegment * > &segments = trace->getTraceSegments ();
  std::list < ExecutionTraceSegment * > &clonedSegments =
      clonedTrace->getTraceSegments ();
  for (std::list < ExecutionTraceSegment * >::iterator oit = segments.begin (),
      cit = clonedSegments.begin (); oit != segments.end (); ++oit, ++cit) {
    if (it == oit) {
      return new TimedTrace (clonedTrace, cit);
    }
  }
  edu::sharif::twinner::util::Logger::error () << "TimedTrace::clone ():"
      " cannot find a matching iterator in segments list\n";
  abort ();
}

Expression *TimedTrace::tryToGetSymbolicExpressionByRegister (int size,
    REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
    StateSummary &state) {
  return trace->tryToGetSymbolicExpressionByRegister
      (size, reg, regval, state, it);
}

Expression *TimedTrace::tryToGetSymbolicExpressionByRegister (int size,
    REG reg) {
  return trace->tryToGetSymbolicExpressionByRegister (size, reg, it);
}

Expression *TimedTrace::tryToGetSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa,
    const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
    StateSummary &state) {
  return trace->tryToGetSymbolicExpressionByMemoryAddress
      (size, memoryEa, memval, state, it);
}

Expression *TimedTrace::tryToGetSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa) {
  return trace->tryToGetSymbolicExpressionByMemoryAddress (size, memoryEa, it);
}

bool TimedTrace::isSymbolicExpressionAvailableInRegister (int size,
    REG reg) const {
  return trace->isSymbolicExpressionAvailableInRegister (size, reg, it);
}

bool TimedTrace::isSymbolicExpressionAvailableInMemoryAddress (int size,
    ADDRINT memoryEa) const {
  return trace->isSymbolicExpressionAvailableInMemoryAddress (size, memoryEa, it);
}

Expression *TimedTrace::getSymbolicExpressionByRegister (int size, REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
    Expression *newExpression, StateSummary &state) {
  return trace->getSymbolicExpressionByRegister
      (size, reg, regval, newExpression, state, it);
}

Expression *TimedTrace::getSymbolicExpressionByRegister (int size, REG reg,
    Expression *newExpression) {
  return trace->getSymbolicExpressionByRegister (size, reg, newExpression, it);
}

Expression *TimedTrace::getSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa,
    const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
    Expression *newExpression, StateSummary &state) {
  return trace->getSymbolicExpressionByMemoryAddress
      (size, memoryEa, memval, newExpression, state, it);
}

Expression *TimedTrace::getSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa,
    Expression *newExpression) {
  return trace->getSymbolicExpressionByMemoryAddress
      (size, memoryEa, newExpression, it);
}

bool TimedTrace::doesLastGetterCallNeedPropagation () const {
  return trace->doesLastGetterCallNeedPropagation ();
}

Expression *TimedTrace::setSymbolicExpressionByRegister (int regsize, REG reg,
    const Expression *exp) {
  return trace->setSymbolicExpressionByRegister (regsize, reg, exp, it);
}

Expression *TimedTrace::setSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa,
    const Expression *exp) {
  return trace->setSymbolicExpressionByMemoryAddress (size, memoryEa, exp, it);
}

void TimedTrace::addPathConstraints (
    const std::list <edu::sharif::twinner::trace::Constraint *> &c,
    const edu::sharif::twinner::trace::Constraint *lastConstraint) {
  trace->addPathConstraints (c, lastConstraint, it);
}

void TimedTrace::printRegistersValues (
    const edu::sharif::twinner::util::Logger &logger) const {
  trace->printRegistersValues (logger, it);
}

void TimedTrace::printMemoryAddressesValues (
    const edu::sharif::twinner::util::Logger &logger) const {
  trace->printMemoryAddressesValues (logger, it);
}

void TimedTrace::printPathConstraints (
    const edu::sharif::twinner::util::Logger &logger) const {
  trace->printPathConstraints (logger, it);
}

void TimedTrace::printCompleteState (
    const edu::sharif::twinner::util::Logger &logger) const {
  trace->printCompleteState (logger, it);
}

void TimedTrace::saveToBinaryStream (std::ofstream &out) const {
  trace->saveToBinaryStream (out);
}

}
}
}
}
