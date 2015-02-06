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

    NOP_INS_MODELS = 0x0000000000000000,

    DST_REG_SRC_REG = 0x0000000000000001,
    DST_REG_SRC_MEM = 0x0000000000000002,
    DST_REG_SRC_IMD = 0x0000000000000004,
    DST_MEM_SRC_REG = 0x0000000000000008,
    DST_MEM_SRC_IMD = 0x0000000000000010,

    DST_STK_SRC_REG = 0x0000000000000020,
    DST_STK_SRC_IMD = 0x0000000000000040,
    DST_STK_SRC_MEM = 0x0000000000000080,

    DST_REG_SRC_STK = 0x0000000000000100,
    DST_MEM_SRC_STK = 0x0000000000000200,

    JMP_CC_INS_MODELS = 0x0000000000000400,

    DST_REG_SRC_ADG = 0x0000000000000800,
    // instructions who update RSP due to routine calls
    DST_RSP_SRC_CALL = 0x0000000000001000,
    // there are two (left and right) destinations
    DST_REG_REG_SRC_REG = 0x0000000000002000,

    OPERAND_LESS = 0x0000000000004000,

    DST_REG_SRC_LARGE_REG = 0x0000000000008000,

    DST_REG_SRC_IMPLICIT = 0x0000000000010000,
    DST_MEM_SRC_IMPLICIT = 0x0000000000020000,

    DST_MEM_SRC_MEM = 0x0000000000040000,
    DST_LARGE_REG_SRC_MEM = 0x0000000000080000,
    DST_LARGE_REG_SRC_LARGE_REG = 0x0000000000100000,
    DST_MEM_SRC_LARGE_REG = 0x0000000000200000,

    DST_REG_SRC_REG_AUX_REG = 0x0000000000400000,
    DST_MEM_SRC_REG_AUX_REG = 0x0000000000800000,

    DST_REG_SRC_MUTABLE_REG = 0x0000000001000000,
    DST_MEM_SRC_MUTABLE_REG = 0x0000000002000000,

    STRING_OPERATION_REG_MEM = 0x0000000004000000, // MEM is implicit [rdi]
    STRING_OPERATION_MEM_REG = 0x0000000008000000, // MEM is implicit [rdi]
    STRING_OPERATION_MEM_MEM = 0x0000000010000000, // MEMs are implicit [rdi] and [rsi]

    LEAVE_INS_MODELS = 0x0000000020000000,

    DST_LARGE_REG_SRC_REG = 0x0000000040000000,

    DST_LARGE_REG_SRC_LARGE_REG_AUX_IMD = 0x0000000080000000,
    DST_LARGE_REG_SRC_MEM_AUX_IMD = 0x0000000100000000,

    DST_REG_SRC_MEM_AUX_RSI = 0x0000000200000000,
    CMOV_INS_MODELS = 0x0000000400000000,

    DST_EITHER_REG_OR_MEM_SRC_MUTABLE_REG = DST_REG_SRC_MUTABLE_REG
    | DST_MEM_SRC_MUTABLE_REG,

    PCMPEQX_INS_MODELS = DST_MEM_SRC_MEM
    | DST_LARGE_REG_SRC_MEM | DST_LARGE_REG_SRC_LARGE_REG,
    MOV_INS_WITH_LARGE_REG_INS_MODELS = DST_LARGE_REG_SRC_MEM | DST_MEM_SRC_LARGE_REG
    | DST_LARGE_REG_SRC_LARGE_REG,

    RET_INS_MODELS = DST_RSP_SRC_CALL,
    CALL_INS_MODELS = DST_RSP_SRC_CALL,
    JMP_INS_MODELS = DST_RSP_SRC_CALL,

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
    DST_EITHER_REG_OR_MEM_SRC_REG_AUX_REG = DST_REG_SRC_REG_AUX_REG
    | DST_MEM_SRC_REG_AUX_REG,
    DST_EITHER_REG_OR_MEM_SRC_IMPLICIT = DST_REG_SRC_IMPLICIT | DST_MEM_SRC_IMPLICIT,
    DST_LARGE_REG_SRC_EITHER_LARGE_REG_OR_MEM = DST_LARGE_REG_SRC_LARGE_REG
    | DST_LARGE_REG_SRC_MEM,
    DST_REG_SRC_EITHER_REG_OR_MEM_ANY_SIZE = DST_REG_SRC_EITHER_REG_OR_MEM
    | DST_LARGE_REG_SRC_EITHER_LARGE_REG_OR_MEM,
    DST_LARGE_REG_SRC_EITHER_REG_OR_MEM_OR_VICE_VERSA = DST_LARGE_REG_SRC_REG
    | DST_LARGE_REG_SRC_MEM | DST_MEM_SRC_LARGE_REG | DST_REG_SRC_LARGE_REG,
    DST_LARGE_REG_SRC_EITHER_LARGE_REG_OR_MEM_AUX_IMD =
    DST_LARGE_REG_SRC_LARGE_REG_AUX_IMD | DST_LARGE_REG_SRC_MEM_AUX_IMD,
  };

  std::map < OPCODE, InstructionModel > managedInstructions;

  std::string traceFilePath; // save final execution trace into this file
  std::string disassemblyFilePath; // save disassembled instructions into this file

  InstructionSymbolicExecuter *ise;

  const std::set < ADDRINT > candidateAddresses;
  bool isWithinInitialStateDetectionMode;

  bool disabled;

public:
  Instrumenter (std::ifstream &symbolsFileInputStream,
      const std::string &traceFilePath, const std::string &disassemblyFilePath,
      bool disabled);
  Instrumenter (const std::set < ADDRINT > &candidateAddresses,
      const std::string &traceFilePath, const std::string &disassemblyFilePath,
      bool disabled);
  Instrumenter (const std::string &traceFilePath, const std::string &disassemblyFilePath,
      bool disabled);
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
  void instrumentRepPrefix (OPCODE op, INS ins, UINT32 insAssembly) const;

  void printDebugInformation (INS ins, const char *insAssembly) const;

  std::map < OPCODE, int > countOfInstructionsPerOpcode;
  int totalCountOfInstructions;

  OPCODE convertConditionalMoveToJumpOpcode (OPCODE cmovcc) const;
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

inline BOOL REG_is_gr_any_size (REG reg) {
  return REG_is_gr8 (reg) || REG_is_gr16 (reg) || REG_is_gr32 (reg) || REG_is_gr64 (reg);
}

#endif /* Instrumenter.h */
