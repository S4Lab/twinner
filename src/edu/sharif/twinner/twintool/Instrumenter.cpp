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

#include <stdexcept>

#include "xed-iclass-enum.h"

#include "InstructionSymbolicExecuter.h"

#include "edu/sharif/twinner/util/Logger.h"

using namespace std;

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

Instrumenter::Instrumenter (string _symbolsFilePath, string _traceFilePath) :
    symbolsFilePath (_symbolsFilePath), traceFilePath (_traceFilePath), ise (
        new InstructionSymbolicExecuter ()) {
  if (!(edu::sharif::twinner::util::Logger::debug ()
      << "Instrumenter class created [verboseness level: debug]\n")) {
    edu::sharif::twinner::util::Logger::info ()
        << "Instrumenter class created [verboseness level: info]\n";
  }
  const instrumentationMethod nullmethod = 0;
  instrumentationMethods.insert (
      make_pair (XED_ICLASS_MOV, &Instrumenter::instrumentMOVInstruction)); // 5 models
  instrumentationMethods.insert (
      make_pair (XED_ICLASS_PUSH, &Instrumenter::instrumentPUSHInstruction)); // 3 models
  instrumentationMethods.insert (
      make_pair (XED_ICLASS_POP, &Instrumenter::instrumentPOPInstruction)); // 2 models
  instrumentationMethods.insert (make_pair (XED_ICLASS_TEST, nullmethod));
  instrumentationMethods.insert (make_pair (XED_ICLASS_JZ, nullmethod));
  instrumentationMethods.insert (make_pair (XED_ICLASS_JNZ, nullmethod));
}

Instrumenter::~Instrumenter () {
  delete ise;
}

static std::map < OPCODE, int > countOfInstructionsPerOpcode;
static int totalCountOfInstructions = 0;

void Instrumenter::instrumentMOVInstruction (INS ins) {
  bool destIsReg = INS_OperandIsReg (ins, 0);
  bool destIsMem = INS_OperandIsMemory (ins, 0);
  bool sourceIsReg = INS_OperandIsReg (ins, 1);
  bool sourceIsMem = INS_OperandIsMemory (ins, 1);
  bool sourceIsImmed = INS_OperandIsImmediate (ins, 1);
  if (destIsReg && sourceIsMem) { // read from memory, e.g. mov eax, dword ptr [rbp-0x8]
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) movToRegisterFromMemoryAddress,
        IARG_PTR, ise, IARG_UINT32, INS_OperandReg (ins, 0), IARG_MEMORYOP_EA, 0, IARG_END
        );

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
}

void Instrumenter::instrumentPUSHInstruction (INS ins) {
  //TODO: Take care of ESP or RSP register too
  switch (INS_MemoryOperandCount (ins)) {
  case 1: { // push a register, e.g. push rbp OR push an immediate value, e.g. push 0x0
    bool sourceIsReg = INS_OperandIsReg (ins, 0);
    bool sourceIsImmed = INS_OperandIsImmediate (ins, 0);
    if (sourceIsReg) {
      REG reg = INS_OperandReg (ins, 0);
      INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) pushToStackFromRegister, IARG_PTR,
          ise, IARG_MEMORYOP_EA, 0, IARG_UINT32, reg, IARG_REG_VALUE, reg, IARG_END);
    } else if (sourceIsImmed) {
      INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) pushToStackFromImmediateValue,
          IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_ADDRINT, INS_OperandImmediate (ins, 0),
          IARG_END);
    } else {
      throw std::runtime_error ("Unknown PUSH instruction (with 1 mem operand)");
    }
    break;
  }
  case 2: { // push a memory cell's content, e.g. push qword ptr [rip+0x200532]. First mem op is src, second is dest
    bool sourceIsMem = INS_OperandIsMemory (ins, 0);
    if (sourceIsMem) {
      INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) pushToStackFromMemoryAddress,
          IARG_PTR, ise, IARG_MEMORYOP_EA, 1, IARG_MEMORYOP_EA, 0, IARG_END);
    } else {
      throw std::runtime_error ("Unknown PUSH instruction (with 2 mem operands)");
    }
    break;
  }
  default:
    throw std::runtime_error ("Unknown PUSH instruction");
  }
}

