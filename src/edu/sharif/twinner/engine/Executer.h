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

#include <string>
#include <set>
#include <map>
#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Trace;
class MarInfo;

namespace exptoken {

class Symbol;
struct SymbolRecord;
}
}
namespace engine {

struct Measurement;

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
  static const char *OVERHEAD_MEASUREMENT_OPTION;

  /**
   * Indicating name of the temp file, being used to communicate information
   * about the main() args when code is instrumented in main()-only mode.
   */
  static const char *MAIN_ARGS_COMMUNICATION_TEMP_FILE;

private:
  const std::string baseCommand;
  bool signaled;
  std::string inputArguments;
  const bool overheads;

public:
  Executer (std::string pinLauncher, std::string twintool,
      std::string inputBinary, std::string inputArguments,
      std::string endpoints, bool main,
      bool naive, bool overheads);

  void setCandidateAddresses (
      const std::set < std::pair < ADDRINT, int > > &addresses) const;
  void setSymbolsValues (const std::set <
      const edu::sharif::twinner::trace::exptoken::Symbol * > &symbols) const;

  edu::sharif::twinner::trace::Trace *executeSingleTraceInNormalMode ();
  bool isLastExecutionSignaled () const;

  void changeArguments ();
  std::map < std::pair < ADDRINT, int >, UINT64 >
  executeSingleTraceInInitialStateDetectionMode () const;

  /**
   * Reads main() args info and wraps read info in a new MarInfo instance.
   * Caller owns the returned object.
   *
   * @return an instance of MarInfo containing main() args info of last run.
   */
  edu::sharif::twinner::trace::MarInfo *readMarInfo () const;

private:
  typedef edu::sharif::twinner::trace::exptoken::SymbolRecord Record;

  edu::sharif::twinner::trace::Trace *executeSystemCommand (std::string command);
  edu::sharif::twinner::trace::Trace *executeSystemCommand (std::string command,
      Measurement &measurement);

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
