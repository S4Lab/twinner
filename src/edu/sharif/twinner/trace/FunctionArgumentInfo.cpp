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

#include "FunctionArgumentInfo.h"

#include <sstream>

#include "Expression.h"

#include "edu/sharif/twinner/util/Logger.h"


namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

FunctionArgumentInfo::FunctionArgumentInfo (std::string typeStr,
    bool _requiresStringResolution,
    Expression *_exp,
    std::string _expAsString) :
    type (typeStr),
    requiresStringResolution (_requiresStringResolution),
    exp (_exp),
    expAsString (_expAsString) {
}

FunctionArgumentInfo::FunctionArgumentInfo (std::string typeStr,
    bool _requiresStringResolution) :
    type (typeStr),
    requiresStringResolution (_requiresStringResolution),
    exp (0),
    expAsString ("") {
}

FunctionArgumentInfo::~FunctionArgumentInfo () {
  delete exp;
}

FunctionArgumentInfo *FunctionArgumentInfo::resolve (int i,
    Trace *trace, const CONTEXT *context) const {
  edu::sharif::twinner::util::Logger::error ()
      << "FunctionArgumentInfo::resolve (): is abstract!\n";
  abort ();
}

FunctionArgumentInfo *FunctionArgumentInfo::clone () const {
  return new FunctionArgumentInfo (type,
                                   requiresStringResolution,
                                   (exp == 0 ? 0 : exp->clone ()),
                                   expAsString);
}

const std::string &FunctionArgumentInfo::getResolvedString () const {
  return expAsString;
}

Expression *FunctionArgumentInfo::getExpression () {
  return exp;
}

const Expression *FunctionArgumentInfo::getExpression () const {
  return exp;
}

void FunctionArgumentInfo::saveToBinaryStream (std::ofstream &out) const {
  const char *typesMagicString = "TYP";
  out.write (typesMagicString, 3);
  const int len = type.length ();
  out.write (reinterpret_cast<const char *> (&len), sizeof (len));
  out.write (type.c_str (), len);
  const char *rsr = requiresStringResolution ? "s" : "S";
  out.write (rsr, 1);
  const char *expSign = exp ? "e" : "E";
  out.write (expSign, 1);
  if (exp) {
    exp->saveToBinaryStream (out);
  }
  const int strLength = expAsString.length ();
  out.write (reinterpret_cast<const char *> (&strLength), sizeof (strLength));
  out.write (expAsString.c_str (), expAsString.length ());
}

FunctionArgumentInfo *FunctionArgumentInfo::loadFromBinaryStream (std::ifstream &in) {
  char magicString[3];
  in.read (magicString, 3);
  if (strncmp (magicString, "TYP", 3) != 0) {
    edu::sharif::twinner::util::Logger::error ()
        << "FunctionArgumentInfo::loadListFromBinaryStream (in): "
        "Unexpected magic string while loading arguments\n";
    abort ();
  }
  int typeLength = 0;
  in.read (reinterpret_cast<char *> (&typeLength), sizeof (typeLength));
  char *typeStr = new char[typeLength];
  in.read (typeStr, typeLength);
  const std::string type (typeStr, typeLength);
  delete[] typeStr;
  char codes[2];
  in.read (codes, 2);
  const bool requiresStringResolution = codes[0] == 's';
  Expression *exp = 0;
  if (codes[1] == 'e') {
    exp = Expression::loadFromBinaryStream (in);
  }
  int strLength = 0;
  in.read (reinterpret_cast<char *> (&strLength), sizeof (strLength));
  char *str = new char[strLength];
  in.read (str, strLength);
  const std::string expAsString (str, strLength);
  delete[] str;
  return new FunctionArgumentInfo (type, requiresStringResolution,
                                   exp, expAsString);
}

void encodeString (std::stringstream &ss, std::string str);

void FunctionArgumentInfo::writeToStream (std::stringstream &ss,
    bool useNonTechnicalNames) const {
  if (!expAsString.empty ()) {
    ss << "/*";
    encodeString (ss, expAsString);
    ss << "*/ ";
  }
  ss << '(' << type << ") " << exp->toString (useNonTechnicalNames);
}

const edu::sharif::twinner::util::Logger &operator<<
(const edu::sharif::twinner::util::Logger &log,
 const FunctionArgumentInfo *fai) {
  log << fai->type;
  if (fai->requiresStringResolution) {
    log << "^s";
  }
  return log;
}

void encodeString (std::stringstream &ss, std::string str) {
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

std::stringstream &operator<< (std::stringstream &ss,
                               const FunctionArgumentInfo *fai) {
  if (!fai->expAsString.empty ()) {
    ss << "/*";
    encodeString (ss, fai->expAsString);
    ss << "*/ ";
  }
  ss << '(' << fai->type << ") " << fai->exp->toString ();
  return ss;
}

}
}
}
}
