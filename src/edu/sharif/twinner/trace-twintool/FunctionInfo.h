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

#ifndef FUNCTION_INFO_H
#define FUNCTION_INFO_H

#include <string>

#include "edu/sharif/twinner/pin-wrapper.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

class Logger;
}
namespace trace {

class Expression;
class Trace;

class FunctionInfo {
private:
  std::string name;
  ADDRINT address;
  int argsNo;
  bool autoArgs;

public:
  FunctionInfo (std::string encodedInfo);
  virtual ~FunctionInfo ();

  bool isAutoArgs () const;
  int getArgsNo () const;
  ADDRINT getAddress () const;
  std::string getName () const;
};

const edu::sharif::twinner::util::Logger &operator<< (
    const edu::sharif::twinner::util::Logger &log, const FunctionInfo &fi);

}
}
}
}

#endif /* FunctionInfo.h */
