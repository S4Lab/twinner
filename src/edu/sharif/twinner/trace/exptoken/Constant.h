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

#ifndef CONSTANT_H_
#define CONSTANT_H_

#include "Operand.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

class Constant : public Operand {
private:
  Constant (const Constant &c);

public:
  Constant (UINT64 val);
  Constant (const edu::sharif::twinner::trace::cv::ConcreteValue &val);
  Constant (edu::sharif::twinner::trace::cv::ConcreteValue *val);

  void setValue (const edu::sharif::twinner::trace::cv::ConcreteValue &value);
  void setValue (edu::sharif::twinner::trace::cv::ConcreteValue *value);
  virtual Constant *clone () const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static Constant *loadFromBinaryStream (std::ifstream &in);

  virtual std::string toString () const;

  virtual bool operator== (const ExpressionToken &token) const;

  virtual bool isConstant () const;
};

}
}
}
}
}

#endif /* CONSTANT_H_ */
