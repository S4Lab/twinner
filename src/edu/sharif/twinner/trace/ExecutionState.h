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

#ifndef EXECUTIONSTATE_H_
#define EXECUTIONSTATE_H_

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
class StateSummary;

namespace cv {

class ConcreteValue;
}

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
   * ASSERT: The precision of regval must match with size; the precision of reg.
   *
   * @param size The size of the reg in bits.
   * @param reg The register which its value is returned.
   * @param regval The concrete value which currently lives in given register.
   * @param state Indicates whether the last concrete value of the reg's
   * corresponding expression differs from the expected regval value.
   *
   * @return symbolic expression which is living in given register at current state
   * or 0/null if there is no such expression.
   */
  virtual Expression *tryToGetSymbolicExpressionByRegister (int size, REG reg,
      const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
      StateSummary &state) = 0;

  /**
   * Overload of tryToGetSymbolicExpressionByRegister (reg, regval) method.
   * This overload does not take care of concrete values.
   *
   * @param size The size of the reg in bits.
   * @param reg The register which its value is returned.
   * @return symbolic expression which is living in given register at current state
   * or 0/null if there is no such expression.
   */
  virtual Expression *tryToGetSymbolicExpressionByRegister (int size, REG reg) = 0;

  /**
   * The getter returns current value stored in one memory address.
   * If null is stored in the requested address, it means that the expression
   * should be lazy-loaded. In this situation, this method won't be const.
   * Ownership of returned expression is kept and caller should clone it if required.
   * Returned expression can be changed if desired.
   * ASSERT: The precision of memval must match with precision of memoryEa location.
   *
   * @param size The bit-precision of the memory location which its value is being read.
   * @param memoryEa The memory effective address which its value will be returned.
   * @param memval The concrete value which currently lives at given memory address.
   * @param state Indicates whether the last concrete value of the mem's
   * corresponding expression differs from the expected memval value.
   *
   * @return symbolic expression which is living at the given memory address at current
   * state or 0/null if there is no such expression.
   */
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa,
      const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
      StateSummary &state) = 0;

  /**
   * Overloads of tryToGetSymbolicExpressionByMemoryAddress (memoryEa, memval) method.
   * If null is stored in the requested address, it means that the expression
   * should be lazy-loaded. In this situation, this method won't be const.
   * This overload does not take care of concrete values.
   *
   * @param size The bit-precision of the memory location which its value is being read.
   * @param memoryEa The memory effective address which its value will be returned.
   * @return symbolic expression which is living at the given memory address at current
   * state or 0/null if there is no such expression.
   */
  virtual Expression *tryToGetSymbolicExpressionByMemoryAddress (int size,
      ADDRINT memoryEa) = 0;

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
   * ASSERT: The precision of regval must match with size; the precision of reg.
   *
   * @param size The size of the reg in bits.
   * @param reg The register which its value is returned.
   * @param regval The concrete value which currently lives in given register.
   * @param newExpression The expression which will be set if there was no current value.
   * @param state Can be used to indicate that the state is normal or whether
   * 1. there exists some set exp in the last/current execution state (so its
   * concrete value must match with the expected regval value) AND
   * 2. concrete values differ AND
   * 3. there was no syscall between corresponding instructions (since when
   * the last concrete value had been captured till now).
   * Thus there must be some ignored instruction in the trace.
   *
   * @return symbolic expression which is living in register at current state.
   */
  virtual Expression *getSymbolicExpressionByRegister (int size, REG reg,
      const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
      Expression *newExpression, StateSummary &state) = 0;

  /**
   * Overload of getSymbolicExpressionByRegister (reg, regval, newExpression)
   * method. This overload is used when the regval is irrelevant. So if there is any
   * expression kept in asked register, it will be returned and only when no expression,
   * regardless of its last concrete value, were being kept in asked register, a new
   * symbol will be created.
   *
   * @param size The size of the reg in bits.
   * @param reg The register which its value is returned.
   * @param newExpression The expression which will be set if there was no current value.
   * @return symbolic expression which is living in register at current state.
   */
  virtual Expression *getSymbolicExpressionByRegister (int size, REG reg,
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
   * ASSERT: The precision of memval must match with size; precision of memoryEa location.
   *
   * @param size The bit-precision of the memory location which its value is being read.
   * @param memoryEa The memory effective address which its value will be returned.
   * @param memval The concrete value which currently lives at given memory address.
   * @param newExpression The expression which will be set if there was no current value.
   * @param state Can be used to indicate that the state is normal or whether
   * 1. there exists some set exp in the last/current execution trace segment
   * (so its concrete value must match with the expected memval value) AND
   * 2. concrete values differ AND
   * 3. there was no syscall between corresponding instructions (since when
   * the last concrete value had been captured till now).
   * Thus there must be some ignored instruction in the trace.
   *
   * @return symbolic expression which is living at the given memory address at current state.
   */
  virtual Expression *getSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
      const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
      Expression *newExpression, StateSummary &state) = 0;

  /**
   * Overload of getSymbolicExpressionByMemoryAddress (memoryEa, memval, newExpression)
   * method. This overload is used when the memval is irrelevant. So if there is any
   * expression kept at asked memory address, it will be returned and only when
   * no expression, regardless of its last concrete value, were being kept at asked
   * address, a new symbol will be created.
   *
   * @param size The bit-precision of the memory location which its value is being read.
   * @param memoryEa The memory effective address which its value will be returned.
   * @param newExpression The expression which will be set if there was no current value.
   * @return symbolic expression which is living at the given memory address at current state.
   */
  virtual Expression *getSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
      Expression *newExpression) = 0;

  /**
   * The setter clones given expression and stores it as the new value living
   * in given register. It takes ownership of the cloned object.
   * The exp will be casted to regsize-bits in order to fit in the reg.
   *
   * @param regsize Size of the reg in bits (the exp will be casted to this size)
   * @param reg The register which its value is being set.
   * @param exp The expression which will be cloned and will determine new value of
   * given register.
   * @return The cloned expression which is set in given register will be returned.
   */
  virtual Expression *setSymbolicExpressionByRegister (int regsize, REG reg,
      const Expression *exp) = 0;

  /**
   * The setter clones given expression and stores it as the new value living
   * in given memory address. It takes ownership of the cloned object.
   * The memory address must be aligned. For example, for 64 bits expressions, the
   * address must be multiple of 8 bytes.
   * The exp will be casted to fit in memoryEa location.
   *
   * @param size The bit-precision of memory location which its value is being written.
   * @param memoryEa The memory effective address which its value is being set.
   * @param exp The expression which will be cloned and will determine new value of
   * given memory address.
   * @return The cloned expression which is set at given address will be returned.
   */
  virtual Expression *setSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
      const Expression *exp) = 0;

  /**
   * Adds given constraints to current series of path constraints.
   * All path constraints must be satisfied simultaneously, in order to reach to the
   * current execution state in the running program.
   *
   * @param c The constraints which are being added.
   * @param lastConstraint The last constraint which is already added.
   */
  virtual void addPathConstraints (
      const std::list <edu::sharif::twinner::trace::Constraint *> &c,
      const edu::sharif::twinner::trace::Constraint *lastConstraint) = 0;

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
