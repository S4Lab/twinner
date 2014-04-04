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

#include "Trace.h"

#include <stdexcept>
#include <set>
#include <fstream>

#include "Expression.h"
#include "Constraint.h"
#include "Syscall.h"
#include "ExecutionTraceSegment.h"

#include "edu/sharif/twinner/engine/Executer.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"
#include "ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

inline void write_map_entry (std::ofstream &out,
    const ADDRINT &addr, const UINT64 &content);

Trace::Trace (const std::list < ExecutionTraceSegment * > &list) :
segments (list) {
  currentSegmentIterator = segments.end ();
  if (currentSegmentIterator != segments.begin ()) {
    currentSegmentIterator--;
  }
  currentSegmentIndex = 0;
}

Trace::Trace () {
  segments.push_front (new ExecutionTraceSegment ());
  currentSegmentIterator = segments.begin ();
  currentSegmentIndex = 0;
}

Trace::~Trace () {
  while (!segments.empty ()) {
    delete segments.front ();
    segments.pop_front ();
  }
}

Expression *Trace::tryToGetSymbolicExpressionByRegister (REG reg,
    const ConcreteValue &regval) const throw (WrongStateException) {
  throw std::runtime_error ("PIN infrastructure is not available");
}

Expression *Trace::tryToGetSymbolicExpressionByRegister (REG reg) const {
  throw std::runtime_error ("PIN infrastructure is not available");
}

Expression *Trace::tryToGetSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
    const ConcreteValue &memval) const throw (WrongStateException) {
  throw std::runtime_error ("PIN infrastructure is not available");
}

Expression *Trace::tryToGetSymbolicExpressionByMemoryAddress (ADDRINT memoryEa) const {
  throw std::runtime_error ("PIN infrastructure is not available");
}

Expression *Trace::getSymbolicExpressionByRegister (REG reg, const ConcreteValue &regval,
    Expression *newExpression) {
  throw std::runtime_error ("PIN infrastructure is not available");
}

Expression *Trace::getSymbolicExpressionByRegister (REG reg, Expression *newExpression) {
  throw std::runtime_error ("PIN infrastructure is not available");
}

Expression *Trace::getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
    const ConcreteValue &memval, Expression *newExpression) {
  throw std::runtime_error ("PIN infrastructure is not available");
}

Expression *Trace::getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
    Expression *newExpression) {
  throw std::runtime_error ("PIN infrastructure is not available");
}

Expression *Trace::setSymbolicExpressionByRegister (REG reg, const Expression *exp) {
  return getCurrentTraceSegment ()->setSymbolicExpressionByRegister (reg, exp);
}

Expression *Trace::setSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
    const Expression *exp) {
  return getCurrentTraceSegment ()->setSymbolicExpressionByMemoryAddress (memoryEa, exp);
}

void Trace::addPathConstraint (const Constraint *c) {
  getCurrentTraceSegment ()->addPathConstraint (c);
}

void Trace::syscallInvoked (Syscall s) {
  getCurrentTraceSegment ()->setSyscall (s);
  if (currentSegmentIterator == segments.begin ()) {
    segments.push_front (new ExecutionTraceSegment ());
  }
  currentSegmentIterator--;
  currentSegmentIndex++;
}

void Trace::syscallReturned (CONTEXT *context) const {
  const ExecutionTraceSegment *segment = *currentSegmentIterator;
  const std::map < REG, Expression * > &map = segment->getRegisterToExpression ();
  for (std::map < REG, Expression * >::const_iterator it = map.begin ();
      it != map.end (); ++it) {
    const REG reg = it->first;
    const Expression *exp = it->second;
    // The expression is an overwriting symbol for sure
    exp->getLastConcreteValue ().writeToRegister (context, reg);
  }
}

bool Trace::saveToFile (const char *path) const {
  std::ofstream out;
  out.open (path, ios_base::out | ios_base::trunc | ios_base::binary);
  if (!out.is_open ()) {
    edu::sharif::twinner::util::Logger::error () << "Can not write trace info:"
        " Error in open function: " << path << '\n';
    return false;
  }
  if (currentSegmentIterator != segments.begin ()) {
    edu::sharif::twinner::util::Logger::warning () << "Some unvisited segments are found"
        " at front of the segments list\n";
  }
  saveToBinaryStream (out);
  out.close ();
  return true;
}

void Trace::saveToBinaryStream (std::ofstream &out) const {
  saveListToBinaryStream (out, "TRA", segments);
}

/**
 * Check for existence of path and loads a Trace from it. The Trace is newed
 * and the caller must delete it. If path does not exist, an exception will be raised.
 * @return A newed Trace object loaded from file "path".
 */
Trace *Trace::loadFromFile (const char *path) {
  std::ifstream in;
  in.open (path, ios_base::in | ios_base::binary);
  if (!in.is_open ()) {
    edu::sharif::twinner::util::Logger::error () << "Can not read trace info:"
        " Error in open function: " << path << '\n';
    return 0;
  }
  Trace *trace = Trace::loadFromBinaryStream (in);
  in.close ();
  return trace;
}

Trace *Trace::loadFromBinaryStream (std::ifstream &in) {
  std::list < ExecutionTraceSegment * > list;
  loadListFromBinaryStream (in, "TRA", list);
  return new Trace (list);
}

bool Trace::saveAddressToValueMapToFile (const std::map < ADDRINT, UINT64 > &map,
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

void Trace::saveAddressToValueMapToBinaryStream (const std::map < ADDRINT, UINT64 > &map,
    std::ofstream &out) {
  out.write ("TRA", 3);

  std::map < ADDRINT, UINT64 >::size_type s = map.size ();
  out.write ((const char *) &s, sizeof (s));
  edu::sharif::twinner::util::foreach (map, &write_map_entry, out);
}

void write_map_entry (std::ofstream &out, const ADDRINT &addr, const UINT64 &content) {
  out.write ((const char *) &addr, sizeof (addr));
  out.write ((const char *) &content, sizeof (content));
}

map < ADDRINT, UINT64 > Trace::loadAddressToValueMapFromFile (const char *path) {
  map < ADDRINT, UINT64 > map;
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

std::map < ADDRINT, UINT64 > Trace::loadAddressToValueMapFromBinaryStream (
    std::ifstream &in) {
  std::map < ADDRINT, UINT64 > map;
  char magicString[3];
  in.read (magicString, 3);
  if (strncmp (magicString, "TRA", 3) != 0) {
    throw std::runtime_error
        ("Unexpected magic string while loading map from binary stream");
  }
  std::map < ADDRINT, UINT64 >::size_type s;
  in.read ((char *) &s, sizeof (s));

  repeat (s) {
    ADDRINT a;
    in.read ((char *) &a, sizeof (a));
    UINT64 b;
    in.read ((char *) &b, sizeof (b));

    std::pair < std::map < ADDRINT, UINT64 >::iterator, bool > res =
        map.insert (make_pair (a, b));
    if (!res.second) {
      throw std::runtime_error ("Can not read map's entry from binary stream");
    }
  }
  return map;
}

const std::list < ExecutionTraceSegment * > &Trace::getTraceSegments () const {
  return segments;
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

}
}
}
}
