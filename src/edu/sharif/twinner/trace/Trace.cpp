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

#include "Trace.h"

#include <stdexcept>
#include <set>
#include <fstream>
#include <list>

#include "Expression.h"
#include "Constraint.h"
#include "ExecutionTraceSegment.h"
#include "SymbolRepresentation.h"

#include "edu/sharif/twinner/trace/exptoken/Symbol.h"

#include "edu/sharif/twinner/engine/Executer.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"
#include "edu/sharif/twinner/util/MemoryManager.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"
#include "edu/sharif/twinner/trace/syscall/Syscall.h"
#include "SnapshotIterator.h"
#include "exptoken/Constant.h"
#include "exptoken/BitwiseAndOperator.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

inline void write_map_entry (std::ofstream &out,
    const std::pair < ADDRINT, int > &addr, const UINT64 &content);

Trace::Trace (const std::list < ExecutionTraceSegment * > &list,
    edu::sharif::twinner::util::MemoryManager *_memoryManager) :
    segments (list), memoryManager (_memoryManager) {
  current = TimedTrace (this, segments.begin ());
  currentSegmentIndex = segments.size () - 1;
  for (std::list < ExecutionTraceSegment * >::iterator it = segments.begin ();
      it != segments.end (); ++it) {
    (*it)->setTimedTrace (TimedTrace (this, it));
  }
}

Trace::Trace () :
    memoryManager (edu::sharif::twinner::util::MemoryManager::getInstance ()) {
  addNewSegment (new ExecutionTraceSegment (0));
  current = TimedTrace (this, segments.begin ());
  currentSegmentIndex = 0;
}

Trace::Trace (const Trace &trace) :
    memoryManager (trace.memoryManager) {
  for (std::list < ExecutionTraceSegment * >::const_iterator it =
      trace.segments.begin (); it != trace.segments.end (); ++it) {
    segments.push_back ((*it)->clone ());
  }
  current = TimedTrace (this, segments.begin ());
  currentSegmentIndex = segments.size () - 1;
  for (std::list < ExecutionTraceSegment * >::iterator it = segments.begin ();
      it != segments.end (); ++it) {
    (*it)->setTimedTrace (TimedTrace (this, it));
  }
}

Trace::~Trace () {
  while (!segments.empty ()) {
    delete segments.front ();
    segments.pop_front ();
  }
}

Trace *Trace::clone () const {
  return new Trace (*this);
}

Expression *Trace::tryToGetSymbolicExpressionByRegister (int size, REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
    StateSummary &state) {
  return current.tryToGetSymbolicExpressionByRegister
      (size, reg, regval, state);
}

Expression *Trace::tryToGetSymbolicExpressionByRegister (int size, REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
    StateSummary &state,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  edu::sharif::twinner::util::Logger::error ()
      << "Trace::tryToGetSymbolicExpressionByRegister (...): "
      "PIN infrastructure is not available\n";
  abort ();
}

Expression *Trace::tryToGetSymbolicExpressionByRegister (int size, REG reg) {
  return current.tryToGetSymbolicExpressionByRegister (size, reg);
}

Expression *Trace::tryToGetSymbolicExpressionByRegister (int size, REG reg,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  edu::sharif::twinner::util::Logger::error ()
      << "Trace::tryToGetSymbolicExpressionByRegister (...): "
      "PIN infrastructure is not available\n";
  abort ();
}

Expression *Trace::tryToGetSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa,
    const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
    StateSummary &state) {
  return current.tryToGetSymbolicExpressionByMemoryAddress
      (size, memoryEa, memval, state);
}

Expression *Trace::tryToGetSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa,
    const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
    StateSummary &state,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  edu::sharif::twinner::util::Logger::error ()
      << "Trace::tryToGetSymbolicExpressionByMemoryAddress (...): "
      "PIN infrastructure is not available\n";
  abort ();
}

Expression *Trace::tryToGetSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa) {
  return current.tryToGetSymbolicExpressionByMemoryAddress (size, memoryEa);
}

Expression *Trace::tryToGetSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  edu::sharif::twinner::util::Logger::error ()
      << "Trace::tryToGetSymbolicExpressionByMemoryAddress (...): "
      "PIN infrastructure is not available\n";
  abort ();
}

bool Trace::isSymbolicExpressionAvailableInRegister (int size,
    REG reg) const {
  return current.isSymbolicExpressionAvailableInRegister (size, reg);
}

