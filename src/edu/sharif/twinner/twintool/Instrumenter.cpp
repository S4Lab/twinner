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
#include <map>

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
  edu::sharif::twinner::util::Logger::debug ()
      << //
      "\t--> op0 is " << (destIsMem ? "mem" : (destIsReg ? "reg" : "unknown"))
      << "\n\t--> op1 is "
      << (sourceIsMem ? "mem" :
          (sourceIsReg ? "reg" : (sourceIsImmed ? "immediate" : "unknown"))) << '\n';
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

void Instrumenter::instrumentSingleInstruction (INS ins) {
  bool isMemoryRead = INS_IsMemoryRead (ins);
  bool isMemoryWrite = INS_IsMemoryWrite (ins);
  bool isOriginal = INS_IsOriginal (ins);
  edu::sharif::twinner::util::Logger::debug () //
  << "Instrumenting assembly instruction: " << INS_Disassemble (ins) << '\n'
      << "\t--> Count of memory operands: " << INS_MemoryOperandCount (ins) << '\n'
      << (isMemoryRead ? "\t--> Reading from memory\n" : "")
      << (isMemoryWrite ? "\t--> Writing to memory\n" : "")
      << (!isOriginal ? "\t--> NON-ORIGINAL instruction!\n" : "");
  if (isOriginal) {
    totalCountOfInstructions++;
  }

  switch (INS_Opcode (ins)) { // Intel has 1024 opcodes. And each opcode has several types/models :)
#define CASE_OF_OPCODE(_OPC) CASE_OF_OPCODE_WITH_CALL(_OPC)

#define CASE_OF_OPCODE_WITHOUT_CALL(_OPC) \
  case XED_ICLASS_ ## _OPC: \
    edu::sharif::twinner::util::Logger::debug () << "\t--> " # _OPC " instruction\n"; \
    countOfInstructionsPerOpcode[XED_ICLASS_ ## _OPC]++;

#define CASE_OF_OPCODE_WITH_CALL(_OPC) \
  CASE_OF_OPCODE_WITHOUT_CALL(_OPC) \
    instrument ## _OPC ## Instruction (ins);

  CASE_OF_OPCODE(MOV)
    break;
  CASE_OF_OPCODE_WITHOUT_CALL(ADD)
    break;
  CASE_OF_OPCODE_WITHOUT_CALL(TEST)
    break;
  CASE_OF_OPCODE_WITHOUT_CALL(SUB)
    break;
  CASE_OF_OPCODE_WITHOUT_CALL(CMP)
    break;
  CASE_OF_OPCODE_WITHOUT_CALL(JZ)
    break;
  default:
    edu::sharif::twinner::util::Logger::info ()    //
    << "Ignoring assembly instruction: " << INS_Disassemble (ins) << '\n';
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
