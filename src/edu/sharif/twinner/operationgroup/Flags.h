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
namespace operationgroup {

class OperationGroup;

class Flags {
public:

  enum FlagState {
    UNDEFINED_FSTATE,
    DEFAULT_FSTATE,
    SET_FSTATE,
    CLEAR_FSTATE,
    MANUAL_FSTATE,
  };

private:
  const edu::sharif::twinner::operationgroup::OperationGroup *op;

  FlagState of; // overflow flag
  FlagState df; // direction flag
  FlagState sf; // sign flag
  FlagState zf; // zero flag
  FlagState pf; // parity flag
  FlagState cf; // carry flag
  const edu::sharif::twinner::trace::Expression *cfexp;

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
   * Get carry flag (CF) as a symbolic expressions based on the last operation which
   * affected the CF. Caller owns the returned expression.
   *
   * @return The expressions which indicates state of CF.
   */
  edu::sharif::twinner::trace::Expression *getCarryFlag () const;

  /**
   * Sets all flags based on given operation group.
   * The operation group contains required expressions and is owned by this flags object.
   * This method is used for signed and unsigned cases (e.g. CMP instruction) where flags
   * are determined by signed or unsigned subtraction (i.e. exp1 - exp2) or other
   * operations as indicated by the operations argument. The actual constraint type
   * will be determined during constraint instantiation according to the used jump type.
   *
   * @param operation The operation group which must be performed to set flags.
   */
  void setFlags (
      const edu::sharif::twinner::operationgroup::OperationGroup *operation);

  /**
   * Fills the output parameter flags with the EFLAGS/RFLAGS register content
   * and returns a list of constraints which should be satisfied in order
   * to make the EFLAGS equal to the obtained value.
   * The flags parameter initially contains the concrete value of the EFLAGS
   * which will be used for initializing those flag bits which are in the
   * unknown state.
   *
   * @param flags The output parameter filled with the concrete EFLAGS value; it
   * initially contains the concrete value of the expected EFLAGS.
   * @param instruction The instruction which was cause of this constraint instantiation.
   *
   * @return Constraints which should be satisfied to lead to the current flags value.
   */
  std::list <edu::sharif::twinner::trace::Constraint *> getFlagsExpression (
      uint32_t &flags, uint32_t instruction) const;

  /**
   * Sets or clears the OF independent of other flags.
   *
   * @param set Indicates whether OF should be set or cleared
   */
  void setOverflowFlag (bool set);

  /**
   * Sets or clears the DF independent of other flags.
   * The DF, in string operations, indicate whether string pointer (RDI/RSI) should be
   * decremented (DF == 1) or incremented (DF == 0).
   *
   * @param set Indicates whether DF should be set or cleared
   */
  void setDirectionFlag (bool set);

  /**
   * Sets or clears the CF independent of other flags.
   *
   * @param set Indicates whether CF should be set or cleared
   */
  void setCarryFlag (bool set);

  /**
   * Sets the CF to one-bit exp expression. Other flags remain untouched.
   *
   * @param exp The one-bit expressions which indicates new state of CF.
   */
  void setCarryFlag (const edu::sharif::twinner::trace::Expression *exp);

  /**
   * Instantiates a new constraint object denoting that whether the underlying
   * expression matches with the overflow (O) condition.
   * The last concrete value of the expression indicates that O should be
   * satisfied or its negate should be satisfied.
   * @param instruction The instruction which was cause of this constraint instantiation.
   * @param overflow Indicates whether requested constraint is created or its negated.
   *
   * @return A new constraint instance denoting whether current eflags matches with O.
   */
  std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForOverflowCase (bool &overflow, uint32_t instruction) const;

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
   * matches with parity (P) condition. The last concrete value of the expression
   * indicates that P should be satisfied or its negate should be satisfied.
   * @param instruction The instruction which was cause of this constraint instantiation.
   * @param parity Indicates whether requested constraint is created or its negated.
   *
   * @return A constraint instance denoting whether current eflags matches with P.
   */
  std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForParityCase (bool &parity, uint32_t instruction) const;

  /**
   * Alias of instantiateConstraintForBelowCase method.
   */
  std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForCarryCase (bool &carry, uint32_t instruction) const;

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
   * matches with sign (S) condition. The last concrete value of the expression
   * indicates that S should be satisfied or its negate should be satisfied.
   * @param instruction The instruction which was cause of this constraint instantiation.
   * @param sign Indicates whether requested constraint is created or its negated.
   *
   * @return A new constraint instance denoting whether current eflags matches with S.
   */
  std::list <edu::sharif::twinner::trace::Constraint *>
  instantiateConstraintForSignCase (bool &sign, uint32_t instruction) const;

private:
  /**
   * Calculates the XOR of eight least significant bits of the given expression.
   */
  edu::sharif::twinner::trace::Expression *calculateParity (
      edu::sharif::twinner::trace::Expression *exp) const;
};

}
}
}
}

#endif /* Flags.h */
