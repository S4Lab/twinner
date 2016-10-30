//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2016 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef INSTRUMENTER_H
#define INSTRUMENTER_H

#include "edu/sharif/twinner/pin-wrapper.h"

#include <string>
#include <map>
#include <set>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class FunctionInfo;
}
namespace twintool {

class InstructionSymbolicExecuter;

namespace instructionmodels {

const UINT64 NOP_INS_MODELS = 0x0000000000000000;

const UINT64 DST_REG_SRC_REG = 0x0000000000000001;
const UINT64 DST_REG_SRC_MEM = 0x0000000000000002;
const UINT64 DST_REG_SRC_IMD = 0x0000000000000004;
const UINT64 DST_MEM_SRC_REG = 0x0000000000000008;
const UINT64 DST_MEM_SRC_IMD = 0x0000000000000010;

const UINT64 DST_STK_SRC_REG = 0x0000000000000020;
const UINT64 DST_STK_SRC_IMD = 0x0000000000000040;
const UINT64 DST_STK_SRC_MEM = 0x0000000000000080;

const UINT64 DST_REG_SRC_STK = 0x0000000000000100;
const UINT64 DST_MEM_SRC_STK = 0x0000000000000200;

const UINT64 JMP_CC_INS_MODELS = 0x0000000000000400;

const UINT64 DST_REG_SRC_ADG = 0x0000000000000800;
// instructions who update RSP due to routine calls
const UINT64 DST_RSP_SRC_CALL = 0x0000000000001000;
// there are two (left and right) destinations
const UINT64 DST_REG_REG_SRC_REG = 0x0000000000002000;

const UINT64 OPERAND_LESS = 0x0000000000004000;

const UINT64 DST_REG_SRC_LARGE_REG = 0x0000000000008000;

const UINT64 DST_REG_SRC_IMPLICIT = 0x0000000000010000;
const UINT64 DST_MEM_SRC_IMPLICIT = 0x0000000000020000;

const UINT64 DST_MEM_SRC_MEM = 0x0000000000040000;
const UINT64 DST_LARGE_REG_SRC_MEM = 0x0000000000080000;
const UINT64 DST_LARGE_REG_SRC_LARGE_REG = 0x0000000000100000;
const UINT64 DST_MEM_SRC_LARGE_REG = 0x0000000000200000;

const UINT64 DST_REG_SRC_REG_AUX_REG = 0x0000000000400000;
const UINT64 DST_MEM_SRC_REG_AUX_REG = 0x0000000000800000;

const UINT64 DST_REG_SRC_MUTABLE_REG = 0x0000000001000000;
const UINT64 DST_MEM_SRC_MUTABLE_REG = 0x0000000002000000;

// MEM is implicit [rdi]
const UINT64 STRING_OPERATION_REG_MEM = 0x0000000004000000;
// MEM is implicit [rdi]
const UINT64 STRING_OPERATION_MEM_REG = 0x0000000008000000;
// MEMs are implicit [rdi] and [rsi]
const UINT64 STRING_OPERATION_MEM_MEM = 0x0000000010000000;

const UINT64 LEAVE_INS_MODELS = 0x0000000020000000;

const UINT64 DST_LARGE_REG_SRC_REG = 0x0000000040000000;

const UINT64 DST_LARGE_REG_SRC_LARGE_REG_AUX_IMD = 0x0000000080000000;
const UINT64 DST_LARGE_REG_SRC_MEM_AUX_IMD = 0x0000000100000000;

const UINT64 DST_REG_SRC_MEM_AUX_RSI = 0x0000000200000000;
const UINT64 CMOV_INS_MODELS = 0x0000000400000000;

// there are two (left and right) destinations
const UINT64 DST_REG_REG_SRC_MEM = 0x0000000800000000;

const UINT64 DST_REG_SRC_REG_AUX_IMD = 0x0000001000000000;
const UINT64 DST_REG_SRC_MEM_AUX_IMD = 0x0000002000000000;

const UINT64 DST_REG_SRC_EITHER_REG_OR_MEM_AUX_IMD =
    DST_REG_SRC_REG_AUX_IMD | DST_REG_SRC_MEM_AUX_IMD;

const UINT64 DST_REG_REG_SRC_EITHER_REG_OR_MEM =
    DST_REG_REG_SRC_REG | DST_REG_REG_SRC_MEM;

const UINT64 DST_EITHER_REG_OR_MEM_SRC_MUTABLE_REG = DST_REG_SRC_MUTABLE_REG
    | DST_MEM_SRC_MUTABLE_REG;

const UINT64 PCMPEQX_INS_MODELS = DST_MEM_SRC_MEM
    | DST_LARGE_REG_SRC_MEM | DST_LARGE_REG_SRC_LARGE_REG;
const UINT64 MOV_INS_WITH_LARGE_REG_INS_MODELS = DST_LARGE_REG_SRC_MEM
    | DST_MEM_SRC_LARGE_REG | DST_LARGE_REG_SRC_LARGE_REG;

const UINT64 RET_INS_WITH_ARG_MODEL = 0x0000004000000000;
const UINT64 RET_INS_MODELS = DST_RSP_SRC_CALL | RET_INS_WITH_ARG_MODEL;
const UINT64 CALL_INS_MODELS = DST_RSP_SRC_CALL;
const UINT64 JMP_INS_MODELS = DST_RSP_SRC_CALL;

const UINT64 DST_MEM_SRC_REG_AUX_IMD = 0x0000008000000000;
const UINT64 DST_EITHER_REG_OR_MEM_SRC_REG_AUX_IMD =
    DST_REG_SRC_REG_AUX_IMD | DST_MEM_SRC_REG_AUX_IMD;

const UINT64 SYSCALL_INS_MODEL = 0x0000010000000000;

const UINT64 DST_STK_SRC_IMPLICIT = 0x0000020000000000;

const UINT64 COMMON_INS_MODELS = DST_REG_SRC_REG | DST_REG_SRC_MEM
    | DST_REG_SRC_IMD | DST_MEM_SRC_REG | DST_MEM_SRC_IMD;
const UINT64 MOV_ZX_AND_SX_INS_MODELS = DST_REG_SRC_REG | DST_REG_SRC_MEM;
const UINT64 PUSH_INS_MODELS = DST_STK_SRC_REG
    | DST_STK_SRC_IMD | DST_STK_SRC_MEM;
const UINT64 POP_INS_MODELS = DST_REG_SRC_STK | DST_MEM_SRC_STK;
const UINT64 LEA_INS_MODELS = DST_REG_SRC_ADG;
const UINT64 SHIFT_INS_MODELS = DST_REG_SRC_IMD | DST_REG_SRC_REG
    | DST_MEM_SRC_IMD | DST_MEM_SRC_REG;
const UINT64 DST_REG_SRC_EITHER_REG_OR_MEM = DST_REG_SRC_REG | DST_REG_SRC_MEM;
const UINT64 DST_EITHER_REG_OR_MEM_SRC_REG_AUX_REG = DST_REG_SRC_REG_AUX_REG
    | DST_MEM_SRC_REG_AUX_REG;
const UINT64 SHIFT_DOUBLE_PRECISION_INS_MODELS =
    DST_EITHER_REG_OR_MEM_SRC_REG_AUX_REG
    | DST_EITHER_REG_OR_MEM_SRC_REG_AUX_IMD;

const UINT64 DST_EITHER_REG_OR_MEM_SRC_IMPLICIT =
    DST_REG_SRC_IMPLICIT | DST_MEM_SRC_IMPLICIT;
const UINT64 DST_LARGE_REG_SRC_EITHER_LARGE_REG_OR_MEM =
    DST_LARGE_REG_SRC_LARGE_REG | DST_LARGE_REG_SRC_MEM;
const UINT64 DST_REG_SRC_EITHER_REG_OR_MEM_ANY_SIZE =
    DST_REG_SRC_EITHER_REG_OR_MEM | DST_LARGE_REG_SRC_EITHER_LARGE_REG_OR_MEM;
const UINT64 DST_LARGE_REG_SRC_EITHER_REG_OR_MEM_OR_VICE_VERSA =
    DST_LARGE_REG_SRC_REG | DST_LARGE_REG_SRC_MEM | DST_MEM_SRC_LARGE_REG
    | DST_REG_SRC_LARGE_REG;
const UINT64 DST_LARGE_REG_SRC_EITHER_LARGE_REG_OR_MEM_AUX_IMD =
    DST_LARGE_REG_SRC_LARGE_REG_AUX_IMD | DST_LARGE_REG_SRC_MEM_AUX_IMD;
const UINT64 IMUL_INS_MODELS =
    DST_REG_REG_SRC_EITHER_REG_OR_MEM /* one operand mode */
    | DST_REG_SRC_EITHER_REG_OR_MEM /* two operands mode */
    | DST_REG_SRC_EITHER_REG_OR_MEM_AUX_IMD; /* three operands mode */
const UINT64 IDIV_INS_MODELS =
    DST_REG_REG_SRC_EITHER_REG_OR_MEM; /* one operand mode */
}

class Instrumenter {
private:
  typedef UINT64 InstructionModel;

