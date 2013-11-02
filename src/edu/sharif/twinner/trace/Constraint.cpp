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

#include "Constraint.h"

#include <stdexcept>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

Constraint::Constraint (const Expression *_exp, ComparisonType _type) :
exp (_exp->clone ()), type (_type) {
}

Constraint::~Constraint () {
  delete exp;
}

}
}
}
}
