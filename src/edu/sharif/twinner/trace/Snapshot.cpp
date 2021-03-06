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

#include "Snapshot.h"

#include "Constraint.h"
#include "StateSummary.h"
#include "TraceSegmentTerminator.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"

#include "edu/sharif/twinner/trace/exptoken/RegisterEmergedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/MemoryEmergedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/Constant.h"
#include "edu/sharif/twinner/trace/ExpressionImp.h"
#include "edu/sharif/twinner/trace/MarInfo.h"

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

Snapshot::Snapshot (int _segmentIndex, int _snapshotIndex,
    const std::map < REG, Expression * > &regi,
    const std::map < ADDRINT, Expression * > &memo128,
    const std::map < ADDRINT, Expression * > &memo64,
    const std::map < ADDRINT, Expression * > &memo32,
    const std::map < ADDRINT, Expression * > &memo16,
    const std::map < ADDRINT, Expression * > &memo8,
    const std::list < Constraint * > &cnrt) :
    registerToExpression (regi),
    memoryAddressTo128BitsExpression (memo128),
    memoryAddressTo64BitsExpression (memo64),
    memoryAddressTo32BitsExpression (memo32),
    memoryAddressTo16BitsExpression (memo16),
    memoryAddressTo8BitsExpression (memo8),
    pathConstraints (cnrt),
    segmentIndex (_segmentIndex), snapshotIndex (_snapshotIndex) {
  /*
   * This constructor is called by Twinner to reacquire registers/memory/constraints
   * info and use them to build its behavioral model.
   */
}

Snapshot::Snapshot (int _segmentIndex, int _snapshotIndex) :
    segmentIndex (_segmentIndex), snapshotIndex (_snapshotIndex) {
}

