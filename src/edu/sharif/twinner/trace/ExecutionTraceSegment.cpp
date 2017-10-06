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

#include "ExecutionTraceSegment.h"

#include "Expression.h"
#include "Constraint.h"
#include "Snapshot.h"
#include "StateSummary.h"
#include "TraceSegmentTerminator.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"

#include "edu/sharif/twinner/trace/exptoken/RegisterEmergedSymbol.h"
#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"

#include <utility>
#include <stdexcept>
#include <fstream>
#include <string.h>
#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

ExecutionTraceSegment::ExecutionTraceSegment (int index,
    const std::list < Snapshot * > &snas,
    TraceSegmentTerminator *_terminator) :
    snapshots (snas), terminator (_terminator), segmentIndex (index) {
}

ExecutionTraceSegment::ExecutionTraceSegment (int index) :
    terminator (0), segmentIndex (index) {
  snapshots.push_back (new Snapshot (index, 0));
}

ExecutionTraceSegment::ExecutionTraceSegment (int index,
    const std::map < REG, Expression * > &regMap,
    const std::map < ADDRINT, Expression * > &memMap) :
    terminator (0), segmentIndex (index) {
  snapshots.push_back (new Snapshot (index, 0, regMap, memMap));
}

ExecutionTraceSegment::~ExecutionTraceSegment () {
  while (!snapshots.empty ()) {
    delete snapshots.front ();
    snapshots.pop_front ();
  }
  delete terminator;
}

ExecutionTraceSegment *ExecutionTraceSegment::clone () const {
  std::list < Snapshot * > clonedSnapshots;
  for (std::list < Snapshot * >::const_iterator it = snapshots.begin ();
      it != snapshots.end (); ++it) {
    clonedSnapshots.push_back ((*it)->clone ());
  }
  TraceSegmentTerminator *_terminator = terminator ? terminator->clone () : 0;
  ExecutionTraceSegment *res = new ExecutionTraceSegment (segmentIndex, clonedSnapshots,
                                                          _terminator);
  return res;
}

void ExecutionTraceSegment::setTimedTrace (TimedTrace _timedTrace) {
  timedTrace = _timedTrace;
  Snapshot *snapshot = snapshots.back ();
  snapshot->setTimedTrace (timedTrace);
}

Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionByRegister (int size,
    REG reg, const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
    StateSummary &state) {
  Snapshot *snapshot = snapshots.back ();
  return snapshot->tryToGetSymbolicExpressionByRegister
      (size, reg, regval, state);
}

Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionByRegister (int size,
    REG reg) {
  Snapshot *snapshot = snapshots.back ();
  return snapshot->tryToGetSymbolicExpressionByRegister
      (size, reg);
}

Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa, const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
    StateSummary &state) {
  Snapshot *snapshot = snapshots.back ();
  return snapshot->tryToGetSymbolicExpressionByMemoryAddress
      (size, memoryEa, memval, state);
}

Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa) {
  Snapshot *snapshot = snapshots.back ();
  return snapshot->tryToGetSymbolicExpressionByMemoryAddress
      (size, memoryEa);
}

Expression *ExecutionTraceSegment::getSymbolicExpressionByRegister (int size, REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
    Expression *newExpression, StateSummary &state) {
  Snapshot *snapshot = snapshots.back ();
  return snapshot->getSymbolicExpressionByRegister
      (size, reg, regval, newExpression, state);
}

Expression *ExecutionTraceSegment::getSymbolicExpressionByRegister (int size, REG reg,
    Expression *newExpression) {
  Snapshot *snapshot = snapshots.back ();
  return snapshot->getSymbolicExpressionByRegister
      (size, reg, newExpression);
}

Expression *ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa, const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
    Expression *newExpression, StateSummary &state) {
  Snapshot *snapshot = snapshots.back ();
  return snapshot->getSymbolicExpressionByMemoryAddress
      (size, memoryEa, memval, newExpression, state);
}

Expression *ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa, Expression *newExpression) {
  Snapshot *snapshot = snapshots.back ();
  return snapshot->getSymbolicExpressionByMemoryAddress
      (size, memoryEa, newExpression);
}

Expression *ExecutionTraceSegment::setSymbolicExpressionByRegister (int regsize, REG reg,
    const Expression *exp) {
  Snapshot *snapshot = snapshots.back ();
  return snapshot->setSymbolicExpressionByRegister
      (regsize, reg, exp);
}

