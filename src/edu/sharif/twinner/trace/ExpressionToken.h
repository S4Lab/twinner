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

#ifndef EXPRESSIONTOKEN_H_
#define EXPRESSIONTOKEN_H_

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

/**
 * @interface
 */
class ExpressionToken {
protected:
  ExpressionToken () {
  }

  ExpressionToken (const ExpressionToken &et) {
  }

public:
  virtual ~ExpressionToken () {
  }

  virtual ExpressionToken *clone () const = 0;
};

}
}
}
}

#endif /* EXPRESSIONTOKEN_H_ */
