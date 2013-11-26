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

#include "Twinner.h"

#include <iostream>
#include <stdexcept>
#include <sstream>

#include "Executer.h"

#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/ExecutionTraceSegment.h"
#include "edu/sharif/twinner/trace/MemoryEmergedSymbol.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"

using namespace std;

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {

struct TwinCodeGenerationAux {

  int depth;
  std::stringstream &out;
};

inline void code_trace_into_twin_code (std::stringstream &out,
    const edu::sharif::twinner::trace::Trace * const &trace);
inline void code_segment_into_twin_code (std::stringstream &out,
    edu::sharif::twinner::trace::ExecutionTraceSegment * const &segment);
inline void code_constraint_into_twin_code (TwinCodeGenerationAux &aux,
    const edu::sharif::twinner::trace::Constraint * const &constraint);

inline void delete_symbol (
    const edu::sharif::twinner::trace::MemoryEmergedSymbol * const &symbol);

template < typename Key, typename Value >
inline std::set < Value > getValuesSet (const std::map < Key, Value > &map);
template < typename Key, typename Value >
inline void add_value_to_set (std::set < Value > &set,
    const Key &key, const Value &value);

Twinner::Twinner () {
}

void Twinner::setInputBinaryPath (string input) {
  this->input = input;
}

void Twinner::setTwinToolPath (string twintool) {
  this->twintool = twintool;
}

void Twinner::setPinLauncherPath (string pin) {
  this->pin = pin;
}

void Twinner::setTwinBinaryPath (string twin) {
  this->twin = twin;
}

void Twinner::setInputBinaryArguments (string arguments) {
  this->arguments = arguments;
}

/**
 * Generates twin code and twin binary.
 * Steps:
 * 1. Without any special initialization (just giving as much arguments as possible,
 * to the program; indeed we do not need to reach even near max limits, just we must be
 * sure to use a lot of bytes to find places whom are used by user input to ignore
 * initializing them while generating the twin code), run program and obtain first trace,
 * 2. With completely different arguments, run program in change-detection mode for the
 * second trace,
 * 3. Find all symbols who change their initial values and mark them as user inputs (do
 * not store 2nd trace as it has no info else of position of user inputs),
 * 4. While generating twin code, initialize all symbols who were visited in first trace
 * segment, else of user inputs (user inputs and next trace segments' symbols remain
 * uninitialized to obtain and keep their values from user and syscalls),
 * 5. Since now (3rd execution and so on) use DFS or another search strategy to obtain a
 * set of new values for symbols to follow a new execution path and run program in
 * initialized mode (which despite change-detection mode, enforces symbols values)
 * with those symbols' values (initializing user inputs is not required when generating
 * twin code, but they should be initialized while running program as different execution
 * traces may be obtained with different user inputs),
 * 6. By each run, some new (surely non-user-input) symbols may be added to first
 * segment. If there was a symbol (at an address) in 2nd or later segments while there was
 * no symbols at that address in 1st segment, it should be added to first segment symbols
 * too (to have it initialized in the final twin code by its pre-syscall value),
 * 7. To obtain step-6's values, run program again (just one time for all symbols who are
 * tracked) at the end in initial-state-detection mode and look for values of those
 * symbols before first syscall. It's possible that a later trace, removes a symbol from
 * the set of non-initialized-in-1st-segment symbols. So search for those symbols just at
 * the end and then do that last/final run to look for their pre-1st-syscall values.
 */
void Twinner::generateTwinBinary () {
  Executer ex (pin, twintool, input, arguments);
  set < const edu::sharif::twinner::trace::MemoryEmergedSymbol * > symbols;
  bool somePathsAreNotCovered = true;
  int i = 1;
  while (somePathsAreNotCovered) {
    edu::sharif::twinner::util::Logger::debug () << "Executing trace # " << i++ << '\n';
    ex.setSymbolsValues (Executer::INITIALIZED_MODE, symbols);
    edu::sharif::twinner::trace::Trace *trace = ex.executeSingleTraceInInitializedMode ();

    edu::sharif::twinner::util::ForEach
        < int, const edu::sharif::twinner::trace::MemoryEmergedSymbol * >
        ::iterate (symbols, &delete_symbol);
    symbols.clear ();
    addExecutionTrace (trace);
    if (userInputAddresses.empty ()) { // step 2
      ex.setSymbolsValues (Executer::CHANGE_DETECTION_MODE,
                           getValuesSet (firstSegmentSymbols));
      userInputAddresses = ex.executeSingleTraceInChangeDetectionMode ();
    }
    // step 5
    // symbols will be filled with newly instantiated objects and should be deleted...
    somePathsAreNotCovered = calculateSymbolsValuesForCoveringNextPath (*trace, symbols);
  }
  // step 7
  // symbols are pointing to traces' symbols and should not be deleted
  symbols = retrieveSymbolsWithoutValueInFirstSegment ();
  ex.setSymbolsValues (Executer::INITIAL_STATE_DETECTION_MODE, symbols);
  symbols = ex.executeSingleTraceInInitialStateDetectionMode ();
  addToFirstSegmentSymbols (symbols);
  codeTracesIntoTwinBinary ();
}

void delete_symbol (
    const edu::sharif::twinner::trace::MemoryEmergedSymbol * const &symbol) {
  delete symbol;
}

void Twinner::addExecutionTrace (const edu::sharif::twinner::trace::Trace *trace) {
  edu::sharif::twinner::util::Logger log = edu::sharif::twinner::util::Logger::debug ();
  log << "Adding execution trace:\n";
  trace->printCompleteState (log);
  traces.push_back (trace);
  // TODO: Update firstSegmentSymbols field...
}

set < const edu::sharif::twinner::trace::MemoryEmergedSymbol * >
Twinner::retrieveSymbolsWithoutValueInFirstSegment () const {
  throw std::runtime_error
      ("Twinner::retrieveSymbolsWithoutValueInFirstSegment (): Not yet implemented");
}

bool Twinner::calculateSymbolsValuesForCoveringNextPath (
    const edu::sharif::twinner::trace::Trace &trace,
    set < const edu::sharif::twinner::trace::MemoryEmergedSymbol * > &symbols) {
  return false;
}

void Twinner::addToFirstSegmentSymbols (
    const set < const edu::sharif::twinner::trace::MemoryEmergedSymbol * > &symbols) {
  throw std::runtime_error
      ("Twinner::addToFirstSegmentSymbols (): Not yet implemented");
}

void Twinner::codeTracesIntoTwinBinary () {
  std::stringstream out;
  out << '\n';

  edu::sharif::twinner::util::ForEach
      < int, const edu::sharif::twinner::trace::Trace *, std::stringstream >
      ::iterate (traces, &code_trace_into_twin_code, out);
  edu::sharif::twinner::util::Logger::info () << out.str ();
}

void code_trace_into_twin_code (std::stringstream &out,
    const edu::sharif::twinner::trace::Trace * const &trace) {
  edu::sharif::twinner::util::ForEach
      < int, edu::sharif::twinner::trace::ExecutionTraceSegment *, std::stringstream >
      ::iterate (trace->getTraceSegments (), &code_segment_into_twin_code, out);
}

void code_segment_into_twin_code (std::stringstream &out,
    edu::sharif::twinner::trace::ExecutionTraceSegment * const &segment) {
  TwinCodeGenerationAux aux = {1, out};
  edu::sharif::twinner::util::ForEach
      < int, const edu::sharif::twinner::trace::Constraint *, TwinCodeGenerationAux >
      ::iterate (segment->getPathConstraints (), &code_constraint_into_twin_code, aux);
  for (int j = aux.depth - 1; j > 0; --j) {

    repeat (j) {
      out << '\t';
    }
    out << "}\n";
  }
}

void code_constraint_into_twin_code (TwinCodeGenerationAux &aux,
    const edu::sharif::twinner::trace::Constraint * const &constraint) {

  repeat (aux.depth) {
    aux.out << '\t';
  }
  aux.out << "if (" << constraint->toString () << ") {\n";
  aux.depth++;
}

template < typename Key, typename Value >
std::set < Value > getValuesSet (const std::map < Key, Value > &map) {
  std::set < Value > set;
  edu::sharif::twinner::util::ForEach
      < Key, Value, std::set < Value > >
      ::iterate (map, &add_value_to_set, set);

  return set;
}

template < typename Key, typename Value >
void add_value_to_set (std::set < Value > &set,
    const Key &key, const Value &value) {
  set.insert (value);
}


}
namespace trace {

/*
 * Following functions implementations are specific to Twinner.
 * Similar but more advanced implementations are provided for TwinTool.
 */

void throw_exception_about_unexpected_change_in_memory_or_register_address
(REG reg, UINT64 expectedVal, UINT64 currentVal) {
  char errorMessage[200];
  snprintf (errorMessage, 200, "Value of an address changed unexpectedly"
            " without any interfering syscall\n"
            "\tExpected 0x%lX, Got 0x%lX; at register 0x%X",
            expectedVal, currentVal, (unsigned int) reg);
  throw std::runtime_error (errorMessage);
}

void throw_exception_about_unexpected_change_in_memory_or_register_address
(ADDRINT address, UINT64 expectedVal, UINT64 currentVal) {
  char errorMessage[200];
  snprintf (errorMessage, 200, "Value of an address changed unexpectedly"
            " without any interfering syscall\n"
            "\tExpected 0x%lX, Got 0x%lX; at address 0x%lX",
            expectedVal, currentVal, address);
  throw std::runtime_error (errorMessage);
}

}
namespace util {

const Logger &operator<< (const Logger &logger, REG reg) {
  return logger << (UINT32) reg;
}

}
}
}
}
