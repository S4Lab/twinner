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
namespace cv {

class ConcreteValue;
}

/**
 * This exception indicates that current execution state differs from the expected state.
 */
class WrongStateException : public edu::sharif::twinner::exception::AbstractException {

private:
  const edu::sharif::twinner::trace::cv::ConcreteValue &expectedValue;
  const edu::sharif::twinner::trace::cv::ConcreteValue &currentValue;

public:

  WrongStateException (const edu::sharif::twinner::trace::cv::ConcreteValue &_expectedVal,
      const edu::sharif::twinner::trace::cv::ConcreteValue &_currentVal) :
      AbstractException ("Execution state differ from what we expected "
      "(probably, user space memory is changed by a syscall)."),
      expectedValue (_expectedVal), currentValue (_currentVal) {
  }

  WrongStateException (const std::string &msg,
      const edu::sharif::twinner::trace::cv::ConcreteValue &_expectedVal,
      const edu::sharif::twinner::trace::cv::ConcreteValue &_currentVal) :
      AbstractException (msg), expectedValue (_expectedVal), currentValue (_currentVal) {
  }

  const edu::sharif::twinner::trace::cv::ConcreteValue &getExpectedStateValue () const {
    return expectedValue;
  }

  const edu::sharif::twinner::trace::cv::ConcreteValue &getCurrentStateValue () const {
    return currentValue;
  }
};

}
}
}
}

#endif	/* WrongStateException.h */
