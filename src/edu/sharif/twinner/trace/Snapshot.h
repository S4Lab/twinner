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

#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "ExecutionState.h"
#include "Constraint.h"
#include "SymbolRepresentation.h"
#include "SnapshotIterator.h"
#include "TimedTrace.h"

#include "edu/sharif/twinner/trace/syscall/Syscall.h"

#include <map>
#include <set>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

class ExpressionToken;
}

class Snapshot : public ExecutionState {
public:
  typedef SnapshotIterator snapshot_iterator;
  typedef std::reverse_iterator<snapshot_iterator> snapshot_reverse_iterator;

private:
  std::map < REG, Expression * > registerToExpression;
  /// The ADDRINT must be aligned
  std::map < ADDRINT, Expression * > memoryAddressTo128BitsExpression;
  std::map < ADDRINT, Expression * > memoryAddressTo64BitsExpression;
  std::map < ADDRINT, Expression * > memoryAddressTo32BitsExpression;
  std::map < ADDRINT, Expression * > memoryAddressTo16BitsExpression;
  std::map < ADDRINT, Expression * > memoryAddressTo8BitsExpression;
  std::list < Constraint * > pathConstraints;

  int segmentIndex;
  int snapshotIndex;

  TimedTrace timedTrace;

  std::set<SymbolRepresentation> criticalSymbols;

  Snapshot (int segmentIndex, int snapshotIndex,
      const std::map < REG, Expression * > &regi,
      const std::map < ADDRINT, Expression * > &memo128,
      const std::map < ADDRINT, Expression * > &memo64,
      const std::map < ADDRINT, Expression * > &memo32,
      const std::map < ADDRINT, Expression * > &memo16,
      const std::map < ADDRINT, Expression * > &memo8,
      const std::list < Constraint * > &cnrt);

public:
  Snapshot (int segmentIndex, int snapshotIndex);
  Snapshot (int segmentIndex, int snapshotIndex,
      const std::map < REG, Expression * > &regMap,
      const std::map < ADDRINT, Expression * > &memMap);
  virtual ~Snapshot ();

  static Snapshot *instantiateNexSnapshot (const Snapshot &previousSnapshot);

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
      const edu::sharif::twinner::trace::Constraint *lastConstraint = 0);

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static Snapshot *loadFromBinaryStream (std::ifstream &in);

  int getSegmentIndex () const;
  int getSnapshotIndex () const;

  void addTemporaryExpressions (const Snapshot *sna,
      REG fullReg, int size);
  void addTemporaryExpressions (const Snapshot *sna,
      ADDRINT address, int size);

private:
  void addTemporaryExpressions (ADDRINT memoryEa, int size,
      const std::map < ADDRINT, Expression * > * const *memoryToExpressionMaps,
      int level);

  template < typename KEY >
  Expression *tryToGetSymbolicExpressionImplementation (
      int size, std::map < KEY, Expression * > &map, const KEY key,
      const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal,
      StateSummary &state);
  template < typename KEY >
  Expression *tryToGetSymbolicExpressionImplementation (
      int size, std::map < KEY, Expression * > &map, const KEY key);
  template < typename KEY >
  Expression *getSymbolicExpressionImplementation (int size,
      std::map < KEY, Expression * > &map, const KEY key,
      const edu::sharif::twinner::trace::cv::ConcreteValue &currentConcreteValue,
      Expression *newExpression, StateSummary &state);
  template < typename KEY >
  Expression *getSymbolicExpressionImplementation (int size,
      std::map < KEY, Expression * > &map, const KEY key,
      Expression *newExpression);
  template < typename KEY >
  Expression *setSymbolicExpressionImplementation (int size,
      std::map < KEY, Expression * > &map, const KEY key, const Expression *exp);

  template < typename KEY >
  void setExpression (std::map < KEY, Expression * > &map,
      const KEY key, Expression *exp);

  void saveNonNullExpressions (std::ofstream &out,
      const std::map < ADDRINT, Expression * > &memToExp) const;
  template <typename ADDRESS>
  void saveMapToBinaryStream (std::ofstream &out,
      const char *magicString, const std::map < ADDRESS, Expression * > &map) const;
  template <typename ADDRESS>
  static void loadMapFromBinaryStream (std::ifstream &in,
      const char *expectedMagicString, std::map < ADDRESS, Expression * > &map);

