//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2014  Behnam Momeni
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

#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/ExecutionTraceSegment.h"
#include "edu/sharif/twinner/trace/MemoryEmergedSymbol.h"

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
    const edu::sharif::twinner::trace::Trace * const &trace);
std::map < int, std::set < ADDRINT > > gather_symbols_addresses_of_trace (
    const edu::sharif::twinner::trace::Trace *trace);
void extract_memory_addresses_of_trace_segment (
    std::map < int, std::set < ADDRINT > > &addr,
    edu::sharif::twinner::trace::ExecutionTraceSegment * const &segment);
void extract_memory_addresses_of_constraint (
    std::map < int, std::set < ADDRINT > > &addr,
    const edu::sharif::twinner::trace::Constraint * const &constraint);
template < typename Key >
void extract_memory_addresses_of_expression (
    std::map < int, std::set < ADDRINT > > &addr,
    const Key &key, edu::sharif::twinner::trace::Expression * const &exp);
void extract_memory_addresses_of_expression (
    std::map < int, std::set < ADDRINT > > &addr,
    const edu::sharif::twinner::trace::Expression *exp);
void check_symbol_type_and_add_it_to_set (
    std::map < int, std::set < ADDRINT > > &addr,
    edu::sharif::twinner::trace::ExpressionToken * const &token);

void code_segment_into_twin_code (const std::set < ADDRINT > &addresses,
    TwinCodeGenerationAux &aux,
    edu::sharif::twinner::trace::ExecutionTraceSegment * const &segment);

void delete_symbol (const edu::sharif::twinner::trace::Symbol * const &symbol);

template < typename Key, typename Value >
std::set < Value > get_values_set (const std::map < Key, Value > &map);
template < typename Key, typename Value >
void add_value_to_set (std::set < Value > &set,
    const Key &key, const Value &value);

void remove_mismatches_from_map (std::map < ADDRINT, UINT64 > &initialValues,
    const ADDRINT &address, const UINT64 &content);
void code_initial_value_into_twin_code (std::stringstream &out,
    const ADDRINT &address, const UINT64 &content);

void code_memory_symbolic_changes_of_one_segment (IndentedStringStream &out,
    const edu::sharif::twinner::trace::ExecutionTraceSegment *segment);
void code_registers_symbolic_changes_of_one_segment (IndentedStringStream &out,
    const edu::sharif::twinner::trace::ExecutionTraceSegment *segment,
    unsigned int &index);

void code_registers_symbols_initiation_into_twin_code (std::stringstream &out,
    int index);
void code_symbol_initiation_into_twin_code (TwinCodeGenerationAux &aux,
    const ADDRINT &address);
void code_memory_changes (IndentedStringStream &out,
    const ADDRINT &memoryEa, edu::sharif::twinner::trace::Expression * const &exp);

Twinner::Twinner () :
    ctree (new edu::sharif::twinner::engine::search::ConstraintTree ()) {
  edu::sharif::twinner::engine::smt::SmtSolver::init
      (new edu::sharif::twinner::engine::smt::Cvc4SmtSolver ());
}

Twinner::~Twinner () {
  delete ctree;
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
  set < const edu::sharif::twinner::trace::Symbol * > symbols;
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
  codeTracesIntoTwinCode (initialValues);
}

