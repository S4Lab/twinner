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
#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Expression;
class Constraint;
}
namespace twintool {

class OperationGroup;

class Flags {

public:

  enum FlagState {

    UNDEFINED_FSTATE,
    DEFAULT_FSTATE,
    SET_FSTATE,
    CLEAR_FSTATE,
  };

private:
  const edu::sharif::twinner::trace::Expression *leftExp;
  const edu::sharif::twinner::trace::Expression *rightExp;

  const OperationGroup *op;

  FlagState of; // overflow flag
  FlagState df; // direction flag
  FlagState sf; // sign flag
  FlagState zf; // zero flag
  FlagState pf; // parity flag
  FlagState cf; // carry flag

public:
  Flags ();
  ~Flags ();

  /**
   * Get direction flag (DF). When DF == 1, the index register in string operations will
   * be decremented and when DF == 0, it will be incremented.
   * @return true iff DF is set.
   */
  bool getDirectionFlag () const;

  /**
   * Sets all flags based on comparison of given expressions. It's assumed that given
   * expressions are owned by this object.
   * This method is used for signed and unsigned cases (e.g. CMP instruction) where flags
   * are determined by signed or unsigned subtraction (i.e. exp1 - exp2) or other
   * operations as indicated by the operations argument. The actual constraint type
   * will be determined during constraint instantiation according to used jump type.
   *
   * @param operation The operation group which must be performed between exp1 and exp2
   * @param exp1 The first expression which flags are set based on it.
   * @param exp2 The second expression which flags are set based on it. This may be null.
   */
  void setFlags (const OperationGroup *operation,
      const edu::sharif::twinner::trace::Expression *exp1,
      const edu::sharif::twinner::trace::Expression *exp2);

  /**
   * Same as setFlags (operation, exp, 0), but also clears OF and CF.
   *
   * @param operation The operation group which operate on exp or its results is exp
   * @param exp The main expression which may be an operand or result of operation
   */
  void setFlags (const OperationGroup *operation,
      const edu::sharif::twinner::trace::Expression *exp);

  /**
   * Instantiates a new constraint object denoting ZF's (zero flag) state. The last
   * concrete value of the expression indicates that ZF should be satisfied or its negate
   * should be satisfied.
   * @param instruction The instruction which was cause of this constraint instantiation.
   * @param zero Indicates whether requested constraint is created or its negated.
   *
   * @return A new constraint instance denoting current zero flag's state.
   */
  std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForZeroCase (bool &zero, uint32_t instruction) const;

  /**
   * Instantiates a new constraint object denoting that whether underlying expression
   * matches with less or equal (LE) condition. The last concrete value of the expression
   * indicates that LE should be satisfied or its negate should be satisfied.
   * @param instruction The instruction which was cause of this constraint instantiation.
   * @param lessOrEqual Indicates whether requested constraint is created or its negated.
   *
   * @return A new constraint instance denoting whether current eflags matches with LE.
   */
  std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForLessOrEqualCase (bool &lessOrEqual, uint32_t instruction) const;

  /**
   * Instantiates a new constraint object denoting that whether underlying expression
   * matches with less (L) condition. The last concrete value of the expression
   * indicates that L should be satisfied or its negate should be satisfied.
   * @param instruction The instruction which was cause of this constraint instantiation.
   * @param less Indicates whether requested constraint is created or its negated.
   *
   * @return A new constraint instance denoting whether current eflags matches with L.
   */
  std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForLessCase (bool &less, uint32_t instruction) const;

  /**
   * Instantiates a new constraint object denoting that whether underlying expression
   * matches with below or equal (BE) condition. The last concrete value of the expression
   * indicates that BE should be satisfied or its negate should be satisfied.
   * @param instruction The instruction which was cause of this constraint instantiation.
   * @param belowOrEqual Indicates whether requested constraint is created or its negated.
   *
   * @return A new constraint instance denoting whether current eflags matches with BE.
   */
  std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForBelowOrEqualCase (
      bool &belowOrEqual, uint32_t instruction) const;

  /**
   * Instantiates a new constraint object denoting that whether underlying expression
   * matches with below (B) condition. The last concrete value of the expression
   * indicates that B should be satisfied or its negate should be satisfied.
   * @param instruction The instruction which was cause of this constraint instantiation.
   * @param below Indicates whether requested constraint is created or its negated.
   *
   * @return Two constraint instances denoting whether current eflags matches with B.
   */
  std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForBelowCase (bool &below, uint32_t instruction) const;

  /**
   * Instantiates a new constraint object denoting that whether underlying expression
   * matches with sing (S) condition. The last concrete value of the expression
   * indicates that S should be satisfied or its negate should be satisfied.
   * @param instruction The instruction which was cause of this constraint instantiation.
   * @param sign Indicates whether requested constraint is created or its negated.
   *
   * @return A new constraint instance denoting whether current eflags matches with S.
   */
  std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForSignCase (bool &sign, uint32_t instruction) const;
};

}
}
}
}

#endif /* Flags.h */
