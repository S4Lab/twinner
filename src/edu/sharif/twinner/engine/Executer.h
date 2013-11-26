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
class MemoryEmergedSymbol;
struct SymbolRecord;
}
namespace engine {

class Executer {

public:

  enum ExecutionMode {

    INITIALIZED_MODE = 0x1,
    CHANGE_DETECTION_MODE = 0x2,
    INITIAL_STATE_DETECTION_MODE = 0x4,
  };

  /**
   * Indicating name of the temp file, being used to communicate with TwinTool about the initial symbols values.
   */
  static const char *SYMBOLS_VALUES_COMMUNICATION_TEMP_FILE;

  /**
   * Indicating name of the temp file, being used to communicate with TwinTool about the last execution trace.
   */
  static const char *EXECUTION_TRACE_COMMUNICATION_TEMP_FILE;

private:
  const std::string baseCommand;
  const std::string inputArguments;

public:
  Executer (std::string pinLauncher, std::string twintool, std::string inputBinary,
      std::string inputArguments);

  void setSymbolsValues (ExecutionMode mode,
      const std::set < const edu::sharif::twinner::trace::MemoryEmergedSymbol * > &
      symbols);

  edu::sharif::twinner::trace::Trace *executeSingleTraceInInitializedMode () const;
  std::set < ADDRINT > executeSingleTraceInChangeDetectionMode () const;
  set < const edu::sharif::twinner::trace::MemoryEmergedSymbol * >
  executeSingleTraceInInitialStateDetectionMode () const;

private:
  typedef edu::sharif::twinner::trace::SymbolRecord Record;

  bool saveSymbolRecordsToFile (ExecutionMode mode,
      std::map < int, std::list < Record > > records) const;
  void saveSymbolRecordsToBinaryStream (std::ofstream &out,
      ExecutionMode mode, std::map < int, std::list < Record > > records) const;
};

}
}
}
}

#endif /* Executer.h */
