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

#include "Symbol.h"

#include "ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

Symbol::Symbol (const ConcreteValue &_concreteValue, int _generationIndex) :
Operand (_concreteValue), generationIndex (_generationIndex) {
}

Symbol::Symbol (const Symbol &s) :
Operand (s), generationIndex (s.generationIndex) {
}

Symbol::Symbol () :
Operand (), generationIndex (0) {
}

void Symbol::saveToBinaryStream (std::ofstream &out) const {
  concreteValue->saveToBinaryStream (out);
  out.write ((const char *) &generationIndex, sizeof (generationIndex));
}

void Symbol::loadFromBinaryStream (std::ifstream &in) {
  delete concreteValue;
  concreteValue = ConcreteValue::loadFromBinaryStream (in);
  in.read ((char *) &generationIndex, sizeof (generationIndex));
}

}
}
}
}
