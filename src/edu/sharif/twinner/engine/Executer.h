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

#ifndef EXECUTER_H
#define EXECUTER_H

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

class Executer {

public:
  /**
   * Indicating name of the temp file, being used to communicate with TwinTool about the initial symbols values.
   */
  static const char *SYMBOLS_VALUES_COMMUNICATION_TEMP_FILE;

  /**
   * Indicating name of the temp file, being used to communicate with TwinTool about the last execution trace.
   */
  static const char *EXECUTION_TRACE_COMMUNICATION_TEMP_FILE;

private:
  std::string command;

public:
  Executer (std::string pinLauncher, std::string twintool, std::string inputBinary);

  void setSymbolsValues (
      const std::set < edu::sharif::twinner::trace::Symbol * > &symbols);

  edu::sharif::twinner::trace::Trace *executeSingleTrace ();
};

}
}
}
}

#endif /* Executer.h */
