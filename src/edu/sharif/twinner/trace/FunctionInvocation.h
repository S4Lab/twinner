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

class FunctionInvocation : public TraceSegmentTerminator {
private:
  std::string name;
  std::list<Expression *> args;

public:
  FunctionInvocation (std::string name, std::list<Expression *> args);
  virtual ~FunctionInvocation ();

  virtual std::string toString () const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static FunctionInvocation *loadFromBinaryStream (std::ifstream &in);
};

}
}
}
}

#endif /* FunctionInvocation.h */
