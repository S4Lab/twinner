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

#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Savable.h"

#include "Expression.h"
#include "Constant.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Constraint : public Savable {

public:

  enum ComparisonType {

    NON_POSITIVE, NON_NEGATIVE,
    POSITIVE, NEGATIVE, ZERO, NON_ZERO
  };

private:
  Expression *exp;
  ComparisonType type;

  /**
   * Disable copy constructor.
   * @param c
   */
  Constraint (const Constraint &c);

  /**
   * Instantiate a constraint with the given comparison type. Its expression should
   * be set manually later.
   * @param type The comparison type of this constraint.
   */
  Constraint (ComparisonType type);

public:
  /**
   * Constructs a constraint indicating that given expression should obey the
   * (in)equality which is specified by type. Expression is cloned and deleted upon
   * destruction of constraint object.
   * @param exp The expression which will be cloned and should satisfy (in)equality.
   * @param type Indicates the (in)equality which exp should satisfy it.
   */
  Constraint (const Expression *exp, ComparisonType type);

  ~Constraint ();

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static Constraint *loadFromBinaryStream (std::ifstream &in);

  std::string toString () const;

  const Expression *getExpression () const;
  ComparisonType getComparisonType () const;

  Constraint *instantiateNegatedConstraint () const;
};

}
}
}
}

#endif /* Constraint.h */
