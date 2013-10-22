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

#include "TwinTool.h"

#include <unistd.h>
#include <fstream>

#include "Instrumenter.h"

#include "edu/sharif/twinner/util/Logger.h"

using namespace std;

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

KNOB < string > symbolsInputFilePath (KNOB_MODE_WRITEONCE, "pintool", "symbols",
    "/tmp/twinner/symbols.dat", //
    "specify file path for loading initial symbols values");

KNOB < string > traceOutputFilePath (KNOB_MODE_WRITEONCE, "pintool", "trace",
    "/tmp/twinner/trace.dat", //
    "specify file path for saving execution trace");

KNOB < string > verbose (KNOB_MODE_WRITEONCE, "pintool", "verbose", "warning",
    "specify the level of verboseness: { quiet, error, warning, info, debug }");

TwinTool::TwinTool () :
    im (0) {
}

TwinTool::~TwinTool () {
  if (im) {
    delete im;
  }
}

INT32 TwinTool::run (int argc, char *argv[]) {
  /*
   * Initialize PIN library. Print help message if -h(elp) is specified
   * in the command line or the command line is invalid.
   */
  if (PIN_Init (argc, argv)) {
    return printUsage ();
  }
  // AT&T syntax is the one which is used by GDB
  //PIN_SetSyntaxATT ();
  // But order of operands matches with Intel syntax (destination on the left side). So this mode is better for debugging.
  PIN_SetSyntaxIntel ();

  if (!parseArgumentsAndInitializeTool ()) {
    return -2;
  }

  registerInstrumentationRoutines ();

  // Start the program, never returns
  PIN_StartProgram ();

  return 0;
}

/**
 * @return true iff arguments are parsed without any problem.
 */
bool TwinTool::parseArgumentsAndInitializeTool () {
  string symbolsFilePath = symbolsInputFilePath.Value ();
  string traceFilePath = traceOutputFilePath.Value ();
  if (!symbolsFilePath.empty ()) { // optional
    if (access (symbolsFilePath.c_str (), F_OK) == 0) { // optional
      if (access (symbolsFilePath.c_str (), R_OK) != 0) {
        printError (
            "permission denied: can not read initial symbols: " + symbolsFilePath);
        return false;
      }
    } else { // file does not exist
      symbolsFilePath.clear ();
    }
  }
  if (traceFilePath.empty ()) {
    printError (
        "The execution trace must be saved somewhere. Use --trace to specify the path!");
    return false;
  }
  if (access (traceFilePath.c_str (), F_OK) == 0
      && access (traceFilePath.c_str (), W_OK) != 0) {
    printError (
        "permission denied: can not write to execution trace file: " + traceFilePath);
    return false;
  }
  // At the end, traceFilePath will be opened and execution trace will be saved in it.
  if (!edu::sharif::twinner::util::Logger::setVerbosenessLevel (verbose.Value ())) {
    printError ("undefined verboseness level: " + verbose.Value ());
    return false;
  }
  im = new Instrumenter (symbolsFilePath, traceFilePath);
  return true;
}

void TwinTool::registerInstrumentationRoutines () {
  //TODO: Consider instrumenting at higher granularity for more performance
  INS_AddInstrumentFunction (instrumentSingleInstruction, im);

  PIN_AddSyscallEntryFunction (syscallIsAboutToBeCalled, im);
  PIN_AddSyscallExitFunction (syscallIsReturned, im);

  PIN_AddFiniFunction (applicationIsAboutToExit, im);
}

template < typename T >
INT32 TwinTool::printError (const T &msg) const {
  edu::sharif::twinner::util::Logger::error () << "TwinTool: " << msg << '\n';
  return -2;
}

INT32 TwinTool::printUsage () const {
  edu::sharif::twinner::util::Logger::error ()
      << "This tool is not intended to be ran manually. Run it using \"Twinner\" application!\n\n"

      << KNOB_BASE::StringKnobSummary () << '\n';

  return -1;
}

}
}
}
}

/**
 * The main function for the TwinTool pintool.
 *
 * @param argc total number of elements in the argv array
 * @param argv array of command line arguments,
 *             including pin -t <toolname> -- ...
 */
int main (int argc, char *argv[]) {
  edu::sharif::twinner::twintool::TwinTool tt;

  return tt.run (argc, argv);
}
