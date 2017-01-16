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

#include "FunctionInvocation.h"

#include "Savable.h"
#include "Expression.h"

#include <sstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

FunctionInvocation::FunctionInvocation (std::string _name) :
    TraceSegmentTerminator (),
    name (_name) {
}

FunctionInvocation::FunctionInvocation (std::string _name,
    const std::list<Expression *> &_args,
    const std::list<std::string> &_types) :
    TraceSegmentTerminator (),
    name (_name), args (_args), types (_types) {
}

FunctionInvocation::FunctionInvocation (std::string _name,
    const std::list<Expression *> &_args, const std::list<std::string> &_types,
    std::string _firstArgumentAsString) :
    TraceSegmentTerminator (),
    name (_name), args (_args), types (_types),
    firstArgumentAsString (_firstArgumentAsString) {
}

FunctionInvocation::~FunctionInvocation () {
}

std::string FunctionInvocation::getCallingLine () const {
  std::stringstream ss;
  ss << name << " (";
  bool first = true;
  std::list<std::string>::const_iterator tt = types.begin ();
  for (std::list<Expression *>::const_iterator it = args.begin ();
      it != args.end (); ++it, ++tt) {
    const Expression *arg = *it;
    const std::string &type = *tt;
    if (first) {
      first = false;
      if (!firstArgumentAsString.empty ()) {
        ss << "/*";
        encodeString (ss, firstArgumentAsString);
        ss << "*/ ";
      }
    } else {
      ss << ", ";
    }
    ss << '(' << type << ") " << arg->toString ();
  }
  ss << ");";
  return ss.str ();
}

std::string FunctionInvocation::toString () const {
  return "FunctionInvocation (calling-line=" + getCallingLine () + ")";
}

void FunctionInvocation::saveToBinaryStream (std::ofstream &out) const {
  const char *terminatorMagicString = "FUN";
  out.write (terminatorMagicString, 3);

  const int nameLength = name.length ();
  out.write (reinterpret_cast<const char *> (&nameLength), sizeof (nameLength));
  out.write (name.c_str (), nameLength);

  saveListToBinaryStream (out, "ARG", args);
  {
    const char *typesMagicString = "TYP";
    out.write (typesMagicString, 3);
    for (std::list<std::string>::const_iterator it = types.begin ();
        it != types.end (); ++it) {
      const std::string &type = *it;
      const int len = type.length ();
      out.write (reinterpret_cast<const char *> (&len), sizeof (len));
      out.write (type.c_str (), len);
    }
  }
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
  std::list<std::string> types;
  {
    char magicString[3];
    in.read (magicString, 3);
    if (strncmp (magicString, "TYP", 3) != 0) {
      edu::sharif::twinner::util::Logger::error ()
          << "FunctionInvocation::loadListFromBinaryStream (in): "
          "Unexpected magic string while loading argument types\n";
      abort ();
    }
    const int argsNo = args.size ();
    for (int i = 0; i < argsNo; ++i) {
      int typeLength = 0;
      in.read (reinterpret_cast<char *> (&typeLength), sizeof (typeLength));
      char *typeStr = new char[typeLength];
      in.read (typeStr, typeLength);
      const std::string type (typeStr, typeLength);
      delete[] typeStr;
      types.push_back (type);
    }
  }
  int strLength = 0;
  in.read (reinterpret_cast<char *> (&strLength), sizeof (strLength));
  char *str = new char[strLength];
  in.read (str, strLength);
  const std::string firstArgumentAsString (str, strLength);
  delete[] str;

  return new FunctionInvocation (nameStr, args, types, firstArgumentAsString);
}

void FunctionInvocation::encodeString (std::stringstream &ss, std::string str) const {
  ss << '"';
  for (std::string::const_iterator it = str.begin (); it != str.end (); ++it) {
    const char c = *it;
    if (c == '\\') {
      ss << "\\\\";
    } else if (c == ' ' || ispunct (c) || isalnum (c)) {
      ss << c;
    } else if (c == '\r') {
      ss << "\\r";
    } else if (c == '\n') {
      ss << "\\n";
    } else if (c == '\a') {
      ss << "\\a";
    } else if (c == '\b') {
      ss << "\\b";
    } else if (c == '\f') {
      ss << "\\f";
    } else if (c == '\t') {
      ss << "\\t";
    } else if (c == '\v') {
      ss << "\\v";
    } else {
      ss << "\\x" << std::hex << int (c);
    }
  }
  ss << '"';
}


}
}
}
}