Expression *ExecutionTraceSegment::setSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa, const Expression *exp) {
  Snapshot *snapshot = snapshots.back ();
  return snapshot->setSymbolicExpressionByMemoryAddress
      (size, memoryEa, exp);
}

void ExecutionTraceSegment::addPathConstraints (
    const std::list <edu::sharif::twinner::trace::Constraint *> &constraints,
    const edu::sharif::twinner::trace::Constraint *lastConstraint) {
  std::list < Constraint * > effectiveConstraints;
  for (std::list <edu::sharif::twinner::trace::Constraint *>::const_iterator it =
      constraints.begin (); it != constraints.end (); ++it) {
    edu::sharif::twinner::trace::Constraint *c = *it;
    if (c->isTrivial (false) || (lastConstraint && (*lastConstraint) == (*c))) {
      delete c;
    } else {
      effectiveConstraints.push_back (c);
      lastConstraint = c;
    }
  }
  Snapshot *snapshot = snapshots.back ();
  snapshot->addPathConstraints (effectiveConstraints);
  Snapshot *nextSnapshot = Snapshot::instantiateNexSnapshot (*snapshot);
  addNewSnapshot (nextSnapshot);
}

void ExecutionTraceSegment::saveToBinaryStream (std::ofstream &out) const {
  // USE CASE: TwinTool saves segments, so Twinner can read them back
  const char *segmentMagicString = "SEG";
  out.write (segmentMagicString, 3);
  out.write (reinterpret_cast<const char *> (&segmentIndex), sizeof (segmentIndex));

  saveListToBinaryStream (out, "SNA", snapshots);
  if (terminator) {
    terminator->saveToBinaryStream (out);
  } else {
    const char *noTerminatorString = "NTM";
    out.write (noTerminatorString, 3);
  }
}

ExecutionTraceSegment *ExecutionTraceSegment::loadFromBinaryStream (std::ifstream &in) {
  // USE CASE: Twinner loads segments which were saved by TwinTool
  char segmentMagicString[3];
  in.read (segmentMagicString, 3);
  if (strncmp (segmentMagicString, "SEG", 3) != 0) {
    edu::sharif::twinner::util::Logger::error ()
        << "ExecutionTraceSegment::loadFromBinaryStream (...): Unexpected "
        "magic string while loading trace segment from binary stream\n";
    abort ();
  }
  int segmentIndex;
  in.read (reinterpret_cast<char *> (&segmentIndex), sizeof (segmentIndex));
  std::list < Snapshot * > snas;
  loadListFromBinaryStream (in, "SNA", snas);

  TraceSegmentTerminator *terminator =
      TraceSegmentTerminator::loadFromBinaryStream (in);
  return new ExecutionTraceSegment (segmentIndex, snas, terminator);
}

int ExecutionTraceSegment::getSegmentIndex () const {
  return segmentIndex;
}

void ExecutionTraceSegment::printRegistersValues (
    const edu::sharif::twinner::util::Logger &logger) const {
  for (std::list < Snapshot * >::const_iterator it = snapshots.begin ();
      it != snapshots.end (); ++it) {
    const Snapshot *sna = *it;
    sna->printRegistersValues (logger);
  }
}

void ExecutionTraceSegment::printMemoryAddressesValues (
    const edu::sharif::twinner::util::Logger &logger) const {
  for (std::list < Snapshot * >::const_iterator it = snapshots.begin ();
      it != snapshots.end (); ++it) {
    const Snapshot *sna = *it;
    sna->printMemoryAddressesValues (logger);
  }
}

void ExecutionTraceSegment::printPathConstraints (
    const edu::sharif::twinner::util::Logger &logger) const {
  for (std::list < Snapshot * >::const_iterator it = snapshots.begin ();
      it != snapshots.end (); ++it) {
    const Snapshot *sna = *it;
    sna->printPathConstraints (logger);
  }
}

void ExecutionTraceSegment::printCompleteState (
    const edu::sharif::twinner::util::Logger &logger) const {
  for (std::list < Snapshot * >::const_iterator it = snapshots.begin ();
      it != snapshots.end (); ++it) {
    const Snapshot *sna = *it;
    sna->printCompleteState (logger);
  }
  if (terminator) {
    logger << "Terminator: " << terminator->toString () << '\n';
  } else {
    logger << "Terminator: no terminator\n";
  }
}

