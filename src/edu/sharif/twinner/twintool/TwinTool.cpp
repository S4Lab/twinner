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

#include "TwinTool.h"

#ifdef TARGET_LINUX
#include <unistd.h>
#endif
#ifdef TARGET_WINDOWS
#include "inc/unistd-win.h"
#endif
#include <fstream>
#include <list>
#include <stdexcept>
#include <string.h>
#include <stdlib.h>

#include "Instrumenter.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"

#include "edu/sharif/twinner/trace-twintool/FunctionInfo.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue128Bits.h"

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

KNOB < string > endpoints (KNOB_MODE_WRITEONCE, "pintool", "endpoints", "",
    "comma separated instruction addresses to start/end analysis");

KNOB < string > safeFunctions (KNOB_MODE_WRITEONCE, "pintool", "safe-functions", "",
    "comma separated functions to be preserved");

KNOB < string > mainArgsReportingOutputFilePath (KNOB_MODE_WRITEONCE,
    "pintool", "mar", "/tmp/twinner/main-args-reporting.dat",
    "specify file path for saving main() arguments information"
    " (in -main and -endpoints modes");

KNOB < int > stackOffset (KNOB_MODE_WRITEONCE,
    "pintool", "stack-offset", "0",
    "the stack offset (in terms of the number of arguments) for main() args"
    " (in -main and -endpoints modes");

KNOB < BOOL > naive (KNOB_MODE_WRITEONCE, "pintool", "naive", "",
    "if presents, just print info about instructions with no instrumentation");

KNOB < BOOL > measure (KNOB_MODE_WRITEONCE, "pintool", "measure", "",
    "if presents, trivial instruction counting instrumentation will be used instead of normal behavior");

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

  im->registerInstrumentationRoutines ();

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
  string mainArgsReportingFilePath = mainArgsReportingOutputFilePath.Value ();
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
  const bool naiveMode = naive.Value ();
  if (naiveMode) {
    edu::sharif::twinner::util::Logger::info ()
        << "Naive mode: just print info with no instrumentation.\n";
  }
  justAnalyzeMainRoutine = main.Value ();
  if (justAnalyzeMainRoutine) {
    edu::sharif::twinner::util::Logger::info ()
        << "Only main() routine will be analyzed.\n";
  }
  string endpointsStr = endpoints.Value ();
  ADDRINT start = 0, end = 0;
  if (endpointsStr != "") {
    const std::string::size_type separator = endpointsStr.find (",");
    if (separator == std::string::npos) {
      edu::sharif::twinner::util::Logger::error ()
          << "Analysis endpoints are not well formed.\n";
      return false;
    }
    std::stringstream startStr (endpointsStr.substr (0, separator));
    startStr >> std::hex >> start;
    std::stringstream endStr (endpointsStr.substr (separator + 1));
    endStr >> std::hex >> end;
    edu::sharif::twinner::util::Logger::info ()
        << "Analysis endpoints are specified:"
        " 0x" << std::hex << start << " - 0x" << end << '\n';
  }
  string safeFunctionsStr = safeFunctions.Value ();
  vector<edu::sharif::twinner::trace::FunctionInfo> safeFunctionsInfo;
  if (safeFunctionsStr != "") {
    safeFunctionsStr += ",";
    std::string::size_type last = 0;
    for (std::string::size_type separator = safeFunctionsStr.find (",");
        separator != std::string::npos;
        separator = safeFunctionsStr.find (",", last = separator + 1)) {
      if (separator == last) {
        edu::sharif::twinner::util::Logger::error ()
            << "Safe functions string is not well formed.\n";
        return false;
      }
      safeFunctionsInfo.push_back
          (edu::sharif::twinner::trace::FunctionInfo
           (safeFunctionsStr.substr (last, separator - last)));
    }
    edu::sharif::twinner::util::Logger logger =
        edu::sharif::twinner::util::Logger::info ();
    logger << "Safe functions are:";
    for (vector<edu::sharif::twinner::trace::FunctionInfo>
        ::const_iterator it = safeFunctionsInfo.begin ();
        it != safeFunctionsInfo.end (); ++it) {
      logger << ' ' << *it;
    }
    logger << '\n';
  }
  if (justAnalyzeMainRoutine || start != end) {
    if (mainArgsReportingFilePath.empty ()) {
      printError ("In the --main and --endpoints modes,"
                  " main() arguments information must be saved somewhere."
                  " Use --mar to specify the path!");
      return false;
    }
    if (access (mainArgsReportingFilePath.c_str (), F_OK) == 0
        && access (mainArgsReportingFilePath.c_str (), W_OK) != 0) {
      printError ("permission denied: can not write to mar file: "
                  + mainArgsReportingFilePath);
      return false;
    }
    justAnalyzeMainRoutine = true;
  }
  const int stackOffsetValue = stackOffset.Value ();
  bool measureMode = measure.Value ();
  if (measureMode) {
    edu::sharif::twinner::util::Logger::info () << "Measure mode: "
        "instruction counting instrumentation will be used instead of normal behavior.\n";
  }
  if (hasSymbolsInputFile) {
    std::ifstream in;
    openFileForReading (in, symbolsFilePath);
    ExecutionMode mode = readExecutionModeFromBinaryStream (in);
    switch (mode) {
    case NORMAL_MODE:
      im = new Instrumenter (in, traceFilePath, disassemblyFilePath,
                             justAnalyzeMainRoutine, stackOffsetValue,
                             start, end, safeFunctionsInfo,
                             naiveMode, measureMode);
      break;
    case INITIAL_STATE_DETECTION_MODE:
      if (measureMode) {
        edu::sharif::twinner::util::Logger::error ()
            << "TwinTool::parseArgumentsAndInitializeTool (): "
            "measure and initial-state-detection modes "
            "cannot be enabled simultaneously\n";
        return false;
      }
      if (naiveMode) {
        edu::sharif::twinner::util::Logger::error ()
            << "INITIAL_STATE_DETECTION_MODE conflicts with naive mode.\n";
        return false;
      }
      im = new Instrumenter (readSetOfAddressesFromBinaryStream (in),
                             traceFilePath, disassemblyFilePath,
                             justAnalyzeMainRoutine, stackOffsetValue,
                             start, end);
      break;
    default:
      in.close ();
      edu::sharif::twinner::util::Logger::error ()
          << "TwinTool::parseArgumentsAndInitializeTool (): "
          "Unknown execution mode\n";
      return false;
    }
    in.close ();
  } else {
    im = new Instrumenter (traceFilePath, disassemblyFilePath,
                           justAnalyzeMainRoutine, stackOffsetValue,
                           start, end, safeFunctionsInfo, naiveMode);
  }
  if (justAnalyzeMainRoutine) { // this includes  {|| start != end} scenario
    im->setMainArgsReportingFilePath (mainArgsReportingFilePath);
  }
  return true;
}

