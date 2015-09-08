//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2015  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef MAR_INFO_H
#define MAR_INFO_H

#include "Savable.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Trace;
class Expression;

class MarInfo : public Savable {
private:
  int argc;
  char **argv;

  static void *initialArgv;

public:
  MarInfo (int argc, char **argv);

  bool isConsistent () const;
  void simplifyTrace (Trace *trace) const;

  void saveToFile (const char *path) const;
  virtual void saveToBinaryStream (std::ofstream &out) const;
  static MarInfo *readMarInfoFromFile (const char *path);

private:
  void simplifyExpression (Expression *exp) const;
};

}
}
}
}

#endif /* MarInfo.h */