bool Trace::isSymbolicExpressionAvailableInRegister (int size, REG reg,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) const {
  edu::sharif::twinner::util::Logger::error ()
      << "Trace::isSymbolicExpressionAvailableInRegister (...): "
      "PIN infrastructure is not available\n";
  abort ();
}

bool Trace::isSymbolicExpressionAvailableInMemoryAddress (int size,
    ADDRINT memoryEa) const {
  return current.isSymbolicExpressionAvailableInMemoryAddress (size, memoryEa);
}

bool Trace::isSymbolicExpressionAvailableInMemoryAddress (int size,
    ADDRINT memoryEa,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) const {
  edu::sharif::twinner::util::Logger::error ()
      << "Trace::isSymbolicExpressionAvailableInMemoryAddress (...): "
      "PIN infrastructure is not available\n";
  abort ();
}

Expression *Trace::getSymbolicExpressionByRegister (int size, REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
    Expression *newExpression, StateSummary &state) {
  return current.getSymbolicExpressionByRegister
      (size, reg, regval, newExpression, state);
}

Expression *Trace::getSymbolicExpressionByRegister (int size, REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
    Expression *newExpression, StateSummary &state,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  edu::sharif::twinner::util::Logger::error ()
      << "Trace::getSymbolicExpressionByRegister (...): "
      "PIN infrastructure is not available\n";
  abort ();
}

Expression *Trace::getSymbolicExpressionByRegister (int size, REG reg,
    Expression *newExpression) {
  return current.getSymbolicExpressionByRegister (size, reg, newExpression);
}

Expression *Trace::getSymbolicExpressionByRegister (int size, REG reg,
    Expression *newExpression,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  edu::sharif::twinner::util::Logger::error ()
      << "Trace::getSymbolicExpressionByRegister (...): "
      "PIN infrastructure is not available\n";
  abort ();
}

Expression *Trace::getSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
    const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
    Expression *newExpression, StateSummary &state) {
  return current.getSymbolicExpressionByMemoryAddress
      (size, memoryEa, memval, newExpression, state);
}

Expression *Trace::getSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
    const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
    Expression *newExpression, StateSummary &state,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  edu::sharif::twinner::util::Logger::error ()
      << "Trace::getSymbolicExpressionByMemoryAddress (...): "
      "PIN infrastructure is not available\n";
  abort ();
}

bool Trace::doesLastGetterCallNeedPropagation () const {
  return needsPropagation;
}

Expression *Trace::getSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
    Expression *newExpression) {
  return current.getSymbolicExpressionByMemoryAddress
      (size, memoryEa, newExpression);
}

Expression *Trace::getSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
    Expression *newExpression,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  edu::sharif::twinner::util::Logger::error ()
      << "Trace::getSymbolicExpressionByMemoryAddress (...): "
      "PIN infrastructure is not available\n";
  abort ();
}

Expression *Trace::setSymbolicExpressionByRegister (int regsize, REG reg,
    const Expression *exp) {
  return current.setSymbolicExpressionByRegister (regsize, reg, exp);
}

Expression *Trace::setSymbolicExpressionByRegister (int regsize, REG reg,
    const Expression *exp,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  return (*currentSegmentIterator)->setSymbolicExpressionByRegister
      (regsize, reg, exp);
}

Expression *Trace::setSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
    const Expression *exp) {
  return current.setSymbolicExpressionByMemoryAddress (size, memoryEa, exp);
}

Expression *Trace::setSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
    const Expression *exp,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  return (*currentSegmentIterator)->setSymbolicExpressionByMemoryAddress
      (size, memoryEa, exp);
}

void Trace::addPathConstraints (
    const std::list <edu::sharif::twinner::trace::Constraint *> &c,
    const edu::sharif::twinner::trace::Constraint *lastConstraint) {
  current.addPathConstraints (c, lastConstraint);
}

void Trace::addPathConstraints (
    const std::list <edu::sharif::twinner::trace::Constraint *> &c,
    const edu::sharif::twinner::trace::Constraint *lastConstraint,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  if (lastConstraint) {
    edu::sharif::twinner::util::Logger::error ()
        << "Trace::addPathConstraints (): "
        "lastConstraint argument must be null\n";
    abort ();
  }
  for (std::list < ExecutionTraceSegment * >::iterator it =
      currentSegmentIterator; it != segments.end (); ++it) {
    ExecutionTraceSegment *segment = *it;
    lastConstraint = segment->getLastPathConstraint ();
    if (lastConstraint) {
      break;
    }
  }
  (*currentSegmentIterator)->addPathConstraints (c, lastConstraint);
}

