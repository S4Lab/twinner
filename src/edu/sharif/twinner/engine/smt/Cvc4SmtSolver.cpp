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

#include "Cvc4SmtSolver.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace smt {

Cvc4SmtSolver::Cvc4SmtSolver () :
SmtSolver () {
}

Cvc4SmtSolver::~Cvc4SmtSolver () {
}

void Cvc4SmtSolver::solveConstraints (
    const std::list < const edu::sharif::twinner::trace::Constraint * > &constraints,
    std::set < const edu::sharif::twinner::trace::MemoryEmergedSymbol * > &symbols)
throw (UnsatisfiableConstraintsException) {
  throw UnsatisfiableConstraintsException ();
}

}
}
}
}
}
