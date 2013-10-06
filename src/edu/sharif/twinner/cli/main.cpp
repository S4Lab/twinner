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

#include <iostream>
#include <iomanip>

#include "ArgParser.h"

using namespace std;

using namespace edu::sharif::twinner::cli;

const char *VERSION_NUMBER = "0.1.0";

void printError (string progName, string errorMessage);
void printVersion ();
void printHelp (string progName, bool verbose);
void printLicense (bool verbose);

int main (int argc, char *argv[]) {
  bool verbose = false;
  char *progName = argv[0];

  const ArgParser::Option options[] = { //
      //
          { 'h', "help", ArgParser::NO }, //
          { 'V', "version", ArgParser::NO }, //
          { 'v', "verbose", ArgParser::NO }, //
          { 'L', "license", ArgParser::NO }, //
          { 0, 0, ArgParser::NO } //
      };

  const ArgParser parser (argc, argv, options);
  if (parser.error ().size ()) {
    printError (progName, parser.error ());
    return -1;
  }
  for (int argind = 0; argind < parser.arguments (); ++argind) {
    const int code = parser.code (argind);
    if (!code) { // no more options
      break;
    }
    switch (code) {
    case 'V':
      printVersion ();
      return 0;
    case 'h':
      printHelp (progName, verbose);
      return 0;
    case 'v':
      verbose = true;
      break;
    case 'L':
      printLicense (verbose);
      return 0;
    default:
      printError (progName, "Can not parse arguments!");
      return -1;
    }
  }

  return 0;
}

void printError (string progName, string errorMessage) {
  cerr << progName << ": error: " << errorMessage << endl;
  cerr << "Use --help for more details." << endl;
}

void printVersion () {
  cout << VERSION_NUMBER << endl;
}

#define MY_HELP_PRINT_LINE(OPTION, DESCRIPTION) \
  cout << "\t" << left << setw (16) << (OPTION) << (DESCRIPTION) << endl

void printHelp (string progName, bool verbose) {
  cout << "Twinner: An unpacker which utilizes concolic execution." << endl;
  cout << "Copyright © 2013  Behnam Momeni" << endl;
  cout << endl;
  cout << "Usage: " << progName << " [options]" << endl;
  cout << "Options:" << endl;
  MY_HELP_PRINT_LINE("-h, --help", "display this help message and exit");
  MY_HELP_PRINT_LINE("-V, --version", "output version number string and exit");
  MY_HELP_PRINT_LINE("-v, --verbose", "verbose operation");
  MY_HELP_PRINT_LINE("-L, --license", "output license information and exit");
}

void printLicense (bool verbose) {
  cout << "Twinner: An unpacker which utilizes concolic execution." << endl;
  cout << "Copyright © 2013  Behnam Momeni" << endl;
  cout << endl;
  cout << "Not implemented yet!!!! See COPYING file instead." << endl;
}
