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

#include "TwinTool.h"

#include <unistd.h>
#include <fstream>
#include <list>
#include <stdexcept>
#include <string.h>

#include "Instrumenter.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"
#include "edu/sharif/twinner/trace/ConcreteValue64Bits.h"

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

KNOB < string > disassemblyOutputFilePath (KNOB_MODE_WRITEONCE, "pintool", "memory",
    "/tmp/twinner/memory.dat", //
    "specify file path for saving disassembled instructions memory");

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
  bool hasSymbolsInputFile = false;
  string symbolsFilePath = symbolsInputFilePath.Value ();
  string traceFilePath = traceOutputFilePath.Value ();
  string disassemblyFilePath = disassemblyOutputFilePath.Value ();
  if (!symbolsFilePath.empty ()) { // optional
    if (access (symbolsFilePath.c_str (), F_OK) == 0) { // optional
      if (access (symbolsFilePath.c_str (), R_OK) != 0) {
        printError
            ("permission denied: can not read initial symbols: " + symbolsFilePath);
        return false;
      }
      hasSymbolsInputFile = true;
    } else { // file does not exist
      symbolsFilePath.clear ();
    }
  }
  if (traceFilePath.empty ()) {
    printError ("The execution trace must be saved somewhere."
                " Use --trace to specify the path!");
    return false;
  }
  if (disassemblyFilePath.empty ()) {
    printError ("Disassembled instructions must be saved somewhere."
                " Use --memory to specify the path!");
    return false;
  }
  if (access (traceFilePath.c_str (), F_OK) == 0
      && access (traceFilePath.c_str (), W_OK) != 0) {
    printError ("permission denied: can not write to execution trace file: "
                + traceFilePath);
    return false;
  }
  if (access (disassemblyFilePath.c_str (), F_OK) == 0
      && access (disassemblyFilePath.c_str (), W_OK) != 0) {
    printError ("permission denied: can not write to disassembly memory file: "
                + disassemblyFilePath);
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
  if (hasSymbolsInputFile) {
    std::ifstream in;
    openFileForReading (in, symbolsFilePath);
    ExecutionMode mode = readExecutionModeFromBinaryStream (in);
    switch (mode) {
    case NORMAL_MODE:
      im = new Instrumenter (in, traceFilePath, disassemblyFilePath,
                             justAnalyzeMainRoutine);
      break;
    case INITIAL_STATE_DETECTION_MODE:
      im = new Instrumenter (readSetOfAddressesFromBinaryStream (in),
                             traceFilePath, disassemblyFilePath, justAnalyzeMainRoutine);
      break;
    default:
      in.close ();
      throw std::runtime_error ("Unknown execution mode");
    }
    in.close ();
  } else {
    im = new Instrumenter (traceFilePath, disassemblyFilePath, justAnalyzeMainRoutine);
  }
  return true;
}

void TwinTool::openFileForReading (std::ifstream &in, const std::string &path) const {
  in.open (path.c_str (), ios_base::in | ios_base::binary);
  if (!in.is_open ()) {
    edu::sharif::twinner::util::Logger::error () << "Can not read from binary file:"
        " Error in open function: " << path << '\n';
    throw std::runtime_error ("Can not read from binary file");
  }
}

TwinTool::ExecutionMode TwinTool::readExecutionModeFromBinaryStream (
    std::ifstream &in) const {
  char magicString[3];
  in.read (magicString, 3);
  if (strncmp (magicString, "SYM", 3) != 0) {
    throw std::runtime_error
        ("Unexpected magic string while reading symbols input file from binary stream");
  }
  ExecutionMode mode;
  in.read ((char *) &mode, sizeof (mode));
  return mode;
}

std::set < ADDRINT > TwinTool::readSetOfAddressesFromBinaryStream (
    std::ifstream &in) const {
  std::set < ADDRINT > addresses;
  std::set < ADDRINT >::size_type s;
  in.read ((char *) &s, sizeof (s));

  repeat (s) {
    ADDRINT address;
    in.read ((char *) &address, sizeof (address));
    addresses.insert (address);
  }
  return addresses;
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
namespace util {

/*
 * Following functions implementations are specific to TwinTool and employ PIN calls.
 * Similar but more limited implementations are provided for Twinner.
 */

const Logger &operator<< (const Logger &logger, REG reg) {
  return logger << "Reg(" << REG_StringShort (reg) << ")";
}

edu::sharif::twinner::trace::ConcreteValue *readRegisterContent (
    const CONTEXT *context, REG reg) {
  /// XXX: Only full-size registers (e.g. RAX, RSP) are safe to be read
  PIN_REGISTER buffer;
  PIN_GetContextRegval (context, REG_FullRegName (reg), buffer.byte);
  UINT64 value;
  switch (REG_Size (reg)) {
  case 1:
    if (REG_is_Lower8 (reg)) { // e.g. AL
      value = buffer.byte[0]; // Little Endian
      break;
    } else { // e.g. AH
      value = buffer.byte[1]; // Little Endian
      break;
    }
  case 2:
    value = buffer.word[0];
    break;
  case 4:
    value = buffer.dword[0];
    break;
  case 8:
    value = buffer.qword[0];
    break;
  default:
    throw std::runtime_error ("util::readRegisterContent (...) method: "
                              "Size of requested register is unsupported.");
  }
  return new edu::sharif::twinner::trace::ConcreteValue64Bits (value);
}

UINT64 readMemoryContent (ADDRINT memoryEa) {
  UINT64 currentConcreteValue;
  PIN_SafeCopy (&currentConcreteValue, (const VOID *) (memoryEa), sizeof (UINT64));
  return currentConcreteValue;
}

VOID writeMemoryContent (ADDRINT memoryEa, const UINT64 value) {
  PIN_SafeCopy ((VOID *) memoryEa, (const VOID *) &value, sizeof (value));
}

VOID writeMemoryContent (ADDRINT memoryEa, const UINT8 *value, size_t size) {
  PIN_SafeCopy ((VOID *) memoryEa, (const VOID *) value, size);
}

VOID writeRegisterContent (CONTEXT *context, REG reg, const UINT8 *value) {
  PIN_SetContextRegval (context, reg, value);
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
