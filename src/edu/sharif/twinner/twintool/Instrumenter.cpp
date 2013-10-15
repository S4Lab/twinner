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
#include <stdexcept>

#include "xed-iclass-enum.h"

#include "InstructionSymbolicExecuter.h"

using namespace std;

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

Instrumenter::Instrumenter (string _symbolsFilePath, string _traceFilePath,
    VerbosenessLevel _verbose) :
    verbose (_verbose), symbolsFilePath (_symbolsFilePath), traceFilePath (
        _traceFilePath), ise (new InstructionSymbolicExecuter ()) {
  if (verbose == INFO) {
    cout << "Instrumenter class created [verboseness level: info]" << endl;
  } else if (verbose == DEBUG) {
    cout << "Instrumenter class created [verboseness level: debug]" << endl;
  }
}

Instrumenter::~Instrumenter () {
  delete ise;
}

static int countOfMemoryReadInstructions = 0;
static int countOfMemoryWriteInstructions = 0;
static int countOfMOVInstructions = 0;
static int totalCountOfInstructions = 0;

void Instrumenter::instrumentSingleInstruction (INS ins) {
  if (verbose == DEBUG) {
    bool isMemoryRead = INS_IsMemoryRead (ins);
    bool isMemoryWrite = INS_IsMemoryWrite (ins);
    cout << "Instrumenting assembly instruction: " << INS_Disassemble (ins) << endl;
    cout << "\t--> Count of memory operands: " << INS_MemoryOperandCount (ins) << endl;
    if (isMemoryRead) {
      cout << "\t--> Reading from memory" << endl;
      countOfMemoryReadInstructions++;
    }
    if (isMemoryWrite) {
      cout << "\t--> Writing to memory" << endl;
      countOfMemoryWriteInstructions++;
    }
    bool isOriginal = INS_IsOriginal (ins);
    if (!isOriginal) {
      cout << "\t--> NON-ORIGINAL instruction!" << endl;
    } else {
      totalCountOfInstructions++;
    }
  }
  switch (INS_Opcode (ins)) { // Intel has 1024 opcodes. And each opcode has several types/models :)
  case XED_ICLASS_MOV: // MOV instruction -- 5 models
    bool destIsReg = INS_OperandIsReg (ins, 0);
    bool destIsMem = INS_OperandIsMemory (ins, 0);

    bool sourceIsReg = INS_OperandIsReg (ins, 1);
    bool sourceIsMem = INS_OperandIsMemory (ins, 1);
    bool sourceIsImmed = INS_OperandIsImmediate (ins, 1);

    if (verbose == DEBUG) {
      cout << "\t--> MOV instruction" << endl;
      countOfMOVInstructions++;
      cout << "\t--> op0 is " << (destIsMem ? "mem" : (destIsReg ? "reg" : "unknown"))
          << endl;
      cout << "\t--> op1 is "
          << (sourceIsMem ? "mem" :
              (sourceIsReg ? "reg" : (sourceIsImmed ? "immediate" : "unknown"))) << endl;
    }
    if (destIsReg && sourceIsMem) { // read from memory, e.g. mov eax, dword ptr [rbp-0x8]
      INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) movToRegisterFromMemoryAddress,
          IARG_PTR, ise, IARG_UINT32, INS_OperandReg (ins, 0), IARG_MEMORYOP_EA, 0,
          IARG_END);

    } else if (destIsMem && sourceIsReg) { // write to memory, e.g. mov dword ptr [rbp-0x10], eax
      REG reg = INS_OperandReg (ins, 1);
      INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) movToMemoryAddressFromRegister,
          IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_UINT32, reg, IARG_REG_VALUE, reg,
          IARG_END);

    } else if (destIsMem && sourceIsImmed) { // write immediate to memory, e.g. mov dword ptr [rbp-0xc], 0x5
      INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) movToMemoryAddressFromImmediateValue,
          IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_ADDRINT, INS_OperandImmediate (ins, 1),
          IARG_END);

    } else if (destIsReg && sourceIsImmed) { // load immediate to register, e.g. mov rbx, 0x3d8
      INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) movToRegisterFromImmediateValue,
          IARG_PTR, ise, IARG_UINT32, INS_OperandReg (ins, 0), IARG_ADDRINT,
          INS_OperandImmediate (ins, 1), IARG_END);

    } else if (destIsReg && sourceIsReg) { // move data between registers, e.g. mov r13, rdx
      REG srcreg = INS_OperandReg (ins, 1);
      INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) movToRegisterFromRegister, IARG_PTR,
          ise, IARG_UINT32, INS_OperandReg (ins, 0), IARG_UINT32, srcreg, IARG_REG_VALUE,
          srcreg, IARG_END);

    } else { // unknown case!
      throw std::runtime_error ("Unknown MOV instruction");
    }
    break;
  }
}

void Instrumenter::syscallEntryPoint (THREADID threadIndex, CONTEXT *ctxt,
    SYSCALL_STANDARD std) {
  //TODO: Implement...
}

void Instrumenter::syscallExitPoint (THREADID threadIndex, CONTEXT *ctxt,
    SYSCALL_STANDARD std) {
  //TODO: Implement...
}

void Instrumenter::aboutToExit (INT32 code) {
  //TODO: Implement...
  if (verbose == DEBUG) {
    cout << "countOfMemoryReadInstructions: " << countOfMemoryReadInstructions << endl;
    cout << "countOfMemoryWriteInstructions: " << countOfMemoryWriteInstructions << endl;
    cout << "countOfMOVInstructions: " << countOfMOVInstructions << " ("
        << (countOfMOVInstructions * 100.0 / totalCountOfInstructions) << " %)" << endl;
  }
}

VOID instrumentSingleInstruction (INS ins, VOID *v) {
  Instrumenter *im = (Instrumenter *) v;
  im->instrumentSingleInstruction (ins);
}

VOID syscallIsAboutToBeCalled (THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std,
    VOID *v) {
  Instrumenter *im = (Instrumenter *) v;
  im->syscallEntryPoint (threadIndex, ctxt, std);
}

VOID syscallIsReturned (THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std,
    VOID *v) {
  Instrumenter *im = (Instrumenter *) v;
  im->syscallExitPoint (threadIndex, ctxt, std);
}

VOID applicationIsAboutToExit (INT32 code, VOID *v) {
  Instrumenter *im = (Instrumenter *) v;
  im->aboutToExit (code);
}

}
}
}
}
