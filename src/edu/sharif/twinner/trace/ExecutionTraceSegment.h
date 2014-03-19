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
  std::map < ADDRINT, Expression * > memoryAddressToExpression;
  std::list < const Constraint * > pathConstraints;
  /// Each execution trace segment is terminated with some syscall.
  Syscall syscall;

  ExecutionTraceSegment (const std::map < REG, Expression * > &regi,
      const std::map < ADDRINT, Expression * > &memo,
      const std::list < const Constraint * > &cnrt);

public:
  ExecutionTraceSegment ();
  ExecutionTraceSegment (const std::map < ADDRINT, Expression * > &memo);
  virtual ~ExecutionTraceSegment ();

  virtual Expression *tryToGetSymbolicExpressionByRegister (REG reg,
      const ConcreteValue &regval)
  throw (WrongStateException);
  virtual Expression *tryToGetSymbolicExpressionByRegister (REG reg);
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      const ConcreteValue &memval) throw (WrongStateException);

  virtual Expression *getSymbolicExpressionByRegister (REG reg,
      const ConcreteValue &regval, Expression *newExpression);
  virtual Expression *getSymbolicExpressionByRegister (REG reg,
      Expression *newExpression);
  virtual Expression *getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      const ConcreteValue &memval, Expression *newExpression);

  virtual Expression *setSymbolicExpressionByRegister (REG reg, const Expression *exp);
  virtual Expression *setSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      const Expression *exp);

  virtual void addPathConstraint (const Constraint *c);

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static ExecutionTraceSegment *loadFromBinaryStream (std::ifstream &in);

private:
  template < typename KEY >
  Expression *tryToGetSymbolicExpressionImplementation (
      std::map < KEY, Expression * > &map, const KEY key,
      const ConcreteValue &concreteVal) const
  throw (WrongStateException);
  template < typename KEY >
  Expression *tryToGetSymbolicExpressionImplementation (
      std::map < KEY, Expression * > &map, const KEY key) const;
  template < typename KEY >
  Expression *getSymbolicExpressionImplementation (std::map < KEY, Expression * > &map,
      const KEY key, const ConcreteValue &currentConcreteValue,
      Expression *newExpression);
  template < typename KEY >
  Expression *getSymbolicExpressionImplementation (std::map < KEY, Expression * > &map,
      const KEY key, Expression *newExpression);
  template < typename KEY >
  Expression *setSymbolicExpressionImplementation (std::map < KEY, Expression * > &map,
      const KEY key, const Expression *exp);

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
  const std::map < ADDRINT, Expression * > &getMemoryAddressToExpression () const;
  const std::list < const Constraint * > &getPathConstraints () const;

  void setSyscall (Syscall syscall);
  Syscall getSyscall () const;
};

}
}
}
}

#endif /* ExecutionTraceSegment.h */
