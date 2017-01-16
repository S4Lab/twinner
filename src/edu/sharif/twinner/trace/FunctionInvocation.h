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
  std::list<std::string> types;
  std::string firstArgumentAsString;

public:
  FunctionInvocation (std::string name);
  FunctionInvocation (std::string name,
      const std::list<Expression *> &args, const std::list<std::string> &types);
  FunctionInvocation (std::string name,
      const std::list<Expression *> &args, const std::list<std::string> &types,
      std::string firstArgumentAsString);
  virtual ~FunctionInvocation ();

  virtual std::string getCallingLine () const;
  virtual std::string toString () const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static FunctionInvocation *loadFromBinaryStream (std::ifstream &in);

private:
  void encodeString (std::stringstream &ss, std::string str) const;
};

}
}
}
}

#endif /* FunctionInvocation.h */
