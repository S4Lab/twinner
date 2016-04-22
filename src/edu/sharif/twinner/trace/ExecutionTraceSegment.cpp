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

#include "ExecutionTraceSegment.h"

#include "Expression.h"
#include "Constraint.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"

#include "edu/sharif/twinner/trace/exptoken/RegisterEmergedSymbol.h"
#include "edu/sharif/twinner/trace/Expression.h"

#include "cv/ConcreteValue64Bits.h"
#include "StateSummary.h"
#include "TraceSegmentTerminator.h"

#include <utility>
#include <stdexcept>
#include <fstream>
#include <string.h>
#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

template < >
Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionImplementation (
    int size, std::map < REG, Expression * > &map, const REG key,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal,
    StateSummary &state);
template < >
Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionImplementation (
    int size, std::map < ADDRINT, Expression * > &map, const ADDRINT key,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal,
    StateSummary &state);

template < >
Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionImplementation (
    int size, std::map < REG, Expression * > &map, const REG key);
template < >
Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionImplementation (
    int size, std::map < ADDRINT, Expression * > &map, const ADDRINT key);

ExecutionTraceSegment::ExecutionTraceSegment (int index,
    const std::map < REG, Expression * > &regi,
    const std::map < ADDRINT, Expression * > &memo,
    const std::list < Constraint * > &cnrt,
    TraceSegmentTerminator *_terminator) :
    registerToExpression (regi), memoryAddressTo64BitsExpression (memo),
    pathConstraints (cnrt), terminator (_terminator), segmentIndex (index) {
  /*
   * This constructor is called by Twinner to reacquire registers/memory/constraints
   * info and use them to build its behavioral model.
   * So other (128/32 bits) memory addresses won't be addressed and are not required to
   * be initialized at this time.
   */
}

ExecutionTraceSegment::ExecutionTraceSegment (int index) :
    terminator (0), segmentIndex (index) {
}

ExecutionTraceSegment::ExecutionTraceSegment (int index,
    const std::map < REG, Expression * > &regMap,
    const std::map < ADDRINT, Expression * > &memMap) :
    registerToExpression (regMap), terminator (0), segmentIndex (index) {
  /*
   * This constructor is called by TwinTool and so other overlapping
   * memory addresses may be accessed too. Downwards propagation is performed
   * right now as there is no collision between pure symbols and upwards
   * propagation is performed lazily.
   * Also overlapping registers must be initialized based on their enclosing registers.
   */
  for (std::map < ADDRINT, Expression * >::const_iterator it = memMap.begin ();
      it != memMap.end (); ++it) {
    const ADDRINT memoryEa = it->first;
    const Expression *exp = it->second;
    const int size = exp->getLastConcreteValue ().getSize ();
    setOverwritingMemoryExpression (size, memoryEa, exp);
    initializeOverlappingMemoryLocationsDownwards (size, memoryEa, *exp);
    delete exp; // setOverwritingMemoryExpression has cloned the exp
    initializeOverlappingMemoryLocationsUpwards (size, memoryEa);
  }
  for (std::map < REG, Expression * >::const_iterator it = regMap.begin ();
      it != regMap.end (); ++it) {
    const REG reg = it->first;
    const Expression *exp = it->second;
    edu::sharif::twinner::trace::exptoken::RegisterEmergedSymbol::initializeSubRegisters (reg, this, *exp);
  }
}

ExecutionTraceSegment::~ExecutionTraceSegment () {
  for (std::map < REG, Expression * >::iterator it = registerToExpression.begin ();
      it != registerToExpression.end (); ++it) {
    Expression *exp = it->second;
    delete exp;
  }
  registerToExpression.clear ();
  std::map < ADDRINT, Expression * > *memToExp[]
      = {&memoryAddressTo128BitsExpression, &memoryAddressTo64BitsExpression,
    &memoryAddressTo32BitsExpression, &memoryAddressTo16BitsExpression,
    &memoryAddressTo8BitsExpression, 0};
  for (int i = 0; memToExp[i]; ++i) {
    for (std::map < ADDRINT, Expression * >::iterator it = memToExp[i]->begin ();
        it != memToExp[i]->end (); ++it) {
      Expression *exp = it->second;
      delete exp;
    }
    memToExp[i]->clear ();
  }
  while (!pathConstraints.empty ()) {
    delete pathConstraints.front ();
    pathConstraints.pop_front ();
  }
  delete terminator;
}