Snapshot::Snapshot (int _segmentIndex, int _snapshotIndex,
    const std::map < REG, Expression * > &regMap,
    const std::map < ADDRINT, Expression * > &memMap) :
    registerToExpression (regMap),
    segmentIndex (_segmentIndex), snapshotIndex (_snapshotIndex) {
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
    setOverwritingMemoryExpression
        (size, memoryEa, exp, exp->isOverwritingExpression ());
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

Snapshot::~Snapshot () {
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
}

Snapshot *Snapshot::clone () const {
  std::map < REG, Expression * > clonedRegisterToExpression;
  for (std::map < REG, Expression * >::const_iterator it =
      registerToExpression.begin (); it != registerToExpression.end (); ++it) {
    clonedRegisterToExpression.insert
        (make_pair (it->first, it->second->clone ()));
  }
  std::map < ADDRINT, Expression * > clonedMemToExp[5];
  const std::map < ADDRINT, Expression * > *memToExp[]
      = {&memoryAddressTo128BitsExpression, &memoryAddressTo64BitsExpression,
    &memoryAddressTo32BitsExpression, &memoryAddressTo16BitsExpression,
    &memoryAddressTo8BitsExpression, 0};
  for (int i = 0; memToExp[i]; ++i) {
    for (std::map < ADDRINT, Expression * >::const_iterator it =
        memToExp[i]->begin (); it != memToExp[i]->end (); ++it) {
      clonedMemToExp[i].insert (make_pair (it->first, it->second->clone ()));
    }
  }
  std::list < Constraint * > clonedPathConstraints;
  for (std::list < Constraint * >::const_iterator it = pathConstraints.begin ();
      it != pathConstraints.end (); ++it) {
    clonedPathConstraints.push_back ((*it)->clone ());
  }
  return new Snapshot (segmentIndex, snapshotIndex, clonedRegisterToExpression,
                       clonedMemToExp[0], clonedMemToExp[1],
                       clonedMemToExp[2], clonedMemToExp[3],
                       clonedMemToExp[4], clonedPathConstraints);
}

Snapshot *Snapshot::instantiateNexSnapshot (const Snapshot &previousSnapshot) {
  const int segmentIndex = previousSnapshot.getSegmentIndex ();
  const int snapshotIndex = previousSnapshot.getSnapshotIndex () + 1;
  /*
   * This method is called after adding some new constraints to an existing
   * execution trace segment to create a new snapshot based on
   * the previousSnapshot registers/memory contents.
   * As this is not the first snapshot of a segment, all registers are in
   * the non-overwriting mode.
   */
  const std::map < REG, Expression * > &regMap =
      edu::sharif::twinner::trace::exptoken::RegisterEmergedSymbol
      ::instantiateTemporarySymbols
      (previousSnapshot.registerToExpression, segmentIndex, snapshotIndex);

  const std::map < ADDRINT, Expression * > * const memoryToExpressionMaps[] = {
    &previousSnapshot.memoryAddressTo128BitsExpression,
    &previousSnapshot.memoryAddressTo64BitsExpression,
    &previousSnapshot.memoryAddressTo32BitsExpression,
    &previousSnapshot.memoryAddressTo16BitsExpression,
    &previousSnapshot.memoryAddressTo8BitsExpression,
    NULL
  };
  const std::map < ADDRINT, Expression * > &memMap =
      edu::sharif::twinner::trace::exptoken::MemoryEmergedSymbol
      ::instantiateTemporarySymbols
      (memoryToExpressionMaps, segmentIndex, snapshotIndex);
  return new Snapshot (segmentIndex, snapshotIndex, regMap, memMap);
}

void Snapshot::setTimedTrace (TimedTrace _timedTrace) {
  timedTrace = _timedTrace;
}

void Snapshot::setOverwritingMemoryExpression (int size,
    ADDRINT memoryEa, const Expression *expression, bool isOverwriting) {
  Expression *exp = setSymbolicExpressionByMemoryAddress (size, memoryEa, expression);
  exp->setOverwriting (isOverwriting);
}

bool Snapshot::areExpressionsEquivalent (const Expression &first,
    const Expression &second) const {
  if (first == second) {
    return true;
  }
  return false;
}

void Snapshot::initializeOverlappingMemoryLocationsDownwards (int size,
    ADDRINT memoryEa, const Expression &expression, int shiftAmount) {
  size /= 2;
  if (size >= 8) {
    if (!isSymbolicExpressionAvailableInMemoryAddress (size, memoryEa)) {
      Expression *exp = expression.clone ();
      if (shiftAmount > 0) {
        exp->shiftToRight (shiftAmount);
      }
      exp->truncate (size); // LSB (left-side in little-endian)
      setOverwritingMemoryExpression
          (size, memoryEa, exp, expression.isOverwritingExpression ());
      delete exp;
      initializeOverlappingMemoryLocationsDownwards (size, memoryEa, expression,
                                                     shiftAmount);
    }
    if (!isSymbolicExpressionAvailableInMemoryAddress (size, memoryEa + size / 8)) {
      Expression *exp = expression.clone ();
      exp->shiftToRight (shiftAmount + size); // MSB (right-side in little-endian)
      exp->truncate (size);
      setOverwritingMemoryExpression
          (size, memoryEa + size / 8, exp, expression.isOverwritingExpression ());
      delete exp;
      initializeOverlappingMemoryLocationsDownwards (size, memoryEa + size / 8, expression,
                                                     shiftAmount + size);
    }
  }
}

void Snapshot::initializeOverlappingMemoryLocationsUpwards (
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

Expression *Snapshot::tryToGetSymbolicExpressionByRegister (int size,
    REG reg, const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
    StateSummary &state) {
  return tryToGetSymbolicExpressionImplementation
      (size, registerToExpression, reg, regval, state);
}

Expression *Snapshot::tryToGetSymbolicExpressionByRegister (int size,
    REG reg) {
  return tryToGetSymbolicExpressionImplementation (size, registerToExpression, reg);
}

Expression *Snapshot::tryToGetSymbolicExpressionByMemoryAddress (int size,
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
        "Snapshot::tryToGetSymbolicExpressionByMemoryAddress"
        " (size=" << std::dec << size
        << ", memoryEa=0x" << std::hex << memoryEa
        << ", memval=" << memval << "): Memory read size is not supported\n";
    abort ();
  }
}

Expression *Snapshot::tryToGetSymbolicExpressionByMemoryAddress (int size,
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
        "Snapshot::tryToGetSymbolicExpressionByMemoryAddress"
        " (size=" << std::dec << size
        << ", memoryEa=0x" << std::hex << memoryEa
        << "): Memory read size is not supported\n";
    abort ();
  }
}

bool Snapshot::isSymbolicExpressionAvailableInRegister (int size,
    REG reg) const {
  return isSymbolicExpressionAvailableImplementation (size, registerToExpression, reg);
}

