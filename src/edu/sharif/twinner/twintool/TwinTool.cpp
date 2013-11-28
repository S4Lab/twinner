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
#include <list>
#include <stdexcept>

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

KNOB < BOOL > main (KNOB_MODE_WRITEONCE, "pintool", "main", "",
    "if presents, only main() routine and what is called by it will be analyzed");

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
        printError
            ("permission denied: can not read initial symbols: " + symbolsFilePath);
        return false;
      }
    } else { // file does not exist
      symbolsFilePath.clear ();
    }
  }
  if (traceFilePath.empty ()) {
    printError
        ("The execution trace must be saved somewhere. Use --trace to specify the path!");
    return false;
  }
  if (access (traceFilePath.c_str (), F_OK) == 0
      && access (traceFilePath.c_str (), W_OK) != 0) {
    printError
        ("permission denied: can not write to execution trace file: " + traceFilePath);
    return false;
  }
  // At the end, traceFilePath will be opened and execution trace will be saved in it.
  if (!edu::sharif::twinner::util::Logger::setVerbosenessLevel (verbose.Value ())) {
    printError ("undefined verboseness level: " + verbose.Value ());
    return false;
  }
  /*{
    // testing logger
    edu::sharif::twinner::util::Logger::error () << "Testing error messages\n";
    edu::sharif::twinner::util::Logger::warning () << "Testing warning messages\n";
    edu::sharif::twinner::util::Logger::info () << "Testing info messages\n";
    edu::sharif::twinner::util::Logger::debug () << "Testing debug messages\n";
    edu::sharif::twinner::util::Logger::loquacious () << "Testing loquacious messages\n";
  }*/
  bool justAnalyzeMainRoutine = main.Value ();
  if (justAnalyzeMainRoutine) {
    edu::sharif::twinner::util::Logger::info ()
        << "Only main() routine will be analyzed.\n";
  }
  im = new Instrumenter (symbolsFilePath, traceFilePath, justAnalyzeMainRoutine);
  return true;
}

void TwinTool::registerInstrumentationRoutines () {
  bool justAnalyzeMainRoutine = main.Value ();
  if (justAnalyzeMainRoutine) {
    /**
     * This is required for -main option. That option commands instrumentation to start
     * from the main() routine instead of RTLD start point. Finding the main() routine
     * requires symbols (so it's not reliable and is not recommended for real malwares).
     */
    PIN_InitSymbols ();

    IMG_AddInstrumentFunction ((IMAGECALLBACK) imageIsLoaded, im);
  }
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
namespace trace {

/*
 * Following functions implementations are specific to TwinTool and employ PIN calls.
 * Similar but more limited implementations are provided for Twinner.
 */

void throw_exception_about_unexpected_change_in_memory_or_register_address
(REG reg, UINT64 expectedVal, UINT64 currentVal) {
  char errorMessage[200];
  std::string addr = REG_StringShort (reg);
  snprintf (errorMessage, 200, "Value of an address changed unexpectedly"
            " without any interfering syscall\n"
            "\tExpected 0x%lX, Got 0x%lX; at register %s (code: 0x%X)",
            expectedVal, currentVal, addr.c_str (), (unsigned int) reg);
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
  return logger << "Reg(" << REG_StringShort (reg) << ")";
}

UINT64 readRegisterContent (const CONTEXT *context,
    REG reg) {
  REGVAL regVal;
  PIN_GetContextRegval (context, reg, &regVal);
  UINT64 value;
  PIN_ReadRegvalQWord (&regVal, &value, 0);
  return value;
}

UINT64 readMemoryContent (ADDRINT memoryEa) {
  UINT64 currentConcreteValue;
  PIN_SafeCopy (&currentConcreteValue, (const VOID *) (memoryEa), sizeof (UINT64));
  return currentConcreteValue;
}

VOID writeMemoryContent (ADDRINT memoryEa, UINT64 value) {
  PIN_SafeCopy ((VOID *) memoryEa, (const VOID *) &value, sizeof (value));
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
