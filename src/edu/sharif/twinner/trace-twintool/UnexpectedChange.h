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

#ifndef UNEXPECTED_CHANGE_H
#define UNEXPECTED_CHANGE_H

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"
#include "edu/sharif/twinner/trace/StateSummary.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class UnexpectedChange {
public:

  static void adoptStateSummary (StateSummary &state, REG reg) {
    state.setUnexpectedChangeState
        (prepareRegisterMessage (reg, state.getExpectedStateValue (),
        state.getCurrentStateValue ()));
  }

  static void adoptStateSummary (StateSummary &state, ADDRINT address) {
    state.setUnexpectedChangeState
        (prepareMemoryMessage (address, state.getExpectedStateValue (),
        state.getCurrentStateValue ()));
  }

  static const std::string prepareRegisterMessage (REG reg,
      const edu::sharif::twinner::trace::cv::ConcreteValue &expectedVal,
      const edu::sharif::twinner::trace::cv::ConcreteValue &currentVal) {
    const std::string addr = REG_StringShort (reg);
    std::stringstream ss;
    ss << "Value of an address changed unexpectedly"
        " without any interfering syscall\n"
        "\tExpected " << expectedVal << ", Got " << currentVal
        << "; at register " << addr.c_str () << " (code: 0x" << (unsigned int) reg << ")";
    return ss.str ();
  }

  static const std::string prepareMemoryMessage (ADDRINT address,
      const edu::sharif::twinner::trace::cv::ConcreteValue &expectedVal,
      const edu::sharif::twinner::trace::cv::ConcreteValue &currentVal) {
    std::stringstream ss;
    ss << "Value of an address changed unexpectedly"
        " without any interfering syscall\n"
        "\tExpected " << expectedVal << ", Got " << currentVal
        << "; at address 0x" << address;
    return ss.str ();
  }
};

}
}
}
}

#endif /* UnexpectedChange.h */
