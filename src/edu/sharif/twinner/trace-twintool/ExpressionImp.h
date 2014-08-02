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

#ifndef EXPRESSION_FOR_TWINTOOL_H
#define EXPRESSION_FOR_TWINTOOL_H

#include "edu/sharif/twinner/trace/Expression.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class ExpressionImp : public Expression {

private:
  ExpressionImp (const ExpressionImp &exp);

public:
  /**
   * Instantiates an expression containing a new (yet unused) symbol,
   * initiated from a register.
   * ASSERT: The precision of concreteValue must match with precision of reg
   */
  ExpressionImp (REG reg, const ConcreteValue &concreteValue, int generationIndex);

  /**
   * Instantiates an expression containing a new (yet unused) symbol,
   * initiated from a memory address.
   * ASSERT: The precision of concreteValue must match with precision of memory location
   */
  ExpressionImp (ADDRINT memoryEa, const ConcreteValue &concreteValue,
      int generationIndex, bool isOverwriting = false);

  /**
   * Instantiates an expression containing a constant value (non-symbolic).
   */
  ExpressionImp (const ConcreteValue &value);

  /**
   * Instantiates an expression containing a constant value (non-symbolic) and takes
   * ownership of the given concrete value.
   */
  ExpressionImp (ConcreteValue *value);

  /**
   * Instantiates an expression containing a constant value (non-symbolic).
   */
  ExpressionImp (UINT64 value = 0);

  virtual ExpressionImp *clone () const;
};

}
}
}
}

#endif /* ExpressionImp.h */