void ExecutionTraceSegment::setOverwritingMemoryExpression (int size,
    ADDRINT memoryEa, const Expression *expression) {
  Expression *exp = setSymbolicExpressionByMemoryAddress (size, memoryEa, expression);
  exp->setOverwriting (true);
}

void ExecutionTraceSegment::initializeOverlappingMemoryLocationsDownwards (int size,
    ADDRINT memoryEa, const Expression &expression,
    bool shouldTruncate, int shiftAmount) {
  size /= 2;
  if (size >= 8) {
    Expression *exp = expression.clone ();
    if (shiftAmount > 0) {
      exp->shiftToRight (shiftAmount);
    }
    exp->truncate (size); // LSB (left-side in little-endian)
    setOverwritingMemoryExpression (size, memoryEa, exp);
    delete exp;
    initializeOverlappingMemoryLocationsDownwards (size, memoryEa, expression,
                                                   true, shiftAmount);
    exp = expression.clone ();
    exp->shiftToRight (shiftAmount + size); // MSB (right-side in little-endian)
    if (shouldTruncate) {
      exp->truncate (size);
    }
    setOverwritingMemoryExpression (size, memoryEa + size / 8, exp);
    delete exp;
    initializeOverlappingMemoryLocationsDownwards (size, memoryEa + size / 8, expression,
                                                   shouldTruncate, shiftAmount + size);
  }
}

void ExecutionTraceSegment::initializeOverlappingMemoryLocationsUpwards (
    int size, ADDRINT address) {
  while (size <= 64) {
    const bool twoSizeBitsAligned = (address % (size / 4) == 0);
    if (!twoSizeBitsAligned) {
      address -= size / 8;
    }
    size *= 2;
    setSymbolicExpressionByMemoryAddress (size, address, NULL);
  }
}

Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionByRegister (int size,
    REG reg, const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
    StateSummary &state) {
  UNUSED_VARIABLE (size);
  return tryToGetSymbolicExpressionImplementation
      (size, registerToExpression, reg, regval, state);
}

Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionByRegister (int size,
    REG reg) {
  UNUSED_VARIABLE (size);
  return tryToGetSymbolicExpressionImplementation (size, registerToExpression, reg);
}

Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa, const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
    StateSummary &state) {
  switch (size) {
  case 128:
    return tryToGetSymbolicExpressionImplementation
        (128, memoryAddressTo128BitsExpression, memoryEa, memval, state);
  case 64:
    return tryToGetSymbolicExpressionImplementation
        (64, memoryAddressTo64BitsExpression, memoryEa, memval, state);
  case 32:
    return tryToGetSymbolicExpressionImplementation
        (32, memoryAddressTo32BitsExpression, memoryEa, memval, state);
  case 16:
    return tryToGetSymbolicExpressionImplementation
        (16, memoryAddressTo16BitsExpression, memoryEa, memval, state);
  case 8:
    return tryToGetSymbolicExpressionImplementation
        (8, memoryAddressTo8BitsExpression, memoryEa, memval, state);
  default:
    edu::sharif::twinner::util::Logger::error () <<
        "ExecutionTraceSegment::tryToGetSymbolicExpressionByMemoryAddress"
        " (size=" << std::dec << size
        << ", memoryEa=0x" << std::hex << memoryEa
        << ", memval=" << memval << "): Memory read size is not supported\n";
    abort ();
  }
}

Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa) {
  switch (size) {
  case 128:
    return tryToGetSymbolicExpressionImplementation
        (128, memoryAddressTo128BitsExpression, memoryEa);
  case 64:
    return tryToGetSymbolicExpressionImplementation
        (64, memoryAddressTo64BitsExpression, memoryEa);
  case 32:
    return tryToGetSymbolicExpressionImplementation
        (32, memoryAddressTo32BitsExpression, memoryEa);
  case 16:
    return tryToGetSymbolicExpressionImplementation
        (16, memoryAddressTo16BitsExpression, memoryEa);
  case 8:
    return tryToGetSymbolicExpressionImplementation
        (8, memoryAddressTo8BitsExpression, memoryEa);
  default:
    edu::sharif::twinner::util::Logger::error () <<
        "ExecutionTraceSegment::tryToGetSymbolicExpressionByMemoryAddress"
        " (size=" << std::dec << size
        << ", memoryEa=0x" << std::hex << memoryEa
        << "): Memory read size is not supported\n";
    abort ();
  }
}

