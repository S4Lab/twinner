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

#ifndef MAR_INFO_H
#define MAR_INFO_H

#include "Savable.h"

#include <vector>

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
  std::vector<char *> argvis;
  std::vector<int> argvilens;
  std::vector<char *> argvicontents;
  bool inspectionMode;

  static void *initialArgv;

public:
  MarInfo (int argc, char **argv);
  MarInfo (int argc, char **argv, std::vector<char *> argvis,
      std::vector<int> argvilens, std::vector<char *> argvicontents);
  ~MarInfo ();

  bool isConsistent () const;
  void simplifyTrace (Trace *trace) const;
  void simplifyExpression (Expression *exp) const;

  void saveToFile (const char *path) const;
  virtual void saveToBinaryStream (std::ofstream &out) const;
  void saveToOutputStream (std::ostream &out) const;
  static MarInfo *readMarInfoFromFile (std::string path);

  static void **getInitialArgv ();

private:
  static MarInfo *loadFromBinaryStream (std::ifstream &in);
};

}
}
}
}

#endif /* MarInfo.h */
