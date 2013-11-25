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
#include "edu/sharif/twinner/trace/Symbol.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"

using namespace std;

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {

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
 * tracked) at the end in initial-value-finding mode and look for values of those symbols
 * before first syscall. It's possible that a later trace, removes a symbol from the set
 * of non-initialized-in-1st-segment symbols. So search for those symbols just at the end
 * and then do that last/final run to look for their pre-1st-syscall values,
 */
void Twinner::generateTwinBinary () {
  Executer ex (pin, twintool, input);
  set < edu::sharif::twinner::trace::Symbol * > symbols;
  bool somePathsAreNotCovered = true;
  int i = 1;
  while (somePathsAreNotCovered) {
    edu::sharif::twinner::util::Logger::debug () << "Executing trace # " << i++ << '\n';
    ex.setSymbolsValues (symbols);

    edu::sharif::twinner::trace::Trace *trace = ex.executeSingleTrace ();
    addExecutionTrace (trace);

    for (set < edu::sharif::twinner::trace::Symbol * >::iterator it = symbols.begin ();
        it != symbols.end (); ++it) {
      delete *it;
    }
    symbols.clear ();
    somePathsAreNotCovered = calculateSymbolsValuesForCoveringNextPath (*trace, symbols);
  }
  codeTracesIntoTwinBinary ();
}

void Twinner::addExecutionTrace (const edu::sharif::twinner::trace::Trace *trace) {
  edu::sharif::twinner::util::Logger log = edu::sharif::twinner::util::Logger::debug ();
  log << "Adding execution trace:\n";
  trace->printCompleteState (log);
  traces.push_back (trace);
}

bool Twinner::calculateSymbolsValuesForCoveringNextPath (
    const edu::sharif::twinner::trace::Trace &trace,
    set < edu::sharif::twinner::trace::Symbol * > &symbols) {
  return false;
}


void Twinner::codeTracesIntoTwinBinary () {
  std::stringstream out;
  out << '\n';

  for_each_lst_const (const edu::sharif::twinner::trace::Trace,
                      traces, trace) {

    for_each_lst (edu::sharif::twinner::trace::ExecutionTraceSegment,
                  trace->getTraceSegments (), segment) {
      int depth = 1;

      for_each_lst_const (const edu::sharif::twinner::trace::Constraint,
                          segment->getPathConstraints (), constraint) {

        repeat (depth) {
          out << '\t';
        }
        out << "if (" << constraint->toString () << ") {\n";
        ++depth;
      }
      for (int j = depth - 1; j > 0; --j) {

        repeat (j) {
          out << '\t';
        }
        out << "}\n";
      }
    }
  }
  edu::sharif::twinner::util::Logger::info () << out.str ();
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