template < typename Address >
void check_concrete_value_for_possible_state_mismatch (Expression *exp,
    Address address, const edu::sharif::twinner::trace::cv::ConcreteValue &val,
    StateSummary &state);

template < >
void check_concrete_value_for_possible_state_mismatch (Expression *exp,
    REG reg, const edu::sharif::twinner::trace::cv::ConcreteValue &val,
    StateSummary &state);
template < >
void check_concrete_value_for_possible_state_mismatch (Expression *exp,
    ADDRINT address, const edu::sharif::twinner::trace::cv::ConcreteValue &val,
    StateSummary &state);

Expression *lazy_load_symbolic_expression (ExecutionTraceSegment *me,
    int size, std::map < ADDRINT, Expression * > &map, const ADDRINT key,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal,
    edu::sharif::twinner::trace::StateSummary &state);
Expression *lazy_load_symbolic_expression (ExecutionTraceSegment *me,
    int size, std::map < ADDRINT, Expression * > &map, const ADDRINT key);

template < >
Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionImplementation (
    int size, std::map < REG, Expression * > &map, const REG key,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal,
    StateSummary &state) {
  typedef REG KEY;
  std::map < KEY, Expression * >::const_iterator it = map.find (key);
  if (it == map.end ()) { // not found!
    return 0;
  } else {
    Expression *exp = it->second;
    check_concrete_value_for_possible_state_mismatch
        (exp, key, concreteVal, state);
    if (state.isWrongState ()) {
      return 0;
    }
    return exp;
  }
}

template < >
Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionImplementation (
    int size, std::map < ADDRINT, Expression * > &map, const ADDRINT key,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal,
    StateSummary &state) {
  typedef ADDRINT KEY;
  std::map < KEY, Expression * >::const_iterator it = map.find (key);
  if (it == map.end ()) { // not found!
    return 0;
  } else {
    Expression *exp = it->second;
    if (exp == NULL) { // expression is lazy-loaded && KEY == ADDRINT
      return lazy_load_symbolic_expression
          (this, size, map, key, concreteVal, state);
    }
    check_concrete_value_for_possible_state_mismatch
        (exp, key, concreteVal, state);
    if (state.isWrongState ()) {
      return 0;
    }
    return exp;
  }
}

template < >
void check_concrete_value_for_possible_state_mismatch (Expression *exp,
    REG reg, const edu::sharif::twinner::trace::cv::ConcreteValue &val,
    StateSummary &state) {
  exp->checkConcreteValueReg (reg, val, state);
}

template < >
void check_concrete_value_for_possible_state_mismatch (Expression *exp,
    ADDRINT memoryEa, const edu::sharif::twinner::trace::cv::ConcreteValue &val,
    StateSummary &state) {
  exp->checkConcreteValueMemory (memoryEa, val, state);
}

template < >
Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionImplementation (
    int size, std::map < REG, Expression * > &map, const REG key) {
  typedef REG KEY;
  std::map < KEY, Expression * >::const_iterator it = map.find (key);
  if (it == map.end ()) { // not found!
    return 0;
  } else {
    return it->second;
  }
}

template < >
Expression *ExecutionTraceSegment::tryToGetSymbolicExpressionImplementation (
    int size, std::map < ADDRINT, Expression * > &map, const ADDRINT key) {
  typedef ADDRINT KEY;
  std::map < KEY, Expression * >::const_iterator it = map.find (key);
  if (it == map.end ()) { // not found!
    return 0;
  } else {
    Expression *exp = it->second;
    if (exp == NULL) { // expression is lazy-loaded
      return lazy_load_symbolic_expression (this, size, map, key);
    }
    return exp;
  }
}

