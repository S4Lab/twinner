//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2016 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "FunctionInfo.h"

#include <sstream>
#include <stdlib.h>

#include "edu/sharif/twinner/twintool/RegisterResidentExpressionValueProxy.h"

#include "edu/sharif/twinner/util/Logger.h"

#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/StateSummary.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

FunctionInfo::FunctionInfo (std::string encodedInfo) {
  const std::string::size_type atsign = encodedInfo.find ("@");
  if (atsign == std::string::npos) {
    edu::sharif::twinner::util::Logger::error ()
        << "FunctionInfo::FunctionInfo (encodedInfo=" << encodedInfo << "):"
        " function address is missing."
        " Expected format: <func-name>@0x<hex-address>#(<args-no>|auto)\n";
    abort ();
  }
  name = encodedInfo.substr (0, atsign);
  const std::string::size_type hashsign = encodedInfo.find ("#");
  if (hashsign == std::string::npos) {
    edu::sharif::twinner::util::Logger::error ()
        << "FunctionInfo::FunctionInfo (encodedInfo=" << encodedInfo << "):"
        " args number is missing."
        " Expected format: <func-name>@0x<hex-address>#(<args-no>|auto)\n";
    abort ();
  }
  const std::string addressStr =
      encodedInfo.substr (atsign + 1, hashsign - atsign - 1);
  if (addressStr.substr (0, 2) != "0x") {
    edu::sharif::twinner::util::Logger::error ()
        << "FunctionInfo::FunctionInfo (encodedInfo=" << encodedInfo << "):"
        " address must be in 0xHexAddress format.\n";
    abort ();
  }
  char *endptr;
  address = ADDRINT (strtoll (addressStr.c_str (), &endptr, 16));
  if (*endptr) {
    edu::sharif::twinner::util::Logger::error ()
        << "FunctionInfo::FunctionInfo (encodedInfo=" << encodedInfo << "):"
        " address is not a valid hex number.\n";
    abort ();
  }
  const std::string argsStr = encodedInfo.substr (hashsign + 1);
  if (argsStr == "auto") {
    argsNo = 0;
    autoArgs = true;
  } else {
    autoArgs = false;
    argsNo = int (strtol (argsStr.c_str (), &endptr, 10));
    if (*endptr) {
      edu::sharif::twinner::util::Logger::error ()
          << "FunctionInfo::FunctionInfo (encodedInfo=" << encodedInfo << "):"
          " arguments number should be either ``auto'' or a decimal number.\n";
      abort ();
    }
  }
}

FunctionInfo::~FunctionInfo () {
}

Expression *FunctionInfo::getArgument (int i, Trace *trace,
    const CONTEXT *context) const {
  int offset;
  const REG regs[] = {
    REG_RDI, REG_RSI, REG_RDX, REG_RCX, REG_R8, REG_R9
  };
  return getArgument (regs[i], trace, context);
}

Expression *FunctionInfo::getArgument (REG reg, Trace *trace,
    const CONTEXT *context) const {
  edu::sharif::twinner::twintool::RegisterResidentExpressionValueProxy proxy
      (reg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits
       (PIN_GetContextReg (context, reg)));
  edu::sharif::twinner::trace::StateSummary state;
  edu::sharif::twinner::trace::Expression *exp =
      proxy.getExpression (trace, state);
  if (state.isWrongState ()) {
    edu::sharif::twinner::util::Logger::error () << state.getMessage () << '\n';
    abort ();
  }
  return exp;
}

bool FunctionInfo::isAutoArgs () const {
  return autoArgs;
}

int FunctionInfo::getArgsNo () const {
  return argsNo;
}

ADDRINT FunctionInfo::getAddress () const {
  return address;
}

std::string FunctionInfo::getName () const {
  return name;
}

const edu::sharif::twinner::util::Logger &operator<<
(const edu::sharif::twinner::util::Logger &log, const FunctionInfo &fi) {
  log << "FunctionInfo (name=" << fi.getName ()
      << ", address=0x" << std::hex << fi.getAddress ();
  if (fi.isAutoArgs ()) {
    return log << ", args=auto)";
  } else {
    return log << ", args=" << fi.getArgsNo () << ")";
  }
}

}
}
}
}