bool Snapshot::isSymbolicExpressionAvailableInMemoryAddress (int size,
    ADDRINT memoryEa) const {
  switch (size) {
  case 128:
    return isSymbolicExpressionAvailableImplementation
        (128, memoryAddressTo128BitsExpression, memoryEa);
  case 64:
    return isSymbolicExpressionAvailableImplementation
        (64, memoryAddressTo64BitsExpression, memoryEa);
  case 32:
    return isSymbolicExpressionAvailableImplementation
        (32, memoryAddressTo32BitsExpression, memoryEa);
  case 16:
    return isSymbolicExpressionAvailableImplementation
        (16, memoryAddressTo16BitsExpression, memoryEa);
  case 8:
    return isSymbolicExpressionAvailableImplementation
        (8, memoryAddressTo8BitsExpression, memoryEa);
  default:
    edu::sharif::twinner::util::Logger::error () <<
        "Snapshot::isSymbolicExpressionAvailableInMemoryAddress"
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

template < typename KEY >
Expression *Snapshot::tryToGetSymbolicExpressionImplementation (
    int size, std::map < KEY, Expression * > &map, const KEY key,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal,
    StateSummary &state) {
  typename std::map < KEY, Expression * >::const_iterator it = map.find (key);
  if (it == map.end ()) { // not found!
    return 0;
  } else {
    Expression *exp = it->second;
    if (exp == NULL) { // expression is lazy-loaded && KEY == ADDRINT
      return lazyLoad (size, map, key, concreteVal, state);
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

template < typename KEY >
Expression *Snapshot::tryToGetSymbolicExpressionImplementation (
    int size, std::map < KEY, Expression * > &map, const KEY key) {
  typename std::map < KEY, Expression * >::const_iterator it = map.find (key);
  if (it == map.end ()) { // not found!
    return 0;
  } else {
    Expression *exp = it->second;
    if (exp == NULL) { // expression is lazy-loaded
      return lazyLoad (size, map, key);
    }
    return exp;
  }
}

template < typename KEY >
bool Snapshot::isSymbolicExpressionAvailableImplementation (
    int size, const std::map < KEY, Expression * > &map, const KEY key) const {
  typename std::map < KEY, Expression * >::const_iterator it = map.find (key);
  if (it == map.end ()) { // not found!
    return false;
  } else {
    const Expression *exp = it->second;
    if (exp == NULL) { // expression is lazy-loaded
      return false;
    }
    return true;
  }
}

Expression *Snapshot::getSymbolicExpressionByRegister (int size, REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
    Expression *newExpression, StateSummary &state) {
  UNUSED_VARIABLE (size);
  return getSymbolicExpressionImplementation
      (size, registerToExpression, reg, regval, newExpression, state);
}

Expression *Snapshot::getSymbolicExpressionByRegister (int size, REG reg,
    Expression *newExpression) {
  UNUSED_VARIABLE (size);
  return getSymbolicExpressionImplementation
      (size, registerToExpression, reg, newExpression);
}

Expression *Snapshot::getSymbolicExpressionByMemoryAddress (int size,
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
        "Snapshot::getSymbolicExpressionByMemoryAddress"
        " (size=" << std::dec << size
        << ", memoryEa=0x" << std::hex << memoryEa
        << ", memval=" << memval << "): Memory read size is not supported\n";
    abort ();
  }
}

Expression *Snapshot::getSymbolicExpressionByMemoryAddress (int size,
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
        "Snapshot::getSymbolicExpressionByMemoryAddress"
        " (size=" << std::dec << size
        << ", memoryEa=0x" << std::hex << memoryEa
        << "): Memory read size is not supported\n";
    abort ();
  }
}

template < typename KEY >
Expression *Snapshot::getSymbolicExpressionImplementation (
    int size, std::map < KEY, Expression * > &map, const KEY key,
    const edu::sharif::twinner::trace::cv::ConcreteValue &currentConcreteValue,
    Expression *newExpression, StateSummary &state) {
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
        << "Snapshot::getSymbolicExpressionImplementation method:"
        " newExpression is null!\n";
    abort ();
  }
  setExpression (map, key, newExpression);
  return newExpression;
}

template < typename KEY >
Expression *Snapshot::getSymbolicExpressionImplementation (
    int size, std::map < KEY, Expression * > &map, const KEY key,
    Expression *newExpression) {
  Expression *exp = tryToGetSymbolicExpressionImplementation
      (size, map, key);
  if (exp) {
    return exp;
  }
  if (!newExpression) {
    edu::sharif::twinner::util::Logger::error ()
        << "Snapshot::getSymbolicExpressionImplementation method:"
        " newExpression is null!\n";
    abort ();
  }
  setExpression (map, key, newExpression);
  return newExpression;
}

Expression *Snapshot::setSymbolicExpressionByRegister (int regsize, REG reg,
    const Expression *exp) {
  return setSymbolicExpressionImplementation
      (regsize, registerToExpression, reg, exp);
}

Expression *Snapshot::setSymbolicExpressionByMemoryAddress (int size,
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
        << "Snapshot::setSymbolicExpressionByMemoryAddress"
        " (size=" << std::dec << size
        << ", memoryEa=0x" << std::hex << memoryEa
        << ", exp=" << exp
        << "): Memory write size is not supported\n";
    abort ();
  }
}

template < typename KEY >
Expression *Snapshot::setSymbolicExpressionImplementation (int size,
    std::map < KEY, Expression * > &map, const KEY key,
    const Expression *nonOwnedExpression) {
  // The nonOwnedExpression is owned by caller.
  // We must clone it and take ownership of the cloned object.
  // A null expression means that the expression should be lazy-loaded.
  Expression *exp = nonOwnedExpression != NULL
      ? nonOwnedExpression->clone (size) : NULL;
  setExpression (map, key, exp);
  return exp;
}

template < typename KEY >
void Snapshot::setExpression (std::map < KEY, Expression * > &map,
    const KEY key, Expression *exp) {
  typedef typename std::map < KEY, Expression * >::iterator MapIterator;
  std::pair < MapIterator, bool > res = map.insert (make_pair (key, exp));
  if (!res.second) { // another expression already exists. overwriting...
    MapIterator it = res.first;
    delete it->second;
    it->second = exp;
  }
}

void Snapshot::addPathConstraints (
    const std::list <edu::sharif::twinner::trace::Constraint *> &constraints,
    const edu::sharif::twinner::trace::Constraint *lastConstraint) {
  if (lastConstraint) {
    edu::sharif::twinner::util::Logger::error ()
        << "Snapshot::addPathConstraints (): "
        "lastConstraint argument must be null\n";
    abort ();
  }
  if (!pathConstraints.empty ()) {
    edu::sharif::twinner::util::Logger::error ()
        << "Snapshot::addPathConstraints (): "
        "constraints can be added just once; "
        "this snapshot constraints are already added!\n";
    abort ();
  }
  pathConstraints = constraints;
}

void Snapshot::saveToBinaryStream (std::ofstream &out) const {
  // USE CASE: TwinTool saves segments, so Twinner can read them back
  const char *segmentMagicString = "PRT";
  out.write (segmentMagicString, 3);
  out.write (reinterpret_cast<const char *> (&segmentIndex), sizeof (segmentIndex));
  out.write (reinterpret_cast<const char *> (&snapshotIndex), sizeof (snapshotIndex));

  saveMapToBinaryStream (out, "REG", registerToExpression);
  saveNonNullExpressions (out, memoryAddressTo128BitsExpression);
  saveNonNullExpressions (out, memoryAddressTo64BitsExpression);
  saveNonNullExpressions (out, memoryAddressTo32BitsExpression);
  saveNonNullExpressions (out, memoryAddressTo16BitsExpression);
  saveNonNullExpressions (out, memoryAddressTo8BitsExpression);
  saveListToBinaryStream (out, "CON", pathConstraints);
}

void Snapshot::saveNonNullExpressions (std::ofstream &out,
    const std::map < ADDRINT, Expression * > &memToExp) const {
  std::map < ADDRINT, Expression * > memory;
  for (std::map < ADDRINT, Expression * >::const_iterator it =
      memToExp.begin (); it != memToExp.end (); ++it) {
    ADDRINT address = it->first;
    Expression *exp = it->second;
    if (exp) {
      memory.insert (make_pair (address, exp));
    }
  }
  saveMapToBinaryStream (out, "MEM", memory);
}

template <typename ADDRESS>
void Snapshot::saveMapToBinaryStream (std::ofstream &out,
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

Snapshot *Snapshot::loadFromBinaryStream (std::ifstream &in) {
  // USE CASE: Twinner loads segments which were saved by TwinTool
  char segmentMagicString[3];
  in.read (segmentMagicString, 3);
  if (strncmp (segmentMagicString, "PRT", 3) != 0) {
    edu::sharif::twinner::util::Logger::error ()
        << "Snapshot::loadFromBinaryStream (...): Unexpected "
        "magic string while loading trace segment from binary stream\n";
    abort ();
  }
  int segmentIndex;
  in.read (reinterpret_cast<char *> (&segmentIndex), sizeof (segmentIndex));
  int snapshotIndex;
  in.read (reinterpret_cast<char *> (&snapshotIndex), sizeof (snapshotIndex));
  std::map < REG, Expression * > regi;
  // The memo only contains 64-bits memory symbols (+ 128/64-bits reg symbols of course)
  // Also the precision of ADDRINT is 64-bits (both memory cells and formulas are 64-bits)
  std::map < ADDRINT, Expression * > memo128, memo64, memo32, memo16, memo8;
  std::list < Constraint * > cnrt;
  loadMapFromBinaryStream (in, "REG", regi);
  loadMapFromBinaryStream (in, "MEM", memo128);
  loadMapFromBinaryStream (in, "MEM", memo64);
  loadMapFromBinaryStream (in, "MEM", memo32);
  loadMapFromBinaryStream (in, "MEM", memo16);
  loadMapFromBinaryStream (in, "MEM", memo8);
  loadListFromBinaryStream (in, "CON", cnrt);

  return new Snapshot (segmentIndex, snapshotIndex, regi,
                       memo128, memo64, memo32, memo16, memo8, cnrt);
}

int Snapshot::getSegmentIndex () const {
  return segmentIndex;
}

int Snapshot::getSnapshotIndex () const {
  return snapshotIndex;
}

int Snapshot::getMemorySize () const {
  int size = sizeof (Snapshot);
  for (std::map < REG, Expression * >::const_iterator it =
      registerToExpression.begin (); it != registerToExpression.end (); ++it) {
    Expression *exp = it->second;
    size += (exp ? exp->getMemorySize () : sizeof (exp)) + sizeof (REG);
  }
  const std::map < ADDRINT, Expression * > * const memoryToExpressionMaps[] = {
    &memoryAddressTo128BitsExpression,
    &memoryAddressTo64BitsExpression,
    &memoryAddressTo32BitsExpression,
    &memoryAddressTo16BitsExpression,
    &memoryAddressTo8BitsExpression,
    NULL
  };
  for (int i = 0; memoryToExpressionMaps[i]; ++i) {
    const std::map < ADDRINT, Expression * > *ptr = memoryToExpressionMaps[i];
    for (std::map < ADDRINT, Expression * >::const_iterator it = ptr->begin ();
        it != ptr->end (); ++it) {
      Expression *exp = it->second;
      size += (exp ? exp->getMemorySize () : sizeof (exp)) + sizeof (ADDRINT);
    }
  }
  for (std::list < Constraint * >::const_iterator it = pathConstraints.begin ();
      it != pathConstraints.end (); ++it) {
    const Constraint *c = *it;
    size += c->getMemorySize ();
  }
  return size;
}

void Snapshot::addTemporaryExpressions (const Snapshot *sna,
    REG fullReg, int size) {
  std::map < REG, Expression * >::const_iterator it =
      sna->registerToExpression.find (fullReg);
  if (it == sna->registerToExpression.end ()) {
    return;
  }
  const Expression *exp = it->second;
  Expression *tmpExp = new ExpressionImp
      (fullReg, exp->getLastConcreteValue (), segmentIndex, snapshotIndex);
  Expression *fullExp = setSymbolicExpressionByRegister (size, fullReg, tmpExp);
  delete tmpExp;
  edu::sharif::twinner::trace::exptoken::RegisterEmergedSymbol
      ::initializeSubRegisters (fullReg, this, *fullExp);
}

void Snapshot::addTemporaryExpressions (const Snapshot *sna,
    ADDRINT address, int size) {
  const std::map < ADDRINT, Expression * > * const memoryToExpressionMaps[] = {
    &sna->memoryAddressTo128BitsExpression,
    &sna->memoryAddressTo64BitsExpression,
    &sna->memoryAddressTo32BitsExpression,
    &sna->memoryAddressTo16BitsExpression,
    &sna->memoryAddressTo8BitsExpression,
    NULL
  };
  int level;
  switch (size) {
  case 128:
    level = 0;
    break;
  case 64:
    level = 1;
    break;
  case 32:
    level = 2;
    break;
  case 16:
    level = 3;
    break;
  case 8:
    level = 4;
    break;
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Snapshot::addTemporaryExpressions (): size=" << size
        << "is not supported\n";
    abort ();
  }
  addTemporaryExpressions (address, size, memoryToExpressionMaps, level);
}

void Snapshot::addTemporaryExpressions (ADDRINT memoryEa, int size,
    const std::map < ADDRINT, Expression * > * const *memoryToExpressionMaps,
    int level) {
  std::map < ADDRINT, Expression * >::const_iterator it =
      memoryToExpressionMaps[level]->find (memoryEa);
  if (it == memoryToExpressionMaps[level]->end ()) {
    return;
  }
  Expression *exp = it->second;
  if (exp == 0) {
    addTemporaryExpressions (memoryEa, size / 2,
                             memoryToExpressionMaps, level + 1);
    addTemporaryExpressions (memoryEa + size / 16, size / 2,
                             memoryToExpressionMaps, level + 1);
  } else {
    Expression *tmpExp = new ExpressionImp
        (memoryEa, exp->getLastConcreteValue (),
         segmentIndex, false, snapshotIndex);
    setOverwritingMemoryExpression
        (size, memoryEa, tmpExp, false);
    initializeOverlappingMemoryLocationsDownwards (size, memoryEa, *tmpExp);
    delete tmpExp; // setOverwritingMemoryExpression has cloned the tmpExp
    initializeOverlappingMemoryLocationsUpwards (size, memoryEa);
  }
}

template <typename ADDRESS>
void Snapshot::loadMapFromBinaryStream (std::ifstream &in,
    const char *expectedMagicString, std::map < ADDRESS, Expression * > &map) {
  typedef typename std::map < ADDRESS, Expression * >::iterator MapIterator;
  char magicString[3];
  in.read (magicString, 3);
  if (strncmp (magicString, expectedMagicString, 3) != 0) {
    edu::sharif::twinner::util::Logger::error ()
        << "Snapshot::loadMapFromBinaryStream (...): "
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
          << "Snapshot::loadMapFromBinaryStream (...): "
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

void Snapshot::abandonTrivialMemoryExpressions () {
  std::set<ADDRINT> trivialAddresses;
  for (std::map < ADDRINT, Expression * >::const_iterator it =
      memoryAddressTo128BitsExpression.begin ();
      it != memoryAddressTo128BitsExpression.end (); ++it) {
    if (it->second->isTrivial (false)) {
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

void Snapshot::printRegistersValues (
    const edu::sharif::twinner::util::Logger &logger) const {
  logger << registerToExpression;
}

void Snapshot::printMemoryAddressesValues (
    const edu::sharif::twinner::util::Logger &logger) const {
  logger << memoryAddressTo64BitsExpression;
}

void Snapshot::printPathConstraints (
    const edu::sharif::twinner::util::Logger &logger) const {
  logger << pathConstraints;
}

void Snapshot::printCompleteState (
    const edu::sharif::twinner::util::Logger &logger) const {
  logger << "Snapshot (index=" << snapshotIndex << "):\n";
  logger << "Registers:\n";
  printRegistersValues (logger);
  logger << "Memory:\n";
  printMemoryAddressesValues (logger);
  logger << "Constraints:\n";
  printPathConstraints (logger);
}

const std::map < REG, Expression * > &
Snapshot::getRegisterToExpression () const {
  return registerToExpression;
}

const std::map < ADDRINT, Expression * > &
Snapshot::getMemoryAddressTo64BitsExpression () const {
  return memoryAddressTo64BitsExpression;
}

const std::map < ADDRINT, Expression * > &
Snapshot::getMemoryAddressTo8BitsExpression () const {
  return memoryAddressTo8BitsExpression;
}

const std::list < Constraint * > &Snapshot::getPathConstraints () const {
  return pathConstraints;
}

std::list < Constraint * > &Snapshot::getPathConstraints () {
  return pathConstraints;
}

void Snapshot::replaceTemporarySymbols (const Snapshot *previousSnapshot) {
  replaceTemporarySymbols (previousSnapshot, registerToExpression);
  replaceTemporarySymbols (previousSnapshot, memoryAddressTo128BitsExpression);
  replaceTemporarySymbols (previousSnapshot, memoryAddressTo64BitsExpression);
  replaceTemporarySymbols (previousSnapshot, memoryAddressTo32BitsExpression);
  replaceTemporarySymbols (previousSnapshot, memoryAddressTo16BitsExpression);
  replaceTemporarySymbols (previousSnapshot, memoryAddressTo8BitsExpression);
  replaceTemporarySymbols (previousSnapshot, pathConstraints);
}

void Snapshot::simplify (const MarInfo *mar) {
  simplify (mar, registerToExpression);
  simplify (mar, memoryAddressTo128BitsExpression);
  simplify (mar, memoryAddressTo64BitsExpression);
  simplify (mar, memoryAddressTo32BitsExpression);
  simplify (mar, memoryAddressTo16BitsExpression);
  simplify (mar, memoryAddressTo8BitsExpression);
  const std::list < Constraint * > &constraints = pathConstraints;
  for (std::list < Constraint * >::const_iterator it3 = constraints.begin ();
      it3 != constraints.end (); ++it3) {
    Constraint *constraint = *it3;
    mar->simplifyExpression (constraint->getMainExpression ());
    mar->simplifyExpression (constraint->getAuxExpression ());
  }
}

template<typename KEY>
void Snapshot::simplify (const MarInfo *mar,
    std::map < KEY, Expression * > &expressions) {
  typedef typename std::map < KEY, Expression * >::const_iterator Iterator;
  for (Iterator it = expressions.begin (); it != expressions.end (); ++it) {
    mar->simplifyExpression (it->second);
  }
}

template<typename KEY>
void Snapshot::replaceTemporarySymbols (const Snapshot *previousSnapshot,
    std::map < KEY, Expression * > &expressions) {
  typedef typename std::map < KEY, Expression * >::const_iterator Iterator;
  for (Iterator it = expressions.begin (); it != expressions.end (); ++it) {
    previousSnapshot->replaceTemporarySymbols (it->second);
  }
}

void Snapshot::replaceTemporarySymbols (const Snapshot *previousSnapshot,
    std::list < Constraint * > &constraints) {
  typedef std::list < Constraint * >::const_iterator Iterator;
  for (Iterator it = constraints.begin (); it != constraints.end (); ++it) {
    Constraint *c = *it;
    previousSnapshot->replaceTemporarySymbols (c->getMainExpression ());
    if (c->getAuxExpression ()) {
      previousSnapshot->replaceTemporarySymbols (c->getAuxExpression ());
    }
  }
}

class ReplaceTemporarySymbolsVisitor :
public edu::sharif::twinner::trace::exptoken::ExpressionVisitor<Expression *> {
private:
  const Snapshot *previousSnapshot;

public:

  ReplaceTemporarySymbolsVisitor (const Snapshot *_previousSnapshot) :
      previousSnapshot (_previousSnapshot) {
  }

  virtual ResultType visitTrinary (const Expression::Operator *op,
      ResultType &left, ResultType &mid, ResultType &right) {
    if (!mid->isTrivial (true) || !right->isTrivial (true)) {
      edu::sharif::twinner::util::Logger::error ()
          << "ReplaceTemporarySymbolsVisitor::visitTrinary ():"
          " mid/right operators are not concrete\n";
      abort ();
    }
    {
      int size;
      if (op->getIdentifier () == Expression::Operator::SIGN_EXTEND) {
        size = right->getLastConcreteValue ().toUint64 ();
      } else {
        size = mid->getLastConcreteValue ().toUint64 ();
      }
      Expression *exp = left->clone (size);
      delete left;
      left = exp;
    }
    Expression::Operator *opc = op->clone ();
    if (opc->apply (left,
                    mid->getLastConcreteValue ().clone (),
                    right->getLastConcreteValue ().clone ())) {
      delete opc;
    }
    delete mid;
    delete right;
    return left;
  }

  virtual ResultType visitFunctionalBinary (const Expression::Operator *op,
      ResultType &left, ResultType &right) {
    return visitBinary (op, left, right);
  }

  virtual ResultType visitBinary (const Expression::Operator *op,
      ResultType &left, ResultType &right) {
    left->binaryOperation (op->clone (), right);
    delete right;
    return left;
  }

  virtual ResultType visitUnary (const Expression::Operator *op,
      ResultType &mainExp) {
    // The only supported unary operation is bitwise negation
    mainExp->bitwiseNegate ();
    return mainExp;
  }

  virtual ResultType visitOperand (
      const edu::sharif::twinner::trace::exptoken::Operand *operand) {
    const edu::sharif::twinner::trace::exptoken::Constant *cte =
        dynamic_cast<const edu::sharif::twinner::trace::exptoken::Constant *> (operand);
    if (cte) {
      return new ExpressionImp (cte->getValue ());
    } else {
      const edu::sharif::twinner::trace::exptoken::Symbol *sym =
          static_cast<const edu::sharif::twinner::trace::exptoken::Symbol *> (operand);
      if (!sym->isTemporary ()) {
        return new ExpressionImp (sym->clone ());
      } else {
        const edu::sharif::twinner::trace::exptoken::RegisterEmergedSymbol *reg =
            dynamic_cast<const edu::sharif::twinner::trace::exptoken
            ::RegisterEmergedSymbol *> (sym);
        Expression *exp;
        if (reg) {
          exp = previousSnapshot->resolveRegister (reg->getAddress ());
        } else {
          const edu::sharif::twinner::trace::exptoken::MemoryEmergedSymbol *mem =
              dynamic_cast<const edu::sharif::twinner::trace::exptoken
              ::MemoryEmergedSymbol *> (sym);
          if (mem) {
            exp = previousSnapshot->resolveMemory
                (mem->getValue ().getSize (), mem->getAddress ());
          } else {
            edu::sharif::twinner::util::Logger::error ()
                << "ReplaceTemporarySymbolsVisitor::visitOperand ():"
                " symbol is neither reg nor mem\n";
            abort ();
          }
        }
        return exp;
      }
    }
  }
};

void Snapshot::replaceTemporarySymbols (Expression *exp) const {
  ReplaceTemporarySymbolsVisitor visitor (this);
  Expression *newExp = exp->visit (visitor);
  (*exp) = (*newExp);
  delete newExp;
}

std::list< std::pair< const Expression *, bool > >
Snapshot::getCriticalExpressions (
    std::list < int > constraintIndices) const {
  std::list < std::pair < const Expression *, bool> > criticalExpressions;
  int constraintIndex = 0;
  for (std::list < Constraint * >::const_iterator it = pathConstraints.begin ();
      !constraintIndices.empty () && it != pathConstraints.end ();
      ++it, ++constraintIndex) {
    if (constraintIndices.front () != constraintIndex) {
      continue;
    }
    constraintIndices.pop_front ();
    const Constraint *c = *it;
    criticalExpressions.push_back (make_pair (c->getMainExpression (), false));
    if (c->getAuxExpression ()) {
      criticalExpressions.push_back (make_pair (c->getAuxExpression (), false));
    }
  }
  for (std::set<SymbolRepresentation>::const_iterator it = criticalSymbols.begin ();
      it != criticalSymbols.end (); ++it) {
    Expression *exp = it->resolve (this);
    criticalExpressions.push_back (make_pair (exp, true));
  }
  return criticalExpressions;
}

void Snapshot::addCriticalSymbols (const std::set<SymbolRepresentation> &symbols) {
  criticalSymbols.insert (symbols.begin (), symbols.end ());
}

const std::set<SymbolRepresentation> &Snapshot::getCriticalSymbols () const {
  return criticalSymbols;
}

bool Snapshot::satisfiesMemoryRegisterCriticalExpressions (
    const Snapshot *sna) const {
  for (std::set<SymbolRepresentation>::const_iterator it = sna->criticalSymbols.begin ();
      it != sna->criticalSymbols.end (); ++it) {
    Expression *ourExp = it->resolve (this);
    Expression *targetExp = it->resolve (sna);
    const bool match = ourExp
        && targetExp
        && areExpressionsEquivalent (*ourExp, *targetExp);
    delete ourExp;
    delete targetExp;
    if (!match) {
      return false;
    }
  }
  return true;
}

Expression *Snapshot::resolveMemory (int sizeInBits, ADDRINT address) const {
  const std::map < ADDRINT, Expression * > *memToExp;
  switch (sizeInBits) {
  case 128:
    memToExp = &memoryAddressTo128BitsExpression;
    break;
  case 64:
    memToExp = &memoryAddressTo64BitsExpression;
    break;
  case 32:
    memToExp = &memoryAddressTo32BitsExpression;
    break;
  case 16:
    memToExp = &memoryAddressTo16BitsExpression;
    break;
  case 8:
    memToExp = &memoryAddressTo8BitsExpression;
    break;
  default:
    edu::sharif::twinner::util::Logger::error () <<
        "Snapshot::resolveMemory (sizeInBits=" << std::dec << sizeInBits
        << ", address=0x" << std::hex << address
        << "): Memory size is not supported\n";
    abort ();
  }
  std::map < ADDRINT, Expression * >::const_iterator it =
      memToExp->find (address);
  if (it != memToExp->end ()) {
    return it->second->clone ();
  }
  edu::sharif::twinner::util::Logger::error () <<
      "Snapshot::resolveMemory (sizeInBits=" << std::dec << sizeInBits
      << ", address=0x" << std::hex << address
      << "): No expression is found\n";
  return 0;
}

Expression *Snapshot::resolveRegister (REG address) const {
  std::map < REG, Expression * >::const_iterator it =
      registerToExpression.find (address);
  if (it != registerToExpression.end ()) {
    return it->second->clone ();
  }
  edu::sharif::twinner::util::Logger::error () <<
      "Snapshot::resolveRegister (address=" << address
      << "): No expression is found\n";
  return 0;
}

Expression *Snapshot::lazyLoad (int size,
    std::map < REG, Expression * > &map, const REG key,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal,
    edu::sharif::twinner::trace::StateSummary &state) {
  const char *msg = "The Snapshot::lazyLoad(...) method is"
      " not defined for REG case.\n";
  edu::sharif::twinner::util::Logger::error () << msg;
  abort ();
}

Expression *Snapshot::lazyLoad (int size,
    std::map < REG, Expression * > &map, const REG key) {
  const char *msg = "The Snapshot::lazyLoad(...) method is"
      " not defined for REG case.\n";
  edu::sharif::twinner::util::Logger::error () << msg;
  abort ();
}

}
}
}
}
