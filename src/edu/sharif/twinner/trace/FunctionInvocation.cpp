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
    std::list<Expression *> _args, std::string _firstArgumentAsString) :
    TraceSegmentTerminator (),
    name (_name), args (_args),
    firstArgumentAsString (_firstArgumentAsString) {
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

  const int strLength = firstArgumentAsString.length ();
  out.write (reinterpret_cast<const char *> (&strLength), sizeof (strLength));
  out.write (firstArgumentAsString.c_str (), firstArgumentAsString.length ());
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

  int strLength = 0;
  in.read (reinterpret_cast<char *> (&strLength), sizeof (strLength));
  char *str = new char[strLength];
  in.read (str, strLength);
  const std::string firstArgumentAsString (str, strLength);
  delete[] str;

  return new FunctionInvocation (nameStr, args, firstArgumentAsString);
}


}
}
}
}
