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

#ifndef EXPRESSIONTOKEN_H_
#define EXPRESSIONTOKEN_H_

#include "edu/sharif/twinner/trace/Savable.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

/**
 * @interface
 */
class ExpressionToken : public edu::sharif::twinner::trace::Savable {
protected:

  ExpressionToken ();

  ExpressionToken (const ExpressionToken &et);

public:

  virtual ~ExpressionToken ();

  virtual ExpressionToken *clone () const = 0;

  static ExpressionToken *loadFromBinaryStream (std::ifstream &in);

  virtual std::string toString () const = 0;
  virtual std::string getTechnicalName () const;

  virtual bool operator== (const ExpressionToken &token) const = 0;
};

}
}
}
}
}

#endif /* EXPRESSIONTOKEN_H_ */