Expression *ExecutionTraceSegment::getSymbolicExpressionByRegister (int size, REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
    Expression *newExpression, StateSummary &state) {
  UNUSED_VARIABLE (size);
  return getSymbolicExpressionImplementation
      (size, registerToExpression, reg, regval, newExpression, state);
}

Expression *ExecutionTraceSegment::getSymbolicExpressionByRegister (int size, REG reg,
    Expression *newExpression) {
  UNUSED_VARIABLE (size);
  return getSymbolicExpressionImplementation
      (size, registerToExpression, reg, newExpression);
}

Expression *ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa, const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
    Expression *newExpression, StateSummary &state) {
  switch (size) {
  case 128:
    return getSymbolicExpressionImplementation
        (128, memoryAddressTo128BitsExpression, memoryEa, memval,
         newExpression, state);
  case 64:
    return getSymbolicExpressionImplementation
        (64, memoryAddressTo64BitsExpression, memoryEa, memval,
         newExpression, state);
  case 32:
    return getSymbolicExpressionImplementation
        (32, memoryAddressTo32BitsExpression, memoryEa, memval,
         newExpression, state);
  case 16:
    return getSymbolicExpressionImplementation
        (16, memoryAddressTo16BitsExpression, memoryEa, memval,
         newExpression, state);
  case 8:
    return getSymbolicExpressionImplementation
        (8, memoryAddressTo8BitsExpression, memoryEa, memval,
         newExpression, state);
  default:
    edu::sharif::twinner::util::Logger::error () <<
        "ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress"
        " (size=" << std::dec << size
        << ", memoryEa=0x" << std::hex << memoryEa
        << ", memval=" << memval << "): Memory read size is not supported\n";
    abort ();
  }
}

Expression *ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa, Expression *newExpression) {
  switch (size) {
  case 128:
    return getSymbolicExpressionImplementation
        (128, memoryAddressTo128BitsExpression, memoryEa, newExpression);
  case 64:
    return getSymbolicExpressionImplementation
        (64, memoryAddressTo64BitsExpression, memoryEa, newExpression);
  case 32:
    return getSymbolicExpressionImplementation
        (32, memoryAddressTo32BitsExpression, memoryEa, newExpression);
  case 16:
    return getSymbolicExpressionImplementation
        (16, memoryAddressTo16BitsExpression, memoryEa, newExpression);
  case 8:
    return getSymbolicExpressionImplementation
        (8, memoryAddressTo8BitsExpression, memoryEa, newExpression);
  default:
    edu::sharif::twinner::util::Logger::error () <<
        "ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress"
        " (size=" << std::dec << size
        << ", memoryEa=0x" << std::hex << memoryEa
        << "): Memory read size is not supported\n";
    abort ();
  }
}

template < typename KEY >
Expression *ExecutionTraceSegment::getSymbolicExpressionImplementation (
    int size, std::map < KEY, Expression * > &map, const KEY key,
    const edu::sharif::twinner::trace::cv::ConcreteValue &currentConcreteValue,
    Expression *newExpression, StateSummary &state) {
  typedef typename std::map < KEY, Expression * >::iterator MapIterator;
  Expression *exp = tryToGetSymbolicExpressionImplementation
      (size, map, key, currentConcreteValue, state);
  if (exp) {
    return exp;
  }
  if (state.isWrongState ()) {
    return 0;
  }
  if (!newExpression) {
    edu::sharif::twinner::util::Logger::error ()
        << "ExecutionTraceSegment::getSymbolicExpressionImplementation method:"
        " newExpression is null!\n";
    abort ();
  }
  std::pair < MapIterator, bool > res = map.insert (make_pair (key, newExpression));
  if (!res.second) { // another expression already exists. overwriting...
    MapIterator it = res.first;
    delete it->second;
    it->second = newExpression;
  }
  return newExpression;
}

