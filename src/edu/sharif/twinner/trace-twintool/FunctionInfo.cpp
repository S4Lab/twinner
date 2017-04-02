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

#include "FunctionInfo.h"

#include <sstream>
#include <stdlib.h>

#include "edu/sharif/twinner/proxy/RegisterResidentExpressionValueProxy.h"
#include "edu/sharif/twinner/proxy/MemoryResidentExpressionValueProxy.h"

#include "edu/sharif/twinner/util/Logger.h"

#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/StateSummary.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

#ifdef TARGET_IA32E
static const int STACK_OPERATION_UNIT_SIZE = 8; // bytes
#else
static const int STACK_OPERATION_UNIT_SIZE = 4; // bytes
#endif

FunctionInfo::FunctionInfo (std::string encodedInfo) {
  const std::string::size_type atsign = encodedInfo.find ("@");
  if (atsign == std::string::npos) {
    edu::sharif::twinner::util::Logger::error ()
        << "FunctionInfo::FunctionInfo (encodedInfo=" << encodedInfo << "):"
        " function address is missing."
        " Expected format: <func-name>@0x<hex-address>"
        "#(<args-no>{arg1type!arg2type!...!argntype}|auto)\n";
    abort ();
  }
  name = encodedInfo.substr (0, atsign);
  const std::string::size_type hashsign = encodedInfo.find ("#");
  if (hashsign == std::string::npos) {
    edu::sharif::twinner::util::Logger::error ()
        << "FunctionInfo::FunctionInfo (encodedInfo=" << encodedInfo << "):"
        " args number is missing."
        " Expected format: <func-name>@0x<hex-address>"
        "#(<args-no>{arg1type!arg2type!...!argntype}|auto)\n";
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
    return;
  }
  autoArgs = false;
  const std::string::size_type typesSign = argsStr.find ("{");
  if (typesSign == std::string::npos
      || argsStr[argsStr.length () - 1] != '}') {
    edu::sharif::twinner::util::Logger::error ()
        << "FunctionInfo::FunctionInfo (encodedInfo=" << encodedInfo << "):"
        " argument types are required in the non-auto mode."
        " Expected format: <func-name>@0x<hex-address>"
        "#(<args-no>{arg1type!arg2type!...!argntype}|auto)\n"
        "Example 1: test@0x400400#2{int!const int *}\n"
        "Example 2: test@0x400400#0{}\n";
    abort ();
  }
  argsNo = int (strtol (argsStr.substr (0, typesSign).c_str (), &endptr, 10));
  if (*endptr || argsNo < 0) {
    edu::sharif::twinner::util::Logger::error ()
        << "FunctionInfo::FunctionInfo (encodedInfo=" << encodedInfo << "):"
        " arguments number should be either ``auto'' or a decimal number.\n";
    abort ();
  }
  const std::string typesStr =
      argsStr.substr (typesSign + 1, argsStr.length () - typesSign - 2) + '!';
  if (argsNo == 0 && typesStr == "!") {
    return;
  }
  std::string::size_type last = 0;
  for (std::string::size_type separator = typesStr.find ("!");
      separator != std::string::npos;
      separator = typesStr.find ("!", last = separator + 1)) {
    if (separator == last) {
      edu::sharif::twinner::util::Logger::error ()
          << "FunctionInfo::FunctionInfo (encodedInfo=" << encodedInfo << "):"
          " empty argument type is given.\n";
      abort ();
    }
    types.push_back (typesStr.substr (last, separator - last));
  }
  if (unsigned (argsNo) != types.size ()) {
    edu::sharif::twinner::util::Logger::error ()
        << "FunctionInfo::FunctionInfo (encodedInfo=" << encodedInfo << "):"
        " the number of args (" << std::dec << argsNo << ") does not match"
        " with the number of given types (" << types.size () << ")\n";
    abort ();
  }
}

FunctionInfo::~FunctionInfo () {
}

Expression *FunctionInfo::getArgument (int i, Trace *trace,
    const CONTEXT *context) const {
#if defined(TARGET_IA32E) && defined(TARGET_LINUX)
  // args are in RDI, RSI, RDX, RCX, R8, R9, and in the stack respectively
  if (i < 6) {
    const REG regs[] = {
      REG_RDI, REG_RSI, REG_RDX, REG_RCX, REG_R8, REG_R9
    };
    return getArgument (regs[i], trace, context);
  }
  const int offset = (i - 6) * STACK_OPERATION_UNIT_SIZE;
  const ADDRINT topOfStack = PIN_GetContextReg (context, REG_RSP);
#elif defined(TARGET_IA32)
  // args are in the stack respectively (first argument is pushed last)
  const int offset = i * STACK_OPERATION_UNIT_SIZE;
  const ADDRINT topOfStack = PIN_GetContextReg (context, REG_ESP);
#else
#error "Unsupported architecture and/or OS"
#endif
  return getArgument (offset, topOfStack, trace, context);
}
#ifdef TARGET_IA32E

Expression *FunctionInfo::getArgument (REG reg, Trace *trace,
    const CONTEXT *context) const {
  edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy proxy
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
#endif

Expression *FunctionInfo::getArgument (int offset, ADDRINT topOfStack,
    Trace *trace, const CONTEXT *context) const {
  edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy proxy
      (topOfStack + offset, STACK_OPERATION_UNIT_SIZE);
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

const std::list<std::string> &FunctionInfo::getTypes () const {
  return types;
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
    log << ", args=" << fi.getArgsNo () << '{';
    bool first = true;
    for (std::list<std::string>::const_iterator it = fi.getTypes ().begin ();
        it != fi.getTypes ().end (); ++it) {
      if (first) {
        first = false;
      } else {
        log << '!';
      }
      log << (*it);
    }
    return log << "})";
  }
}

}
}
}
}
