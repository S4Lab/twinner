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

    NON_POSITIVE, NON_NEGATIVE,
    POSITIVE, NEGATIVE, ZERO, NON_ZERO
  };

private:
  Expression *exp;
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
   * @param exp The expression which will be cloned and should satisfy (in)equality.
   * @param type Indicates the (in)equality which exp should satisfy it.
   * @param instruction The instruction which is cause of this constraint instantiation.
   */
  Constraint (const Expression *exp, ComparisonType type, uint32_t instruction);

  ~Constraint ();

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static Constraint *loadFromBinaryStream (std::ifstream &in);

  std::string toString () const;

  const Expression *getExpression () const;
  ComparisonType getComparisonType () const;
  uint32_t getCausingInstructionIdentifier () const;

  Constraint *instantiateNegatedConstraint () const;

  bool operator!= (const Constraint &constraint) const {
    return !((*this) == constraint);
  }

  bool operator== (const Constraint &constraint) const;

  bool isTrivial () const;
};

}
}
}
}

#endif /* Constraint.h */
