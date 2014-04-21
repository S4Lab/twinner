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

#ifndef FLAGS_H
#define FLAGS_H

#include <stdint.h>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Expression;
class Constraint;
}
namespace twintool {

class Flags {

private:
  edu::sharif::twinner::trace::Expression *leftExp;
  edu::sharif::twinner::trace::Expression *rightExp;

public:
  Flags ();
  ~Flags ();

  /**
   * Sets all flags based on given expression. It's assumed that given
   * expression is owned by this object.
   * This method is used for unsigned cases (e.g. TEST instruction) where flags can be
   * determined by just inspecting bits of the concrete value of the given expression.
   * @param exp The expression which flags are set based on it.
   */
  void setFlags (edu::sharif::twinner::trace::Expression *exp);

  /**
   * Sets all flags based on comparison of given expressions. It's assumed that given
   * expressions are owned by this object.
   * This method is used for signed and unsigned cases (e.g. CMP instruction) where flags
   * are determined by signed or unsigned subtraction (i.e. exp1 - exp2) and the actual
   * operation should be selected during constraint instantiation.
   * @param exp1 The first expression which flags are set based on it.
   * @param exp2 The second expression which flags are set based on it.
   */
  void setFlags (edu::sharif::twinner::trace::Expression *exp1,
      edu::sharif::twinner::trace::Expression *exp2);

  /**
   * Instantiates a new constraint object denoting ZF's (zero flag) state. The last
   * concrete value of the expression indicates that ZF should be satisfied or its negate
   * should be satisfied.
   * @param instruction The instruction which was cause of this constraint instantiation.
   * @param zero Indicates whether requested constraint is created or its negated.
   *
   * @return A new constraint instance denoting current zero flag's state.
   */
  edu::sharif::twinner::trace::Constraint *instantiateConstraintForZeroCase (bool &zero,
      uint32_t instruction) const;

  /**
   * Instantiates a new constraint object denoting that whether underlying expression
   * matches with less or equal (LE) condition. The last concrete value of the expression
   * indicates that LE should be satisfied or its negate should be satisfied.
   * @param instruction The instruction which was cause of this constraint instantiation.
   * @param lessOrEqual Indicates whether requested constraint is created or its negated.
   *
   * @return A new constraint instance denoting whether current eflags matches with LE.
   */
  edu::sharif::twinner::trace::Constraint *instantiateConstraintForLessOrEqualCase (
      bool &lessOrEqual, uint32_t instruction) const;

  /**
   * Instantiates a new constraint object denoting that whether underlying expression
   * matches with below or equal (BE) condition. The last concrete value of the expression
   * indicates that BE should be satisfied or its negate should be satisfied.
   * @param instruction The instruction which was cause of this constraint instantiation.
   * @param belowOrEqual Indicates whether requested constraint is created or its negated.
   *
   * @return A new constraint instance denoting whether current eflags matches with BE.
   */
  edu::sharif::twinner::trace::Constraint *instantiateConstraintForBelowOrEqualCase (
      bool &belowOrEqual, uint32_t instruction) const;

  /**
   * Instantiates a new constraint object denoting that whether underlying expression
   * matches with sing (S) condition. The last concrete value of the expression
   * indicates that S should be satisfied or its negate should be satisfied.
   * @param instruction The instruction which was cause of this constraint instantiation.
   * @param sign Indicates whether requested constraint is created or its negated.
   *
   * @return A new constraint instance denoting whether current eflags matches with S.
   */
  edu::sharif::twinner::trace::Constraint *instantiateConstraintForSignCase (
      bool &sign, uint32_t instruction) const;
};

}
}
}
}

#endif /* Flags.h */