void Trace::terminateTraceSegment (TraceSegmentTerminator *tst) {
  std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator =
      current.getSegmentIterator ();
  (*currentSegmentIterator)->setTerminator (tst);
  currentSegmentIndex++;
  if (currentSegmentIterator == segments.begin ()) {
    addNewSegment (new ExecutionTraceSegment (currentSegmentIndex));
  }
  currentSegmentIterator--;
  current = TimedTrace (this, currentSegmentIterator);
}

bool Trace::initializeNewTraceSegment (CONTEXT *context) const {
  bool isContextModified = false;
  std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator =
      current.getSegmentIterator ();
  const ExecutionTraceSegment *segment = *currentSegmentIterator;
  const std::map < REG, Expression * > &map = segment->getFirstRegisterToExpression ();
  for (std::map < REG, Expression * >::const_iterator it = map.begin ();
      it != map.end (); ++it) {
    const REG reg = it->first;
    Expression *exp = it->second;
    if (exp->isOverwritingExpression ()) {
      if (!exp->getLastConcreteValue ().writeToRegister (context, reg)) {
        edu::sharif::twinner::util::Logger::error ()
            << "Error in writeToRegister (...)"
            " called from Trace::initializeNewTraceSegment (...)\n";
        abort ();
      }
      exp->setOverwriting (false);
      isContextModified = true;
    }
  }
  return isContextModified;
}

bool Trace::saveToFile (const char *path, const char *memoryPath) const {
  std::ofstream out;
  out.open (path, ios_base::out | ios_base::trunc | ios_base::binary);
  if (!out.is_open ()) {
    edu::sharif::twinner::util::Logger::error () << "Can not write trace info:"
        " Error in open function: " << path << '\n';
    return false;
  }
  std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator =
      current.getSegmentIterator ();
  if (currentSegmentIterator != segments.begin ()) {
    edu::sharif::twinner::util::Logger::warning ()
        << "Some unvisited segments are found at front of the segments list\n";
  }
  saveToBinaryStream (out);
  out.close ();
  return memoryManager->saveToFile (memoryPath);
}

void Trace::saveToBinaryStream (std::ofstream &out) const {
  saveListToBinaryStream (out, "TRA", segments);
}

/**
 * Check for existence of path and loads a Trace from it. The Trace is newed
 * and the caller must delete it. If path does not exist, an exception will be raised.
 * @return A newed Trace object loaded from file "path".
 */
Trace *Trace::loadFromFile (std::string path, std::string memoryPath) {
  std::ifstream in;
  in.open (path.c_str (), ios_base::in | ios_base::binary);
  if (!in.is_open ()) {
    edu::sharif::twinner::util::Logger::error () << "Can not read trace info:"
        " Error in open function: " << path << '\n';
    return 0;
  }
  Trace *trace = Trace::loadFromBinaryStream (in, memoryPath.c_str ());
  in.close ();
  return trace;
}

Trace *Trace::loadFromBinaryStream (std::ifstream &in, const char *memoryPath) {
  std::list < ExecutionTraceSegment * > list;
  loadListFromBinaryStream (in, "TRA", list);
  edu::sharif::twinner::util::MemoryManager *mm =
      edu::sharif::twinner::util::MemoryManager::loadFromFile (memoryPath);
  if (mm == 0) {
    return 0;
  }
  return new Trace (list, mm);
}

bool Trace::saveAddressToValueMapToFile (
    const std::map < std::pair < ADDRINT, int >, UINT64 > &map,
    const char *path) {
  std::ofstream out;
  out.open (path, ios_base::out | ios_base::trunc | ios_base::binary);
  if (!out.is_open ()) {
    edu::sharif::twinner::util::Logger::error () << "Can not write addr-to-val map:"
        " Error in open function: " << path << '\n';
    return false;
  }
  saveAddressToValueMapToBinaryStream (map, out);
  out.close ();
  return true;
}

void Trace::saveAddressToValueMapToBinaryStream (
    const std::map < std::pair < ADDRINT, int >, UINT64 > &map,
    std::ofstream &out) {
  out.write ("TRA", 3);

  std::map < std::pair < ADDRINT, int >, UINT64 >::size_type s = map.size ();
  out.write ((const char *) &s, sizeof (s));
  edu::sharif::twinner::util::foreach (map, &write_map_entry, out);
}

void write_map_entry (std::ofstream &out,
    const std::pair < ADDRINT, int > &addr, const UINT64 &content) {
  const ADDRINT symbolAddress = addr.first;
  const int symbolSize = addr.second;
  out.write ((const char *) &symbolAddress, sizeof (symbolAddress));
  out.write ((const char *) &symbolSize, sizeof (symbolSize));
  out.write ((const char *) &content, sizeof (content));
}

