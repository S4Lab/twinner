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

#ifndef UNEXPECTED_CHANGE_EXCEPTION_H
#define UNEXPECTED_CHANGE_EXCEPTION_H

#include "edu/sharif/twinner/trace/WrongStateException.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class UnexpectedChangeException : public WrongStateException {

public:

  UnexpectedChangeException (REG reg, const ConcreteValue &expectedVal,
      const ConcreteValue &currentVal) :
      WrongStateException (prepareRegisterMessage (reg, expectedVal, currentVal),
      currentVal) {
  }

  UnexpectedChangeException (ADDRINT address, const ConcreteValue &expectedVal,
      const ConcreteValue &currentVal) :
      WrongStateException (prepareMemoryMessage (address, expectedVal, currentVal),
      currentVal) {
  }

  static const std::string prepareRegisterMessage (REG reg,
      const ConcreteValue &expectedVal, const ConcreteValue &currentVal) {
    const std::string addr = REG_StringShort (reg);
    std::stringstream ss;
    ss << "Value of an address changed unexpectedly"
        " without any interfering syscall\n"
        "\tExpected " << expectedVal << ", Got " << currentVal
        << "; at register " << addr.c_str () << " (code: 0x" << (unsigned int) reg << ")";
    return ss.str ();
  }

  static const std::string prepareMemoryMessage (ADDRINT address,
      const ConcreteValue &expectedVal, const ConcreteValue &currentVal) {
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

#endif	/* UnexpectedChangeException.h */