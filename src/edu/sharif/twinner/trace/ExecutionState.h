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

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Expression;
class Constraint;

/*
 * @interface This interface, specifies that current execution state, including
 * symbolic expressions living in memory addresses or registers and path constraints,
 * are kept and can be reported.
 */
class ExecutionState {
public:
  /**
   * Virtual destructor is required for pure virtual members.
   * Implementors must free all owned expressions/constraints.
   */
  virtual ~ExecutionState () {
  }

  /**
   * The getter returns current value stored in one register.
   * It may allocate new symbols (~ has side effects) if required.
   * Ownership of returned expression is kept and caller should clone it if required.
   *
   * @param reg The register which its value is returned.
   * @return symbolic expression which is living in register at current state.
   */
  virtual Expression getSymbolicExpressionByRegister (REG reg) = 0;

  /**
   * The getter returns current value stored in one memory address.
   * It may allocate new symbols (~ has side effects) if required.
   * Ownership of returned expression is kept and caller should clone it if required.
   *
   * @param memoryEa The memory effective address which its value will be returned.
   * @return symbolic expression which is living at the given memory address at current state.
   */
  virtual Expression getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa) = 0;

  /**
   * The setter clones given expression and stores it as the new value living
   * in given register. It takes ownership of the cloned object.
   *
   * @param reg The register which its value is being set.
   * @param exp The expression which will be cloned and will determine new value of given register.
   */
  virtual void setSymbolicExpressionByRegister (REG reg, Expression exp) = 0;

  /**
   * The setter clones given expression and stores it as the new value living
   * in given memory address. It takes ownership of the cloned object.
   *
   * @param memoryEa The memory effective address which its value is being set.
   * @param exp The expression which will be cloned and will determine new value of given memory address.
   */
  virtual void setSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
      Expression exp) = 0;

  /**
   * Adds the given constraint to current series of path constraints.
   * All path constraints must be satisfied simultaneously, in order to reach to the
   * current execution state in the running program.
   *
   * @param c The constraint which is being added.
   */
  virtual void addPathConstraint (Constraint c) = 0;
};

}
}
}
}

#endif /* EXECUTIONSTATE_H_ */
