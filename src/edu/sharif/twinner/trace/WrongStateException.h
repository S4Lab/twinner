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

#ifndef WRONG_STATE_EXCEPTION_H
#define WRONG_STATE_EXCEPTION_H

#include "edu/sharif/twinner/exception/AbstractException.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class ConcreteValue;

/**
 * This exception indicates that current execution state differs from the expected state.
 */
class WrongStateException : public edu::sharif::twinner::exception::AbstractException {

private:
  const ConcreteValue &currentValue;

public:

  WrongStateException (const ConcreteValue &_currentValue) :
      AbstractException ("Execution state differ from what we expected "
      "(probably, user space memory is changed by a syscall)."),
      currentValue (_currentValue) {
  }

  WrongStateException (const std::string &msg, const ConcreteValue &_currentValue) :
      AbstractException (msg), currentValue (_currentValue) {
  }

  const ConcreteValue &getCurrentStateValue () const {
    return currentValue;
  }
};

}
}
}
}

#endif	/* WrongStateException.h */
