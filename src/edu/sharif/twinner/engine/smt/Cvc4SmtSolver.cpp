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

#if 1 // inclusion of CVC4 header
/*
 * Of course, Twinner is not supposed to fix warnings of CVC4. Also it does not like
 * to disable -Werror. So we need to disable warnings in CVC4 temporarily. CVC4 is
 * using deprecated code which defining following macro, will prevent its related warning
 * to be raised while compiling following included header file.
 */
#define _BACKWARD_BACKWARD_WARNING_H 1
#include <cvc4/cvc4.h>
#undef _BACKWARD_BACKWARD_WARNING_H
#endif // end of inclusion of CVC4 header

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
