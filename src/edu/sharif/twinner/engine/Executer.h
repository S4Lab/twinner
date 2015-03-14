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

namespace exptoken {

class Symbol;
struct SymbolRecord;
}
}
namespace engine {

class Executer {

public:

  enum ExecutionMode {

    NORMAL_MODE = 0x1,
    INITIAL_STATE_DETECTION_MODE = 0x4,
  };

  /**
   * Indicating name of the temp file, being used to communicate with TwinTool
   * about the initial symbols values.
   */
  static const char *SYMBOLS_VALUES_COMMUNICATION_TEMP_FILE;

  /**
   * Indicating name of the temp file, being used to communicate with TwinTool
   * about the last execution trace.
   */
  static const char *EXECUTION_TRACE_COMMUNICATION_TEMP_FILE;

  /**
   * Indicating name of the temp file, being used to communicate content of
   * shared memory of disassembled instructions.
   */
  static const char *DISASSEMBLED_INSTRUCTIONS_MEMORY_TEMP_FILE;

  /**
   * Presence of this option in arguments of twintool, asks it to execute the program
   * through the requested path, but to perform a minimal instruction counting
   * instrumentation (instead of normal instrumentation).
   */
  static const char *OVERHEAD_MEASURMENT_OPTION;

private:
  const std::string baseCommand;
  std::string inputArguments;
  const bool overheads;

public:
  Executer (std::string pinLauncher, std::string twintool, std::string inputBinary,
      std::string inputArguments, bool main, bool overheads);

  void setCandidateAddresses (const std::set < ADDRINT > &addresses) const;
  void setSymbolsValues (const std::set <
      const edu::sharif::twinner::trace::exptoken::Symbol * > &symbols) const;

  edu::sharif::twinner::trace::Trace *executeSingleTraceInNormalMode () const;

  void changeArguments ();
  map < ADDRINT, UINT64 > executeSingleTraceInInitialStateDetectionMode () const;

private:
  typedef edu::sharif::twinner::trace::exptoken::SymbolRecord Record;

  bool saveSymbolRecordsToFile (ExecutionMode mode,
      std::map < int, std::list < Record > > records) const;
  void saveSymbolRecordsToBinaryStream (std::ofstream &out,
      ExecutionMode mode, std::map < int, std::list < Record > > records) const;

  std::set < ADDRINT > loadChangedAddressesFromFile (const char *path) const;
};

}
}
}
}

#endif /* Executer.h */