void Instrumenter::instrumentPOPInstruction (INS ins) {
  //TODO: Take care of ESP or RSP register too
  switch (INS_MemoryOperandCount (ins)) {
  case 1: { // pop into a register, e.g. pop rbp
    bool destIsReg = INS_OperandIsReg (ins, 0);
    if (destIsReg) {
      INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) popToRegisterFromStack, IARG_PTR, ise,
          IARG_UINT32, INS_OperandReg (ins, 0), IARG_MEMORYOP_EA, 0, IARG_END);
    } else {
      throw std::runtime_error ("Unknown POP instruction (with 1 mem operand)");
    }
    break;
  }
  case 2: { // pop into a memory address, e.g. pop qword ptr [rdx+0x200532]. First mem op is dest, second is src
    bool destIsMem = INS_OperandIsMemory (ins, 0);
    if (destIsMem) {
      INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) popToMemoryAddressFromStack, IARG_PTR,
          ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYOP_EA, 1, IARG_END);
    } else {
      throw std::runtime_error ("Unknown POP instruction (with 2 mem operands)");
    }
    break;
  }
  default:
    throw std::runtime_error ("Unknown POP instruction");
  }
}

void Instrumenter::instrumentSingleInstruction (INS ins) {
  bool isMemoryRead = INS_IsMemoryRead (ins);
  bool isMemoryWrite = INS_IsMemoryWrite (ins);
  bool isOriginal = INS_IsOriginal (ins);
  UINT32 countOfOperands = INS_OperandCount (ins);
  edu::sharif::twinner::util::Logger debug = edu::sharif::twinner::util::Logger::debug ();
  debug << "Instrumenting assembly instruction: " << INS_Disassemble (ins) << '\n'
      << "\t--> Count of operands: " << countOfOperands << '\n'
      << "\t--> Count of memory operands: " << INS_MemoryOperandCount (ins) << '\n'
      << (isMemoryRead ? "\t--> Reading from memory\n" : "")
      << (isMemoryWrite ? "\t--> Writing to memory\n" : "")
      << (!isOriginal ? "\t--> NON-ORIGINAL instruction!\n" : "");
  for (UINT32 i = 0; i < INS_OperandCount (ins); ++i) {
    bool isReg = INS_OperandIsReg (ins, i);
    bool isImmed = INS_OperandIsImmediate (ins, i);
    bool isMem = INS_OperandIsMemory (ins, i);
    bool isFixedMem = INS_OperandIsFixedMemop (ins, i);
    bool isImplicit = INS_OperandIsImplicit (ins, i);
    bool isRead = INS_OperandRead (ins, i);
    bool isWritten = INS_OperandWritten (ins, i);
    REG reg = INS_OperandReg (ins, i);
    debug << "\t--> op" << i << " is ";
    if (isReg) {
      debug << "reg (" << REG_StringShort (reg) << ')';
    } else if (isMem) {
      debug << "mem";
    } else if (isImmed) {
      debug << "immediate (" << INS_OperandImmediate (ins, i) << ')';
    } else if (isFixedMem) {
      debug << "fixed-mem";
    } else if (isImplicit) {
      debug << "implicit";
    } else {
      debug << "unknown";
    }
    debug << " | " << (isRead ? 'R' : '-') << (isWritten ? 'W' : '-') << '\n';
  }
  if (isOriginal) {
    totalCountOfInstructions++;
  }

  OPCODE op = INS_Opcode (ins);
  // Intel has 1024 opcodes. And each opcode has several types/models :)
  std::map < OPCODE, instrumentationMethod >::iterator it = instrumentationMethods.find (
      op);
  if (it == instrumentationMethods.end ()) {
    edu::sharif::twinner::util::Logger::info ()    //
    << "Ignoring assembly instruction: " << INS_Disassemble (ins) << '\n';
  } else {
    edu::sharif::twinner::util::Logger::debug () << "\t--> " << OPCODE_StringShort (op)
        << " instruction\n";
    countOfInstructionsPerOpcode[op]++;
    instrumentationMethod method = it->second;
    if (method) {
      (this->*method) (ins);
    }
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
  int countOfIgnoredInstructions = totalCountOfInstructions;
  for (std::map < OPCODE, int >::iterator it = countOfInstructionsPerOpcode.begin ();
      it != countOfInstructionsPerOpcode.end (); ++it) {
    int op = it->first;
    int c = it->second;
    edu::sharif::twinner::util::Logger::info () //
    << "count of " << OPCODE_StringShort (op) << " instructions: " << c << " ("
        << (c * 100.0 / totalCountOfInstructions) << " %)\n";
    countOfIgnoredInstructions -= c;
  }
  edu::sharif::twinner::util::Logger::info () //
  << "count of ignored instructions: " << countOfIgnoredInstructions << " ("
      << (countOfIgnoredInstructions * 100.0 / totalCountOfInstructions) << " %)\n";
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
