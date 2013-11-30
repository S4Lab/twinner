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

  unsigned int depth;
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
inline std::set < Value > get_values_set (const std::map < Key, Value > &map);
template < typename Key, typename Value >
inline void add_value_to_set (std::set < Value > &set,
    const Key &key, const Value &value);

inline void extract_memory_addresses_of_trace_segment (std::set < ADDRINT > &addresses,
    edu::sharif::twinner::trace::ExecutionTraceSegment * const &segment);
inline void extract_memory_addresses_of_constraint (std::set < ADDRINT > &addresses,
    const edu::sharif::twinner::trace::Constraint * const &constraint);
template < typename Key >
inline void extract_memory_addresses_of_expression (std::set < ADDRINT > &addresses,
    const Key &key, edu::sharif::twinner::trace::Expression * const &exp);
inline void extract_memory_addresses_of_expression (std::set < ADDRINT > &addresses,
    const edu::sharif::twinner::trace::Expression *exp);
inline void check_symbol_type_and_add_it_to_set (std::set < ADDRINT > &addresses,
    edu::sharif::twinner::trace::ExpressionToken * const &token);

inline void remove_mismatches_from_map (std::map < ADDRINT, UINT64 > &initialValues,
    const ADDRINT &address, const UINT64 &content);
inline void code_initial_value_into_twin_code (std::stringstream &out,
    const ADDRINT &address, const UINT64 &content);

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
 * 1. With given arguments, run the program and collect its trace, updating the CFG,
 * 2. Solve constraints of last trace, traversing the CFG (e.g. using DFS), and find
 * a set of symbols' values which enforce a new execution path. While finding this set
 * of values, only symbols which are participating in corresponding constraints will
 * be considered. Thus all valued symbols will be seen by TwinTool in next run for sure
 * and the expected path will be traversed (of course some of old visited symbols, which
 * were ignored as they were not participating in the newly followed path of execution,
 * may be revisited in the next run),
 * 3. Continue collecting new traces by repeating step 2 until there was no remaining
 * untraversed execution path in the discovered CFG,
 * 4. At the end, all addresses which were holding some memory emerged symbol (found by
 * looking in the registers/memory addresses/constraints expressions) should be collected
 * as candidates to be initialized at the beginning of the Twin code,
 * 5. Pass all of step 4's set of addresses and collect their initial values before the
 * first syscall,
 * 6. Change argument of the program and repeat step 5 with new arguments,
 * 7. Remove all addresses who have different values between two runs of steps 5 and 6
 * and initialize remaining addresses in the Twin code.
 */
void Twinner::generateTwinBinary () {
  Executer ex (pin, twintool, input, arguments);
  set < const edu::sharif::twinner::trace::MemoryEmergedSymbol * > symbols;
  bool somePathsAreNotCovered = true;
  int i = 1;
  while (somePathsAreNotCovered) {
    edu::sharif::twinner::util::Logger::debug () << "Executing trace # " << i++ << '\n';
    // steps 1, 2, and 3
    ex.setSymbolsValues (symbols);
    edu::sharif::twinner::trace::Trace *trace = ex.executeSingleTraceInNormalMode ();

    edu::sharif::twinner::util::ForEach
        < int, const edu::sharif::twinner::trace::MemoryEmergedSymbol * >
        ::iterate (symbols, &delete_symbol);
    symbols.clear ();
    addExecutionTrace (trace);
    // symbols will be filled with newly instantiated objects and should be deleted...
    somePathsAreNotCovered = calculateSymbolsValuesForCoveringNextPath (*trace, symbols);
  }
  edu::sharif::twinner::util::ForEach
      < int, const edu::sharif::twinner::trace::MemoryEmergedSymbol * >
      ::iterate (symbols, &delete_symbol);
  symbols.clear ();
  std::map < ADDRINT, UINT64 > initialValues = obtainInitializedMemoryValues (ex);
  codeTracesIntoTwinBinary (initialValues);
}

std::map < ADDRINT, UINT64 > Twinner::obtainInitializedMemoryValues (Executer &ex) const {
  // step 4: `addresses` field holds candidate addresses
  ex.setCandidateAddresses (addresses);
  // step 5
  std::map < ADDRINT, UINT64 > initialValues1 =
      ex.executeSingleTraceInInitialStateDetectionMode ();
  // step 6
  ex.changeArguments ();
  std::map < ADDRINT, UINT64 > initialValues2 =
      ex.executeSingleTraceInInitialStateDetectionMode ();
  // step 7
  edu::sharif::twinner::util::ForEach
      < ADDRINT, UINT64, std::map < ADDRINT, UINT64 > >
      ::iterate (initialValues2, &remove_mismatches_from_map, initialValues1);
  return initialValues1;
}

void remove_mismatches_from_map (std::map < ADDRINT, UINT64 > &initialValues,
    const ADDRINT &address, const UINT64 &content) {
  std::map < ADDRINT, UINT64 >::iterator it = initialValues.find (address);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "at address: 0x" << std::hex << address
      << "\tfirst map: 0x" << it->second
      << "\tsecond map: 0x" << content << "\n";
  if (it->second != content) {
    initialValues.erase (it);
  }
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
  edu::sharif::twinner::util::ForEach
      < int, edu::sharif::twinner::trace::ExecutionTraceSegment *, std::set < ADDRINT > >
      ::iterate (trace->getTraceSegments (), &extract_memory_addresses_of_trace_segment,
                 addresses);
  log << "Done.\n";
}

