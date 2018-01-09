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


#ifndef FUNCTION_INVOCATION_H
#define FUNCTION_INVOCATION_H

#include <string>
#include <vector>

#include "TraceSegmentTerminator.h"


namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Expression;
class FunctionArgumentInfo;

class FunctionInvocation : public TraceSegmentTerminator {
private:
  std::string name;
  std::list<FunctionArgumentInfo *> args;

public:
  FunctionInvocation (std::string name);
  FunctionInvocation (std::string name,
      const std::list<FunctionArgumentInfo *> &args);
  virtual ~FunctionInvocation ();

  virtual FunctionInvocation *clone () const;

  virtual std::string getCallingLine (bool useNonTechnicalNames) const;
  virtual std::string toString () const;
  virtual void replaceTemporarySymbols (const Snapshot *lastSnapshot);

  virtual bool operator== (const TraceSegmentTerminator &t) const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static FunctionInvocation *loadFromBinaryStream (std::ifstream &in);

  std::list<const Expression *> getArgumentExpressions () const;
};

}
}
}
}

#endif /* FunctionInvocation.h */
