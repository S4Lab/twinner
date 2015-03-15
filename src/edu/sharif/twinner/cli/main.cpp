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

#include <iostream>
#include <iomanip>
#include <unistd.h>

#include "ArgParser.h"

#include "edu/sharif/twinner/engine/Twinner.h"
#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/engine/search/ConstraintTree.h"

#include "edu/sharif/twinner/util/Logger.h"

using namespace std;

using namespace edu::sharif::twinner::cli;

const char *VERSION_NUMBER = "0.14.0";

void printError (string progName, string errorMessage);
void printVersion ();
void printHelp (string progName, const ArgParser::Option options[]);
void printLicense ();

enum ArgumentsParsingStatus {

  CHECK_MODE,
  EXIT_NORMALLY, ERROR_OCCURRED, CONTINUE_NORMALLY
};

ArgumentsParsingStatus parseArguments (int argc, char *argv[],
    string &input, string &args, string &twintool, string &pin, string &twin,
    bool &justAnalyzeMainRoutine, bool &measureOverheads);
int run (string input, string args, string twintool, string pin, string twin,
    bool main, bool measureOverheads);
int checkTraceFile (string traceFilePath, string memoryFilePath);

int main (int argc, char *argv[]) {
  string input, args, twintool, pin, twin;
  bool justAnalyzeMainRoutine = false;
  bool measureOverheads = false;
  switch (parseArguments (argc, argv,
                          input, args, twintool, pin, twin,
                          justAnalyzeMainRoutine, measureOverheads)) {
  case CONTINUE_NORMALLY:
    // checking mandatory arguments...

    if (access (input.c_str (), R_OK) != 0) {
      printError (argv[0], "permission denied: can not read input binary file!");
    } else if (access (twintool.c_str (), X_OK) != 0) {
      printError (argv[0], "permission denied: can not execute twintool pintool!");
    } else if (access (pin.c_str (), X_OK) != 0) {
      printError (argv[0], "permission denied: can not execute pin launcher!");
    } else if (access (twin.c_str (), F_OK) == 0 && access (twin.c_str (), W_OK) != 0) {
      printError (argv[0], "permission denied: can not write to output twin binary!");
    } else {
      // all files are OK...
      return run (input, args, twintool, pin, twin,
                  justAnalyzeMainRoutine, measureOverheads);
    }
    return -2;

  case CHECK_MODE:
    if (access (input.c_str (), R_OK) != 0) {
      printError (argv[0], "permission denied: can not read the trace file!");
    } else if (access (args.c_str (), R_OK) != 0) {
      printError (argv[0], "permission denied: can not read the memory file!");
    } else {
      return checkTraceFile (input, args);
    }
    return -3;

  case EXIT_NORMALLY:
    return 0;
  default:
  case ERROR_OCCURRED:
    return -1;
  }
}

int run (string input, string args, string twintool, string pin, string twin,
    bool main, bool measureOverheads) {
  edu::sharif::twinner::util::Logger::info ()
      << "[verboseness level: "
      << edu::sharif::twinner::util::Logger::getVerbosenessLevelAsString () << "]\n"
      "Input binary file: " << input << '\n'
      << (args.empty () ? "" : ("Input binary arguments: " + args))
      << "\nTwinTool pintool: " << twintool
      << "\nPin launcher: " << pin
      << "\nOutput twin file: " << twin << '\n';

  edu::sharif::twinner::engine::Twinner tw;
  tw.setInputBinaryPath (input);
  tw.setTwinToolPath (twintool);
  tw.setPinLauncherPath (pin);
  tw.setTwinBinaryPath (twin);
  tw.setInputBinaryArguments (args);
  tw.setJustAnalyzeMainRoutine (main);
  tw.setMeasureOverheads (measureOverheads);

  tw.generateTwinBinary ();

  return 0;
}

int checkTraceFile (string traceFilePath, string memoryFilePath) {
  edu::sharif::twinner::util::Logger::info ()
      << "[verboseness level: "
      << edu::sharif::twinner::util::Logger::getVerbosenessLevelAsString () << "]\n"
      << "Trace file: " << traceFilePath << '\n'
      << "Memory file: " << memoryFilePath << '\n';

  edu::sharif::twinner::trace::Trace *trace =
      edu::sharif::twinner::trace::Trace::loadFromFile
      (traceFilePath.c_str (), memoryFilePath.c_str ());
  trace->printCompleteState (edu::sharif::twinner::util::Logger::info ());

  edu::sharif::twinner::engine::search::ConstraintTree ct;
  ct.addConstraints (trace);
  return 0;
}