void TwinTool::openFileForReading (std::ifstream &in, const std::string &path) const {
  in.open (path.c_str (), ios_base::in | ios_base::binary);
  if (!in.is_open ()) {
    edu::sharif::twinner::util::Logger::error ()
        << "TwinTool::openFileForReading (in=..., path=" << path << "):"
        " Can not read from binary file due to error in open function\n";
    abort ();
  }
}

TwinTool::ExecutionMode TwinTool::readExecutionModeFromBinaryStream (
    std::ifstream &in) const {
  char magicString[3];
  in.read (magicString, 3);
  if (strncmp (magicString, "SYM", 3) != 0) {
    edu::sharif::twinner::util::Logger::error ()
        << "TwinTool::readExecutionModeFromBinaryStream (...): Unexpected "
        "magic string while reading symbols input file from binary stream\n";
    abort ();
  }
  ExecutionMode mode;
  in.read ((char *) &mode, sizeof (mode));
  return mode;
}

std::set < std::pair < ADDRINT, int > > TwinTool::readSetOfAddressesFromBinaryStream (
    std::ifstream &in) const {
  std::set < std::pair < ADDRINT, int > > addresses;
  std::set < std::pair < ADDRINT, int > >::size_type s;
  in.read ((char *) &s, sizeof (s));

  repeat (s) {
    ADDRINT address;
    int size;
    in.read ((char *) &address, sizeof (address));
    in.read ((char *) &size, sizeof (size));
    addresses.insert (make_pair (address, size));
  }
  return addresses;
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

const Logger &operator<< (const Logger &logger, LEVEL_BASE::REG reg) {
  return logger << "Reg(" << REG_StringShort (reg) << ")";
}

edu::sharif::twinner::trace::cv::ConcreteValue *readRegisterContent (
    const CONTEXT *context, LEVEL_BASE::REG reg) {
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
  case 16:
    return new edu::sharif::twinner::trace::cv::ConcreteValue128Bits
        (buffer.qword[1], buffer.qword[0]);
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "util::readRegisterContent (...) function"
        " [size=" << std::dec << REG_Size (reg) << "]:"
        " size of the requested register is unsupported\n";
    abort ();
  }
  return edu::sharif::twinner::trace::cv::ConcreteValue64Bits (value).clone
      (REG_Size (reg) * 8);
}

BOOL readMemoryContent (UINT64 &outValue, ADDRINT memoryEa, size_t size) {
  edu::sharif::twinner::util::Logger::loquacious () << "readMemoryContent: 0x"
      << std::hex << memoryEa << " / size: 0x" << size << " bytes\n";
  outValue = 0;
  const size_t ret = PIN_SafeCopy (&outValue, (const VOID *) (memoryEa), size);
  if (ret != size) {
    edu::sharif::twinner::util::Logger::error () << "readMemoryContent(...): "
        "trying to read " << std::dec << size << " bytes, but PIN_SafeCopy "
        "read " << ret << " bytes\n";
    return false;
  }
  if (size < 8) {
    outValue &= (1ull << (size * 8)) - 1;
  }
  return true;
}

/// reads an string from the memory upto 1000 bytes

BOOL readStringFromMemory (std::string &outValue, const ADDRINT memoryEa) {
  edu::sharif::twinner::util::Logger::loquacious () << "readStringFromMemory:"
      " 0x" << std::hex << memoryEa << '\n';
#ifdef TARGET_IA32E
  const int step = 8; // bytes
#else
  const int step = 4; // bytes
#endif
  size_t copiedBytes = step;
  const char * const initialAddress = (const char *) memoryEa;
  for (const char *address = initialAddress;
      copiedBytes == step && address < initialAddress + 1000;
      address += step) {
    UINT64 value = 0;
    copiedBytes = PIN_SafeCopy (&value, address, step);
    const char *bytes = (const char *) &value;
    for (size_t i = 0; i < copiedBytes; ++i) {
      const char c = bytes[i];
      if (c == 0) {
        return true;
      }
      outValue += c;
    }
  }
  return false;
}

BOOL writeMemoryContent (ADDRINT memoryEa, const UINT8 *value, size_t size) {
  const size_t ret = PIN_SafeCopy ((VOID *) memoryEa, (const VOID *) value, size);
  if (ret != size) {
    return false;
  }
  return true;
}

BOOL writeRegisterContent (CONTEXT *context,
    LEVEL_BASE::REG reg, const UINT8 *value) {
  PIN_SetContextRegval (context, reg, value);
  return true;
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
