//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2018 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef CONSTRAINT_OPERAND_H
#define CONSTRAINT_OPERAND_H

#include "Operand.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

class ConstraintOperand : public Operand {
private:
  const edu::sharif::twinner::trace::Constraint *constraint;

  ConstraintOperand (const ConstraintOperand &co);

public:
  ConstraintOperand (const edu::sharif::twinner::trace::Constraint *c);
  ConstraintOperand (const edu::sharif::twinner::trace::Constraint *c, bool cv);
  ~ConstraintOperand ();

  virtual ConstraintOperand *clone () const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static ConstraintOperand *loadFromBinaryStream (std::ifstream &in);

  virtual std::string toString () const;
  virtual bool operator== (const ExpressionToken &token) const;
  virtual bool isConstant () const;
};

}
}
}
}
}

#endif /* ConstraintOperand.h */
