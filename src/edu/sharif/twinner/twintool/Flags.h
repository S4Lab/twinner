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
  edu::sharif::twinner::trace::Expression *exp;

public:
  Flags ();
  ~Flags ();

  /**
   * Sets all flags based on given expression. It's assumed that given
   * expression is owned by this object.
   * @param exp The expression which flags are set based on it.
   */
  void setFlags (edu::sharif::twinner::trace::Expression *exp);

  /**
   * Instantiates a new constraint object denoting ZF's (zero flag) state. The concrete
   * status of the ZF should be provided as argument.
   * @param zfIsSet The concrete status of the ZF in processor.
   *
   * @return A new constraint instance denoting current zero flag's state.
   */
  edu::sharif::twinner::trace::Constraint *
  instantiateConstraintForZeroFlag (bool zfIsSet) const;

  /**
   * Instantiates a new constraint object denoting ZF's (zero flag) state. The last
   * concrete value of the expression indicates that ZF should be satisfied or its negate
   * should be satisfied.
   *
   * @return A new constraint instance denoting current zero flag's state.
   */
  edu::sharif::twinner::trace::Constraint *
  instantiateConstraintForZeroFlag () const;

  /**
   * Instantiates a new constraint object denoting that whether underlying expression
   * matches with below or equal (BE) condition. The last concrete value of the expression
   * indicates that BE should be satisfied or its negate should be satisfied.
   * @return A new constraint instance denoting whether current eflags matches with BE.
   */
  edu::sharif::twinner::trace::Constraint *
  instantiateConstraintForBelowOrEqual () const;

private:
  /**
   * Returns the underlying expression which currently, flags are set based on it. The
   * returned expression is still owned by this object and caller must clone it
   * if required.
   * @return The underlying expression of flags object.
   */
  const edu::sharif::twinner::trace::Expression *getFlagsUnderlyingExpression () const;
};

}
}
}
}

#endif /* Flags.h */
