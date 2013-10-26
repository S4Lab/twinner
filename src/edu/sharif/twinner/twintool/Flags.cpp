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

#include "Flags.h"

#include "edu/sharif/twinner/trace/Expression.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

Flags::Flags () :
exp (0) {
}

Flags::~Flags () {
  if (exp) {
    delete exp;
  }
}

void Flags::setFlags (edu::sharif::twinner::trace::Expression *_exp) {
  if (exp) {
    delete exp;
  }
  exp = _exp;
}

}
}
}
}
