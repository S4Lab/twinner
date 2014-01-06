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

#include "edu/sharif/twinner/engine/smt/SmtSolver.h"
#include "smt/Cvc4SmtSolver.h"

using namespace std;

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {

struct TwinCodeGenerationAux {

  unsigned int depth;
  std::stringstream &out;
};

class IndentedStringStream : public std::stringstream {

 private:
  const std::string indentation;

 public:

  IndentedStringStream (const std::string &_indentation) :
      indentation (_indentation) {
  }

  IndentedStringStream &indented () {
    (*this) << (indentation);
    return *this;
  }
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

inline void gather_constraints_of_trace_segment (
    std::list < const edu::sharif::twinner::trace::Constraint * > &constraints,
    edu::sharif::twinner::trace::ExecutionTraceSegment * const &segment);

inline void code_memory_symbolic_changes_of_one_segment (IndentedStringStream &out,
    const edu::sharif::twinner::trace::ExecutionTraceSegment *segment);
inline void code_registers_symbolic_changes_of_one_segment (IndentedStringStream &out,
    const edu::sharif::twinner::trace::ExecutionTraceSegment *segment);

inline void code_symbol_initiation_into_twin_code (std::stringstream &out,
    const ADDRINT &address);
inline void code_memory_changes (IndentedStringStream &out,
    const ADDRINT &memoryEa, edu::sharif::twinner::trace::Expression * const &exp);

Twinner::Twinner () {
  edu::sharif::twinner::engine::smt::SmtSolver::init
      (new edu::sharif::twinner::engine::smt::Cvc4SmtSolver ());
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

    edu::sharif::twinner::util::foreach (symbols, &delete_symbol);
    symbols.clear ();
    addExecutionTrace (trace);
    // symbols will be filled with newly instantiated objects and should be deleted...
    somePathsAreNotCovered = calculateSymbolsValuesForCoveringNextPath (symbols);
  }
  edu::sharif::twinner::util::foreach (symbols, &delete_symbol);
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
  edu::sharif::twinner::util::foreach (initialValues2,
                                       &remove_mismatches_from_map, initialValues1);
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
  //TODO: Add (refactor out of this class) constraints to a search strategy class
  constraints.clear ();
  edu::sharif::twinner::util::foreach (trace->getTraceSegments (),
                                       &gather_constraints_of_trace_segment,
                                       constraints);
  edu::sharif::twinner::util::foreach (trace->getTraceSegments (),
                                       &extract_memory_addresses_of_trace_segment,
                                       addresses);
  log << "Done.\n";
}

void gather_constraints_of_trace_segment (
    std::list < const edu::sharif::twinner::trace::Constraint * > &constraints,
    edu::sharif::twinner::trace::ExecutionTraceSegment * const &segment) {
  constraints.insert (constraints.end (),
                      segment->getPathConstraints ().begin (),
                      segment->getPathConstraints ().end ());
}

void extract_memory_addresses_of_trace_segment (std::set < ADDRINT > &addresses,
    edu::sharif::twinner::trace::ExecutionTraceSegment * const &segment) {
  edu::sharif::twinner::util::foreach (segment->getMemoryAddressToExpression (),
                                       &extract_memory_addresses_of_expression,
                                       addresses);
  edu::sharif::twinner::util::foreach (segment->getRegisterToExpression (),
                                       &extract_memory_addresses_of_expression,
                                       addresses);
  edu::sharif::twinner::util::foreach (segment->getPathConstraints (),
                                       &extract_memory_addresses_of_constraint,
                                       addresses);
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
  edu::sharif::twinner::util::foreach (exp->getStack (),
                                       &check_symbol_type_and_add_it_to_set, addresses);
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
    set < const edu::sharif::twinner::trace::MemoryEmergedSymbol * > &symbols) {
  static bool first = true;
  if (first) {
    first = false;
    const edu::sharif::twinner::trace::Constraint *cns = constraints.back ();
    edu::sharif::twinner::trace::Constraint *cc = cns->instantiateNegatedConstraint ();
    constraints.pop_back ();
    constraints.push_back (cc);
    try {
      edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()->solveConstraints
          (constraints, symbols);
      return true;
    } catch (
        const edu::sharif::twinner::engine::smt::UnsatisfiableConstraintsException &e) {
    }
  }
  return false;
}

void Twinner::codeTracesIntoTwinBinary (
    const std::map < ADDRINT, UINT64 > &initialValues) {
  std::stringstream out;
  out << '\n';
  edu::sharif::twinner::util::foreach (addresses,
                                       &code_symbol_initiation_into_twin_code, out);
  codeInitialValuesIntoTwinCode (out, initialValues);
  edu::sharif::twinner::util::foreach (traces, &code_trace_into_twin_code, out);
  edu::sharif::twinner::util::Logger::info () << out.str ();
}

void code_symbol_initiation_into_twin_code (std::stringstream &out,
    const ADDRINT &address) {
  out << "const UINT64 m" << std::hex << address
      << "_0 = *((UINT64 *) 0x" << address << ");\n";
}

void Twinner::codeInitialValuesIntoTwinCode (std::stringstream &out,
    const std::map < ADDRINT, UINT64 > &initialValues) const {
  edu::sharif::twinner::util::foreach (initialValues,
                                       &code_initial_value_into_twin_code, out);
}

void code_initial_value_into_twin_code (std::stringstream &out,
    const ADDRINT &address, const UINT64 &content) {
  out << "UINT64 &m" << std::hex << address << " = *((UINT64 *) 0x" << address << ");\n"
      << "m" << address << " = 0x" << content << ";\n";
}

void code_trace_into_twin_code (std::stringstream &out,
    const edu::sharif::twinner::trace::Trace * const &trace) {
  edu::sharif::twinner::util::foreach (trace->getTraceSegments (),
                                       &code_segment_into_twin_code, out);
}

void code_segment_into_twin_code (std::stringstream &out,
    edu::sharif::twinner::trace::ExecutionTraceSegment * const &segment) {
  TwinCodeGenerationAux aux = {1, out};
  edu::sharif::twinner::util::foreach (segment->getPathConstraints (),
                                       &code_constraint_into_twin_code, aux);
  std::stringstream indentation;

  repeat (aux.depth) {
    indentation << '\t';
  }
  IndentedStringStream iss (indentation.str ());
  code_memory_symbolic_changes_of_one_segment (iss, segment);
  code_registers_symbolic_changes_of_one_segment (iss, segment);
  out << iss.str ();
  for (unsigned int j = aux.depth - 1; j > 0; --j) {

    repeat (j) {
      out << '\t';
    }
    out << "}\n";
  }
}

void code_memory_symbolic_changes_of_one_segment (IndentedStringStream &out,
    const edu::sharif::twinner::trace::ExecutionTraceSegment *segment) {
  out.indented () << "/*Memory Changes*/\n";
  const std::map < ADDRINT, edu::sharif::twinner::trace::Expression * > &memToExp =
      segment->getMemoryAddressToExpression ();
  edu::sharif::twinner::util::foreach (memToExp, &code_memory_changes, out);
}

void code_memory_changes (IndentedStringStream &out,
    const ADDRINT &memoryEa, edu::sharif::twinner::trace::Expression * const &exp) {
  out.indented () << "*((UINT64 *) 0x" << std::hex << memoryEa << ") = "
      << exp->toString () << ";\n";
}

void code_registers_symbolic_changes_of_one_segment (IndentedStringStream &out,
    const edu::sharif::twinner::trace::ExecutionTraceSegment *segment) {
  out.indented () << "/*Registers Changes*/\n";
  //  const std::map < REG, edu::sharif::twinner::trace::Expression * > &regToExp =
  //      segment->getRegisterToExpression ();
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
  edu::sharif::twinner::util::foreach (map, &add_value_to_set, set);
  return set;
}

template < typename Key, typename Value >
void add_value_to_set (std::set < Value > &set,
    const Key &key, const Value &value) {
  set.insert (value);
}


}
namespace util {

/*
 * Following functions implementations are specific to Twinner.
 * Similar but more advanced implementations are provided for TwinTool.
 */

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
