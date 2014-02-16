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

/**
 * @interface
 */
class Operand : public ExpressionToken {

protected:

  Operand () :
      ExpressionToken () {
  }

  Operand (const Operand &op) :
      ExpressionToken (op) {
  }
};

}
}
}
}

#endif /* OPERAND_H_ */
