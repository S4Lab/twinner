//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2017 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef STATE_SUMMARY_H
#define STATE_SUMMARY_H

#include "cv/ConcreteValue.h"


namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

/**
 * This class indicates a summary of the execution state. It indicates whether
 * the read value is valid or there was an unexpected change in the execution
 * state during the last operation.
 */
class StateSummary {
private:
  const edu::sharif::twinner::trace::cv::ConcreteValue *expectedValue;
  const edu::sharif::twinner::trace::cv::ConcreteValue *currentValue;
  std::string msg;
  bool unexpectedChange;

  StateSummary (const StateSummary &disabledCopyConstructor);

public:

  StateSummary () :
  expectedValue (0), currentValue (0), unexpectedChange (false) {
  }

  ~StateSummary () {
    clear ();
  }

  void clear () {
    delete expectedValue;
    delete currentValue;
    expectedValue = 0;
    currentValue = 0;
    unexpectedChange = false;
  }

  void initWrongState (
      const edu::sharif::twinner::trace::cv::ConcreteValue &_expectedVal,
      const edu::sharif::twinner::trace::cv::ConcreteValue &_currentVal,
      const std::string _msg = "Execution state differ from what we expected "
      "(probably, user space memory is changed by a syscall).") {
    expectedValue = _expectedVal.clone ();
    currentValue = _currentVal.clone ();
    msg = _msg;
  }

  bool isWrongState () const {
    return expectedValue != 0;
  }

  void setUnexpectedChangeState (const std::string _msg) {
    msg = _msg;
    unexpectedChange = true;
  }

  void setChangeAsExpected (
      const std::string _msg = "Execution state differ from what we expected "
      "(probably, user space memory is changed by a syscall).") {
    msg = _msg;
    unexpectedChange = false;
  }

  bool isUnexpectedChangeState () const {
    return unexpectedChange;
  }

  const edu::sharif::twinner::trace::cv::ConcreteValue &
  getExpectedStateValue () const {
    return *expectedValue;
  }

  const edu::sharif::twinner::trace::cv::ConcreteValue &
  getCurrentStateValue () const {
    return *currentValue;
  }

  std::string getMessage () const {
    return msg;
  }
};

}
}
}
}

#endif /* StateSummary.h */