std::map < ADDRINT, UINT64 > Twinner::obtainInitializedMemoryValues (Executer &ex) const {
  // step 4: `addresses` field holds candidate addresses
  std::set < ADDRINT > addresses;
  for (std::list < const edu::sharif::twinner::trace::Trace * >::const_iterator it =
      traces.begin (); it != traces.end (); ++it) {
    const edu::sharif::twinner::trace::Trace *trace = *it;
    const std::map < int, std::set < ADDRINT > > addr =
        gather_symbols_addresses_of_trace (trace);
    for (std::map < int, std::set < ADDRINT > >::const_iterator it2 = addr.begin ();
        it2 != addr.end (); ++it2) {
      const std::set < ADDRINT > &a = it2->second;
      addresses.insert (a.begin (), a.end ());
    }
  }
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

void delete_symbol (const edu::sharif::twinner::trace::Symbol * const &symbol) {
  delete symbol;
}

void Twinner::addExecutionTrace (const edu::sharif::twinner::trace::Trace *trace) {
  edu::sharif::twinner::util::Logger log = edu::sharif::twinner::util::Logger::debug ();
  log << "Adding execution trace:\n";
  trace->printCompleteState (log);
  traces.push_back (trace);
  ctree->addConstraints (trace);
  log << "Done.\n";
}

bool Twinner::calculateSymbolsValuesForCoveringNextPath (
    set < const edu::sharif::twinner::trace::Symbol * > &symbols) {
  //TODO: Refactor these codes out of this class (to a search strategy class)
  std::list < const edu::sharif::twinner::trace::Constraint * > clist;
  while (ctree->getNextConstraintsList (clist)) {
    try {
      ctree->dumpTree ();
      edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()->solveConstraints
          (clist, symbols);
      return true;
    } catch
      (const edu::sharif::twinner::engine::smt::UnsatisfiableConstraintsException &e) {
    }
  }
  return false;
}

void Twinner::codeTracesIntoTwinCode (
    const std::map < ADDRINT, UINT64 > &initialValues) {
  std::stringstream out;
  out << '\n';
  out << "#include \"twincode.h\"\n";
  out << '\n';
  out << "int main () {\n";
  out << "\tstruct RegistersSet regs;\n";
  out << "\tSAVE_REGISTERS (regs);\n";
  out << '\t';
  code_registers_symbols_initiation_into_twin_code (out, 0);
  codeInitialValuesIntoTwinCode (out, initialValues);
  int i = 0;
  for (std::list < const edu::sharif::twinner::trace::Trace * >::iterator it =
      traces.begin (); it != traces.end (); ++it) {
    out << "\t// coding trace #" << i << '\n';
    code_trace_into_twin_code (out, *it);
    out << "\t// end of trace #" << i << '\n';
    ++i;
  }
  out << "}\n";
  edu::sharif::twinner::util::Logger::info () << out.str ();

  std::ofstream fileout;
  fileout.open (twin.c_str (), ios_base::out | ios_base::trunc);
  if (!fileout.is_open ()) {
    edu::sharif::twinner::util::Logger::error () << "Can not write twincode"
        " to file: Error in open function: "
        << twin << '\n';
    throw std::runtime_error ("Error in saving twincode in file");
  }
  fileout << out.str ();

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
    const ADDRINT &address) {

  repeat (aux.depth) {
    aux.out << '\t';
  }
  aux.out << "const UINT64 m" << std::hex << address << "_" << aux.index
      << " = *((UINT64 *) 0x" << address << ");\n";
}

void Twinner::codeInitialValuesIntoTwinCode (std::stringstream &out,
    const std::map < ADDRINT, UINT64 > &initialValues) const {
  edu::sharif::twinner::util::foreach (initialValues,
                                       &code_initial_value_into_twin_code, out);
}

void code_initial_value_into_twin_code (std::stringstream &out,
    const ADDRINT &address, const UINT64 &content) {
  out << std::hex << "\t*((UINT64 *) 0x" << address << ") = 0x" << content << ";\n";
}

void code_trace_into_twin_code (std::stringstream &out,
    const edu::sharif::twinner::trace::Trace * const &trace) {
  TwinCodeGenerationAux aux = {
                               1, // depth
                               out,
                               0 // index
  };
  out << "\t{\n";
  std::map < int, std::set < ADDRINT > > addresses =
      gather_symbols_addresses_of_trace (trace);
  for (std::list < edu::sharif::twinner::trace::ExecutionTraceSegment * >
      ::const_reverse_iterator it = trace->getTraceSegments ().rbegin ();
      it != trace->getTraceSegments ().rend (); ++it) {
    edu::sharif::twinner::trace::ExecutionTraceSegment * const segment = *it;
    code_segment_into_twin_code (addresses[aux.index], aux, segment);
  }
  for (unsigned int j = aux.depth - 1; j > 0; --j) {

    repeat (j) {
      out << '\t';
    }
    out << "}\n";
  }
  out << "\t}\n";
}

std::map < int, std::set < ADDRINT > > gather_symbols_addresses_of_trace (
    const edu::sharif::twinner::trace::Trace *trace) {
  std::map < int, std::set < ADDRINT > > addr;
  edu::sharif::twinner::util::foreach (trace->getTraceSegments (),
                                       &extract_memory_addresses_of_trace_segment,
                                       addr);
  return addr;
}

void extract_memory_addresses_of_trace_segment (
    std::map < int, std::set < ADDRINT > > &addr,
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

void extract_memory_addresses_of_constraint (std::map < int, std::set < ADDRINT > > &addr,
    const edu::sharif::twinner::trace::Constraint * const &constraint) {
  extract_memory_addresses_of_expression (addr, constraint->getMainExpression ());
  if (constraint->getAuxExpression ()) {
    extract_memory_addresses_of_expression (addr, constraint->getAuxExpression ());
  }
}

template < typename Key >
void extract_memory_addresses_of_expression (std::map < int, std::set < ADDRINT > > &addr,
    const Key &key, edu::sharif::twinner::trace::Expression * const &exp) {
  extract_memory_addresses_of_expression (addr, exp);
}

void extract_memory_addresses_of_expression (std::map < int, std::set < ADDRINT > > &addr,
    const edu::sharif::twinner::trace::Expression *exp) {
  edu::sharif::twinner::util::foreach (exp->getStack (),
                                       &check_symbol_type_and_add_it_to_set, addr);
}

void check_symbol_type_and_add_it_to_set (std::map < int, std::set < ADDRINT > > &addr,
    edu::sharif::twinner::trace::ExpressionToken * const &token) {
  const edu::sharif::twinner::trace::MemoryEmergedSymbol *symbol =
      dynamic_cast<edu::sharif::twinner::trace::MemoryEmergedSymbol *> (token);
  if (symbol) {
    addr[symbol->getGenerationIndex ()].insert (symbol->getAddress ());
  }
}

void code_segment_into_twin_code (const std::set < ADDRINT > &addresses,
    TwinCodeGenerationAux &aux,
    edu::sharif::twinner::trace::ExecutionTraceSegment * const &segment) {
  edu::sharif::twinner::util::foreach
      (addresses, &code_symbol_initiation_into_twin_code, aux);
  std::list < const edu::sharif::twinner::trace::Constraint * > simplifiedConstraints =
      edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
      ->simplifyConstraints (segment->getPathConstraints ());
  std::stringstream out;
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
      out << " && (";
    }
    out << simplifiedConstraint->toString () << ')';
    delete simplifiedConstraint;
  }
  if (!first) {

    repeat (aux.depth) {
      aux.out << '\t';
    }
    aux.out << "if (" << out.str () << ") {\n";
    aux.depth++;
  }
  std::stringstream indentation;

  repeat (aux.depth) {
    indentation << '\t';
  }
  IndentedStringStream iss (indentation.str ());
  code_memory_symbolic_changes_of_one_segment (iss, segment);
  code_registers_symbolic_changes_of_one_segment (iss, segment, aux.index);
  aux.out << iss.str ();
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
      << exp->toString () << ";\n";
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
      out << it->second->toString () << ";\n";
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

edu::sharif::twinner::trace::ConcreteValue *readRegisterContent (
    const CONTEXT *context, REG reg) {
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
}
}
}
