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

#include <string>
#include <map>

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

class InstructionSymbolicExecuter;

class Instrumenter {
private:
  std::string symbolsFilePath; // read initial symbols from this file
  std::string traceFilePath; // save final execution trace into this file

  InstructionSymbolicExecuter *ise;

public:
  Instrumenter (std::string symbolsFilePath, std::string traceFilePath);
  ~Instrumenter ();

  void instrumentSingleInstruction (INS ins);

  void syscallEntryPoint (THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std);
  void syscallExitPoint (THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std);

  void aboutToExit (INT32 code);

private:
  typedef void (Instrumenter::*instrumentationMethod) (INS ins);

  void instrumentMOVInstruction (INS ins);
  void instrumentPUSHInstruction (INS ins);
  void instrumentPOPInstruction (INS ins);

  std::map < OPCODE, instrumentationMethod > instrumentationMethods;
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
