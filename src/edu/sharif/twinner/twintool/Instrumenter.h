//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2014  Behnam Momeni
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
#include <set>

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

class InstructionSymbolicExecuter;

class Instrumenter {

private:

  enum InstructionModel {

    NOP_INS_MODELS = 0x00000000,

    DST_REG_SRC_REG = 0x00000001,
    DST_REG_SRC_MEM = 0x00000002,
    DST_REG_SRC_IMD = 0x00000004,
    DST_MEM_SRC_REG = 0x00000008,
    DST_MEM_SRC_IMD = 0x00000010,

    DST_STK_SRC_REG = 0x00000020,
    DST_STK_SRC_IMD = 0x00000040,
    DST_STK_SRC_MEM = 0x00000080,

    DST_REG_SRC_STK = DST_REG_SRC_MEM,
    DST_MEM_SRC_STK = 0x00000100,

    JMP_CC_INS_MODELS = 0x00000200,

    DST_REG_SRC_ADG = 0x00000400,

    DST_RSP_SRC_CALL = 0x00000800, // instructions who update RSP due to routine calls

    DST_REG_REG_SRC_REG = 0x00001000, // there are two (left and right) destinations

    OPERAND_LESS = 0x00002000,

    DST_REG_SRC_LARGE_REG = 0x00004000,

    DST_REG_SRC_IMPLICIT = 0x00008000,
    DST_MEM_SRC_IMPLICIT = 0x00010000,

    DST_MEM_SRC_MEM = DST_MEM_SRC_STK,
    DST_LARGE_REG_SRC_MEM = 0x00020000,
    DST_LARGE_REG_SRC_LARGE_REG = 0x00040000,

    PCMPEQX_INS_MODELS = DST_MEM_SRC_MEM
    | DST_LARGE_REG_SRC_MEM | DST_LARGE_REG_SRC_LARGE_REG,

    RET_INS_MODELS = DST_RSP_SRC_CALL,
    CALL_INS_MODELS = DST_RSP_SRC_CALL,

    COMMON_INS_MODELS = DST_REG_SRC_REG | DST_REG_SRC_MEM | DST_REG_SRC_IMD
    | DST_MEM_SRC_REG | DST_MEM_SRC_IMD,
    MOV_ZX_AND_SX_INS_MODELS = DST_REG_SRC_REG | DST_REG_SRC_MEM,
    PUSH_INS_MODELS = DST_STK_SRC_REG | DST_STK_SRC_IMD | DST_STK_SRC_MEM,
    POP_INS_MODELS = DST_REG_SRC_STK | DST_MEM_SRC_STK,
    LEA_INS_MODELS = DST_REG_SRC_ADG,
    SHIFT_INS_MODELS = DST_REG_SRC_IMD | DST_REG_SRC_REG
    | DST_MEM_SRC_IMD | DST_MEM_SRC_REG,
    TEST_INS_MODELS = DST_REG_SRC_IMD | DST_MEM_SRC_IMD
    | DST_REG_SRC_REG | DST_MEM_SRC_REG,
    DST_REG_SRC_EITHER_REG_OR_MEM = DST_REG_SRC_REG | DST_REG_SRC_MEM,
    DST_EITHER_REG_OR_MEM_SRC_IMPLICIT = DST_REG_SRC_IMPLICIT | DST_MEM_SRC_IMPLICIT,
  };

  std::map < OPCODE, InstructionModel > managedInstructions;

  std::string traceFilePath; // save final execution trace into this file

  InstructionSymbolicExecuter *ise;

  const std::set < ADDRINT > candidateAddresses;
  bool isWithinInitialStateDetectionMode;

  bool disabled;

public:
  Instrumenter (std::ifstream &symbolsFileInputStream,
      const std::string &traceFilePath, bool disabled);
  Instrumenter (const std::set < ADDRINT > &candidateAddresses,
      const std::string &traceFilePath, bool disabled);
  Instrumenter (const std::string &traceFilePath, bool disabled);
  ~Instrumenter ();

  void instrumentSingleInstruction (INS ins);

  void syscallEntryPoint (THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std);
  void syscallExitPoint (THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std);

  void aboutToExit (INT32 code);
  void disable ();
  void enable ();

private:
  void initialize ();

  void saveMemoryContentsToFile (const char *path) const;

  void printInstructionsStatisticsInfo () const;

  InstructionModel getInstructionModel (OPCODE op, INS ins) const;
  InstructionModel getInstructionModelForPushInstruction (INS ins) const;
  InstructionModel getInstructionModelForPopInstruction (INS ins) const;
  InstructionModel getInstructionModelForNormalInstruction (INS ins) const;
  inline void checkForInitialState (INS ins) const;
  void instrumentSingleInstruction (InstructionModel model, OPCODE op, INS ins,
      UINT32 insAssembly) const;

  void printDebugInformation (INS ins, const char *insAssembly) const;

  std::map < OPCODE, int > countOfInstructionsPerOpcode;
  int totalCountOfInstructions;
};

VOID instrumentSingleInstruction (INS ins, VOID *v);
VOID imageIsLoaded (IMG img, VOID *v);
VOID startAnalysis (VOID *v);
VOID syscallIsAboutToBeCalled (THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std,
    VOID *v);
VOID syscallIsReturned (THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std,
    VOID *v);
VOID applicationIsAboutToExit (INT32 code, VOID *v);
VOID terminateAnalysis (VOID *imptr);

}
}
}
}

#endif /* Instrumenter.h */
