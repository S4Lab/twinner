//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2016 Behnam Momeni
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

#include "edu/sharif/twinner/engine/Executer.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"
#include "edu/sharif/twinner/util/MemoryManager.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"
#include "edu/sharif/twinner/trace/syscall/Syscall.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

inline void write_map_entry (std::ofstream &out,
    const std::pair < ADDRINT, int > &addr, const UINT64 &content);

Trace::Trace (const std::list < ExecutionTraceSegment * > &list,
    edu::sharif::twinner::util::MemoryManager *_memoryManager) :
    segments (list), memoryManager (_memoryManager) {
  currentSegmentIterator = segments.end ();
  if (currentSegmentIterator != segments.begin ()) {
    currentSegmentIterator--;
  }
  currentSegmentIndex = 0;
}

Trace::Trace () :
    memoryManager (edu::sharif::twinner::util::MemoryManager::getInstance ()) {
  segments.push_front (new ExecutionTraceSegment (0));
  currentSegmentIterator = segments.begin ();
  currentSegmentIndex = 0;
}

Trace::~Trace () {
  while (!segments.empty ()) {
    delete segments.front ();
    segments.pop_front ();
  }
}

Expression *Trace::tryToGetSymbolicExpressionByRegister (int size, REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
    StateSummary &state) {
  edu::sharif::twinner::util::Logger::error ()
      << "Trace::tryToGetSymbolicExpressionByRegister (...): "
      "PIN infrastructure is not available\n";
  abort ();
}

Expression *Trace::tryToGetSymbolicExpressionByRegister (int size, REG reg) {
  edu::sharif::twinner::util::Logger::error ()
      << "Trace::tryToGetSymbolicExpressionByRegister (...): "
      "PIN infrastructure is not available\n";
  abort ();
}

Expression *Trace::tryToGetSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
    const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
    StateSummary &state) {
  edu::sharif::twinner::util::Logger::error ()
      << "Trace::tryToGetSymbolicExpressionByMemoryAddress (...): "
      "PIN infrastructure is not available\n";
  abort ();
}

Expression *Trace::tryToGetSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa) {
  edu::sharif::twinner::util::Logger::error ()
      << "Trace::tryToGetSymbolicExpressionByMemoryAddress (...): "
      "PIN infrastructure is not available\n";
  abort ();
}

Expression *Trace::getSymbolicExpressionByRegister (int size, REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
    Expression *newExpression, StateSummary &state) {
  edu::sharif::twinner::util::Logger::error ()
      << "Trace::getSymbolicExpressionByRegister (...): "
      "PIN infrastructure is not available\n";
  abort ();
}

Expression *Trace::getSymbolicExpressionByRegister (int size, REG reg,
    Expression *newExpression) {
  edu::sharif::twinner::util::Logger::error ()
      << "Trace::getSymbolicExpressionByRegister (...): "
      "PIN infrastructure is not available\n";
  abort ();
}

Expression *Trace::getSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
    const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
    Expression *newExpression, StateSummary &state) {
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
  edu::sharif::twinner::util::Logger::error ()
      << "Trace::getSymbolicExpressionByMemoryAddress (...): "
      "PIN infrastructure is not available\n";
  abort ();
}

Expression *Trace::setSymbolicExpressionByRegister (int regsize, REG reg,
    const Expression *exp) {
  return getCurrentTraceSegment ()->setSymbolicExpressionByRegister (regsize, reg, exp);
}

Expression *Trace::setSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
    const Expression *exp) {
  return getCurrentTraceSegment ()->setSymbolicExpressionByMemoryAddress
      (size, memoryEa, exp);
}

void Trace::addPathConstraints (
    const std::list <edu::sharif::twinner::trace::Constraint *> &c,
    const edu::sharif::twinner::trace::Constraint *lastConstraint) {
  if (lastConstraint) {
    edu::sharif::twinner::util::Logger::error ()
        << "Trace::addPathConstraints (): "
        "lastConstraint argument must be null\n";
    abort ();
  }
  for (std::list < ExecutionTraceSegment * >::iterator it =
      currentSegmentIterator; it != segments.end (); ++it) {
    ExecutionTraceSegment *segment = *it;
    if (!segment->getPathConstraints ().empty ()) {
      lastConstraint = segment->getPathConstraints ().back ();
      break;
    }
  }
  getCurrentTraceSegment ()->addPathConstraints (c, lastConstraint);
}

void Trace::terminateTraceSegment (TraceSegmentTerminator *tst) {
  getCurrentTraceSegment ()->setTerminator (tst);
  currentSegmentIndex++;
  if (currentSegmentIterator == segments.begin ()) {
    segments.push_front (new ExecutionTraceSegment (currentSegmentIndex));
  }
  currentSegmentIterator--;
}

void Trace::initializeNewTraceSegment (CONTEXT *context) const {
  const ExecutionTraceSegment *segment = *currentSegmentIterator;
  const std::map < REG, Expression * > &map = segment->getRegisterToExpression ();
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
    }
  }
}

bool Trace::saveToFile (const char *path, const char *memoryPath) const {
  std::ofstream out;
  out.open (path, ios_base::out | ios_base::trunc | ios_base::binary);
  if (!out.is_open ()) {
    edu::sharif::twinner::util::Logger::error () << "Can not write trace info:"
        " Error in open function: " << path << '\n';
    return false;
  }
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
Trace *Trace::loadFromFile (const char *path, const char *memoryPath) {
  std::ifstream in;
  in.open (path, ios_base::in | ios_base::binary);
  if (!in.is_open ()) {
    edu::sharif::twinner::util::Logger::error () << "Can not read trace info:"
        " Error in open function: " << path << '\n';
    return 0;
  }
  Trace *trace = Trace::loadFromBinaryStream (in, memoryPath);
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
Trace::loadAddressToValueMapFromFile (const char *path) {
  map < std::pair < ADDRINT, int >, UINT64 > map;
  std::ifstream in;
  in.open (path, ios_base::in | ios_base::binary);
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

void Trace::abandonTrivialMemoryExpressions () {
  getCurrentTraceSegment ()->abandonTrivialMemoryExpressions ();
}

void Trace::printRegistersValues (
    const edu::sharif::twinner::util::Logger &logger) const {
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
  int i = 0;
  for (std::list < ExecutionTraceSegment * >::const_iterator it = segments.begin ();
      it != segments.end (); ++it) {
    const ExecutionTraceSegment *seg = *it;
    logger << "Segment #" << i++ << ":\n";
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

void Trace::printMemoryUsageStats (
    const edu::sharif::twinner::util::Logger &logger) const {
  logger << "Trace has " << segments.size () << " segments overall\n";
  int i = 0;
  int size = 0;
  for (std::list < ExecutionTraceSegment * >::const_iterator it = segments.begin ();
      it != segments.end (); ++it) {
    const ExecutionTraceSegment *seg = *it;
    logger << "Segment #" << i++ << ": ";
    size += seg->printMemoryUsageStats (logger);
    logger << '\n';
  }
  logger << "Total size of trace is approximately equal to "
      << std::dec << size << " bytes\n";
}

}
}
}
}
