//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2017 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "edu/sharif/twinner/pin-wrapper.h"

#include "Executer.h"

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <algorithm>
#include <sys/stat.h>

#include "edu/sharif/twinner/trace/MarInfo.h"
#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/ExecutionTraceSegment.h"
#include "edu/sharif/twinner/trace/exptoken/MemoryEmergedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/RegisterEmergedSymbol.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/LogStream.h"
#include "edu/sharif/twinner/util/iterationtools.h"
#include "edu/sharif/twinner/util/MemoryManager.h"

#ifdef TARGET_WINDOWS
#define WIFEXITED(w)    (((w) & 0xffffff00) == 0)
#define WIFSIGNALED(w)  (!WIFEXITED(w))
#define WEXITSTATUS(w)  (w)
#define WTERMSIG(w)     (w)
#endif

using namespace std;

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {

struct Measurement {
  int ret; // full return value including termination and exit codes
  uint64_t cputime; // including user/system/kernel times in microseconds
  uint64_t mss; // maximum segment size (in kilobytes)
};

bool executeAndMeasure (std::string command, Measurement &m);

inline void save_address (std::ofstream &out,
    const std::pair < ADDRINT, int > &address);
inline void add_symbol_to_map (
    std::map < int, std::set < edu::sharif::twinner::trace::exptoken::SymbolRecord > > &records,
    const edu::sharif::twinner::trace::exptoken::Symbol * const &symbol);
inline void save_records_list (std::ofstream &out, const int &segmentIndex,
    const std::set < edu::sharif::twinner::trace::exptoken::SymbolRecord > &recordsList);
inline void save_record (std::ofstream &out,
    const edu::sharif::twinner::trace::exptoken::SymbolRecord &record);

std::string Executer::getUniqueLogfileName (int uniqueId) {
  std::stringstream ss;
  ss << edu::sharif::twinner::util::LogStream::getInstance ()->getLogfileName ()
      << "-" << uniqueId;
  return ss.str ();
}

const char *Executer::SYMBOLS_VALUES_COMMUNICATION_TEMP_FILE = "/twinner/symbols.dat";
const char *Executer::EXECUTION_TRACE_COMMUNICATION_TEMP_FILE = "/twinner/trace.dat";
const char *Executer::DISASSEMBLED_INSTRUCTIONS_MEMORY_TEMP_FILE =
    "/twinner/memory.dat";
const char *Executer::OVERHEAD_MEASUREMENT_OPTION = " -measure";
const char *Executer::MAIN_ARGS_COMMUNICATION_TEMP_FILE =
    "/twinner/main-args-reporting.dat";

Executer::Executer (int uniqueId,
    std::string pinLauncher, std::string pinMemoryRange,
    std::string twintool,
    std::string inputBinary, std::string _inputArguments,
    std::string endpoints, bool _newRecord, bool _replayRecord,
    std::string safeFunctions, std::string _tmpfolder,
    bool main, std::string stackOffset, bool naive, bool _overheads) :
    tmpfolder (_tmpfolder),
    baseCommand (pinLauncher
    + " -pin_memory_range " + pinMemoryRange
    + " -t " + twintool
    + " -symbols " + tmpfolder + SYMBOLS_VALUES_COMMUNICATION_TEMP_FILE
    + " -trace " + tmpfolder + EXECUTION_TRACE_COMMUNICATION_TEMP_FILE
    + " -memory " + tmpfolder + DISASSEMBLED_INSTRUCTIONS_MEMORY_TEMP_FILE
    + " -verbose " + edu::sharif::twinner::util::LogStream::getInstance ()
    ->getVerbosenessLevelAsString ()
    + " -logfilename " + getUniqueLogfileName (uniqueId)
    + (_overheads ? OVERHEAD_MEASUREMENT_OPTION : "")
    + (main ? std::string (" -main -mar ") + tmpfolder + MAIN_ARGS_COMMUNICATION_TEMP_FILE : "")
    + (endpoints != "" ? std::string (" -endpoints ") + endpoints
       + " -mar " + tmpfolder + MAIN_ARGS_COMMUNICATION_TEMP_FILE : "")
    + (safeFunctions != "" ? std::string (" -safe-functions ")
       + "'" + safeFunctions + "'" : "")
    + (stackOffset != "" ? std::string (" -stack-offset ") + stackOffset : "")
    + (naive ? " -naive" : "")
    + " -- " + inputBinary),
    inputBinaryHash (calculateHash (inputBinary)),
    signaled (false),
    inputArguments (_inputArguments),
    newRecord (_newRecord), replayRecord (_replayRecord),
    overheads (_overheads) {
  if ((main && endpoints != "") || (naive && (main || endpoints != ""))) {
    edu::sharif::twinner::util::Logger::error ()
        << "The -main, -endpoints, and -naive are mutually exclusive.\n";
    abort ();
  }
}

void Executer::setCandidateAddresses (
    const std::set < std::pair < ADDRINT, int > > &addresses) const {
  std::ofstream out;
  out.open ((tmpfolder + SYMBOLS_VALUES_COMMUNICATION_TEMP_FILE).c_str (),
            ios_base::out | ios_base::trunc | ios_base::binary);
  if (!out.is_open ()) {
    edu::sharif::twinner::util::Logger::error ()
        << "Executer::setCandidateAddresses (...): "
        "Can not write memory addresses to file due to error in open function: "
        << tmpfolder << SYMBOLS_VALUES_COMMUNICATION_TEMP_FILE << '\n';
    abort ();
  }
  out.write ("SYM", 3);
  ExecutionMode mode = INITIAL_STATE_DETECTION_MODE;
  out.write ((const char *) &mode, sizeof (ExecutionMode));

  std::set < std::pair < ADDRINT, int > >::size_type s = addresses.size ();
  out.write ((const char *) &s, sizeof (s));

  edu::sharif::twinner::util::foreach (addresses, &save_address, out);
  out.close ();
}

void save_address (std::ofstream &out, const std::pair < ADDRINT, int > &address) {
  const ADDRINT symbolAddress = address.first;
  const int symbolSize = address.second;
  out.write ((const char *) &symbolAddress, sizeof (symbolAddress));
  out.write ((const char *) &symbolSize, sizeof (symbolSize));
}

void Executer::setSymbolsValues (
    const std::set < const edu::sharif::twinner::trace::exptoken::Symbol * > &symbols) const {
  std::map < int, std::set < Record > > records;
  edu::sharif::twinner::util::foreach (symbols, &add_symbol_to_map, records);

  if (!saveSymbolRecordsToFile (NORMAL_MODE, records)) {
    edu::sharif::twinner::util::Logger::error ()
        << "Executer::setSymbolsValues (...): "
        "Error in saving symbols in binary file\n";
    abort ();
  }
}

void add_symbol_to_map (
    std::map < int, std::set < edu::sharif::twinner::trace::exptoken::SymbolRecord > > &records,
    const edu::sharif::twinner::trace::exptoken::Symbol * const &symbol) {
  std::pair < int, edu::sharif::twinner::trace::exptoken::SymbolRecord > pair =
      symbol->toSymbolRecord ();
  records[pair.first].insert (pair.second);
}

bool Executer::saveSymbolRecordsToFile (ExecutionMode mode,
    std::map < int, std::set < Record > > records) const {
  std::ofstream out;
  out.open ((tmpfolder + SYMBOLS_VALUES_COMMUNICATION_TEMP_FILE).c_str (),
            ios_base::out | ios_base::trunc | ios_base::binary);
  if (!out.is_open ()) {
    edu::sharif::twinner::util::Logger::error ()
        << "Can not write symbols to file:"
        " Error in open function: "
        << tmpfolder << SYMBOLS_VALUES_COMMUNICATION_TEMP_FILE << '\n';
    return false;
  }
  saveSymbolRecordsToBinaryStream (out, mode, records);
  out.close ();
  return true;
}

void Executer::saveSymbolRecordsToBinaryStream (std::ofstream &out,
    ExecutionMode mode, std::map < int, std::set < Record > > records) const {
  out.write ("SYM", 3);
  out.write ((const char *) &mode, sizeof (ExecutionMode));

  std::map < int, std::set < Record > >::size_type s = records.size ();
  out.write ((const char *) &s, sizeof (s));

  edu::sharif::twinner::util::foreach (records, &save_records_list, out);
}

void save_records_list (std::ofstream &out, const int &segmentIndex,
    const std::set < edu::sharif::twinner::trace::exptoken::SymbolRecord > &recordsList) {
  out.write ((const char *) &segmentIndex, sizeof (segmentIndex));
  std::set < edu::sharif::twinner::trace::exptoken::SymbolRecord >::size_type s =
      recordsList.size ();
  out.write ((const char *) &s, sizeof (s));
  edu::sharif::twinner::util::foreach (recordsList, &save_record, out);
}

void save_record (std::ofstream &out,
    const edu::sharif::twinner::trace::exptoken::SymbolRecord &record) {
  out.write ((const char *) &record.address, sizeof (record.address));
  out.write ((const char *) &record.type, sizeof (record.type));
  out.write ((const char *) &record.concreteValueLsb, sizeof (record.concreteValueLsb));
  out.write ((const char *) &record.concreteValueMsb, sizeof (record.concreteValueMsb));
}

void aggregate_symbols (const edu::sharif::twinner::trace::Expression *exp,
    std::map < ADDRINT, const edu::sharif::twinner::trace::exptoken::Symbol * > &msyms,
    std::map < REG, const edu::sharif::twinner::trace::exptoken::Symbol * > &rsyms) {
  if (!exp) {
    return;
  }
  const std::list < edu::sharif::twinner::trace::exptoken::ExpressionToken * > &stack =
      exp->getStack ();
  for (std::list < edu::sharif::twinner::trace::exptoken::ExpressionToken * >
      ::const_iterator it = stack.begin (); it != stack.end (); ++it) {
    const edu::sharif::twinner::trace::exptoken::ExpressionToken *token = *it;
    if (dynamic_cast<const edu::sharif::twinner::trace::exptoken
        ::MemoryEmergedSymbol *> (token)) {
      const edu::sharif::twinner::trace::exptoken::MemoryEmergedSymbol *symbol =
          static_cast<const edu::sharif::twinner::trace::exptoken
          ::MemoryEmergedSymbol *> (token);
      msyms.insert (make_pair (symbol->getAddress (), symbol));
    } else if (dynamic_cast<const edu::sharif::twinner::trace::exptoken
        ::RegisterEmergedSymbol *> (token)) {
      const edu::sharif::twinner::trace::exptoken::RegisterEmergedSymbol *symbol =
          static_cast<const edu::sharif::twinner::trace::exptoken
          ::RegisterEmergedSymbol *> (token);
      rsyms.insert (make_pair (symbol->getAddress (), symbol));
    }
  }
}

template <typename Address>
void aggregate_symbols (
    const std::map < Address, edu::sharif::twinner::trace::Expression * > &expressions,
    std::map < ADDRINT, const edu::sharif::twinner::trace::exptoken::Symbol * > &msyms,
    std::map < REG, const edu::sharif::twinner::trace::exptoken::Symbol * > &rsyms) {
  for (typename std::map < Address, edu::sharif::twinner::trace::Expression * >
      ::const_iterator it = expressions.begin (); it != expressions.end (); ++it) {
    const edu::sharif::twinner::trace::Expression *exp = it->second;
    aggregate_symbols (exp, msyms, rsyms);
  }
}

void aggregate_symbols (
    const std::list < edu::sharif::twinner::trace::Constraint * > &constraints,
    std::map < ADDRINT, const edu::sharif::twinner::trace::exptoken::Symbol * > &msyms,
    std::map < REG, const edu::sharif::twinner::trace::exptoken::Symbol * > &rsyms) {
  for (std::list < edu::sharif::twinner::trace::Constraint * >
      ::const_iterator it = constraints.begin (); it != constraints.end (); ++it) {
    const edu::sharif::twinner::trace::Constraint *constraint = *it;
    aggregate_symbols (constraint->getMainExpression (), msyms, rsyms);
    aggregate_symbols (constraint->getAuxExpression (), msyms, rsyms);
  }
}

std::string Executer::escapeForShell (std::string str) const {
  size_t pos = 0;
  while ((pos = str.find ("'", pos)) != std::string::npos) {
    str.replace (pos, 1, "'\\''");
    pos += 4;
  }
  pos = 0;
  while ((pos = str.find (" ", pos)) != std::string::npos) {
    str.replace (pos, 1, "' '");
    pos += 3;
  }
  return std::string ("'" + str + "'");
}

/**
 * Uses OS interface to run  twintool in an independent process. The execution
 * trace will be communicated with twintool through file interface.
 * @return The execution trace, recorded by twintool
 */
edu::sharif::twinner::trace::Trace *
Executer::executeSingleTraceInNormalMode () {
  /*
   *  TODO: Run command through another thread and set a timeout for execution.
   *  Also tune twintool, so it saves its progress incrementally. After a timeout,
   *  partial trace can be read and twintool will be killed. Malwares are not
   *  required to finish execution! Another possible approach is to tune twintool
   *  to timeout execution and exit after a while. In this way, this code does not
   *  need to be changed at all.
   */
  const std::string command = baseCommand + " " + escapeForShell (inputArguments);
  if (overheads) {
    std::string cmd = command;
    const std::string whiteSpace (strlen (OVERHEAD_MEASUREMENT_OPTION), ' ');
    cmd.replace (cmd.find (OVERHEAD_MEASUREMENT_OPTION),
                 strlen (OVERHEAD_MEASUREMENT_OPTION), whiteSpace);
    Measurement ourMeasure, baselineMeasure;
    edu::sharif::twinner::trace::Trace *trace = executeSystemCommand (cmd, ourMeasure);
    if (trace == 0) {
      return 0;
    }
    const std::list < edu::sharif::twinner::trace::ExecutionTraceSegment * > &segments =
        trace->getTraceSegments ();
    std::map < ADDRINT, const edu::sharif::twinner::trace::exptoken::Symbol * > msyms;
    std::map < REG, const edu::sharif::twinner::trace::exptoken::Symbol * > rsyms;
    for (std::list < edu::sharif::twinner::trace::ExecutionTraceSegment * >
        ::const_iterator it = segments.begin (); it != segments.end (); ++it) {
      const edu::sharif::twinner::trace::ExecutionTraceSegment *segment = *it;
      aggregate_symbols (segment->getMemoryAddressTo8BitsExpression (), msyms, rsyms);
      aggregate_symbols (segment->getRegisterToExpression (), msyms, rsyms);
      aggregate_symbols (segment->getPathConstraints (), msyms, rsyms);
    }
    std::set < const edu::sharif::twinner::trace::exptoken::Symbol * > symbols;
    for (std::map < ADDRINT, const edu::sharif::twinner::trace::exptoken::Symbol * >
        ::const_iterator it = msyms.begin (); it != msyms.end (); ++it) {
      symbols.insert (it->second);
    }
    for (std::map < REG, const edu::sharif::twinner::trace::exptoken::Symbol * >
        ::const_iterator it = rsyms.begin (); it != rsyms.end (); ++it) {
      symbols.insert (it->second);
    }
    setSymbolsValues (symbols);
    if (executeSystemCommand (command, baselineMeasure) == 0) {
      return 0;
    }
    //    std::string normalCommand = "./obj-intel64/mpwpl-manual-virt-obf.out ";
    //    normalCommand += inputArguments;
    //    executeSystemCommand (normalCommand, baselineMeasure);
    double cpuOverhead = ourMeasure.cputime;
    cpuOverhead = ((cpuOverhead / baselineMeasure.cputime) - 1) * 100;
    double memoryOverhead = ourMeasure.mss;
    memoryOverhead = ((memoryOverhead / baselineMeasure.mss) - 1) * 100;
    edu::sharif::twinner::util::Logger::info () << std::dec << std::setprecision (2)
        << "CPU overhead (%): " << cpuOverhead
        << " | Memory overhead (%): " << memoryOverhead << '\n';
    return trace;
  }
  return executeSystemCommand (command);
}

bool Executer::isLastExecutionSignaled () const {
  return signaled;
}

edu::sharif::twinner::trace::MarInfo *Executer::readMarInfo () const {
  return edu::sharif::twinner::trace::MarInfo::readMarInfoFromFile
      (tmpfolder + MAIN_ARGS_COMMUNICATION_TEMP_FILE);
}

edu::sharif::twinner::trace::Trace *
Executer::executeSystemCommand (std::string command) {
  signaled = false;
  const std::string argsHash = calculateStringHash (inputArguments);
  const std::string symbolsHash =
      calculateHash (tmpfolder + SYMBOLS_VALUES_COMMUNICATION_TEMP_FILE);
  unlinkCommunicationFiles ();
  if (replayRecord) {
    if (restoreExecutionResult (inputBinaryHash, argsHash, symbolsHash)) {
      return edu::sharif::twinner::trace::Trace::loadFromFile
          (tmpfolder + EXECUTION_TRACE_COMMUNICATION_TEMP_FILE,
           tmpfolder + DISASSEMBLED_INSTRUCTIONS_MEMORY_TEMP_FILE);
    }
    unlinkCommunicationFiles ();
  }
  edu::sharif::twinner::util::Logger::debug ()
      << "Calling system (\"" << command << "\");\n";
  int ret = system (command.c_str ());
  edu::sharif::twinner::util::Logger::debug ()
      << "Executer::executeSystemCommand ('" << command << "'): "
      << "The system(...) call returns code: " << ret << '\n';
  if (WIFEXITED (ret)) {
    edu::sharif::twinner::util::Logger log =
        edu::sharif::twinner::util::Logger::loquacious ();
    log << "normally exited; ";
    printReturnValue (log, WEXITSTATUS (ret));
  } else if (WIFSIGNALED (ret)) {
    edu::sharif::twinner::util::Logger::loquacious () << "signaled; "
        "signal was " << WTERMSIG (ret) << '\n';
    signaled = true;
    return 0;
  }
  if (newRecord || replayRecord) {
    if (!recordExecutionResult (inputBinaryHash, argsHash, symbolsHash, true)) {
      signaled = true;
      return 0;
    }
  }
  return edu::sharif::twinner::trace::Trace::loadFromFile
      (tmpfolder + EXECUTION_TRACE_COMMUNICATION_TEMP_FILE,
       tmpfolder + DISASSEMBLED_INSTRUCTIONS_MEMORY_TEMP_FILE);
}

edu::sharif::twinner::trace::Trace *
Executer::executeSystemCommand (std::string command, Measurement &measurement) {
  signaled = false;
  unlinkCommunicationFiles ();
  edu::sharif::twinner::util::Logger::debug ()
      << "Calling system (\"" << command << "\");\n";
  edu::sharif::twinner::trace::Trace *trace = 0;
  if (executeAndMeasure (command, measurement)) {
    trace = edu::sharif::twinner::trace::Trace::loadFromFile
        (tmpfolder + EXECUTION_TRACE_COMMUNICATION_TEMP_FILE,
         tmpfolder + DISASSEMBLED_INSTRUCTIONS_MEMORY_TEMP_FILE);
  }
  const int ret = measurement.ret;
  edu::sharif::twinner::util::Logger::debug ()
      << "Executer::executeSystemCommand ('" << command << "', measurement): "
      << "The system(...) call returns code: " << ret << '\n';
  if (WIFEXITED (ret)) {
    edu::sharif::twinner::util::Logger log =
        edu::sharif::twinner::util::Logger::loquacious ();
    log << "normally exited; ";
    printReturnValue (log, WEXITSTATUS (ret));
  } else if (WIFSIGNALED (ret)) {
    edu::sharif::twinner::util::Logger::loquacious () << "signaled; "
        "signal was " << WTERMSIG (ret) << '\n';
    signaled = true;
  }
  return trace;
}

void Executer::printReturnValue (edu::sharif::twinner::util::Logger &log,
    int ret) const {
  log << "return value was " << std::dec << ret
      << " (char signed: " << int (char (ret)) << ")\n";
}

void Executer::changeArguments () {
  const char * const str = inputArguments.c_str ();
  char * const buffer = new char [inputArguments.size () + 52];
  const char *lastArg = buffer;
  const char *args = str;
  char *ptr = buffer;
  for (; *args; ++args, ++ptr) {
    if (*args == ' ') {
      lastArg = ptr + 1;
      *ptr = ' ';
    } else {
      *ptr = (*args) + 1;
    }
  }
  const char * const end = ptr;
  *ptr++ = ' ';
  for (int i = 0; lastArg < end && i < 50; ++lastArg, ++ptr, ++i) {
    *ptr = (*lastArg) + 1;
  }
  *ptr = '\0';
  inputArguments = std::string (buffer);
  delete[] buffer;
}

map < std::pair < ADDRINT, int >, UINT64 >
Executer::executeSingleTraceInInitialStateDetectionMode () const {
  const std::string argsHash = calculateStringHash (inputArguments);
  const std::string symbolsHash =
      calculateHash (tmpfolder + SYMBOLS_VALUES_COMMUNICATION_TEMP_FILE);
  unlinkCommunicationFiles ();
  if (replayRecord) {
    if (restoreExecutionResult (inputBinaryHash, argsHash, symbolsHash)) {
      return edu::sharif::twinner::trace::Trace::loadAddressToValueMapFromFile
          (tmpfolder + EXECUTION_TRACE_COMMUNICATION_TEMP_FILE);
    }
    unlinkCommunicationFiles ();
  }
  std::string command = baseCommand + " " + escapeForShell (inputArguments);
  if (overheads) {
    command.erase (command.find (OVERHEAD_MEASUREMENT_OPTION),
                   strlen (OVERHEAD_MEASUREMENT_OPTION));
  }
  edu::sharif::twinner::util::Logger::debug ()
      << "Calling system (\"" << command << "\");\n";
  int ret = system (command.c_str ());
  edu::sharif::twinner::util::Logger::debug ()
      << "Executer::executeSingleTraceInInitialStateDetectionMode () "
      "[command: '" << command << "']: "
      << "The system(...) call returns code: " << ret << '\n';
  if (ret != 0) {
    abort ();
  }
  if (newRecord) {
    if (!recordExecutionResult (inputBinaryHash, argsHash, symbolsHash, false)) {
      abort ();
    }
  }
  return edu::sharif::twinner::trace::Trace::loadAddressToValueMapFromFile
      (tmpfolder + EXECUTION_TRACE_COMMUNICATION_TEMP_FILE);
}

void Executer::unlinkCommunicationFiles () const {
  std::list<std::string> tmpFiles;
  tmpFiles.push_back (tmpfolder + EXECUTION_TRACE_COMMUNICATION_TEMP_FILE);
  tmpFiles.push_back (tmpfolder + DISASSEMBLED_INSTRUCTIONS_MEMORY_TEMP_FILE);
  tmpFiles.push_back (tmpfolder + MAIN_ARGS_COMMUNICATION_TEMP_FILE);
  for (std::list<std::string>::const_iterator it = tmpFiles.begin ();
      it != tmpFiles.end (); ++it) {
    const char *path = it->c_str ();
    if (access (path, F_OK) == 0 && unlink (path) == -1) {
      edu::sharif::twinner::util::Logger::error ()
          << "Error: Cannot delete the " << path << " file.\n";
      abort ();
    }
  }
}

std::string Executer::calculateHash (std::string file) const {
  FILE *fp = popen (("md5sum " + file).c_str (), "r");
  if (fp == NULL) {
    edu::sharif::twinner::util::Logger::error ()
        << "Error: Cannot execute the md5sum program.\n";
    abort ();
  }
  char md5str[33];
  if (fgets (md5str, sizeof (md5str), fp) == NULL) {
    edu::sharif::twinner::util::Logger::error ()
        << "Error: Cannot read output of the md5sum program.\n";
    abort ();
  }
  pclose (fp);
  return std::string (md5str, 32);
}

std::string Executer::calculateStringHash (std::string str) const {
  FILE *fp = popen (("echo " + escapeForShell (str) + " | md5sum").c_str (), "r");
  if (fp == NULL) {
    edu::sharif::twinner::util::Logger::error ()
        << "Error: Cannot execute the md5sum program.\n";
    abort ();
  }
  char md5str[33];
  if (fgets (md5str, sizeof (md5str), fp) == NULL) {
    edu::sharif::twinner::util::Logger::error ()
        << "Error: Cannot read output of the md5sum program.\n";
    abort ();
  }
  pclose (fp);
  return std::string (md5str, 32);
}

int createDir (const char *path) {
  if (access (path, F_OK) == 0)
    return 0;
  return mkdir (path, 0755);
}

bool Executer::recordExecutionResult (std::string inputBinaryHash,
    std::string argsHash, std::string symbolsHash, bool complete) const {
  if (createDir ((tmpfolder + "/twinner/record").c_str ()) != 0
      || createDir ((tmpfolder + "/twinner/record/"
                     + inputBinaryHash).c_str ()) != 0
      || createDir ((tmpfolder + "/twinner/record/" + inputBinaryHash
                     + "/" + argsHash).c_str ()) != 0
      || createDir ((tmpfolder + "/twinner/record/" + inputBinaryHash
                     + "/" + argsHash + "/" + symbolsHash).c_str ()) != 0) {
    edu::sharif::twinner::util::Logger::error ()
        << "Error: Cannot create record destination folders.\n";
    abort ();
  }
  if (complete) {
    if (system (("cp " + tmpfolder + EXECUTION_TRACE_COMMUNICATION_TEMP_FILE
                 + " " + tmpfolder + DISASSEMBLED_INSTRUCTIONS_MEMORY_TEMP_FILE
                 + " " + tmpfolder + MAIN_ARGS_COMMUNICATION_TEMP_FILE
                 + " " + tmpfolder + "/twinner/record/" + inputBinaryHash
                 + "/" + argsHash + "/" + symbolsHash + "/").c_str ()) != 0) {
      edu::sharif::twinner::util::Logger::error ()
          << "Error: Cannot copy the execution result files.\n";
      return false;
    }
  } else {
    if (system (("cp " + tmpfolder + EXECUTION_TRACE_COMMUNICATION_TEMP_FILE
                 + " " + tmpfolder + "/twinner/record/" + inputBinaryHash
                 + "/" + argsHash + "/" + symbolsHash + "/").c_str ()) != 0) {
      edu::sharif::twinner::util::Logger::error ()
          << "Error: Cannot copy the execution trace file.\n";
      return false;
    }
  }
  return true;
}

bool Executer::restoreExecutionResult (std::string inputBinaryHash,
    std::string argsHash, std::string symbolsHash) const {
  if (system (("cp " + tmpfolder + "/twinner/record/" + inputBinaryHash
               + "/" + argsHash + "/" + symbolsHash + "/* "
               + tmpfolder + "/twinner/").c_str ()) != 0) {
    edu::sharif::twinner::util::Logger::error ()
        << "Error: Cannot restore the execution result files.\n";
    return false;
  }
  return true;
}

}
}
}
}