map < std::pair < ADDRINT, int >, UINT64 >
Trace::loadAddressToValueMapFromFile (std::string path) {
  map < std::pair < ADDRINT, int >, UINT64 > map;
  std::ifstream in;
  in.open (path.c_str (), ios_base::in | ios_base::binary);
  if (!in.is_open ()) {
    edu::sharif::twinner::util::Logger::error () << "Can not read address-to-value map:"
        " Error in open function: " << path << '\n';
    return map;
  }
  map = Trace::loadAddressToValueMapFromBinaryStream (in);
  in.close ();
  return map;
}

std::map < std::pair < ADDRINT, int >, UINT64 >
Trace::loadAddressToValueMapFromBinaryStream (std::ifstream &in) {
  typedef std::map < std::pair < ADDRINT, int >, UINT64 > AddrToValueMap;
  AddrToValueMap map;
  char magicString[3];
  in.read (magicString, 3);
  if (strncmp (magicString, "TRA", 3) != 0) {
    edu::sharif::twinner::util::Logger::error ()
        << "Trace::loadAddressToValueMapFromBinaryStream (...): "
        "Unexpected magic string while loading map from binary stream\n";
    abort ();
  }
  AddrToValueMap::size_type s;
  in.read ((char *) &s, sizeof (s));

  repeat (s) {
    ADDRINT a;
    in.read ((char *) &a, sizeof (a));
    int size;
    in.read ((char *) &size, sizeof (size));
    UINT64 b;
    in.read ((char *) &b, sizeof (b));

    std::pair < AddrToValueMap::iterator, bool > res =
        map.insert (make_pair (make_pair (a, size), b));
    if (!res.second) {
      edu::sharif::twinner::util::Logger::error ()
          << "Trace::loadAddressToValueMapFromBinaryStream (...): "
          "Can not read map's entry from binary stream\n";
      abort ();
    }
  }
  return map;
}

const std::list < ExecutionTraceSegment * > &Trace::getTraceSegments () const {
  return segments;
}

std::list < ExecutionTraceSegment * > &Trace::getTraceSegments () {
  return segments;
}

Snapshot::snapshot_iterator Trace::begin () const {
  return Snapshot::snapshot_iterator (segments).begin ();
}

Snapshot::snapshot_iterator Trace::end () const {
  return Snapshot::snapshot_iterator (segments).end ();
}

Snapshot::snapshot_reverse_iterator Trace::rbegin () const {
  return Snapshot::snapshot_reverse_iterator (end ());
}

Snapshot::snapshot_reverse_iterator Trace::rend () const {
  return Snapshot::snapshot_reverse_iterator (begin ());
}

void Trace::printRegistersValues (
    const edu::sharif::twinner::util::Logger &logger) const {
  return current.printRegistersValues (logger);
}

void Trace::printRegistersValues (
    const edu::sharif::twinner::util::Logger &logger,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) const {
  int i = 0;
  for (std::list < ExecutionTraceSegment * >::const_iterator it = currentSegmentIterator;
      it != segments.end (); ++it) {
    const ExecutionTraceSegment *seg = *it;
    logger << "Segment #" << i++ << '\n';
    seg->printRegistersValues (logger);
  }
}

void Trace::printMemoryAddressesValues (
    const edu::sharif::twinner::util::Logger &logger) const {
  return current.printMemoryAddressesValues (logger);
}

void Trace::printMemoryAddressesValues (
    const edu::sharif::twinner::util::Logger &logger,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) const {
  int i = 0;
  for (std::list < ExecutionTraceSegment * >::const_iterator it = currentSegmentIterator;
      it != segments.end (); ++it) {
    const ExecutionTraceSegment *seg = *it;
    logger << "Segment #" << i++;
    seg->printMemoryAddressesValues (logger);
  }
}

void Trace::printPathConstraints (
    const edu::sharif::twinner::util::Logger &logger) const {
  return current.printPathConstraints (logger);
}

void Trace::printPathConstraints (
    const edu::sharif::twinner::util::Logger &logger,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) const {
  int i = 0;
  for (std::list < ExecutionTraceSegment * >::const_iterator it = currentSegmentIterator;
      it != segments.end (); ++it) {
    const ExecutionTraceSegment *seg = *it;
    logger << "Segment #" << i++;
    seg->printPathConstraints (logger);
  }
}

void Trace::printCompleteState (
    const edu::sharif::twinner::util::Logger &logger) const {
  current.printCompleteState (logger);
}

