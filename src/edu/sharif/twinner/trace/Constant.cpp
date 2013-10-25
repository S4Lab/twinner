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

}
}
}
}
