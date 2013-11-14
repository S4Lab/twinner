//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information 
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef EXECUTIONSTATE_H_
#define EXECUTIONSTATE_H_

#include "pin.H"

#include "Savable.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

class Logger;
}
namespace trace {

class Expression;
class Constraint;

/**
 * This exception indicates that current execution state differs from the expected state.
 */
class WrongStateException : public std::exception {

private:
  UINT64 currentValue;

public:

  WrongStateException (UINT64 _currentValue) :
      currentValue (_currentValue) {
  }

  const char *what () const throw () {
    return "Execution state differ from what we expected (probably, user space memory is changed by a syscall).";
  }

  UINT64 getCurrentStateValue () const {
    return currentValue;
  }
};

/*
 * @interface This interface, specifies that current execution state, including
 * symbolic expressions living in memory addresses or registers and path constraints,
 * are kept and can be reported.
 */
class ExecutionState : public Savable {

protected:

  ExecutionState () {
  }

public:

  /**
   * Virtual destructor is required for pure virtual members.
   * Implementors must free all owned expressions/constraints.
   */
  virtual ~ExecutionState () {
  }

  /**
   * The getter returns current value stored in one register.
   * Ownership of returned expression is kept and caller should clone it if required.
   * Returned expression can be changed if desired.
   *
   * @param reg The register which its value is returned.
   * @param regval The concrete value which currently lives in given register.
   * @return symbolic expression which is living in given register at current state or 0 if there is no such expression.
   *
   * @except Throws a WrongStateException, if the last concrete value of the reg's corresponding expression differs from expected @c regval value.
   */
  virtual Expression *tryToGetSymbolicExpressionByRegister (REG reg, UINT64 regval)
  throw (WrongStateException) = 0;

  /**
   * The getter returns current value stored in one memory address.
   * Ownership of returned expression is kept and caller should clone it if required.
   * Returned expression can be changed if desired.
   *
   * @param memoryEa The memory effective address which its value will be returned.
   * @param memval The concrete value which currently lives at given memory address.
   * @return symbolic expression which is living at the given memory address at current state.
   */
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      UINT64 memval) throw (WrongStateException) = 0;

  /**
   * The getter returns current value stored in one register.
   * Despite tryToGetSymbolicExpressionByRegister, this method may set a new
   * expression (i.e. has side effects) if there was not any kept value. In this case,
   * the @c newExpression is set as the new value.
   * The regval indicates currently stored concrete value of the queried register and
   * will be used for setting the concrete value of newly instantiated expression (or
   * checking whether the currently stored expression, if any, reflects the real state
   * of the program or not).
   * Ownership of returned expression is kept and caller should clone it if required.
   * Returned expression can be changed if desired.
   *
   * @param reg The register which its value is returned.
   * @param regval The concrete value which currently lives in given register.
   * @param newExpression The expression which will be set if there was no current value.
   * @return symbolic expression which is living in register at current state.
   */
  virtual Expression *getSymbolicExpressionByRegister (REG reg, UINT64 regval,
      Expression *newExpression) = 0;

  /**
   * The getter returns current value stored in one memory address.
   * Despite tryToGetSymbolicExpressionByMemoryAddress, this method may set a new
   * expression (i.e. has side effects) if there was not any kept value. In this case,
   * the @c newExpression is set as the new value.
   * The memval indicates currently stored concrete value of the queried memory address
   * and will be used for setting the concrete value of newly instantiated expression (or
   * checking whether the currently stored expression, if any, reflects the real state
   * of the program or not).
   * Ownership of returned expression is kept and caller should clone it if required.
   * Returned expression can be changed if desired.
   *
   * @param memoryEa The memory effective address which its value will be returned.
   * @param memval The concrete value which currently lives at given memory address.
   * @param newExpression The expression which will be set if there was no current value.
   * @return symbolic expression which is living at the given memory address at current state.
   */
  virtual Expression *getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      UINT64 memval, Expression *newExpression) = 0;

  /**
   * The setter clones given expression and stores it as the new value living
   * in given register. It takes ownership of the cloned object.
   *
   * @param reg The register which its value is being set.
   * @param exp The expression which will be cloned and will determine new value of given register.
   */
  virtual void setSymbolicExpressionByRegister (REG reg, const Expression *exp) = 0;

  /**
   * The setter clones given expression and stores it as the new value living
   * in given memory address. It takes ownership of the cloned object.
   *
   * @param memoryEa The memory effective address which its value is being set.
   * @param exp The expression which will be cloned and will determine new value of given memory address.
   */
  virtual void setSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      const Expression *exp) = 0;

  /**
   * Adds the given constraint to current series of path constraints.
   * All path constraints must be satisfied simultaneously, in order to reach to the
   * current execution state in the running program.
   *
   * @param c The constraint which is being added.
   */
  virtual void addPathConstraint (const Constraint *c) = 0;

  /**
   * Prints values of all registers (both symbolic and concrete values). Only registers
   * which were get/set and so are being considered by this object will be printed.
   * @param logger The logger which registers values will be printed in it.
   */
  virtual void printRegistersValues (
      const edu::sharif::twinner::util::Logger &logger) const = 0;

  /**
   * Prints all values which are kept at any memory address (both symbolic and concrete
   * values). Only memory addresses which were get/set and so are being considered by
   * this object will be printed.
   * @param logger The logger which memory addresses values will be printed in it.
   */
  virtual void printMemoryAddressesValues (
      const edu::sharif::twinner::util::Logger &logger) const = 0;


  /**
   * Prints list of path constraints which are added to this object.
   * @param logger The logger which added path constraints will be printed in it.
   */
  virtual void printPathConstraints (
      const edu::sharif::twinner::util::Logger &logger) const = 0;

  /**
   * Prints complete state of this object, including registers, memory addresses, and
   * path constraints into the logger.
   * @param logger The logger which complete state of this object will be printed in it.
   */
  virtual void printCompleteState (
      const edu::sharif::twinner::util::Logger &logger) const = 0;
};

}
}
}
}

#endif /* EXECUTIONSTATE_H_ */