template < typename KEY >
Expression *ExecutionTraceSegment::getSymbolicExpressionImplementation (
    int size, std::map < KEY, Expression * > &map, const KEY key,
    Expression *newExpression) {
  typedef typename std::map < KEY, Expression * >::iterator MapIterator;
  Expression *exp = tryToGetSymbolicExpressionImplementation
      (size, map, key);
  if (exp) {
    return exp;
  }
  if (!newExpression) {
    edu::sharif::twinner::util::Logger::error ()
        << "ExecutionTraceSegment::getSymbolicExpressionImplementation method:"
        " newExpression is null!\n";
    abort ();
  }
  std::pair < MapIterator, bool > res = map.insert (make_pair (key, newExpression));
  if (!res.second) {
    // as there was no expression (regardless of concrete value), this case is impossible!
    edu::sharif::twinner::util::Logger::error ()
        << "ExecutionTraceSegment::getSymbolicExpressionImplementation method:"
        " can not set expression, while there was no prior expression!\n";
    abort ();
  }
  return newExpression;
}

Expression *ExecutionTraceSegment::setSymbolicExpressionByRegister (int regsize, REG reg,
    const Expression *exp) {
  return setSymbolicExpressionImplementation
      (regsize, registerToExpression, reg, exp);
}

Expression *ExecutionTraceSegment::setSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa, const Expression *exp) {
  switch (size) {
  case 128:
    return setSymbolicExpressionImplementation
        (128, memoryAddressTo128BitsExpression, memoryEa, exp);
  case 64:
    return setSymbolicExpressionImplementation
        (64, memoryAddressTo64BitsExpression, memoryEa, exp);
  case 32:
    return setSymbolicExpressionImplementation
        (32, memoryAddressTo32BitsExpression, memoryEa, exp);
  case 16:
    return setSymbolicExpressionImplementation
        (16, memoryAddressTo16BitsExpression, memoryEa, exp);
  case 8:
    return setSymbolicExpressionImplementation
        (8, memoryAddressTo8BitsExpression, memoryEa, exp);
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "ExecutionTraceSegment::setSymbolicExpressionByMemoryAddress"
        " (size=" << std::dec << size
        << ", memoryEa=0x" << std::hex << memoryEa
        << ", exp=" << exp
        << "): Memory write size is not supported\n";
    abort ();
  }
}

template < typename KEY >
Expression *ExecutionTraceSegment::setSymbolicExpressionImplementation (int size,
    std::map < KEY, Expression * > &map, const KEY key,
    const Expression *nonOwnedExpression) {
  typedef typename std::map < KEY, Expression * >::iterator MapIterator;
  // The nonOwnedExpression is owned by caller.
  // We must clone it and take ownership of the cloned object.
  // A null expression means that the expression should be lazy-loaded.
  Expression *exp = nonOwnedExpression != NULL
      ? nonOwnedExpression->clone (size) : NULL;
  std::pair < MapIterator, bool > res = map.insert (make_pair (key, exp));
  if (!res.second) { // another expression already exists. overwriting...
    // old expression is owned by us; so it should be deleted before missing its pointer!
    MapIterator it = res.first;
    delete it->second;
    it->second = exp;
  }
  return exp;
}

void ExecutionTraceSegment::addPathConstraints (
    const std::list <edu::sharif::twinner::trace::Constraint *> &constraints,
    const edu::sharif::twinner::trace::Constraint *lastConstraint) {
  for (std::list <edu::sharif::twinner::trace::Constraint *>::const_iterator it =
      constraints.begin (); it != constraints.end (); ++it) {
    edu::sharif::twinner::trace::Constraint *c = *it;
    if (c->isTrivial () || (lastConstraint && (*lastConstraint) == (*c))) {
      delete c;
    } else {
      edu::sharif::twinner::util::Logger::warning () << "adding " << c << '\n';
      pathConstraints.push_back (c);
      lastConstraint = c;
    }
  }
}

void ExecutionTraceSegment::saveToBinaryStream (std::ofstream &out) const {
  // USE CASE: TwinTool saves segments, so Twinner can read them back
  const char *segmentMagicString = "SEG";
  out.write (segmentMagicString, 3);
  out.write (reinterpret_cast<const char *> (&segmentIndex), sizeof (segmentIndex));

  saveMapToBinaryStream (out, "REG", registerToExpression);
  std::map < ADDRINT, Expression * > memory;
  for (std::map < ADDRINT, Expression * >::const_iterator it =
      memoryAddressTo64BitsExpression.begin ();
      it != memoryAddressTo64BitsExpression.end (); ++it) {
    ADDRINT address = it->first;
    Expression *exp = it->second;
    if (exp) {
      memory.insert (make_pair (address, exp));
    }
  }
  saveMapToBinaryStream (out, "MEM", memory);
  saveListToBinaryStream (out, "CON", pathConstraints);
}

