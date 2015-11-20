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

#include "Twinner.h"

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <stdlib.h>

#include "Executer.h"

#include "edu/sharif/twinner/trace/MarInfo.h"
#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/ExecutionTraceSegment.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"
#include "edu/sharif/twinner/trace/exptoken/MemoryEmergedSymbol.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"

#include "edu/sharif/twinner/engine/smt/SmtSolver.h"
#include "edu/sharif/twinner/engine/smt/UnsatisfiableConstraintsException.h"
#include "smt/Cvc4SmtSolver.h"
#include "search/ConstraintTree.h"

using namespace std;

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {

struct TwinCodeGenerationAux {
  unsigned int depth;
  std::stringstream &out;
  unsigned int index;
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

void code_trace_into_twin_code (std::stringstream &out,
    const edu::sharif::twinner::trace::Trace * const &trace,
    int traceIndex, std::stringstream &conout);
std::map < int, std::set < std::pair < ADDRINT, int > > >
gather_symbols_addresses_of_trace (
    const edu::sharif::twinner::trace::Trace *trace);
void extract_memory_addresses_of_trace_segment (
    std::map < int, std::set < std::pair < ADDRINT, int > > > &addr,
    edu::sharif::twinner::trace::ExecutionTraceSegment * const &segment);
void extract_memory_addresses_of_constraint (
    std::map < int, std::set < std::pair < ADDRINT, int > > > &addr,
    edu::sharif::twinner::trace::Constraint * const &constraint);
template < typename Key >
void extract_memory_addresses_of_expression (
    std::map < int, std::set < std::pair < ADDRINT, int > > > &addr,
    const Key &key, edu::sharif::twinner::trace::Expression * const &exp);
void extract_memory_addresses_of_expression (
    std::map < int, std::set < std::pair < ADDRINT, int > > > &addr,
    const edu::sharif::twinner::trace::Expression *exp);
void check_symbol_type_and_add_it_to_set (
    std::map < int, std::set < std::pair < ADDRINT, int > > > &addr,
    edu::sharif::twinner::trace::exptoken::ExpressionToken * const &token);

void code_segment_into_twin_code (
    const std::set < std::pair < ADDRINT, int > > &addresses,
    TwinCodeGenerationAux &aux,
    edu::sharif::twinner::trace::ExecutionTraceSegment * const &segment,
    int traceIndex, std::stringstream &conout);

struct Variable {
  std::string type;
  std::string technicalName;
  std::string name;

  Variable (std::string _type, std::string _technicalName, std::string _name)
      : type (_type), technicalName (_technicalName), name (_name) {
  }

  bool operator< (const Variable &v) const {
    return technicalName < v.technicalName;
  }
};

void extract_types_and_names (std::set< Variable > &typesAndNames,
    const edu::sharif::twinner::trace::Constraint *simplifiedConstraint);
void extract_types_and_names (std::set< Variable > &typesAndNames,
    const edu::sharif::twinner::trace::Expression *exp);
void extract_type_and_names_and_add_them_to_set (
    std::set< Variable > &typesAndNames,
    edu::sharif::twinner::trace::exptoken::ExpressionToken * const &token);
void code_condition_function_and_if (IndentedStringStream &iss,
    std::string constraintString, int traceIndex, int segmentIndex,
    std::set< Variable > typesAndNames,
    std::stringstream &conout);

void delete_symbol (const edu::sharif::twinner::trace::exptoken::Symbol * const &symbol);

template < typename Key, typename Value >
std::set < Value > get_values_set (const std::map < Key, Value > &map);
template < typename Key, typename Value >
void add_value_to_set (std::set < Value > &set,
    const Key &key, const Value &value);

void remove_mismatches_from_map (
    std::map < std::pair < ADDRINT, int >, UINT64 > &initialValues,
    const std::pair < ADDRINT, int > &address, const UINT64 &content);
void code_initial_value_into_twin_code (std::stringstream &out,
    const std::pair < ADDRINT, int > &address, const UINT64 &content);

void code_memory_symbolic_changes_of_one_segment (IndentedStringStream &out,
    const edu::sharif::twinner::trace::ExecutionTraceSegment *segment);
void code_registers_symbolic_changes_of_one_segment (IndentedStringStream &out,
    const edu::sharif::twinner::trace::ExecutionTraceSegment *segment,
    unsigned int &index);

void code_registers_symbols_initiation_into_twin_code (std::stringstream &out,
    int index);
void code_symbol_initiation_into_twin_code (TwinCodeGenerationAux &aux,
    const std::pair < ADDRINT, int > &address);
void code_memory_changes (IndentedStringStream &out,
    const ADDRINT &memoryEa, edu::sharif::twinner::trace::Expression * const &exp);

Twinner::Twinner () :
    ctree (new edu::sharif::twinner::engine::search::ConstraintTree ()) {
  edu::sharif::twinner::engine::smt::SmtSolver::init
      (new edu::sharif::twinner::engine::smt::Cvc4SmtSolver ());
}

Twinner::~Twinner () {
  delete ctree;
  edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()->destroy ();
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

void Twinner::setJustAnalyzeMainRoutine (bool main) {
  this->main = main;
}

void Twinner::setMeasureOverheads (bool measureOverheads) {
  this->overheads = measureOverheads;
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
  Executer ex (pin, twintool, input, arguments, main, overheads);
  set < const edu::sharif::twinner::trace::exptoken::Symbol * > symbols;
  bool somePathsAreNotCovered = true;
  int i = 1;
  while (somePathsAreNotCovered) {
    edu::sharif::twinner::util::Logger::debug () << "Executing trace # " << i++ << '\n';
    // steps 1, 2, and 3
    ex.setSymbolsValues (symbols);
    edu::sharif::twinner::trace::Trace *trace = ex.executeSingleTraceInNormalMode ();

    edu::sharif::twinner::util::foreach (symbols, &delete_symbol);
    symbols.clear ();
    if (main) {
      addExecutionTrace (trace, ex.readMarInfo ());
    } else {
      addExecutionTrace (trace);
    }
    // symbols will be filled with newly instantiated objects and should be deleted...
    somePathsAreNotCovered = calculateSymbolsValuesForCoveringNextPath (symbols);
  }
  edu::sharif::twinner::util::foreach (symbols, &delete_symbol);
  symbols.clear ();
  const std::map < std::pair < ADDRINT, int >, UINT64 > initialValues =
      obtainInitializedMemoryValues (ex);
  codeTracesIntoTwinCode (initialValues);
}

std::map < std::pair < ADDRINT, int >, UINT64 >
Twinner::obtainInitializedMemoryValues (Executer &ex) const {
  // step 4: `addresses` field holds candidate addresses
  std::set < std::pair < ADDRINT, int > > addresses;
  for (std::list < const edu::sharif::twinner::trace::Trace * >::const_iterator it =
      traces.begin (); it != traces.end (); ++it) {
    const edu::sharif::twinner::trace::Trace *trace = *it;
    const std::map < int, std::set < std::pair < ADDRINT, int > > > addr =
        gather_symbols_addresses_of_trace (trace);
    for (std::map < int, std::set < std::pair < ADDRINT, int > > >
        ::const_iterator it2 = addr.begin (); it2 != addr.end (); ++it2) {
      const std::set < std::pair < ADDRINT, int > > &a = it2->second;
      addresses.insert (a.begin (), a.end ());
    }
  }
  ex.setCandidateAddresses (addresses);
  // step 5
  std::map < std::pair < ADDRINT, int >, UINT64 > initialValues1 =
      ex.executeSingleTraceInInitialStateDetectionMode ();
  // step 6
  ex.changeArguments ();
  std::map < std::pair < ADDRINT, int >, UINT64 > initialValues2 =
      ex.executeSingleTraceInInitialStateDetectionMode ();
  // step 7
  edu::sharif::twinner::util::foreach (initialValues2,
                                       &remove_mismatches_from_map, initialValues1);
  return initialValues1;
}

void remove_mismatches_from_map (
    std::map < std::pair < ADDRINT, int >, UINT64 > &initialValues,
    const std::pair < ADDRINT, int > &address, const UINT64 &content) {
  std::map < std::pair < ADDRINT, int >, UINT64 >::iterator it =
      initialValues.find (address);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "at address: 0x" << std::hex << address.first
      << "\tfirst map: 0x" << it->second
      << "\tsecond map: 0x" << content << "\n";
  if (it->second != content) {
    initialValues.erase (it);
  }
}

void delete_symbol (const edu::sharif::twinner::trace::exptoken::Symbol * const &symbol) {
  delete symbol;
}

void Twinner::addExecutionTrace (edu::sharif::twinner::trace::Trace *trace,
    edu::sharif::twinner::trace::MarInfo *marInfo) {
  if (!marInfo->isConsistent ()) {
    throw std::runtime_error ("MAR info in inconsistent");
  }
  marInfo->simplifyTrace (trace);
  addExecutionTrace (trace);
}

void Twinner::addExecutionTrace (edu::sharif::twinner::trace::Trace *trace) {
  edu::sharif::twinner::util::Logger log = edu::sharif::twinner::util::Logger::debug ();
  log << "Adding execution trace:\n";
  trace->printCompleteState (log);
  traces.push_back (trace);
  ctree->addConstraints (trace);
  log << "Done.\n";
}

bool Twinner::calculateSymbolsValuesForCoveringNextPath (
    set < const edu::sharif::twinner::trace::exptoken::Symbol * > &symbols) {
  //TODO: Refactor these codes out of this class (to a search strategy class)
  std::list < const edu::sharif::twinner::trace::Constraint * > clist;
  ctree->dumpTree ();
  while (ctree->getNextConstraintsList (clist)) {
    try {
      edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()->solveConstraints
          (clist, symbols);
      return true;
    } catch (const edu::sharif::twinner::engine::smt::UnsatisfiableConstraintsException &e) {
    }
  }
  return false;
}

void Twinner::codeTracesIntoTwinCode (
    const std::map < std::pair < ADDRINT, int >, UINT64 > &initialValues) {
  std::stringstream code;
  std::stringstream out;
  std::stringstream conout;
  code << '\n';
  code << "#include \"twincode.h\"\n";
  code << '\n';
  out << "int main (int argc, char *argv[]) {\n";
  out << "\tstruct RegistersSet regs;\n";
  out << "\tSAVE_REGISTERS (regs);\n";
  out << '\t';
  code_registers_symbols_initiation_into_twin_code (out, 0);
  codeInitialValuesIntoTwinCode (out, initialValues);
  int i = 0;
  for (std::list < const edu::sharif::twinner::trace::Trace * >::iterator it =
      traces.begin (); it != traces.end (); ++it) {
    out << "\t// coding trace #" << i << '\n';
    code_trace_into_twin_code (out, *it, i, conout);
    out << "\t// end of trace #" << i << '\n';
    ++i;
  }
  out << "}\n";
  code << conout.str () << out.str ();
  edu::sharif::twinner::util::Logger::info () << code.str ();

  std::ofstream fileout;
  fileout.open (twin.c_str (), ios_base::out | ios_base::trunc);
  if (!fileout.is_open ()) {
    edu::sharif::twinner::util::Logger::error () << "Can not write twincode"
        " to file: Error in open function: "
        << twin << '\n';
    throw std::runtime_error ("Error in saving twincode in file");
  }
  fileout << code.str ();

  fileout.close ();
}

void code_registers_symbols_initiation_into_twin_code (std::stringstream &out,
    int index) {
  const char *registersNames[] = {// count == 16
    "rax", "rbx", "rcx", "rdx", "rdi", "rsi", "rsp", "rbp",
    "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
  };
  out << std::dec << "const UINT64 " << registersNames[0] << "_" << index <<
      " = regs." << registersNames[0];
  for (int i = 1; i < 16; ++i) {
    out << ", " << registersNames[i] << "_" << index << " = regs." << registersNames[i];
  }
  out << ";\n";
  out << std::dec << "const UINT128 xmm0_" << index << " = UINT128 (regs.xmm0)";
  for (int i = 1; i < 16; ++i) {
    out << ", xmm" << i << "_" << index << " = UINT128 (regs.xmm" << i << ")";
  }
  out << ";\n";
}

void code_symbol_initiation_into_twin_code (TwinCodeGenerationAux &aux,
    const std::pair < ADDRINT, int > &address) {

  repeat (aux.depth) {
    aux.out << '\t';
  }
  aux.out << "const UINT" << std::dec << address.second
      << " m" << std::hex << address.first << "_" << aux.index
      << "_" << std::dec << address.second
      << " = *((UINT" << address.second << " *) 0x"
      << std::hex << address.first << ");\n";
}

void Twinner::codeInitialValuesIntoTwinCode (std::stringstream &out,
    const std::map < std::pair < ADDRINT, int >, UINT64 > &initialValues) const {
  edu::sharif::twinner::util::foreach (initialValues,
                                       &code_initial_value_into_twin_code, out);
}

void code_initial_value_into_twin_code (std::stringstream &out,
    const std::pair < ADDRINT, int > &address, const UINT64 &content) {
  out << "\t*((UINT" << std::dec << address.second << " *) 0x"
      << std::hex << address.first << ") = 0x" << content << ";\n";
}

void code_trace_into_twin_code (std::stringstream &out,
    const edu::sharif::twinner::trace::Trace * const &trace,
    int traceIndex, std::stringstream &conout) {
  TwinCodeGenerationAux aux = {
    1, // depth
    out,
    0 // index
  };
  out << "\t{\n";
  std::map < int, std::set < std::pair < ADDRINT, int > > > addresses =
      gather_symbols_addresses_of_trace (trace);
  for (std::list < edu::sharif::twinner::trace::ExecutionTraceSegment * >
      ::const_reverse_iterator it = trace->getTraceSegments ().rbegin ();
      it != trace->getTraceSegments ().rend (); ++it) {
    edu::sharif::twinner::trace::ExecutionTraceSegment * const segment = *it;
    code_segment_into_twin_code (addresses[aux.index], aux, segment,
                                 traceIndex, conout);
  }
  for (unsigned int j = aux.depth - 1; j > 0; --j) {
    if (j == 1) {
      out << "\t\treturn 0;\n"; // Marking end of trace
    }

    repeat (j) {
      out << '\t';
    }
    out << "}\n";
  }
  out << "\t}\n";
}

std::map < int, std::set < std::pair < ADDRINT, int > > >
gather_symbols_addresses_of_trace (
    const edu::sharif::twinner::trace::Trace *trace) {
  std::map < int, std::set < std::pair < ADDRINT, int > > > addr;
  edu::sharif::twinner::util::foreach (trace->getTraceSegments (),
                                       &extract_memory_addresses_of_trace_segment,
                                       addr);
  return addr;
}

void extract_memory_addresses_of_trace_segment (
    std::map < int, std::set < std::pair < ADDRINT, int > > > &addr,
    edu::sharif::twinner::trace::ExecutionTraceSegment * const &segment) {
  edu::sharif::twinner::util::foreach (segment->getMemoryAddressTo64BitsExpression (),
                                       &extract_memory_addresses_of_expression,
                                       addr);
  edu::sharif::twinner::util::foreach (segment->getRegisterToExpression (),
                                       &extract_memory_addresses_of_expression,
                                       addr);
  edu::sharif::twinner::util::foreach (segment->getPathConstraints (),
                                       &extract_memory_addresses_of_constraint,
                                       addr);
}

void extract_memory_addresses_of_constraint (
    std::map < int, std::set < std::pair < ADDRINT, int > > > &addr,
    edu::sharif::twinner::trace::Constraint * const &constraint) {
  extract_memory_addresses_of_expression (addr, constraint->getMainExpression ());
  if (constraint->getAuxExpression ()) {
    extract_memory_addresses_of_expression (addr, constraint->getAuxExpression ());
  }
}

template < typename Key >
void extract_memory_addresses_of_expression (
    std::map < int, std::set < std::pair < ADDRINT, int > > > &addr,
    const Key &key, edu::sharif::twinner::trace::Expression * const &exp) {
  extract_memory_addresses_of_expression (addr, exp);
}

void extract_memory_addresses_of_expression (
    std::map < int, std::set < std::pair < ADDRINT, int > > > &addr,
    const edu::sharif::twinner::trace::Expression *exp) {
  edu::sharif::twinner::util::foreach (exp->getStack (),
                                       &check_symbol_type_and_add_it_to_set, addr);
}

void check_symbol_type_and_add_it_to_set (
    std::map < int, std::set < std::pair < ADDRINT, int > > > &addr,
    edu::sharif::twinner::trace::exptoken::ExpressionToken * const &token) {
  const edu::sharif::twinner::trace::exptoken::MemoryEmergedSymbol *symbol =
      dynamic_cast<edu::sharif::twinner::trace::exptoken::MemoryEmergedSymbol *> (token);
  if (symbol) {
    addr[symbol->getGenerationIndex ()].insert
        (make_pair (symbol->getAddress (), symbol->getValue ().getSize ()));
  }
}

void code_segment_into_twin_code (
    const std::set < std::pair < ADDRINT, int > > &addresses,
    TwinCodeGenerationAux &aux,
    edu::sharif::twinner::trace::ExecutionTraceSegment * const &segment,
    int traceIndex, std::stringstream &conout) {
  edu::sharif::twinner::util::foreach
      (addresses, &code_symbol_initiation_into_twin_code, aux);
  const std::list < edu::sharif::twinner::trace::Constraint * > &pathConstraints =
      segment->getPathConstraints ();
  std::list < const edu::sharif::twinner::trace::Constraint * > simplifiedConstraints =
      edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
      ->simplifyConstraints
      (std::list < const edu::sharif::twinner::trace::Constraint * >
       (pathConstraints.begin (), pathConstraints.end ()));
  std::stringstream out;
  std::set< Variable > typesAndNames;
  bool first = true;
  for (std::list < const edu::sharif::twinner::trace::Constraint * >
      ::const_reverse_iterator it = simplifiedConstraints.rbegin ();
      it != simplifiedConstraints.rend (); ++it) {
    const edu::sharif::twinner::trace::Constraint *simplifiedConstraint = *it;
    if (simplifiedConstraint->isTrivial ()) {
      continue;
    }
    if (first) {
      out << '(';
      first = false;
    } else {
      out << "\n\t\t&& (";
    }
    out << simplifiedConstraint->toString () << ')';
    extract_types_and_names (typesAndNames, simplifiedConstraint);
    delete simplifiedConstraint;
  }
  std::stringstream indentation;

  repeat (aux.depth) {
    indentation << '\t';
  }
  if (!first) {
    IndentedStringStream iss (indentation.str ());
    code_condition_function_and_if (iss, out.str (), traceIndex, aux.index,
                                    typesAndNames, conout);
    aux.out << iss.str ();
    aux.depth++;
    indentation << '\t';
  }
  IndentedStringStream iss (indentation.str ());
  code_memory_symbolic_changes_of_one_segment (iss, segment);
  code_registers_symbolic_changes_of_one_segment (iss, segment, aux.index);
  aux.out << iss.str ();
}

void extract_types_and_names (std::set< Variable > &typesAndNames,
    const edu::sharif::twinner::trace::Constraint *simplifiedConstraint) {
  extract_types_and_names
      (typesAndNames, simplifiedConstraint->getMainExpression ());
  if (simplifiedConstraint->getAuxExpression ()) {
    extract_types_and_names
        (typesAndNames, simplifiedConstraint->getAuxExpression ());
  }
}

void extract_types_and_names (std::set< Variable > &typesAndNames,
    const edu::sharif::twinner::trace::Expression *exp) {
  edu::sharif::twinner::util::foreach
      (exp->getStack (), &extract_type_and_names_and_add_them_to_set,
       typesAndNames);
}

void extract_type_and_names_and_add_them_to_set (
    std::set< Variable > &typesAndNames,
    edu::sharif::twinner::trace::exptoken::ExpressionToken * const &token) {
  const edu::sharif::twinner::trace::exptoken::Symbol *symbol =
      dynamic_cast<edu::sharif::twinner::trace::exptoken::Symbol *> (token);
  if (symbol) {
    std::stringstream ss;
    ss << "UINT" << std::dec << symbol->getValue ().getSize ();
    typesAndNames.insert (Variable (ss.str (),
                                    symbol->getTechnicalName (),
                                    symbol->toString ()));
  }
}

void code_condition_function_and_if (IndentedStringStream &iss,
    std::string constraintString, int traceIndex, int segmentIndex,
    std::set< Variable > typesAndNames,
    std::stringstream &conout) {
  std::stringstream conditionName;
  conditionName << "trace_" << traceIndex << "_condition_" << segmentIndex;

  std::stringstream typedArguments, arguments;
  bool first = true;
  for (std::set< Variable >::const_iterator it = typesAndNames.begin ();
      it != typesAndNames.end (); ++it) {
    if (first) {
      first = false;
    } else {
      typedArguments << ", ";
      arguments << ", ";
    }
    typedArguments << it->type << ' ' << it->technicalName;
    arguments << "UINT64 (" << it->name << ")";
  }

  conout << "bool " << conditionName.str () << " ("
      << typedArguments.str () << ") {\n";
  conout << "\treturn " << constraintString << ";\n";
  conout << "}\n\n";
  iss.indented () << "if (" << conditionName.str () << " ("
      << arguments.str () << ")) {\n";
}

void code_memory_symbolic_changes_of_one_segment (IndentedStringStream &out,
    const edu::sharif::twinner::trace::ExecutionTraceSegment *segment) {
  out.indented () << "/*Memory Changes*/\n";
  const std::map < ADDRINT, edu::sharif::twinner::trace::Expression * > &memToExp =
      segment->getMemoryAddressTo64BitsExpression ();
  edu::sharif::twinner::util::foreach (memToExp, &code_memory_changes, out);
}

void code_memory_changes (IndentedStringStream &out,
    const ADDRINT &memoryEa, edu::sharif::twinner::trace::Expression * const &exp) {
  out.indented () << "*((UINT64 *) 0x" << std::hex << memoryEa << ") = "
      "UINT64 (" << exp->toString () << ");\n";
}

void code_registers_symbolic_changes_of_one_segment (IndentedStringStream &out,
    const edu::sharif::twinner::trace::ExecutionTraceSegment *segment,
    unsigned int &index) {

  struct RegInfo {
    const char *name;
    REG id;
  } const regs[] = {// count == 16
    {"rax", REG_RAX},
    {"rbx", REG_RBX},
    {"rcx", REG_RCX},
    {"rdx", REG_RDX},
    {"rdi", REG_RDI},
    {"rsi", REG_RSI},
    {"rsp", REG_RSP},
    {"rbp", REG_RBP},
    {"r8", REG_R8},
    {"r9", REG_R9},
    {"r10", REG_R10},
    {"r11", REG_R11},
    {"r12", REG_R12},
    {"r13", REG_R13},
    {"r14", REG_R14},
    {"r15", REG_R15}
  };
  out.indented () << "/*Registers Changes*/\n";
  const std::map < REG, edu::sharif::twinner::trace::Expression * > &regToExp =
      segment->getRegisterToExpression ();
  for (int i = 0; i < 16; ++i) {
    std::map < REG, edu::sharif::twinner::trace::Expression * >::const_iterator it =
        regToExp.find (regs[i].id);
    out.indented () << "regs." << regs[i].name << " = ";
    if (it != regToExp.end ()) {
      out << "UINT64 (" << it->second->toString () << ");\n";
    } else {
      out << regs[i].name << "_" << index << ";\n";
    }
  }
  out.indented () << "regs = setRegistersValuesAndInvokeSyscall (regs);\n";
  code_registers_symbols_initiation_into_twin_code (out.indented (), ++index);
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

edu::sharif::twinner::trace::cv::ConcreteValue *readRegisterContent (
    const CONTEXT *context, REG reg) {
  const char *msg = "The readRegisterContent(...) method is only supported in TwinTool:"
      " Calling error from Twinner.\n";
  edu::sharif::twinner::util::Logger::error () << msg;
  throw std::runtime_error (msg);
}

UINT64 readMemoryContent (ADDRINT memoryEa, size_t size) {
  const char *msg = "The readMemoryContent(...) method is only supported in TwinTool:"
      " Calling error from Twinner.\n";
  edu::sharif::twinner::util::Logger::error () << msg;
  throw std::runtime_error (msg);
}

VOID writeMemoryContent (ADDRINT memoryEa, const UINT8 *value, size_t size) {
  const char *msg = "The writeMemoryContent(...) method is only supported in TwinTool:"
      " Calling error from Twinner.\n";
  edu::sharif::twinner::util::Logger::error () << msg;
  throw std::runtime_error (msg);
}

VOID writeRegisterContent (CONTEXT *context, REG reg, const UINT8 *value) {
  const char *msg = "The writeRegisterContent(...) method is only supported in TwinTool:"
      " Calling error from Twinner.\n";
  edu::sharif::twinner::util::Logger::error () << msg;
  throw std::runtime_error (msg);
}

}
namespace trace {

edu::sharif::twinner::trace::Expression *lazy_load_symbolic_expression (
    edu::sharif::twinner::trace::ExecutionTraceSegment *me, int size,
    std::map < ADDRINT, edu::sharif::twinner::trace::Expression * > &map,
    const ADDRINT key,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal) {
  const char *msg = "The lazy_load_symbolic_expression(...) function is"
      " only supported in TwinTool: Calling error from Twinner.\n";
  edu::sharif::twinner::util::Logger::error () << msg;
  throw std::runtime_error (msg);
}

edu::sharif::twinner::trace::Expression *lazy_load_symbolic_expression (
    edu::sharif::twinner::trace::ExecutionTraceSegment *me, int size,
    std::map < ADDRINT, edu::sharif::twinner::trace::Expression * > &map,
    const ADDRINT key) {
  const char *msg = "The lazy_load_symbolic_expression(...) function is"
      " only supported in TwinTool: Calling error from Twinner.\n";
  edu::sharif::twinner::util::Logger::error () << msg;
  throw std::runtime_error (msg);
}

}
}
}
}
