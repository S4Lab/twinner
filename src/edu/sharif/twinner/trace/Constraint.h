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

#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Savable.h"

#include <stdint.h>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Expression;

class Constraint : public Savable {
public:

  enum ComparisonType {
    INVALID,

    // zero
    NON_POSITIVE, NON_NEGATIVE, NON_ZERO,
    POSITIVE, NEGATIVE, ZERO,

    MAXIMUM_SINGLE_OPERAND_CODED_CONSTRAINT,

    // unsigned
    BELOW_OR_EQUAL, ABOVE_OR_EQUAL, NON_EQUAL,
    ABOVE, BELOW, EQUAL,

    MAXIMUM_UNSIGNED_TWO_OPERANDS_CODED_CONSTRAINT,

    // signed
    LESS_OR_EQUAL, GREATER_OR_EQUAL,
    GREATER, LESS,
  };

private:
  Expression *mainExp;
  Expression *auxExp;
  ComparisonType type;
  uint32_t instruction;

  /**
   * Disable copy constructor.
   * @param c
   */
  Constraint (const Constraint &c);

  /**
   * Instantiate a constraint with the given comparison type. Its expressions should
   * be set manually later.
   * @param type The comparison type of this constraint.\
   * @param instruction The instruction which is cause of this constraint instantiation.
   */
  Constraint (ComparisonType type, uint32_t instruction);

public:
  /**
   * Constructs a constraint indicating that given expression should obey the
   * (in)equality which is specified by type. Expression is cloned and deleted upon
   * destruction of constraint object.
   * Given expression is compared against zero (e.g. exp >= 0).
   * If the comparison type is a signed one (e.g. exp < 0), then the needsSignExtension
   * parameter must be set true to request for sign extension of exp before comparison.
   *
   * @param exp The expression which will be cloned and should satisfy (in)equality.
   * @param type Indicates the (in)equality which exp should satisfy it.
   * @param instruction The instruction which is cause of this constraint instantiation.
   * @param needsSignExtension Indicates that exp should be sign extended
   */
  Constraint (const Expression *exp,
      ComparisonType type, uint32_t instruction, bool needsSignExtension);

  /**
   * Constructs a constraint indicating that given expressions should obey the
   * (in)equality which is specified by type. Both expressions are cloned and deleted upon
   * destruction of constraint object.
   * First expression is left-hand-side and second expression is right-hand-side in the
   * formula (e.g. mainExp >= auxExp).
   * If the comparison type is a signed one (e.g. exp < 0), then the needsSignExtension
   * parameter must be set true to request for sign extension of exp before comparison.
   *
   * @param mainExp The left-hand-side expression.
   * @param auxExp The right-hand-side expression.
   * @param type Indicates the (in)equality which given expressions should satisfy it.
   * @param instruction The instruction which is cause of this constraint instantiation.
   * @param needsSignExtension Indicates that exp should be sign extended
   */
  Constraint (const Expression *mainExp, const Expression *auxExp,
      ComparisonType type, uint32_t instruction, bool needsSignExtension);

  ~Constraint ();

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static Constraint *loadFromBinaryStream (std::ifstream &in);

  Constraint *clone () const;

  std::string toString () const;

  const Expression *getMainExpression () const;
  Expression *getMainExpression ();
  const Expression *getAuxExpression () const;
  Expression *getAuxExpression ();
  ComparisonType getComparisonType () const;
  uint32_t getCausingInstructionIdentifier () const;

  Constraint *instantiateNegatedConstraint () const;

  bool operator!= (const Constraint &constraint) const {
    return !((*this) == constraint);
  }

  bool operator== (const Constraint &constraint) const;

  bool isTrivial (bool requiresValidConcreteValue) const;

  static Constraint *instantiateBelowConstraint (bool &below,
      const Expression *mainExp, const Expression *auxExp, uint32_t instruction);
  static Constraint *instantiateBelowOrEqualConstraint (bool &belowOrEqual,
      const Expression *mainExp, const Expression *auxExp, uint32_t instruction);
  static Constraint *instantiateBelowOrEqualConstraint (bool &belowOrEqual,
      const Expression *mainExp, uint32_t instruction);

  static Constraint *instantiateLessConstraint (bool &less,
      const Expression *mainExp, const Expression *auxExp, uint32_t instruction);
  static Constraint *instantiateLessConstraint (bool &less,
      const Expression *mainExp, uint32_t instruction);
  static Constraint *instantiateLessOrEqualConstraint (bool &lessOrEqual,
      const Expression *mainExp, const Expression *auxExp, uint32_t instruction);
  static Constraint *instantiateLessOrEqualConstraint (bool &lessOrEqual,
      const Expression *mainExp, uint32_t instruction);

  static Constraint *instantiateEqualConstraint (bool &equal,
      const Expression *mainExp, const Expression *auxExp, uint32_t instruction);
  static Constraint *instantiateEqualConstraint (bool &equal,
      const Expression *mainExp, uint32_t instruction);

  static Constraint *instantiateSignConstraint (bool &sign,
      const Expression *mainExp, const Expression *auxExp, uint32_t instruction);
  static Constraint *instantiateSignConstraint (bool &sign,
      const Expression *mainExp, uint32_t instruction);

  static Constraint *instantiateTautology (bool valid);
};

}
}
}
}

#endif /* Constraint.h */
