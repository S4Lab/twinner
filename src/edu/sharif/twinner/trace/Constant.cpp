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

#include <sstream>

#include "Constant.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

Constant::Constant (const Constant &c) :
value (c.value) {
}

Constant::Constant (UINT64 val) :
value (val) {
}

Constant *Constant::clone () const {
  return new Constant (*this);
}

void Constant::saveToBinaryStream (std::ofstream &out) const {
  out.write ("C", 1);
  out.write ((const char *) &value, sizeof (value));
}

Constant *Constant::loadFromBinaryStream (std::ifstream &in) {
  UINT64 value;
  in.read ((char *) &value, sizeof (value));
  return new Constant (value);
}

std::string Constant::toString () const {
  std::stringstream ss;
  ss << "0x" << std::hex << value;
  return ss.str ();
}

}
}
}
}
