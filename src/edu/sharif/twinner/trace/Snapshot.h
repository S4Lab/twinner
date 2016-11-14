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

#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "ExecutionState.h"
#include "Constraint.h"

#include "edu/sharif/twinner/trace/syscall/Syscall.h"

#include <map>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Snapshot : public ExecutionState {
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

  Snapshot (int segmentIndex, int snapshotIndex,
      const std::map < REG, Expression * > &regi,
      const std::map < ADDRINT, Expression * > &memo,
      const std::list < Constraint * > &cnrt);

public:
  Snapshot (int segmentIndex, int snapshotIndex);
  Snapshot (int segmentIndex, int snapshotIndex,
      const std::map < REG, Expression * > &regMap,
      const std::map < ADDRINT, Expression * > &memMap);
  virtual ~Snapshot ();

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

private:
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

private:
  void initializeOverlappingMemoryLocationsDownwards (int size,
      ADDRINT memoryEa, const Expression &changedExp,
      int shiftAmount = 0);
  void initializeOverlappingMemoryLocationsUpwards (int size, ADDRINT memoryEa);
  void setOverwritingMemoryExpression (int size,
      ADDRINT memoryEa, const Expression *expression, bool isOverwriting);
};

}
}
}
}

#endif /* Snapshot.h */