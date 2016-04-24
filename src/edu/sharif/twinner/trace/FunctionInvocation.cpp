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

#include "FunctionInvocation.h"

#include "Savable.h"
#include "Expression.h"

#include <sstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

FunctionInvocation::FunctionInvocation (std::string _name,
    std::list<Expression *> _args) :
    TraceSegmentTerminator (),
    name (_name), args (_args) {
}

FunctionInvocation::~FunctionInvocation () {
}

std::string FunctionInvocation::toString () const {
  return "FunctionInvocation ()";
}

void FunctionInvocation::saveToBinaryStream (std::ofstream &out) const {
  const char *terminatorMagicString = "FUN";
  out.write (terminatorMagicString, 3);

  const int nameLength = name.length ();
  out.write (reinterpret_cast<const char *> (&nameLength), sizeof (nameLength));
  out.write (name.c_str (), name.length ());

  saveListToBinaryStream (out, "ARG", args);
}

FunctionInvocation *FunctionInvocation::loadFromBinaryStream (
    std::ifstream &in) {
  int nameLength = 0;
  in.read (reinterpret_cast<char *> (&nameLength), sizeof (nameLength));
  char *name = new char[nameLength];
  in.read (name, nameLength);
  const std::string nameStr (name, nameLength);
  delete[] name;

  std::list<Expression *> args;
  loadListFromBinaryStream (in, "ARG", args);

  return new FunctionInvocation (nameStr, args);
}


}
}
}
}