void extract_memory_addresses_of_trace_segment (std::set < ADDRINT > &addresses,
    edu::sharif::twinner::trace::ExecutionTraceSegment * const &segment) {
  edu::sharif::twinner::util::ForEach
      < ADDRINT, edu::sharif::twinner::trace::Expression *, std::set < ADDRINT > >
      ::iterate (segment->getMemoryAddressToExpression (),
                 &extract_memory_addresses_of_expression, addresses);
  edu::sharif::twinner::util::ForEach
      < REG, edu::sharif::twinner::trace::Expression *, std::set < ADDRINT > >
      ::iterate (segment->getRegisterToExpression (),
                 &extract_memory_addresses_of_expression, addresses);
  edu::sharif::twinner::util::ForEach
      < int, const edu::sharif::twinner::trace::Constraint *, std::set < ADDRINT > >
      ::iterate (segment->getPathConstraints (),
                 &extract_memory_addresses_of_constraint, addresses);
}

void extract_memory_addresses_of_constraint (std::set < ADDRINT > &addresses,
    const edu::sharif::twinner::trace::Constraint * const &constraint) {
  extract_memory_addresses_of_expression (addresses, constraint->getExpression ());
}

template < typename Key >
void extract_memory_addresses_of_expression (std::set < ADDRINT > &addresses,
    const Key &key, edu::sharif::twinner::trace::Expression * const &exp) {
  extract_memory_addresses_of_expression (addresses, exp);
}

void extract_memory_addresses_of_expression (std::set < ADDRINT > &addresses,
    const edu::sharif::twinner::trace::Expression *exp) {
  edu::sharif::twinner::util::ForEach
      < int, edu::sharif::twinner::trace::ExpressionToken *, std::set < ADDRINT > >
      ::iterate (exp->getStack (), &check_symbol_type_and_add_it_to_set, addresses);
}

void check_symbol_type_and_add_it_to_set (std::set < ADDRINT > &addresses,
    edu::sharif::twinner::trace::ExpressionToken * const &token) {
  const edu::sharif::twinner::trace::MemoryEmergedSymbol *symbol =
      dynamic_cast<edu::sharif::twinner::trace::MemoryEmergedSymbol *> (token);
  if (symbol) {
    addresses.insert (symbol->getAddress ());
  }
}

bool Twinner::calculateSymbolsValuesForCoveringNextPath (
    const edu::sharif::twinner::trace::Trace &trace,
    set < const edu::sharif::twinner::trace::MemoryEmergedSymbol * > &symbols) {
  return false;
}

void Twinner::codeTracesIntoTwinBinary (
    const std::map < ADDRINT, UINT64 > &initialValues) {
  std::stringstream out;
  out << '\n';
  codeInitialValuesIntoTwinCode (out, initialValues);
  edu::sharif::twinner::util::ForEach
      < int, const edu::sharif::twinner::trace::Trace *, std::stringstream >
      ::iterate (traces, &code_trace_into_twin_code, out);
  edu::sharif::twinner::util::Logger::info () << out.str ();
}

void Twinner::codeInitialValuesIntoTwinCode (std::stringstream &out,
    const std::map < ADDRINT, UINT64 > &initialValues) const {
  edu::sharif::twinner::util::ForEach
      < ADDRINT, UINT64, std::stringstream >
      ::iterate (initialValues, &code_initial_value_into_twin_code, out);
}

void code_initial_value_into_twin_code (std::stringstream &out,
    const ADDRINT &address, const UINT64 &content) {
  out << "UINT64 &m" << std::hex << address << " = *((UINT64 *) 0x" << address << ");\n"
      << "m" << address << " = 0x" << content << ";\n";
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
  for (unsigned int j = aux.depth - 1; j > 0; --j) {

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
std::set < Value > get_values_set (const std::map < Key, Value > &map) {
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

UINT64 readRegisterContent (const CONTEXT *context, REG reg) {
  const char *msg = "The readRegisterContent(...) method is only supported in TwinTool:"
      " Calling error from Twinner.\n";
  edu::sharif::twinner::util::Logger::error () << msg;
  throw std::runtime_error (msg);
}

UINT64 readMemoryContent (ADDRINT memoryEa) {
  const char *msg = "The readMemoryContent(...) method is only supported in TwinTool:"
      " Calling error from Twinner.\n";
  edu::sharif::twinner::util::Logger::error () << msg;
  throw std::runtime_error (msg);
}

VOID writeMemoryContent (ADDRINT memoryEa, UINT64 value) {
  const char *msg = "The writeMemoryContent(...) method is only supported in TwinTool:"
      " Calling error from Twinner.\n";
  edu::sharif::twinner::util::Logger::error () << msg;
  throw std::runtime_error (msg);
}

}
}
}
}
