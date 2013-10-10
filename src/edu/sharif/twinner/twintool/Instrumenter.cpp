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

#include "Instrumenter.h"

#include <iostream>

using namespace std;

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

Instrumenter::Instrumenter (string _symbolsFilePath, string _traceFilePath, bool _verbose) :
    verbose (_verbose), symbolsFilePath (_symbolsFilePath), traceFilePath (_traceFilePath) {
  if (verbose) {
    cout << "Instrumenter class created [verbose mode]" << endl;
  }
}

void Instrumenter::instrumentSingleInstruction () {
  throw "Not yet implemented";
}

void Instrumenter::syscallEntryPoint () {
  throw "Not yet implemented";
}

void Instrumenter::syscallExitPoint () {
  throw "Not yet implemented";
}

void Instrumenter::aboutToExit () {
  throw "Not yet implemented";
}

VOID instrumentSingleInstruction (INS ins, VOID *v) {
  Instrumenter *im = (Instrumenter *) v;
  im->instrumentSingleInstruction ();
}

VOID syscallIsAboutToBeCalled (THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std,
    VOID *v) {
  Instrumenter *im = (Instrumenter *) v;
  im->syscallEntryPoint ();
}

VOID syscallIsReturned (THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std,
    VOID *v) {
  Instrumenter *im = (Instrumenter *) v;
  im->syscallExitPoint ();
}

VOID applicationIsAboutToExit (INT32 code, VOID *v) {
  Instrumenter *im = (Instrumenter *) v;
  im->aboutToExit ();
}

}
}
}
}
