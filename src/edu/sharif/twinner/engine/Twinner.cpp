//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2016 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "edu/sharif/twinner/pin-wrapper.h"

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
#include "edu/sharif/twinner/engine/smt/Cvc4SmtSolver.h"

#include "edu/sharif/twinner/engine/etg/ConstraintTree.h"
#include "edu/sharif/twinner/engine/etg/TwinCodeEncoder.h"
#include "edu/sharif/twinner/engine/etg/AddressAggregator.h"

using namespace std;

#ifdef TARGET_IA32E
#define VAR_TYPE "UINT64"
#else
#define VAR_TYPE "UINT32"
#endif

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {

void delete_symbol (const edu::sharif::twinner::trace::exptoken::Symbol * const &symbol);

template < typename Key, typename Value >
std::set < Value > get_values_set (const std::map < Key, Value > &map);
template < typename Key, typename Value >
void add_value_to_set (std::set < Value > &set,
    const Key &key, const Value &value);

void find_intersection (
    std::pair< std::map < std::pair < ADDRINT, int >, UINT64 > &,
    std::map < std::pair < ADDRINT, int >, UINT64 > &> &sets,
    const std::pair < ADDRINT, int > &address, const UINT64 &content);

Twinner::Twinner () :
    ctree (new edu::sharif::twinner::engine::etg::ConstraintTree ()) {
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

void Twinner::setAnalysisEndpoints (string endpoints) {
  this->endpoints = endpoints;
}

void Twinner::setSafeFunctions (string safeFunctions) {
  this->safeFunctions = safeFunctions;
}

void Twinner::setTempFolder (std::string tmpfolder) {
  this->tmpfolder = tmpfolder;
}

void Twinner::setJustAnalyzeMainRoutine (bool main) {
  this->main = main;
}

void Twinner::setStackOffset (string stackOffset) {
  this->stackOffset = stackOffset;
}

void Twinner::setNaiveMode (bool naive) {
  this->naive = naive;
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
bool Twinner::generateTwinBinary () {
  set < const edu::sharif::twinner::trace::exptoken::Symbol * > symbols;
  bool somePathsAreNotCovered = true;
  int i = 1;
  while (somePathsAreNotCovered) {
    Executer ex (i, pin, twintool, input, arguments,
                 endpoints, safeFunctions, tmpfolder,
                 main, stackOffset, naive, overheads);
    edu::sharif::twinner::util::Logger::debug () << "Executing trace # " << i++ << '\n';
    // steps 1, 2, and 3
    ex.setSymbolsValues (symbols);
    edu::sharif::twinner::trace::Trace *trace = ex.executeSingleTraceInNormalMode ();
    if (trace == 0 || ex.isLastExecutionSignaled ()) {
      // TODO: encode the abortion scenario in the twincode
      break;
    }
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
  {
    Executer ex (i, pin, twintool, input, arguments,
                 endpoints, safeFunctions, tmpfolder,
                 main, stackOffset, naive, overheads);
    const std::map < std::pair < ADDRINT, int >, UINT64 > initialValues =
        obtainInitializedMemoryValues (ex);
    codeTracesIntoTwinCode (initialValues);
  }
  return true;
}

std::map < std::pair < ADDRINT, int >, UINT64 >
Twinner::obtainInitializedMemoryValues (Executer &ex) const {
  edu::sharif::twinner::util::Logger::debug ()
      << "Gathering initial memory values...\n";
  // step 4: `addresses` field holds candidate addresses
  std::set < std::pair < ADDRINT, int > > addresses;
  std::map < int, std::set < std::pair < ADDRINT, int > > > addressToSize;
  edu::sharif::twinner::engine::etg::AddressAggregator
      (traces, addressToSize).gatherSymbolsAddressToSize ();
  for (std::map < int, std::set < std::pair < ADDRINT, int > > >
      ::const_iterator it = addressToSize.begin ();
      it != addressToSize.end (); ++it) {
    const std::set < std::pair < ADDRINT, int > > &a = it->second;
    addresses.insert (a.begin (), a.end ());
  }
  ex.setCandidateAddresses (addresses);
  // step 5
  typedef std::map < std::pair < ADDRINT, int >, UINT64 > MemoryMap;
  MemoryMap iv1 = ex.executeSingleTraceInInitialStateDetectionMode ();
  // step 6
  ex.changeArguments ();
  MemoryMap iv2 = ex.executeSingleTraceInInitialStateDetectionMode ();
  // step 7
  MemoryMap intersection;
  std::pair<MemoryMap &, MemoryMap &> sets (iv2, intersection);
  edu::sharif::twinner::util::foreach (iv1, &find_intersection, sets);
  return intersection;
}

void find_intersection (
    std::pair< std::map < std::pair < ADDRINT, int >, UINT64 > &,
    std::map < std::pair < ADDRINT, int >, UINT64 > &> &sets,
    const std::pair < ADDRINT, int > &address, const UINT64 &content) {
  typedef std::map < std::pair < ADDRINT, int >, UINT64 > MemoryMap;
  const MemoryMap &iv2 = sets.first;
  MemoryMap &intersection = sets.second;
  MemoryMap::const_iterator it = iv2.find (address);
  if (it == iv2.end ()) {
    return;
  }
  edu::sharif::twinner::util::Logger::loquacious ()
      << "at address: 0x" << std::hex << address.first
      << "\tfirst map: 0x" << content
      << "\tsecond map: 0x" << it->second << "\n";
  if (it->second == content) {
    intersection.insert (*it);
  }
}

void delete_symbol (const edu::sharif::twinner::trace::exptoken::Symbol * const &symbol) {
  delete symbol;
}

void Twinner::addExecutionTrace (edu::sharif::twinner::trace::Trace *trace,
    edu::sharif::twinner::trace::MarInfo *marInfo) {
  if (!isTraceConsistent (trace)) {
    throw std::runtime_error ("Trace info is inconsistent");
  }
  if (!marInfo->isConsistent ()) {
    throw std::runtime_error ("MAR info is inconsistent");
  }
  marInfo->simplifyTrace (trace);
  addExecutionTrace (trace);
}

bool Twinner::isTraceConsistent (
    edu::sharif::twinner::trace::Trace *trace) const {
  const std::list < edu::sharif::twinner::trace::ExecutionTraceSegment * > &
      segments = trace->getTraceSegments ();
  for (std::list < edu::sharif::twinner::trace::ExecutionTraceSegment * >
      ::const_reverse_iterator it = segments.rbegin ();
      it != segments.rend (); ++it) {
    const edu::sharif::twinner::trace::ExecutionTraceSegment *segment = *it;
    if (!segment->getTerminator ()) {
      return ++it == segments.rend ();
    }
  }
  return true;
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
    const bool ok = edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
        ->solveConstraints (clist, symbols);
    if (ok) {
      return true;
    }
  }
  return false;
}

void Twinner::codeTracesIntoTwinCode (
    const std::map < std::pair < ADDRINT, int >, UINT64 > &initialValues) {
  std::ofstream fileout;
  fileout.open (twin.c_str (), ios_base::out | ios_base::trunc);
  if (!fileout.is_open ()) {
    edu::sharif::twinner::util::Logger::error () << "Can not write twincode"
        " to file: Error in open function: "
        << twin << '\n';
    throw std::runtime_error ("Error in saving twincode in file");
  }
  edu::sharif::twinner::engine::etg::TwinCodeEncoder
      (traces, initialValues, ctree->getRoot ()).encodeToFile (fileout);
  fileout.close ();
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

const Logger &operator<< (const Logger &logger, LEVEL_BASE::REG reg) {
  return logger << (UINT32) reg;
}

edu::sharif::twinner::trace::cv::ConcreteValue *readRegisterContent (
    const CONTEXT *context, LEVEL_BASE::REG reg) {
  const char *msg =
      "The readRegisterContent(...) method is only supported in TwinTool:"
      " Calling error from Twinner.\n";
  edu::sharif::twinner::util::Logger::error () << msg;
  throw std::runtime_error (msg);
}

BOOL readMemoryContent (UINT64 &outValue, ADDRINT memoryEa, size_t size) {
  edu::sharif::twinner::util::Logger::error ()
      << "The readMemoryContent (...) function is only available in TwinTool:"
      " Calling error from Twinner.\n";
  return false;
}

BOOL readStringFromMemory (std::string &outValue, const ADDRINT memoryEa) {
  edu::sharif::twinner::util::Logger::error ()
      << "The readStringFromMemory (...) function is only available in"
      " TwinTool: Calling error from Twinner.\n";
  return false;
}

BOOL writeMemoryContent (ADDRINT memoryEa, const UINT8 *value, size_t size) {
  edu::sharif::twinner::util::Logger::error ()
      << "The writeMemoryContent (...) function is only available in TwinTool:"
      " Calling error from Twinner.\n";
  return false;
}

BOOL writeRegisterContent (CONTEXT *context,
    LEVEL_BASE::REG reg, const UINT8 *value) {
  edu::sharif::twinner::util::Logger::error ()
      << "The writeRegisterContent(...) method is only supported in TwinTool:"
      " Calling error from Twinner.\n";
  return false;
}

}
namespace trace {

edu::sharif::twinner::trace::Expression *lazy_load_symbolic_expression (
    edu::sharif::twinner::trace::ExecutionTraceSegment *me, int size,
    std::map < ADDRINT, edu::sharif::twinner::trace::Expression * > &map,
    const ADDRINT key,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteVal,
    edu::sharif::twinner::trace::StateSummary &state) {
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