template <typename ADDRESS>
void ExecutionTraceSegment::saveMapToBinaryStream (std::ofstream &out,
    const char *magicString, const std::map < ADDRESS, Expression * > &map) const {
  out.write (magicString, strlen (magicString));

  typename std::map < ADDRESS, Expression * >::size_type s = map.size ();
  out.write ((const char *) &s, sizeof (s));
  for (typename std::map < ADDRESS, Expression * >::const_iterator it = map.begin ();
      it != map.end (); ++it) {
    const ADDRESS a = it->first;
    out.write ((const char *) &a, sizeof (a));

    const Expression *exp = it->second;
    exp->saveToBinaryStream (out);
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
  std::map < REG, Expression * > regi;
  // The memo only contains 64-bits memory symbols (+ 128/64-bits reg symbols of course)
  // Also the precision of ADDRINT is 64-bits (both memory cells and formulas are 64-bits)
  std::map < ADDRINT, Expression * > memo;
  std::list < Constraint * > cnrt;
  loadMapFromBinaryStream (in, "REG", regi);
  loadMapFromBinaryStream (in, "MEM", memo);
  loadListFromBinaryStream (in, "CON", cnrt);

  return new ExecutionTraceSegment (segmentIndex, regi, memo, cnrt);
}

int ExecutionTraceSegment::getSegmentIndex () const {
  return segmentIndex;
}

template <typename ADDRESS>
void ExecutionTraceSegment::loadMapFromBinaryStream (std::ifstream &in,
    const char *expectedMagicString, std::map < ADDRESS, Expression * > &map) {
  typedef typename std::map < ADDRESS, Expression * >::iterator MapIterator;
  char magicString[3];
  in.read (magicString, 3);
  if (strncmp (magicString, expectedMagicString, 3) != 0) {
    edu::sharif::twinner::util::Logger::error ()
        << "ExecutionTraceSegment::loadMapFromBinaryStream (...): "
        "Unexpected magic string while loading map from binary stream\n";
    abort ();
  }
  typename std::map < ADDRESS, Expression * >::size_type s;
  in.read ((char *) &s, sizeof (s));
  while (s-- > 0) {
    ADDRESS a;
    in.read ((char *) &a, sizeof (a));

    Expression *exp = Expression::loadFromBinaryStream (in);
    std::pair < MapIterator, bool > res = map.insert (make_pair (a, exp));
    if (!res.second) {
      edu::sharif::twinner::util::Logger::error ()
          << "ExecutionTraceSegment::loadMapFromBinaryStream (...): "
          "Can not read map's entry from binary stream\n";
      abort ();
    }
  }
}

void removeItemFromMap (std::map < ADDRINT, Expression * > &map, ADDRINT addr) {
  std::map < ADDRINT, Expression * >::iterator item = map.find (addr);
  if (item != map.end ()) {
    delete item->second;
    map.erase (item);
  }
};

void ExecutionTraceSegment::abandonTrivialMemoryExpressions () {
  std::set<ADDRINT> trivialAddresses;
  for (std::map < ADDRINT, Expression * >::const_iterator it =
      memoryAddressTo128BitsExpression.begin ();
      it != memoryAddressTo128BitsExpression.end (); ++it) {
    if (it->second->isTrivial ()) {
      trivialAddresses.insert (it->first);
    }
  }
  for (std::set<ADDRINT>::const_iterator it = trivialAddresses.begin ();
      it != trivialAddresses.end (); ++it) {
    const ADDRINT addr128 = *it;
    removeItemFromMap (memoryAddressTo128BitsExpression, addr128);
    for (int i = 0; i < 16; i += 8) {
      removeItemFromMap (memoryAddressTo64BitsExpression, addr128 + i);
    }
    for (int i = 0; i < 16; i += 4) {
      removeItemFromMap (memoryAddressTo32BitsExpression, addr128 + i);
    }
    for (int i = 0; i < 16; i += 2) {
      removeItemFromMap (memoryAddressTo16BitsExpression, addr128 + i);
    }
    for (int i = 0; i < 16; i += 1) {
      removeItemFromMap (memoryAddressTo8BitsExpression, addr128 + i);
    }
  }
}

void ExecutionTraceSegment::printRegistersValues (
    const edu::sharif::twinner::util::Logger &logger) const {
  logger << registerToExpression;
}

void ExecutionTraceSegment::printMemoryAddressesValues (
    const edu::sharif::twinner::util::Logger &logger) const {
  logger << memoryAddressTo64BitsExpression;
}

void ExecutionTraceSegment::printPathConstraints (
    const edu::sharif::twinner::util::Logger &logger) const {
  logger << pathConstraints;
}

void ExecutionTraceSegment::printCompleteState (
    const edu::sharif::twinner::util::Logger &logger) const {
  logger << "Registers:\n";
  printRegistersValues (logger);
  logger << "Memory:\n";
  printMemoryAddressesValues (logger);
  logger << "Constraints:\n";
  printPathConstraints (logger);
  if (terminator) {
    logger << "Terminator: " << terminator->toString () << '\n';
  } else {
    logger << "Terminator: no terminator\n";
  }
}

const std::map < REG, Expression * > &
ExecutionTraceSegment::getRegisterToExpression () const {
  return registerToExpression;
}

const std::map < ADDRINT, Expression * > &
ExecutionTraceSegment::getMemoryAddressTo64BitsExpression () const {
  return memoryAddressTo64BitsExpression;
}

const std::map < ADDRINT, Expression * > &
ExecutionTraceSegment::getMemoryAddressTo8BitsExpression () const {
  return memoryAddressTo8BitsExpression;
}

const std::list < Constraint * > &
ExecutionTraceSegment::getPathConstraints () const {
  return pathConstraints;
}

std::list < Constraint * > &ExecutionTraceSegment::getPathConstraints () {
  return pathConstraints;
}

template <typename Addr>
int ExecutionTraceSegment::calcSizeInBytes (
    const std::map < Addr, Expression * > &map) const {
  int size = 0;
  for (typename std::map < Addr, Expression * >::const_iterator it = map.begin ();
      it != map.end (); ++it) {
    size += sizeof (typename std::map < Addr, Expression * >::value_type);
    if (it->second == NULL) {
      continue;
    }
    const std::list < edu::sharif::twinner::trace::exptoken::ExpressionToken * > &stack = it->second->getStack ();
    for (std::list < edu::sharif::twinner::trace::exptoken::ExpressionToken * >::const_iterator it2 = stack.begin ();
        it2 != stack.end (); ++it2) {
      size += sizeof (std::list < edu::sharif::twinner::trace::exptoken::ExpressionToken * >::value_type);
      const edu::sharif::twinner::trace::exptoken::ExpressionToken *token = *it2;
      size += sizeof (*token);
    }
  }
  return size;
}

int ExecutionTraceSegment::printMemoryUsageStats (
    const edu::sharif::twinner::util::Logger &logger) const {
  const int memTo128bExpSize = calcSizeInBytes (memoryAddressTo128BitsExpression);
  const int memTo64bExpSize = calcSizeInBytes (memoryAddressTo64BitsExpression);
  const int memTo32bExpSize = calcSizeInBytes (memoryAddressTo32BitsExpression);
  const int memTo16bExpSize = calcSizeInBytes (memoryAddressTo16BitsExpression);
  const int memTo8bExpSize = calcSizeInBytes (memoryAddressTo8BitsExpression);
  const int memSize = memTo128bExpSize + memTo64bExpSize
      + memTo32bExpSize + memTo16bExpSize + memTo8bExpSize;
  const int regToExpSize = calcSizeInBytes (registerToExpression);
  const int total = memSize + regToExpSize;
  logger << std::dec << total << " bytes consisting of "
      << memSize << " bytes for memory symbols ("
      << memTo128bExpSize << " bytes for 128, "
      << memTo64bExpSize << " for 64, "
      << memTo32bExpSize << " for 32, "
      << memTo16bExpSize << " for 16, "
      << memTo8bExpSize << " for 8 bits map)"
      " and " << regToExpSize << " bytes for register symbols";
  return total;
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

}
}
}
}
