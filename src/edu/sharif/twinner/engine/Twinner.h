//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef TWINNER_H
#define TWINNER_H

#include <string>
#include <set>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Trace;
class Symbol;
}
namespace engine {

class Twinner {
private:
  std::string input;
  std::string pin;
  std::string twin;

public:
  void setInputBinaryPath (std::string input);
  void setPinLauncherPath (std::string pin);
  void setTwinBinaryPath (std::string twin);

  void generateTwinBinary ();

private:
  void addExecutionTrace (const edu::sharif::twinner::trace::Trace &trace);

  bool calculateSymbolsValuesForCoveringNextPath (
      const edu::sharif::twinner::trace::Trace &trace,
      std::set < edu::sharif::twinner::trace::Symbol > &symbols);

  void codeTracesIntoTwinBinary ();
};

}
}
}
}

#endif /* Twinner.h */
