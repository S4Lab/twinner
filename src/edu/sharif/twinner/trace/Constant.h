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

#ifndef CONSTANT_H_
#define CONSTANT_H_

#include "Operand.h"

#include "pin.H"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

//TODO: support other types else of UINT64 too

class Constant : public Operand {

private:
  Constant (const Constant &c);

public:
  Constant (UINT64 val);
  Constant (const ConcreteValue &val);
  Constant (ConcreteValue *val);

  virtual Constant *clone () const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static Constant *loadFromBinaryStream (std::ifstream &in);

  virtual std::string toString () const;

  UINT64 getValue () const;
};

}
}
}
}

#endif /* CONSTANT_H_ */
