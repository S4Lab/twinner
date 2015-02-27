//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2015  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef UNSATISFIABLE_CONSTRAINTS_EXCEPTION_H
#define UNSATISFIABLE_CONSTRAINTS_EXCEPTION_H

#include "edu/sharif/twinner/exception/AbstractException.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace smt {

/**
 * This exception indicates that constraints are unsatisfiable simultaneously.
 */
class UnsatisfiableConstraintsException : public edu::sharif::twinner::exception::AbstractException {

public:

  UnsatisfiableConstraintsException () :
      AbstractException ("Constraints are not satisfiable simultaneously.") {
  }
};

}
}
}
}
}

#endif	/* UnsatisfiableConstraintsException.h */
