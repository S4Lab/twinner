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
  std::list < const Constraint * > pathConstraints;
  /// Each execution trace segment is terminated with some syscall.
  Syscall syscall;

  ExecutionTraceSegment (const std::map < REG, Expression * > &regi,
      const std::map < ADDRINT, Expression * > &memo,
      const std::list < const Constraint * > &cnrt);

public:
  ExecutionTraceSegment ();
  ExecutionTraceSegment (const std::map < REG, Expression * > &regMap,
      const std::map < ADDRINT, Expression * > &memMap);
  virtual ~ExecutionTraceSegment ();

  virtual Expression *tryToGetSymbolicExpressionByRegister (int size, REG reg,
      const edu::sharif::twinner::trace::cv::ConcreteValue &regval) const
  /* @throw (WrongStateException) */;
  virtual Expression *tryToGetSymbolicExpressionByRegister (int size, REG reg) const;
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa,
      const edu::sharif::twinner::trace::cv::ConcreteValue &memval) const
  /* @throw (WrongStateException) */;
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa) const;

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
      const std::list <edu::sharif::twinner::trace::Constraint *> &c);

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static ExecutionTraceSegment *loadFromBinaryStream (std::ifstream &in);

private:
  template < typename KEY >
  Expression *tryToGetSymbolicExpressionImplementation (
      const std::map < KEY, Expression * > &map, const KEY key,
      const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal) const
  /* @throw (WrongStateException) */;
  template < typename KEY >
  Expression *tryToGetSymbolicExpressionImplementation (
      const std::map < KEY, Expression * > &map, const KEY key) const;
  template < typename KEY >
  Expression *getSymbolicExpressionImplementation (std::map < KEY, Expression * > &map,
      const KEY key,
      const edu::sharif::twinner::trace::cv::ConcreteValue &currentConcreteValue,
      Expression *newExpression);
  template < typename KEY >
  Expression *getSymbolicExpressionImplementation (std::map < KEY, Expression * > &map,
      const KEY key, Expression *newExpression);
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
  const std::list < const Constraint * > &getPathConstraints () const;

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
