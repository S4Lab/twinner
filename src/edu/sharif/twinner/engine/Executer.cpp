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

#include "Executer.h"

#include <stdlib.h>

#include "edu/sharif/twinner/trace/Trace.h"

using namespace std;

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {

Executer::Executer (string pinLauncher, string inputBinary, bool _verbose) :
    verbose (_verbose), command (pinLauncher + " -t TwinTool.so -- " + inputBinary) {
}

void Executer::setSymbolsValues (
    const set < edu::sharif::twinner::trace::Symbol > &symbols) {
  throw "Not yet implemented";
}

/**
 * Uses OS interface to run  twintool in an independent process. The execution
 * trace will be communicated with twintool through file interface.
 * @return The execution trace, recorded by twintool
 */
edu::sharif::twinner::trace::Trace *Executer::executeSingleTrace () {
  /*
   *  TODO: Run command through another thread and set a timeout for execution.
   *  Also tune twintool, so it saves its progress incrementally. After a timeout,
   *  partial trace can be read and twintool will be killed. Malwares are not
   *  required to finish execution! Another possible approach is to tune twintool
   *  to timeout execution and exit after a while. In this way, this code does not
   *  need to change at all.
   */
  if (verbose) {
    cout << "Calling system (\"" << command << "\");" << endl;
  }
  int ret = system (command.c_str ());
  if (verbose) {
    cout << "The system(...) call returns code: " << ret << endl;
  }
  return edu::sharif::twinner::trace::Trace::loadFromFile ("/tmp/twinner/trace.dat");
}

}
}
}
}
