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
#include <iostream>
#include <fstream>

#include "Instrumenter.h"

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

KNOB < BOOL > verbose (KNOB_MODE_WRITEONCE, "pintool", "verbose", "",
    "enables verbose output");

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
  }
  // At the end, traceFilePath will be opened and execution trace will be saved in it.
  im = new Instrumenter (symbolsFilePath, traceFilePath, verbose.Value ());
  return true;
}

void TwinTool::registerInstrumentationRoutines () {
  //TODO: Consider instrumenting at higher granularity for more performance
  INS_AddInstrumentFunction (instrumentSingleInstruction, im);

  PIN_AddSyscallEntryFunction (syscallIsAboutToBeCalled, im);
  PIN_AddSyscallExitFunction (syscallIsReturned, im);

  PIN_AddFiniFunction (applicationIsAboutToExit, im);
}

INT32 TwinTool::printError (string msg) const {
  cerr << "TwinTool: " << msg << endl;
  return -2;
}

INT32 TwinTool::printError (const char *msg) const {
  cerr << "TwinTool: " << msg << endl;
  return -2;
}

INT32 TwinTool::printUsage () const {
  cerr
      << "This tool is not intended to be ran manually. Run it using \"Twinner\" application!"
      << endl << endl;

  cerr << KNOB_BASE::StringKnobSummary () << endl;

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