  std::map < OPCODE, InstructionModel > managedInstructions;

  std::string traceFilePath; // save final execution trace into this file
  std::string disassemblyFilePath; // save disassembled instructions into this file
  std::string marFilePath; // save final main() args report into this file
  std::string marCache;

  InstructionSymbolicExecuter *ise;

  const std::set < std::pair < ADDRINT, int > > candidateAddresses;
  bool isWithinInitialStateDetectionMode;

  bool disabled;
  bool withinSafeFunc;
  int stackOffset;
  bool naive;
  ADDRINT start;
  ADDRINT end;
  std::vector<edu::sharif::twinner::trace::FunctionInfo> safeFunctionsInfo;

public:
  Instrumenter (std::ifstream &symbolsFileInputStream,
      const std::string &traceFilePath, const std::string &disassemblyFilePath,
      bool disabled, int stackOffset,
      ADDRINT start, ADDRINT end,
      std::vector<edu::sharif::twinner::trace::FunctionInfo> safeFunctionsInfo,
      bool naive, bool measureMode);
  Instrumenter (
      const std::set < std::pair < ADDRINT, int > > &candidateAddresses,
      const std::string &traceFilePath, const std::string &disassemblyFilePath,
      bool disabled, int stackOffset,
      ADDRINT start, ADDRINT end);
  Instrumenter (const std::string &traceFilePath,
      const std::string &disassemblyFilePath,
      bool disabled, int stackOffset,
      ADDRINT start, ADDRINT end,
      std::vector<edu::sharif::twinner::trace::FunctionInfo> safeFunctionsInfo,
      bool naive);
  ~Instrumenter ();