void Trace::printCompleteState (
    const edu::sharif::twinner::util::Logger &logger,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) const {
  int i = currentSegmentIndex;
  for (std::list < ExecutionTraceSegment * >::const_iterator it =
      currentSegmentIterator;
      it != segments.end (); ++it) {
    const ExecutionTraceSegment *seg = *it;
    logger << "Segment #" << i-- << ":\n";
    seg->printCompleteState (logger);
  }
}

int Trace::getCurrentSegmentIndex () const {
  return currentSegmentIndex;
}

edu::sharif::twinner::util::MemoryManager *Trace::getMemoryManager () {
  return memoryManager;
}

const edu::sharif::twinner::util::MemoryManager *Trace::getMemoryManager () const {
  return memoryManager;
}

void Trace::markCriticalAddresses (Trace *relativeTrace,
    const std::map < Snapshot *, std::list < int > > &effectiveConstraints) {
  // Iterating from the end of the trace backwards
  std::set<SymbolRepresentation> criticalSymbols;
  for (Snapshot::snapshot_reverse_iterator it = rbegin (),
      it2 = relativeTrace->rbegin (); it != rend (); ++it, ++it2) {
    Snapshot &currentSnapshot = *it;
    Snapshot &relativeSnapshot = *it2;
    currentSnapshot.addCriticalSymbols (criticalSymbols);
    relativeSnapshot.addCriticalSymbols (criticalSymbols);
    std::list < std::pair < const Expression *, bool > > criticalExpressions =
        relativeSnapshot.getCriticalExpressions
        (effectiveConstraints.find (&currentSnapshot)->second);
    criticalSymbols = aggregateTemporarySymbols (criticalExpressions);
    for (std::list < std::pair < const Expression *, bool > >
        ::const_iterator it3 = criticalExpressions.begin ();
        it3 != criticalExpressions.end (); ++it3) {
      if (it3->second) {
        delete it3->first;
      }
    }
  }
}

void Trace::addNewSegment (ExecutionTraceSegment *segment) {
  segments.push_front (segment);
  segment->setTimedTrace (TimedTrace (this, segments.begin ()));
}

std::set<SymbolRepresentation> Trace::aggregateTemporarySymbols (
    const std::list < std::pair < const Expression *, bool > > &exps) const {
  std::set<SymbolRepresentation> temporarySymbols;
  for (std::list < std::pair < const Expression *, bool > >::const_iterator it =
      exps.begin (); it != exps.end (); ++it) {
    const Expression *exp = it->first;
    const Expression::Stack &stack = exp->getStack ();
    for (Expression::Stack::const_iterator it2 = stack.begin ();
        it2 != stack.end (); ++it2) {
      const edu::sharif::twinner::trace::exptoken::Symbol *symbol =
          dynamic_cast<edu::sharif::twinner::trace::exptoken::Symbol *> (*it2);
      if (symbol && symbol->isTemporary ()) {
        Expression::Stack::const_iterator it3 = it2;
        if (++it3 != stack.end ()) {
          const edu::sharif::twinner::trace::exptoken::Constant *mask =
              dynamic_cast<edu::sharif::twinner::trace::exptoken
              ::Constant *> (*it3);
          if (mask) {
            if (++it3 != stack.end ()) {
              const edu::sharif::twinner::trace::exptoken::BitwiseAndOperator *op =
                  dynamic_cast<edu::sharif::twinner::trace::exptoken
                  ::BitwiseAndOperator *> (*it3);
              if (op) {
                temporarySymbols.insert
                    (SymbolRepresentation (symbol, mask->getValue ()));
                continue;
              }
            }
          }
        }
        temporarySymbols.insert (SymbolRepresentation (symbol));
      }
    }
  }
  return temporarySymbols;
}

void Trace::replaceTemporarySymbols () {
  // Iterating from the beginning of the trace forwards
  Snapshot *previousSnapshot = 0;
  for (Snapshot::snapshot_iterator it = begin (); it != end (); ++it) {
    Snapshot &currentSnapshot = *it;
    if (previousSnapshot) {
      currentSnapshot.replaceTemporarySymbols (previousSnapshot);
    }
    previousSnapshot = &currentSnapshot;
  }
  for (std::list < ExecutionTraceSegment * >::const_reverse_iterator it =
      segments.rbegin (); it != segments.rend (); ++it) {
    ExecutionTraceSegment *segment = *it;
    segment->replaceTemporarySymbolsInSegmentTerminator ();
  }
}


}
}
}
}
