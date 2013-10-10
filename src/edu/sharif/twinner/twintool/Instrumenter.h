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

#ifndef INSTRUMENTER_H
#define INSTRUMENTER_H

#include "pin.H"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

class Instrumenter {
private:
  bool verbose;
  std::string symbolsFilePath; // read initial symbols from this file
  std::string traceFilePath; // save final execution trace into this file

public:
  Instrumenter (std::string symbolsFilePath, std::string traceFilePath, bool verbose);

  void instrumentSingleInstruction ();

  void syscallEntryPoint ();
  void syscallExitPoint ();

  void aboutToExit ();
};

VOID instrumentSingleInstruction (INS ins, VOID *v);
VOID syscallIsAboutToBeCalled (THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std,
    VOID *v);
VOID syscallIsReturned (THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std,
    VOID *v);
VOID applicationIsAboutToExit (INT32 code, VOID *v);

}
}
}
}

#endif /* Instrumenter.h */
