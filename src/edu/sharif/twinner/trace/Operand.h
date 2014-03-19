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

#ifndef OPERAND_H_
#define OPERAND_H_

#include "ExpressionToken.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class ConcreteValue;

/**
 * @abstract
 */
class Operand : public ExpressionToken {

protected:
  ConcreteValue *concreteValue;

  Operand ();
  /**
   * Instantiates a new operand and clones the given concrete value for it.
   * @param cv The concrete value which its clone will be used as value of this operand.
   */
  Operand (const ConcreteValue &cv);
  /**
   * Instantiates a new operand and takes ownership of the given concrete value for it.
   * @param cv The concrete value which its ownership is taken for instantiated operand.
   */
  Operand (ConcreteValue *cv);
  Operand (const Operand &op);

public:
  ~Operand ();
};

}
}
}
}

#endif /* OPERAND_H_ */