const std::map < REG, Expression * > &
ExecutionTraceSegment::getFirstRegisterToExpression () const {
  return snapshots.front ()->getRegisterToExpression ();
}

std::map < REG, Expression * > ExecutionTraceSegment::getRegisterToExpression () const {
  std::map < REG, Expression * > allRegToExpressions;
  for (std::list < Snapshot * >::const_iterator it = snapshots.begin ();
      it != snapshots.end (); ++it) {
    const Snapshot *snapshot = *it;
    const std::map < REG, Expression * > &regToExpression =
        snapshot->getRegisterToExpression ();
    allRegToExpressions.insert (regToExpression.begin (), regToExpression.end ());
  }
  return allRegToExpressions;
}

std::map < ADDRINT, Expression * > ExecutionTraceSegment::getMemoryAddressTo64BitsExpression () const {
  std::map < ADDRINT, Expression * > allMemToExpressions;
  for (std::list < Snapshot * >::const_iterator it = snapshots.begin ();
      it != snapshots.end (); ++it) {
    const Snapshot *snapshot = *it;
    const std::map < ADDRINT, Expression * > &memToExpression =
        snapshot->getMemoryAddressTo64BitsExpression ();
    allMemToExpressions.insert (memToExpression.begin (), memToExpression.end ());
  }
  return allMemToExpressions;
}

const std::map < ADDRINT, Expression * > &
ExecutionTraceSegment::getMemoryAddressTo8BitsExpression () const {
  edu::sharif::twinner::util::Logger::error () << "Not implemented yet\n";
  abort ();
}

std::list < Constraint * > ExecutionTraceSegment::getPathConstraints () const {
  std::list < Constraint * > allConstraints;
  for (std::list < Snapshot * >::const_iterator it = snapshots.begin ();
      it != snapshots.end (); ++it) {
    const Snapshot *snapshot = *it;
    const std::list < Constraint * > &constraints = snapshot->getPathConstraints ();
    allConstraints.insert (allConstraints.end (),
                           constraints.begin (), constraints.end ());
  }
  return allConstraints;
}

const Constraint *ExecutionTraceSegment::getLastPathConstraint () const {
  std::list < Constraint * > allConstraints;
  for (std::list < Snapshot * >::const_reverse_iterator it =
      snapshots.rbegin (); it != snapshots.rend (); ++it) {
    const Snapshot *snapshot = *it;
    const std::list < Constraint * > &constraints = snapshot->getPathConstraints ();
    if (!constraints.empty ()) {
      return constraints.back ();
    }
  }
  return 0;
}

const std::list < Snapshot * > &ExecutionTraceSegment::getSnapshots () const {
  return snapshots;
}

void ExecutionTraceSegment::setTerminator (TraceSegmentTerminator *tst) {
  if (terminator) {
    edu::sharif::twinner::util::Logger::error ()
        << "ExecutionTraceSegment::setTerminator (TraceSegmentTerminator):"
        " Segment terminator is already set.\n";
    abort ();
  }
  terminator = tst;
}

const TraceSegmentTerminator *ExecutionTraceSegment::getTerminator () const {
  return terminator;
}

void ExecutionTraceSegment::replaceTemporarySymbolsInSegmentTerminator () {
  if (terminator) {
    terminator->replaceTemporarySymbols (snapshots.back ());
  }
}

void ExecutionTraceSegment::addTemporaryExpressions (
    const ExecutionTraceSegment *seg, REG fullReg, int size) {
  Snapshot *src = seg->snapshots.back ();
  for (std::list < Snapshot * >::iterator it = snapshots.begin ();
      it != snapshots.end (); ++it) {
    Snapshot *dst = *it;
    dst->addTemporaryExpressions (src, fullReg, size);
    src = dst;
  }
}

void ExecutionTraceSegment::addTemporaryExpressions (
    const ExecutionTraceSegment *seg, ADDRINT address, int size) {
  Snapshot *src = seg->snapshots.back ();
  for (std::list < Snapshot * >::iterator it = snapshots.begin ();
      it != snapshots.end (); ++it) {
    Snapshot *dst = *it;
    dst->addTemporaryExpressions (src, address, size);
    src = dst;
  }
}

void ExecutionTraceSegment::addNewSnapshot (Snapshot *snapshot) {
  snapshots.push_back (snapshot);
  snapshot->setTimedTrace (timedTrace);
}

}
}
}
}