ArgumentsParsingStatus parseArguments (int argc, char *argv[],
    string &input, string &args, string &twintool, string &pin, string &twin,
    bool &justAnalyzeMainRoutine, bool &measureOverheads) {
  char *progName = argv[0];

  const ArgParser::Option options[] = {
    { 'h', "help", ArgParser::NO, "display this help message and exit", false, true},
    { 'V', "version", ArgParser::NO, "output version number string and exit", false, true},
    { 'v', "verbose", ArgParser::YES, "verbose operation", false, false},
    { 'L', "license", ArgParser::NO, "output license information and exit", false, true},
    { 'i', "input", ArgParser::YES, "input obfuscated binary file", true, false},
    { 'a', "args", ArgParser::YES, "arguments for input binary file", false, false},
    { 't', "tool", ArgParser::YES, "twintool executable/library file", true, false},
    { 'p', "pin-launcher", ArgParser::YES, "path to the pin.sh launcher", true, false},
    { 'o', "output", ArgParser::YES, "path/name of the generated twin binary", true, false},
    { 'm', "main", ArgParser::NO, "restrict analysis to after the main() function", false, false},
    { 'M', "measure", ArgParser::NO, "run each trace twice and report on overheads of twintool", false, false},
    { 'c', "check", ArgParser::YES, "check validity of a trace file and its memory file"
     " (must be the last argument)", false, true},
    { 0, 0, ArgParser::NO, 0, false, false}
  };
  const ArgParser parser (argc, argv, options, true);
  if (parser.error ().size ()) {
    printError (progName, parser.error ());
    return ERROR_OCCURRED;
  }
  for (int argind = 0; argind < parser.arguments (); ++argind) {
    const int code = parser.code (argind);
    if (!code) { // no more options
      break;
    }
    switch (code) {
    case 'h':
      printHelp (progName, options);
      return EXIT_NORMALLY;
    case 'V':
      printVersion ();
      return EXIT_NORMALLY;
    case 'v':
      if (!edu::sharif::twinner::util::Logger::setVerbosenessLevel
          (parser.argument (argind))) {
        printError (progName, "undefined verboseness level: " + parser.argument (argind));
        return ERROR_OCCURRED;
      }
      break;
    case 'L':
      printLicense ();
      return EXIT_NORMALLY;
    case 'c':
      input = parser.argument (argind);
      if (++argind < parser.arguments ()) {
        args = parser.argument (argind);
        if (++argind < parser.arguments ()) {
          printError (progName, "The --check must be the last argument");
          return ERROR_OCCURRED;
        }
        return CHECK_MODE;
      }
      printError (progName, "The --check requires two arguments (trace and memory files)");
      return ERROR_OCCURRED;
    case 'i':
      input = parser.argument (argind);
      break;
    case 'a':
      args = parser.argument (argind);
      break;
    case 't':
      twintool = parser.argument (argind);
      break;
    case 'p':
      pin = parser.argument (argind);
      break;
    case 'o':
      twin = parser.argument (argind);
      break;
    case 'm':
      justAnalyzeMainRoutine = true;
      break;
    case 'M':
      measureOverheads = true;
      break;
    default:
      printError (progName, "Can not parse arguments!");
      return ERROR_OCCURRED;
    }
  }
  return CONTINUE_NORMALLY;
}

void printError (string progName, string errorMessage) {
  cerr << progName << ": error: " << errorMessage << endl;
  cerr << "Use --help for more details." << endl;
}

void printVersion () {
  cout << VERSION_NUMBER << endl;
}

void printHelp (string progName, const ArgParser::Option options[]) {
  cout << "Twinner: A Deobfuscator and unpacker which utilizes concolic execution." << endl;
  cout << "Copyright © 2013-2015  Behnam Momeni" << endl;
  cout << endl;
  cout << "Usage: " << progName << " [options]" << endl;
  cout << "Options:" << endl;
  for (int i = 0; options[i].code != 0; ++i) {
    switch (options[i].hasArg) {
    case ArgParser::NO:
      cout << "\t-" << (char) (options[i].code) << ", --" << left << setw (19)
          << (options[i].name) << (options[i].description) << endl;
      break;
    case ArgParser::YES:
      cout << "\t-" << (char) (options[i].code) << ", --" << left << setw (12)
          << (options[i].name) << " <arg> " << (options[i].description) << endl;
      break;
    case ArgParser::OPTIONAL:
      cout << "\t-" << (char) (options[i].code) << ", --" << left << setw (12)
          << (options[i].name) << " [arg] " << (options[i].description) << endl;
      break;
    }
  }
}

void printLicense () {
  cout << "Twinner: A Deobfuscator and unpacker which utilizes concolic execution." << endl;
  cout << "Copyright © 2013-2015  Behnam Momeni" << endl;
  cout << endl;
  cout << "Not implemented yet!!!! See COPYING file instead." << endl;
}
