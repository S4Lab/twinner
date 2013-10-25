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

#include "Symbol.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

Symbol::Symbol (UINT64 _concreteValue, int _generationIndex) :
Operand (), concreteValue (_concreteValue), generationIndex (_generationIndex) {
}

Symbol::Symbol (const Symbol &s) :
Operand (s), concreteValue (s.concreteValue), generationIndex (s.generationIndex) {
}

}
}
}
}
