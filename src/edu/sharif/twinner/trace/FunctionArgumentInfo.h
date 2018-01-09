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

#ifndef FUNCTION_ARGUMENT_INFO_H
#define FUNCTION_ARGUMENT_INFO_H

#include "Savable.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Trace;

class FunctionArgumentInfo : public Savable {
protected:
  std::string type;
  bool requiresStringResolution;
  Expression *exp;
  std::string expAsString;

  FunctionArgumentInfo (std::string typeStr, bool requiresStringResolution,
      Expression *exp, std::string expAsString);
  FunctionArgumentInfo (std::string typeStr, bool requiresStringResolution);

public:
  virtual ~FunctionArgumentInfo ();

  virtual FunctionArgumentInfo *resolve (int i,
      Trace *trace, const CONTEXT *context) const;

  virtual FunctionArgumentInfo *clone () const;

  const std::string &getResolvedString () const;
  Expression *getExpression ();
  const Expression *getExpression () const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static FunctionArgumentInfo *loadFromBinaryStream (std::ifstream &in);

  void writeToStream (std::stringstream &ss, bool useNonTechnicalNames) const;

  friend const edu::sharif::twinner::util::Logger &operator<< (
      const edu::sharif::twinner::util::Logger &log,
      const FunctionArgumentInfo *fai);
  friend std::stringstream &operator<< (
      std::stringstream &ss, const FunctionArgumentInfo *fai);
};

}
}
}
}

#endif /* FunctionArgumentInfo.h */