public:
  void abandonTrivialMemoryExpressions ();

  virtual void printRegistersValues (
      const edu::sharif::twinner::util::Logger &logger) const;
  virtual void printMemoryAddressesValues (
      const edu::sharif::twinner::util::Logger &logger) const;
  virtual void printPathConstraints (
      const edu::sharif::twinner::util::Logger &logger) const;
  virtual void printCompleteState (
      const edu::sharif::twinner::util::Logger &logger) const;

  const std::map < REG, Expression * > &getRegisterToExpression () const;
  const std::map < ADDRINT, Expression * > &getMemoryAddressTo64BitsExpression () const;
  const std::map < ADDRINT, Expression * > &getMemoryAddressTo8BitsExpression () const;
  const std::list < Constraint * > &getPathConstraints () const;
  std::list < Constraint * > &getPathConstraints ();

  /**
   * Register/Memory/Constraint expressions of this snapshot may only depend
   * on the temporary symbols which are indexed for exactly this snapshot
   * and/or on the non-temporary symbols. Since all temporary symbols of this
   * snapshot are equal with the final expressions which are kept in the
   * previous snapshot, all expressions of this snapshot can be updated to
   * depend just on non-temporary symbols given the previous snapshot and
   * assuming that the previous snapshot expressions have already been updated
   * to only depend on the non-temporary symbols.
   * This method performs that specific replacement.
   *
   * @param previousSnapshot The previous snapshot.
   */
  void replaceTemporarySymbols (const Snapshot *previousSnapshot);

  /**
   * Gets all expressions which are critical in this snapshot including all
   * main/aux expressions of its constraints and those expressions which are
   * kept in memory/register addresses who have been marked by calls to the
   * addCriticalSymbols method previously.
   *
   * @return A list of all critical expressions.
   */
  std::list<const Expression *> getCriticalExpressions () const;

  /**
   * Adds given criticalSymbols set of memory/register temporary symbols to
   * the critical symbols set of this snapshot. These symbols indicate register
   * and memory addresses which at the end of this snapshot will contain the
   * critical expressions of this snapshot.
   * By default, constraints of each snapshot are assumed to be critical too.
   *
   * @param criticalSymbols New temporary symbols to be assumed critical.
   */
  void addCriticalSymbols (const std::set<SymbolRepresentation> &criticalSymbols);

  /**
   * Gets all critical symbols which are added using the addCriticalSymbols
   * method so far.
   *
   * @return All register/memory critical symbols of this snapshot.
   */
  const std::set<SymbolRepresentation> &getCriticalSymbols () const;

  /**
   * Checks whether this snapshot conforms with the given sna snapshot in the
   * symbolic expressions which are marked as critical in the given sna.
   * It is not important that which addresses are marked as critical in this
   * snapshot. It is just important for this snapshot to have the same
   * expressions as the sna has in those addresses which are critical in
   * the given snapshot.
   *
   * @param sna The main snapshot which this snapshot is being checked with it.
   * @return true iff all memory/register addresses which are marked as critical
   * ones in the sna Snapshot, contain exactly the same expressions in both
   * snapshots.
   */
  bool satisfiesMemoryRegisterCriticalExpressions (const Snapshot *sna) const;

  const Expression *resolveMemory (int sizeInBits, ADDRINT address) const;
  const Expression *resolveRegister (REG address) const;

private:
  void initializeOverlappingMemoryLocationsDownwards (int size,
      ADDRINT memoryEa, const Expression &changedExp,
      int shiftAmount = 0);
  void initializeOverlappingMemoryLocationsUpwards (int size, ADDRINT memoryEa);
  void setOverwritingMemoryExpression (int size,
      ADDRINT memoryEa, const Expression *expression, bool isOverwriting);

  template<typename KEY>
  void replaceTemporarySymbols (const Snapshot *previousSnapshot,
      std::map < KEY, Expression * > &expressions);
  void replaceTemporarySymbols (const Snapshot *previousSnapshot,
      std::list < Constraint * > &constraints);
  void replaceTemporarySymbols (const Snapshot *previousSnapshot,
      Expression *exp);

  Expression *lazyLoad (int size,
      std::map < ADDRINT, Expression * > &map, const ADDRINT key,
      const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal,
      edu::sharif::twinner::trace::StateSummary &state);
  Expression *lazyLoad (int size,
      std::map < ADDRINT, Expression * > &map, const ADDRINT key);

  Expression *lazyLoad (int size,
      std::map < REG, Expression * > &map, const REG key,
      const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal,
      edu::sharif::twinner::trace::StateSummary &state);
  Expression *lazyLoad (int size,
      std::map < REG, Expression * > &map, const REG key);
};

}
}
}
}

#endif /* Snapshot.h */
