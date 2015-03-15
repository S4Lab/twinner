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

#include "Executer.h"

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>

#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/ExecutionTraceSegment.h"
#include "edu/sharif/twinner/trace/exptoken/MemoryEmergedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/RegisterEmergedSymbol.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"
#include "edu/sharif/twinner/util/MemoryManager.h"

using namespace std;

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {

inline void save_address (std::ofstream &out, const ADDRINT &address);
inline void add_symbol_to_map (
    std::map < int, std::list < edu::sharif::twinner::trace::exptoken::SymbolRecord > > &records,
    const edu::sharif::twinner::trace::exptoken::Symbol * const &symbol);
inline void save_records_list (std::ofstream &out, const int &segmentIndex,
    const std::list < edu::sharif::twinner::trace::exptoken::SymbolRecord > &recordsList);
inline void save_record (std::ofstream &out,
    const edu::sharif::twinner::trace::exptoken::SymbolRecord &record);


const char *Executer::SYMBOLS_VALUES_COMMUNICATION_TEMP_FILE = "/tmp/twinner/symbols.dat";
const char *Executer::EXECUTION_TRACE_COMMUNICATION_TEMP_FILE = "/tmp/twinner/trace.dat";
const char *Executer::DISASSEMBLED_INSTRUCTIONS_MEMORY_TEMP_FILE =
    "/tmp/twinner/memory.dat";
const char *Executer::OVERHEAD_MEASUREMENT_OPTION = " -measure";
const char *Executer::OVERHEAD_MEASUREMENT_COMMUNICATION_TEMP_FILE =
    "/tmp/twinner/measurements.dat";

Executer::Executer (std::string pinLauncher, std::string twintool,
    std::string inputBinary, std::string _inputArguments, bool main, bool _overheads) :
    baseCommand (pinLauncher
    + " -t " + twintool
    + " -symbols " + SYMBOLS_VALUES_COMMUNICATION_TEMP_FILE
    + " -trace " + EXECUTION_TRACE_COMMUNICATION_TEMP_FILE
    + " -memory " + DISASSEMBLED_INSTRUCTIONS_MEMORY_TEMP_FILE
    + " -verbose " + edu::sharif::twinner::util::Logger::getVerbosenessLevelAsString ()
    + (_overheads ? OVERHEAD_MEASUREMENT_OPTION : "")
    + (main ? " -main -- " : " -- ") + inputBinary),
    inputArguments (_inputArguments), overheads (_overheads) {
}

void Executer::setCandidateAddresses (const std::set < ADDRINT > &addresses) const {
  std::ofstream out;
  out.open (SYMBOLS_VALUES_COMMUNICATION_TEMP_FILE,
            ios_base::out | ios_base::trunc | ios_base::binary);
  if (!out.is_open ()) {
    edu::sharif::twinner::util::Logger::error () << "Can not write memory addresses"
        " to file: Error in open function: "
        << SYMBOLS_VALUES_COMMUNICATION_TEMP_FILE << '\n';
    throw std::runtime_error ("Error in saving memory addresses in binary file");
  }
  out.write ("SYM", 3);
  ExecutionMode mode = INITIAL_STATE_DETECTION_MODE;
  out.write ((const char *) &mode, sizeof (ExecutionMode));

  std::set < ADDRINT >::size_type s = addresses.size ();
  out.write ((const char *) &s, sizeof (s));

  edu::sharif::twinner::util::foreach (addresses, &save_address, out);
  out.close ();
}

void save_address (std::ofstream &out, const ADDRINT &address) {
  out.write ((const char *) &address, sizeof (address));
}

void Executer::setSymbolsValues (
    const std::set < const edu::sharif::twinner::trace::exptoken::Symbol * > &symbols) const {
  std::map < int, std::list < Record > > records;
  edu::sharif::twinner::util::foreach (symbols, &add_symbol_to_map, records);

  if (!saveSymbolRecordsToFile (NORMAL_MODE, records)) {
    throw std::runtime_error ("Error in saving symbols in binary file");
  }
}

void add_symbol_to_map (
    std::map < int, std::list < edu::sharif::twinner::trace::exptoken::SymbolRecord > > &records,
    const edu::sharif::twinner::trace::exptoken::Symbol * const &symbol) {
  std::pair < int, edu::sharif::twinner::trace::exptoken::SymbolRecord > pair =
      symbol->toSymbolRecord ();
  records[pair.first].push_back (pair.second);
}

bool Executer::saveSymbolRecordsToFile (ExecutionMode mode,
    std::map < int, std::list < Record > > records) const {
  std::ofstream out;
  out.open (SYMBOLS_VALUES_COMMUNICATION_TEMP_FILE,
            ios_base::out | ios_base::trunc | ios_base::binary);
  if (!out.is_open ()) {
    edu::sharif::twinner::util::Logger::error () << "Can not write symbols to file:"
        " Error in open function: " << SYMBOLS_VALUES_COMMUNICATION_TEMP_FILE << '\n';
    return false;
  }
  saveSymbolRecordsToBinaryStream (out, mode, records);
  out.close ();
  return true;
}

void Executer::saveSymbolRecordsToBinaryStream (std::ofstream &out,
    ExecutionMode mode, std::map < int, std::list < Record > > records) const {
  out.write ("SYM", 3);
  out.write ((const char *) &mode, sizeof (ExecutionMode));

  typename std::map < int, std::list < Record > >::size_type s = records.size ();
  out.write ((const char *) &s, sizeof (s));

  edu::sharif::twinner::util::foreach (records, &save_records_list, out);
}

void save_records_list (std::ofstream &out, const int &segmentIndex,
    const std::list < edu::sharif::twinner::trace::exptoken::SymbolRecord > &recordsList) {
  out.write ((const char *) &segmentIndex, sizeof (segmentIndex));
  std::list < edu::sharif::twinner::trace::exptoken::SymbolRecord >::size_type s =
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
    const std::list < const edu::sharif::twinner::trace::Constraint * > &constraints,
    std::map < ADDRINT, const edu::sharif::twinner::trace::exptoken::Symbol * > &msyms,
    std::map < REG, const edu::sharif::twinner::trace::exptoken::Symbol * > &rsyms) {
  for (std::list < const edu::sharif::twinner::trace::Constraint * >
      ::const_iterator it = constraints.begin (); it != constraints.end (); ++it) {
    const edu::sharif::twinner::trace::Constraint *constraint = *it;
    aggregate_symbols (constraint->getMainExpression (), msyms, rsyms);
    aggregate_symbols (constraint->getAuxExpression (), msyms, rsyms);
  }
}

/**
 * Uses OS interface to run  twintool in an independent process. The execution
 * trace will be communicated with twintool through file interface.
 * @return The execution trace, recorded by twintool
 */
edu::sharif::twinner::trace::Trace *
Executer::executeSingleTraceInNormalMode () const {
  /*
   *  TODO: Run command through another thread and set a timeout for execution.
   *  Also tune twintool, so it saves its progress incrementally. After a timeout,
   *  partial trace can be read and twintool will be killed. Malwares are not
   *  required to finish execution! Another possible approach is to tune twintool
   *  to timeout execution and exit after a while. In this way, this code does not
   *  need to be changed at all.
   */
  const std::string command = baseCommand + " " + inputArguments;
  if (overheads) {
    std::string cmd = command;
    cmd.erase (cmd.find (OVERHEAD_MEASUREMENT_OPTION),
               strlen (OVERHEAD_MEASUREMENT_OPTION));
    Measurement ourMeasure, baselineMeasure;
    edu::sharif::twinner::trace::Trace *trace = executeSystemCommand (cmd, ourMeasure);
    const std::list < edu::sharif::twinner::trace::ExecutionTraceSegment * > &segments =
        trace->getTraceSegments ();
    std::map < ADDRINT, const edu::sharif::twinner::trace::exptoken::Symbol * > msyms;
    std::map < REG, const edu::sharif::twinner::trace::exptoken::Symbol * > rsyms;
    for (std::list < edu::sharif::twinner::trace::ExecutionTraceSegment * >
        ::const_iterator it = segments.begin (); it != segments.end (); ++it) {
      const edu::sharif::twinner::trace::ExecutionTraceSegment *segment = *it;
      aggregate_symbols (segment->getMemoryAddressTo64BitsExpression (), msyms, rsyms);
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
    executeSystemCommand (command, baselineMeasure);
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

edu::sharif::twinner::trace::Trace *
Executer::executeSystemCommand (std::string command) const {
  edu::sharif::twinner::util::Logger::debug ()
      << "Calling system (\"" << command << "\");\n";
  int ret = system (command.c_str ());
  edu::sharif::twinner::util::Logger::debug ()
      << "The system(...) call returns code: " << ret << '\n';
  return edu::sharif::twinner::trace::Trace::loadFromFile
      (EXECUTION_TRACE_COMMUNICATION_TEMP_FILE,
       DISASSEMBLED_INSTRUCTIONS_MEMORY_TEMP_FILE);
}

edu::sharif::twinner::trace::Trace *
Executer::executeSystemCommand (std::string command, Measurement &measurement) const {
  edu::sharif::twinner::util::Logger::debug ()
      << "Calling system (\"" << command << "\");\n";
  edu::sharif::twinner::trace::Trace *trace = executeAndMeasure (command, measurement);
  edu::sharif::twinner::util::Logger::debug ()
      << "The system(...) call returns code: " << measurement.ret << '\n';
  return trace;
}

edu::sharif::twinner::trace::Trace *
Executer::executeAndMeasure (std::string command, Measurement &m) const {
  pid_t childPid = fork ();
  if (childPid < 0) {
    edu::sharif::twinner::util::Logger::error () << "Cannot fork!\n";
    abort ();
  } else if (childPid == 0) { // executed in the child process
    int ret = system (command.c_str ());
    Measurement measurement = measureCurrentState (ret);
    std::ofstream out;
    const char *path = OVERHEAD_MEASUREMENT_COMMUNICATION_TEMP_FILE;
    out.open (path, ios_base::out | ios_base::trunc | ios_base::binary);
    if (!out.is_open ()) {
      edu::sharif::twinner::util::Logger::error () << "Can not write measurement info:"
          " Error in open function: " << path << '\n';
    } else {
      out.write ((const char *) &measurement, sizeof (measurement));
      out.close ();
    }
    exit (ret);

  } else { // executed in the parent process
    int status;
    if (childPid != waitpid (childPid, &status, 0)) {
      edu::sharif::twinner::util::Logger::warning () << "Error while waiting for child\n";
    }
    std::ifstream in;
    const char *path = OVERHEAD_MEASUREMENT_COMMUNICATION_TEMP_FILE;
    in.open (path, ios_base::in | ios_base::binary);
    if (!in.is_open ()) {
      edu::sharif::twinner::util::Logger::error () << "Can not read measurement info:"
          " Error in open function: " << path << '\n';
    } else {
      in.read ((char *) &m, sizeof (m));
      in.close ();
      if (m.ret != status) {
        edu::sharif::twinner::util::Logger::error ()
            << "Measurement info are inconsistent\n";
      }
    }
  }
  return edu::sharif::twinner::trace::Trace::loadFromFile
      (EXECUTION_TRACE_COMMUNICATION_TEMP_FILE,
       DISASSEMBLED_INSTRUCTIONS_MEMORY_TEMP_FILE);
}

UINT64 operator+ (struct timeval a, struct timeval b) {
  return (a.tv_sec + b.tv_sec) * 1000 * 1000 + (a.tv_usec + b.tv_usec);
}

Executer::Measurement Executer::measureCurrentState (int ret) const {
  Measurement measurement;
  measurement.ret = ret;
  struct rusage usage;
  if (getrusage (RUSAGE_CHILDREN, &usage) != 0) {
    edu::sharif::twinner::util::Logger::error () << "Error in getrusage()\n";
  } else {
    measurement.cputime = usage.ru_utime + usage.ru_stime;
    measurement.mss = usage.ru_maxrss;
  }
  return measurement;
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

map < ADDRINT, UINT64 >
Executer::executeSingleTraceInInitialStateDetectionMode () const {
  std::string command = baseCommand + " " + inputArguments;
  if (overheads) {
    command.erase (command.find (OVERHEAD_MEASUREMENT_OPTION),
                   strlen (OVERHEAD_MEASUREMENT_OPTION));
  }
  edu::sharif::twinner::util::Logger::debug ()
      << "Calling system (\"" << command << "\");\n";
  int ret = system (command.c_str ());
  edu::sharif::twinner::util::Logger::debug ()
      << "The system(...) call returns code: " << ret << '\n';
  return edu::sharif::twinner::trace::Trace::loadAddressToValueMapFromFile
      (EXECUTION_TRACE_COMMUNICATION_TEMP_FILE);
}

}
}
}
}
