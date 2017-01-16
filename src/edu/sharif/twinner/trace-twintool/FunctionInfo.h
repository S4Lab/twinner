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

#ifndef FUNCTION_INFO_H
#define FUNCTION_INFO_H

#include <string>
#include <list>

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
  std::list<std::string> types;
  bool autoArgs;

public:
  FunctionInfo (std::string encodedInfo);
  virtual ~FunctionInfo ();

  /**
   * Reads the symbolic expression which is going to be passed to the function
   * as described by the trace and context objects.
   * This function works only if context describes the state of the program
   * right before running the call instruction (in the caller-site).
   * The returned symbolic expression is owned by the caller and
   * must be deleted.
   *
   * @param i Index of the requested argument; first argument has 0 index.
   * @param trace The trace describing symbolic state of the program.
   * @param context The context of the program in the caller site.
   * @return A new symbolic expression describing the requested argument.
   */
  Expression *getArgument (int i, Trace *trace, const CONTEXT *context) const;

  bool isAutoArgs () const;
  int getArgsNo () const;
  const std::list<std::string> &getTypes () const;
  ADDRINT getAddress () const;
  std::string getName () const;

private:
#ifdef TARGET_IA32E
  Expression *getArgument (REG reg, Trace *trace, const CONTEXT *context) const;
#endif
  Expression *getArgument (int offset, ADDRINT topOfStack, Trace *trace,
      const CONTEXT *context) const;
};

const edu::sharif::twinner::util::Logger &operator<< (
    const edu::sharif::twinner::util::Logger &log, const FunctionInfo &fi);

}
}
}
}

#endif /* FunctionInfo.h */
