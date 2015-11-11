//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2015  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef EXECUTION_TRACE_SEGMENT_H
#define EXECUTION_TRACE_SEGMENT_H

#include "ExecutionState.h"
#include "Constraint.h"
#include "Syscall.h"

#include <map>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class ExecutionTraceSegment : public ExecutionState {
private:
  std::map < REG, Expression * > registerToExpression;
  /// The ADDRINT must be aligned
  std::map < ADDRINT, Expression * > memoryAddressTo128BitsExpression;
  std::map < ADDRINT, Expression * > memoryAddressTo64BitsExpression;
  std::map < ADDRINT, Expression * > memoryAddressTo32BitsExpression;
  std::map < ADDRINT, Expression * > memoryAddressTo16BitsExpression;
  std::map < ADDRINT, Expression * > memoryAddressTo8BitsExpression;
  std::list < Constraint * > pathConstraints;
  /// Each execution trace segment is terminated with some syscall.
  Syscall syscall;

  int segmentIndex;

  ExecutionTraceSegment (int index,
      const std::map < REG, Expression * > &regi,
      const std::map < ADDRINT, Expression * > &memo,
      const std::list < Constraint * > &cnrt);

public:
  ExecutionTraceSegment (int index);
  ExecutionTraceSegment (int index,
      const std::map < REG, Expression * > &regMap,
      const std::map < ADDRINT, Expression * > &memMap);
  virtual ~ExecutionTraceSegment ();

  virtual Expression *tryToGetSymbolicExpressionByRegister (int size, REG reg,
      const edu::sharif::twinner::trace::cv::ConcreteValue &regval)
  /* @throw (WrongStateException) */;
  virtual Expression *tryToGetSymbolicExpressionByRegister (int size, REG reg);
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa,
      const edu::sharif::twinner::trace::cv::ConcreteValue &memval)
  /* @throw (WrongStateException) */;
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa);

  virtual Expression *getSymbolicExpressionByRegister (int size, REG reg,
      const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
      Expression *newExpression);
  virtual Expression *getSymbolicExpressionByRegister (int size, REG reg,
      Expression *newExpression);
  virtual Expression *getSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
      const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
      Expression *newExpression);
  virtual Expression *getSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
      Expression *newExpression);

  virtual Expression *setSymbolicExpressionByRegister (int regsize, REG reg,
      const Expression *exp);
  virtual Expression *setSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
      const Expression *exp);

  virtual void addPathConstraints (
      const std::list <edu::sharif::twinner::trace::Constraint *> &c,
      const edu::sharif::twinner::trace::Constraint *lastConstraint);

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static ExecutionTraceSegment *loadFromBinaryStream (std::ifstream &in);

  int getSegmentIndex () const;

private:
  template < typename KEY >
  Expression *tryToGetSymbolicExpressionImplementation (
      int size, std::map < KEY, Expression * > &map, const KEY key,
      const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal)
  /* @throw (WrongStateException) */;
  template < typename KEY >
  Expression *tryToGetSymbolicExpressionImplementation (
      int size, std::map < KEY, Expression * > &map, const KEY key);
  template < typename KEY >
  Expression *getSymbolicExpressionImplementation (int size,
      std::map < KEY, Expression * > &map, const KEY key,
      const edu::sharif::twinner::trace::cv::ConcreteValue &currentConcreteValue,
      Expression *newExpression);
  template < typename KEY >
  Expression *getSymbolicExpressionImplementation (int size,
      std::map < KEY, Expression * > &map, const KEY key,
      Expression *newExpression);
  template < typename KEY >
  Expression *setSymbolicExpressionImplementation (int size,
      std::map < KEY, Expression * > &map, const KEY key, const Expression *exp);

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

  int printMemoryUsageStats (const edu::sharif::twinner::util::Logger &logger) const;

  void setSyscall (Syscall syscall);
  Syscall getSyscall () const;

private:
  template <typename Addr>
  int calcSizeInBytes (const std::map < Addr, Expression * > &map) const;

  void initializeOverlappingMemoryLocationsDownwards (int size,
      ADDRINT memoryEa, const Expression &changedExp,
      bool shouldTruncate = false, int shiftAmount = 0);
  void initializeOverlappingMemoryLocationsUpwards (ADDRINT memoryEa,
      const Expression &expression);
  void setOverwritingMemoryExpression (int size,
      ADDRINT memoryEa, const Expression *expression);
};

}
}
}
}

#endif /* ExecutionTraceSegment.h */
