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
#include <set>

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

class InstructionSymbolicExecuter;

class Instrumenter {

private:

  enum InstructionModel {

    NOP_INS_MODELS = 0x0000,

    DST_REG_SRC_REG = 0x0001,
    DST_REG_SRC_MEM = 0x0002,
    DST_REG_SRC_IMD = 0x0004,
    DST_MEM_SRC_REG = 0x0008,
    DST_MEM_SRC_IMD = 0x0010,

    DST_STK_SRC_REG = 0x0020,
    DST_STK_SRC_IMD = 0x0040,
    DST_STK_SRC_MEM = 0x0080,

    DST_REG_SRC_STK = DST_REG_SRC_MEM,
    DST_MEM_SRC_STK = 0x0100,

    JMP_CC_INS_MODELS = 0x0200,

    DST_REG_SRC_ADG = 0x0400,

    DST_RSP_SRC_CALL = 0x0800, // instructions who update RSP due to routine calls

    DST_REG_REG_SRC_REG = 0x1000, // there are two (left and right) destinations

    RET_INS_MODELS = DST_RSP_SRC_CALL,
    CALL_INS_MODELS = DST_RSP_SRC_CALL,

    COMMON_INS_MODELS = DST_REG_SRC_REG | DST_REG_SRC_MEM | DST_REG_SRC_IMD
    | DST_MEM_SRC_REG | DST_MEM_SRC_IMD,
    MOV_ZX_AND_SX_INS_MODELS = DST_REG_SRC_REG | DST_REG_SRC_MEM,
    PUSH_INS_MODELS = DST_STK_SRC_REG | DST_STK_SRC_IMD | DST_STK_SRC_MEM,
    POP_INS_MODELS = DST_REG_SRC_STK | DST_MEM_SRC_STK,
    LEA_INS_MODELS = DST_REG_SRC_ADG,
    SHL_INS_MODELS = DST_REG_SRC_IMD | DST_REG_SRC_REG
    | DST_MEM_SRC_IMD | DST_MEM_SRC_REG,
    TEST_INS_MODELS = DST_REG_SRC_IMD | DST_MEM_SRC_IMD
    | DST_REG_SRC_REG | DST_MEM_SRC_REG,
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
  void instrumentSingleInstruction (InstructionModel model, OPCODE op, INS ins,
      std::string *insAssembly) const;

  void printDebugInformation (INS ins) const;

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