  void registerInstrumentationRoutines ();

  void setMainArgsReportingFilePath (const std::string &marFilePath);
  void instrumentImage (IMG img);
  void instrumentSafeFunctions (IMG img);
  bool instrumentSafeFunctions (INS ins, UINT32 insAssembly) const;

  bool instrumentSingleInstruction (INS ins);

  void syscallEntryPoint (THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std);
  void syscallExitPoint (THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std);

  void aboutToExit (INT32 code);
  void disable ();
  void enable ();

  void beforeSafeFunction (ADDRINT retAddress,
      const edu::sharif::twinner::trace::FunctionInfo &fi,
      UINT32 insAssembly, const CONTEXT *context);
  void afterSafeFunction (const CONTEXT *context);
  void afterSafeFunction (CONTEXT *context);

  void reportMainArguments (int argc, char **argv);

private:
  void initialize ();
  template<int size>
  void initialize (InstructionModel model, const OPCODE opcodes[]);

  void saveMemoryContentsToFile (const char *path) const;

  void printInstructionsStatisticsInfo () const;

  InstructionModel getInstructionModel (OPCODE op, INS ins) const;
  InstructionModel getInstructionModelForPushInstruction (INS ins) const;
  InstructionModel getInstructionModelForPopInstruction (INS ins) const;
  InstructionModel getInstructionModelForNormalInstruction (INS ins) const;
  inline void checkForInitialState (INS ins) const;
  void instrumentSingleInstruction (InstructionModel model, OPCODE op, INS ins,
      UINT32 insAssembly) const;
  void instrumentMemoryRegisterCorrespondence (INS ins,
      UINT32 insAssembly) const;
  void instrumentRepPrefix (OPCODE op, INS ins, UINT32 insAssembly) const;

  void printDebugInformation (INS ins, const char *insAssembly) const;

  std::map < OPCODE, int > countOfInstructionsPerOpcode;
  int totalCountOfInstructions;

  OPCODE convertConditionalMoveToJumpOpcode (OPCODE cmovcc) const;
};

VOID instrumentSingleInst (INS ins, VOID *v);
VOID imageIsLoaded (IMG img, VOID *v);

VOID instrumentSafeFuncs (IMG img, VOID *v);
VOID beforeSafeFunc (VOID *v, ADDRINT retAddress, VOID *p, UINT32 insAssembly,
    const CONTEXT *context);
VOID afterSafeFunc (VOID *v, CONTEXT *context);

VOID startAnalysis (VOID *v);
VOID reportMainArgs (VOID *v, ADDRINT *arg0, ADDRINT *arg1);
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
