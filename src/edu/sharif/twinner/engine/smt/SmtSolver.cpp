//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2018 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "edu/sharif/twinner/pin-wrapper.h"

#include <stdexcept>

#include "SmtSolver.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace smt {

SmtSolver *SmtSolver::me;

SmtSolver::SmtSolver () {
}

SmtSolver::~SmtSolver () {
}

void SmtSolver::init (SmtSolver *instance) {
  if (me) {
    throw std::runtime_error ("SMT Solver instance is already initialized");
  }
  me = instance;
}

SmtSolver *SmtSolver::getInstance () {
  return me;
}

void SmtSolver::destroy () {
  delete me;
  me = 0;
}

}
}
}
}
}
