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

#ifndef EXPRESSION_IMP_H
#define EXPRESSION_IMP_H

#include "Expression.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

class Symbol;
}
namespace cv {

class ConcreteValue64Bits;
}

class ExpressionImp : public Expression {
private:
  ExpressionImp (const ExpressionImp &exp);

public:
  /**
   * Instantiates an expression containing a new (yet unused) symbol,
   * initiated from a register.
   * A temporary symbol will be created iff the snapshotIndex is not -1.
   * ASSERT: The precision of concreteValue must match with precision of reg
   * ASSERT: reg == REG_FullRegName (reg)
   */
  ExpressionImp (REG reg,
      const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue,
      int generationIndex, int snapshotIndex = -1);

  /**
   * Instantiates an expression containing a new (yet unused) symbol,
   * initiated from a memory address.
   * A temporary symbol will be created iff the snapshotIndex is not -1.
   * ASSERT: The precision of concreteValue must match with precision of memory location
   * ASSERT: memoryEa is aligned (based on size of the concrete value)
   */
  ExpressionImp (ADDRINT memoryEa,
      const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue,
      int generationIndex, bool isOverwriting = false, int snapshotIndex = -1);

  /**
   * Instantiates an expression containing the given symbol.
   * Ownership of the given symbol is kept by the expression.
   * The concrete value of the given symbol will be used as the last concrete value of
   * the expression too.
   */
  ExpressionImp (edu::sharif::twinner::trace::exptoken::Symbol *symbol);

  /**
   * Instantiates an expression containing a constant value (non-symbolic).
   */
  ExpressionImp (const edu::sharif::twinner::trace::cv::ConcreteValue &value);

  /**
   * Instantiates an expression containing a constant value (non-symbolic) and takes
   * ownership of the given concrete value.
   */
  ExpressionImp (edu::sharif::twinner::trace::cv::ConcreteValue *value);

  /**
   * Instantiates an expression containing a constant value (non-symbolic).
   */
  ExpressionImp (UINT64 value = 0);

  virtual ExpressionImp *clone () const;

private:
  edu::sharif::twinner::trace::exptoken::ExpressionToken *
  instantiateMemorySymbol (ADDRINT memoryEa,
      const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue,
      int generationIndex, bool isOverwriting, int snapshotIndex) const;
};

}
}
}
}

#endif /* ExpressionImp.h */
