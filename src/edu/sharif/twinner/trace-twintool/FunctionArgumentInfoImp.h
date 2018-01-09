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

#ifndef FUNCTION_ARGUMENT_INFO_IMP_H
#define FUNCTION_ARGUMENT_INFO_IMP_H

#include "edu/sharif/twinner/trace/FunctionArgumentInfo.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class FunctionArgumentInfoImp : public FunctionArgumentInfo {
protected:
  FunctionArgumentInfoImp (std::string typeStr, bool requiresStringResolution,
      Expression *exp, std::string expAsString);

public:
  FunctionArgumentInfoImp (std::string typeStr, bool requiresStringResolution);
  virtual ~FunctionArgumentInfoImp ();

  virtual FunctionArgumentInfo *resolve (int i,
      Trace *trace, const CONTEXT *context) const;
  virtual FunctionArgumentInfoImp *clone () const;

private:
  Expression *getArgument (int i,
      Trace *trace, const CONTEXT *context) const;
#ifdef TARGET_IA32E
  Expression *getArgument (REG reg,
      Trace *trace, const CONTEXT *context) const;
#endif
  Expression *getArgument (int offset, ADDRINT topOfStack,
      Trace *trace, const CONTEXT *context) const;
};

}
}
}
}

#endif /* FunctionArgumentInfoImp.h */
