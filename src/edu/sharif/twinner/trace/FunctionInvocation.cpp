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

#include "FunctionInvocation.h"

#include "Savable.h"
#include "Expression.h"
#include "Snapshot.h"

#include "FunctionArgumentInfo.h"

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
    const std::list<FunctionArgumentInfo *> &_args) :
    TraceSegmentTerminator (),
    name (_name), args (_args) {
}

FunctionInvocation::~FunctionInvocation () {
  while (!args.empty ()) {
    delete args.front ();
    args.pop_front ();
  }
}

FunctionInvocation *FunctionInvocation::clone () const {
  std::list<FunctionArgumentInfo *> clonedArgs;
  for (std::list<FunctionArgumentInfo *>::const_iterator it = args.begin ();
      it != args.end (); ++it) {
    const FunctionArgumentInfo *fai = *it;
    clonedArgs.push_back (fai->clone ());
  }
  return new FunctionInvocation (name, clonedArgs);
}

std::string FunctionInvocation::getCallingLine (bool useNonTechnicalNames) const {
  std::stringstream ss;
  ss << "regs.rax = ";
  ss << name << " (";
  bool first = true;
  for (std::list<FunctionArgumentInfo *>::const_iterator it = args.begin ();
      it != args.end (); ++it) {
    const FunctionArgumentInfo *arg = *it;
    if (first) {
      first = false;
    } else {
      ss << ", ";
    }
    arg->writeToStream (ss, useNonTechnicalNames);
  }
  ss << ");";
  return ss.str ();
}

std::string FunctionInvocation::toString () const {
  return "FunctionInvocation (calling-line=" + getCallingLine (false) + ")";
}

void FunctionInvocation::replaceTemporarySymbols (const Snapshot *lastSnapshot) {
  for (std::list<FunctionArgumentInfo *>::iterator it = args.begin ();
      it != args.end (); ++it) {
    FunctionArgumentInfo *arg = *it;
    lastSnapshot->replaceTemporarySymbols (arg->getExpression ());
  }
}

bool FunctionInvocation::operator== (const TraceSegmentTerminator &t) const {
  if (!dynamic_cast<const FunctionInvocation *> (&t)) {
    return false;
  }
  const FunctionInvocation &fi = static_cast<const FunctionInvocation &> (t);
  if (name != fi.name || args.size () != fi.args.size ()) {
    return false;
  }
  for (std::list<FunctionArgumentInfo *>::const_iterator it1 = args.begin (),
      it2 = fi.args.begin (); it1 != args.end (); ++it1, ++it2) {
    const FunctionArgumentInfo *arg1 = *it1;
    const FunctionArgumentInfo *arg2 = *it2;
    if (!((*arg1->getExpression ()) == (*arg2->getExpression ()))) {
      return false;
    }
  }
  return true;
}

void FunctionInvocation::saveToBinaryStream (std::ofstream &out) const {
  const char *terminatorMagicString = "FUN";
  out.write (terminatorMagicString, 3);

  const int nameLength = name.length ();
  out.write (reinterpret_cast<const char *> (&nameLength), sizeof (nameLength));
  out.write (name.c_str (), nameLength);

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

  std::list<FunctionArgumentInfo *> args;
  loadListFromBinaryStream (in, "ARG", args);

  return new FunctionInvocation (nameStr, args);
}

std::list<const Expression *> FunctionInvocation::getArgumentExpressions () const {
  std::list<const Expression *> exps;
  for (std::list<FunctionArgumentInfo *>::const_iterator it = args.begin ();
      it != args.end (); ++it) {
    const FunctionArgumentInfo *fai = *it;
    exps.push_back (fai->getExpression ());
  }
  return exps;
}


}
}
}
}
