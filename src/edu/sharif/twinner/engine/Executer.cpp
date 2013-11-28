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

#include "Executer.h"

#include <stdlib.h>
#include <iostream>

#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/MemoryEmergedSymbol.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"

using namespace std;

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {

inline void save_address (std::ofstream &out, const ADDRINT &address);
inline void add_symbol_to_map (
    std::map < int, std::list < edu::sharif::twinner::trace::SymbolRecord > > &records,
    const edu::sharif::twinner::trace::MemoryEmergedSymbol * const &symbol);
inline void save_records_list (std::ofstream &out, const int &segmentIndex,
    const std::list < edu::sharif::twinner::trace::SymbolRecord > &recordsList);
inline void save_record (std::ofstream &out,
    const edu::sharif::twinner::trace::SymbolRecord &record);


const char *Executer::SYMBOLS_VALUES_COMMUNICATION_TEMP_FILE = "/tmp/twinner/symbols.dat";
const char *Executer::EXECUTION_TRACE_COMMUNICATION_TEMP_FILE = "/tmp/twinner/trace.dat";

Executer::Executer (std::string pinLauncher, std::string twintool,
    std::string inputBinary, std::string _inputArguments) :
baseCommand (pinLauncher
+ " -t " + twintool
+ " -symbols " + SYMBOLS_VALUES_COMMUNICATION_TEMP_FILE
+ " -trace " + EXECUTION_TRACE_COMMUNICATION_TEMP_FILE
+ " -verbose " + edu::sharif::twinner::util::Logger::getVerbosenessLevelAsString ()
+ " -main -- " + inputBinary),
inputArguments (_inputArguments) {
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

  typename std::set < ADDRINT >::size_type s = addresses.size ();
  out.write ((const char *) &s, sizeof (s));

  edu::sharif::twinner::util::ForEach
      < int, ADDRINT, std::ofstream >
      ::iterate (addresses, &save_address, out);
  out.close ();
}

void save_address (std::ofstream &out, const ADDRINT &address) {
  out.write ((const char *) &address, sizeof (address));
}

void Executer::setSymbolsValues (
    const std::set < const edu::sharif::twinner::trace::MemoryEmergedSymbol * > &
    symbols) const {
  std::map < int, std::list < Record > > records;
  edu::sharif::twinner::util::ForEach
      < int, const edu::sharif::twinner::trace::MemoryEmergedSymbol *,
      std::map < int, std::list < Record > > >
      ::iterate (symbols, &add_symbol_to_map, records);

  if (!saveSymbolRecordsToFile (NORMAL_MODE, records)) {
    throw std::runtime_error ("Error in saving symbols in binary file");
  }
}

void add_symbol_to_map (
    std::map < int, std::list < edu::sharif::twinner::trace::SymbolRecord > > &records,
    const edu::sharif::twinner::trace::MemoryEmergedSymbol * const &symbol) {
  std::pair < int, edu::sharif::twinner::trace::SymbolRecord > pair =
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

  edu::sharif::twinner::util::ForEach
      < int, std::list < Record >, std::ofstream >
      ::iterate (records, &save_records_list, out);
}

void save_records_list (std::ofstream &out, const int &segmentIndex,
    const std::list < edu::sharif::twinner::trace::SymbolRecord > &recordsList) {
  out.write ((const char *) &segmentIndex, sizeof (segmentIndex));
  std::list < edu::sharif::twinner::trace::SymbolRecord >::size_type s =
      recordsList.size ();
  out.write ((const char *) &s, sizeof (s));
  edu::sharif::twinner::util::ForEach
      < int, edu::sharif::twinner::trace::SymbolRecord, std::ofstream >
      ::iterate (recordsList, &save_record, out);
}

void save_record (std::ofstream &out,
    const edu::sharif::twinner::trace::SymbolRecord &record) {
  out.write ((const char *) &record.address, sizeof (record.address));
  out.write ((const char *) &record.concreteValue, sizeof (record.concreteValue));
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
  edu::sharif::twinner::util::Logger::debug ()
      << "Calling system (\"" << command << "\");\n";
  int ret = system (command.c_str ());
  edu::sharif::twinner::util::Logger::debug ()
      << "The system(...) call returns code: " << ret << '\n';
  return edu::sharif::twinner::trace::Trace::loadFromFile
      (EXECUTION_TRACE_COMMUNICATION_TEMP_FILE);
}

void Executer::changeArguments () {
  const char * const str = inputArguments.c_str ();
  char * const buffer = new char [inputArguments.size () + 52];
  *buffer = ' ';
  const char *lastArg = buffer;
  const char *args = str;
  char *ptr = buffer + 1;
  for (; *args; ++args, ++ptr) {
    if (*args == ' ') {
      lastArg = ptr;
      *ptr = ' ';
    } else {
      *ptr = (*args) + 1;
    }
  }
  const char * const end = ptr;
  for (int i = 0; lastArg < end && i < 50; ++lastArg, ++ptr, ++i) {
    *ptr = (*lastArg) + 1;
  }
  *ptr = '\0';
  inputArguments = std::string (buffer + 1); // ignoring the first space character
}

map < ADDRINT, UINT64 >
Executer::executeSingleTraceInInitialStateDetectionMode () const {
  const std::string command = baseCommand + " " + inputArguments;
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
