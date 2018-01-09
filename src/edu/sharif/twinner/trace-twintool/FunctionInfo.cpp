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

#include "FunctionInfo.h"

#include <sstream>
#include <stdlib.h>

#include "edu/sharif/twinner/util/Logger.h"

#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/StateSummary.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"
#include "FunctionArgumentInfoImp.h"

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
        "Example 1: test@0x400400#2{int!const char *^s}\n"
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
    const std::string typeStr = typesStr.substr (last, separator - last);
    const std::string::size_type specifierSign = typeStr.find ("^");
    if (specifierSign != std::string::npos) {
      if (typeStr.substr (specifierSign + 1) == "s") {
        const std::string typeStrWithoutSpecifier =
            typeStr.substr (0, specifierSign);
        arguments.push_back
            (new FunctionArgumentInfoImp (typeStrWithoutSpecifier, true));
      } else {
        edu::sharif::twinner::util::Logger::error ()
            << "FunctionInfo::FunctionInfo (encodedInfo=" << encodedInfo << "):"
            " unsupported type specifier. Supported type specifiers which"
            "should be separated by ^ character from the type string are:\n"
            "\t- s: string resolution is requested\n";
        abort ();
      }
    } else {
      arguments.push_back (new FunctionArgumentInfoImp (typeStr, false));
    }
  }
  if (unsigned (argsNo) != arguments.size ()) {
    edu::sharif::twinner::util::Logger::error ()
        << "FunctionInfo::FunctionInfo (encodedInfo=" << encodedInfo << "):"
        " the number of args (" << std::dec << argsNo << ") does not match"
        " with the number of given types (" << arguments.size () << ")\n";
    abort ();
  }
}

FunctionInfo::FunctionInfo (const FunctionInfo &fi) :
    name (fi.name),
    address (fi.address),
    argsNo (fi.argsNo),
    autoArgs (fi.autoArgs) {
  for (std::vector<FunctionArgumentInfo *>::const_iterator it =
      fi.arguments.begin (); it != fi.arguments.end (); ++it) {
    const FunctionArgumentInfo *fai = *it;
    arguments.push_back (fai->clone ());
  }
}

FunctionInfo::~FunctionInfo () {
  for (std::vector<FunctionArgumentInfo *>::const_iterator it =
      arguments.begin (); it != arguments.end (); ++it) {
    delete *it;
  }
  arguments.clear ();
}

FunctionArgumentInfo *FunctionInfo::getArgument (int i, Trace *trace,
    const CONTEXT *context) const {
  return arguments.at (i)->resolve (i, trace, context);
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
    log << ", args=" << fi.getArgsNo () << '{';
    bool first = true;
    for (std::vector<FunctionArgumentInfo *>::const_iterator it =
        fi.arguments.begin (); it != fi.arguments.end (); ++it) {
      const FunctionArgumentInfo *fai = *it;
      if (first) {
        first = false;
      } else {
        log << '!';
      }
      log << fai;
    }
    return log << "})";
  }
}

}
}
}
}
