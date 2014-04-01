//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2014  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef TWINNER_H
#define TWINNER_H

#include "pin.H"

#include <string>
#include <set>
#include <map>
#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Trace;
class Constraint;
class MemoryEmergedSymbol;
}
namespace engine {
namespace search {

class ConstraintTree;
}

class Executer;

class Twinner {

private:
  std::string input;
  std::string twintool;
  std::string pin;
  std::string twin;
  std::string arguments;

  std::list < const edu::sharif::twinner::trace::Trace * > traces;
  edu::sharif::twinner::engine::search::ConstraintTree *ctree;

public:
  Twinner ();
  ~Twinner ();

  void setInputBinaryPath (std::string input);
  void setTwinToolPath (std::string twintool);
  void setPinLauncherPath (std::string pin);
  void setTwinBinaryPath (std::string twin);
  void setInputBinaryArguments (std::string arguments);

  void generateTwinBinary ();

private:
  void addExecutionTrace (const edu::sharif::twinner::trace::Trace *trace);

  bool calculateSymbolsValuesForCoveringNextPath (
      std::set < const edu::sharif::twinner::trace::MemoryEmergedSymbol * > &symbols);

  std::map < ADDRINT, UINT64 > obtainInitializedMemoryValues (Executer &ex) const;
  void codeTracesIntoTwinCode (const std::map < ADDRINT, UINT64 > &initialValues);
  void codeInitialValuesIntoTwinCode (std::stringstream &out,
      const std::map < ADDRINT, UINT64 > &initialValues) const;
};

}
}
}
}

#endif /* Twinner.h */
