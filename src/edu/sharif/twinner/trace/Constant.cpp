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

#include <sstream>

#include "Constant.h"
#include "ConcreteValue64Bits.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

Constant::Constant (const Constant &c) :
Operand (c) {
}

Constant::Constant (UINT64 val) :
Operand (ConcreteValue64Bits (val)) {
}

Constant::Constant (const ConcreteValue &val) :
Operand (val) {
}

Constant::Constant (ConcreteValue *val) :
Operand (val) {
}

Constant *Constant::clone () const {
  return new Constant (*this);
}

void Constant::saveToBinaryStream (std::ofstream &out) const {
  out.write ("C", 1);
  concreteValue->saveToBinaryStream (out);
}

Constant *Constant::loadFromBinaryStream (std::ifstream &in) {
  return new Constant (ConcreteValue::loadFromBinaryStream (in));
}

std::string Constant::toString () const {
  std::stringstream ss;
  ss << *concreteValue;
  return ss.str ();
}

bool Constant::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const Constant *> (&token)
      && (*static_cast<const Constant *> (&token)->concreteValue) == (*concreteValue);
}

}
}
}
}
