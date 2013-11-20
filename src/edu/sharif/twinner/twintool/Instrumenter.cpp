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

#include "edu/sharif/twinner/trace/Trace.h"

#include "edu/sharif/twinner/util/Logger.h"

using namespace std;

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

Instrumenter::Instrumenter (string _symbolsFilePath, string _traceFilePath,
    bool _justAnalyzeMainRoutine) :
symbolsFilePath (_symbolsFilePath), traceFilePath (_traceFilePath),
justAnalyzeMainRoutine (_justAnalyzeMainRoutine),
ise (new InstructionSymbolicExecuter ()),
totalCountOfInstructions (0) {

  edu::sharif::twinner::util::Logger::info ()
      << "Instrumenter class created [verboseness level: "
      << edu::sharif::twinner::util::Logger::getVerbosenessLevelAsString () << "]\n";
  managedInstructions.insert
      (make_pair (XED_ICLASS_MOV, COMMON_INS_MODELS));
  // TODO: handle more types (data ranges), then add real support for sign/zero extension
  managedInstructions.insert
      (make_pair (XED_ICLASS_MOVZX, // 2 models (r <- zero-extend(r/m))
                  MOV_ZX_AND_SX_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_MOVSX, // 2 models (r <- sign-extend(r/m))
                  MOV_ZX_AND_SX_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_PUSH, PUSH_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_POP, POP_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_NOP, NOP_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_ADD, COMMON_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_SUB, COMMON_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_CMP, COMMON_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_JNZ, JMP_CC_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_LEA, LEA_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_CALL_NEAR, CALL_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_RET_NEAR, RET_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_CALL_FAR, CALL_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_RET_FAR, RET_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_SHL, SHL_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_AND, COMMON_INS_MODELS));
}

Instrumenter::~Instrumenter () {
  delete ise;
}

void Instrumenter::instrumentSingleInstruction (INS ins) {
  if (justAnalyzeMainRoutine) {
    RTN rtn = INS_Rtn (ins);
    if (RTN_Valid (rtn) && RTN_Name (rtn) == "main") {
      /*
       * Routine is valid and is named main. Before this point all instructions are owned
       * by RTLD and should be ignored. But after this point, all instructions are owned
       * by instrumented program. So all of them should be analyzed. Either routine is main
       * and instructions are within main routine or routine name is something different and
       * instructions are called directly or indirectly by the main routine.
       * In order to instrument all remaining instructions, we can simply
       * turn the justAnalyzeMainRoutine flag off.
       */
      justAnalyzeMainRoutine = false;
    } else {
      return;
    }
  }
  printDebugInformation (ins);
  if (INS_IsOriginal (ins)) {
    totalCountOfInstructions++;
  }

  OPCODE op = INS_Opcode (ins);
  // Intel has 1024 opcodes. And each opcode has several types/models :)
  std::map < OPCODE, InstructionModel >::iterator it =
      managedInstructions.find (op);
  /*
   * The INS_Disassemble does not disassemble ins correctly when it is passed to
   * analysis routines. So we must disassemble it here and then pass the string there.
   * But we only can pass pointers, so we must new this string object.
   * Unfortunately there is no clean way to delete these strings as it's not possible to
   * detect when analysis routine is called for the last time. However, it's possible
   * to keep a collection of these string objects and delete them at the Fini call
   * from PIN (the applicationIsAboutToExit(...) method).
   * As that point of time is too near to exit(), I'm not going to implement it
   * right now.
   * TODO: Keep pointers of instantiated strings and delete them in Fini call.
   */
  std::string *insAssembly = new std::string (INS_Disassemble (ins));
  if (it == managedInstructions.end ()) {
    edu::sharif::twinner::util::Logger::info ()
        << "Ignoring assembly instruction: " << *insAssembly << '\n';
  } else {
    edu::sharif::twinner::util::Logger::debug () << "\t--> " << OPCODE_StringShort (op)
        << " instruction\n";
    countOfInstructionsPerOpcode[op]++;
    InstructionModel expectedModels = it->second;
    InstructionModel model = getInstructionModel (op, ins);
    if (model) {
      if (expectedModels & model) {
        instrumentSingleInstruction (model, op, ins, insAssembly);
        return;

      } else {
        throw std::runtime_error ("UnExpected instruction model");
      }
    }
  }
  delete insAssembly;
}

Instrumenter::InstructionModel Instrumenter::getInstructionModel (OPCODE op,
    INS ins) const {
  switch (op) {
  case XED_ICLASS_PUSH:
    return getInstructionModelForPushInstruction (ins);
  case XED_ICLASS_POP:
    return getInstructionModelForPopInstruction (ins);
  case XED_ICLASS_NOP:
    return NOP_INS_MODELS;
  default:
    switch (INS_Category (ins)) {
    case XED_CATEGORY_COND_BR:
      return JMP_CC_INS_MODELS;
    case XED_CATEGORY_CALL:
    case XED_CATEGORY_RET:
      return DST_RSP_SRC_CALL;
    default:
      return getInstructionModelForNormalInstruction (ins);
    }
  }
}

Instrumenter::InstructionModel Instrumenter::getInstructionModelForPushInstruction (
    INS ins) const {
  bool sourceIsReg = INS_OperandIsReg (ins, 0);
  bool sourceIsMem = INS_OperandIsMemory (ins, 0);
  bool sourceIsImmed = INS_OperandIsImmediate (ins, 0);
  if (sourceIsReg) {
    return DST_STK_SRC_REG;
  } else if (sourceIsMem) {
    return DST_STK_SRC_MEM;
  } else if (sourceIsImmed) {
    return DST_STK_SRC_IMD;
  } else {
    throw std::runtime_error ("Unknown PUSH instruction model");
  }
}

Instrumenter::InstructionModel Instrumenter::getInstructionModelForPopInstruction (
    INS ins) const {
  bool destIsReg = INS_OperandIsReg (ins, 0);
  bool destIsMem = INS_OperandIsMemory (ins, 0);
  if (destIsReg) {
    return DST_REG_SRC_STK;
  } else if (destIsMem) {
    return DST_MEM_SRC_STK;
  } else {
    throw std::runtime_error ("Unknown POP instruction model");
  }
}

Instrumenter::InstructionModel Instrumenter::getInstructionModelForNormalInstruction (
    INS ins) const {
  bool destIsReg = INS_OperandIsReg (ins, 0);
  bool destIsMem = INS_OperandIsMemory (ins, 0);
  bool sourceIsReg = INS_OperandIsReg (ins, 1);
  bool sourceIsMem = INS_OperandIsMemory (ins, 1);
  bool sourceIsImmed = INS_OperandIsImmediate (ins, 1);
  bool sourceIsAdg = INS_OperandIsAddressGenerator (ins, 1);

  if (destIsReg && sourceIsReg) {
    return DST_REG_SRC_REG;

  } else if (destIsReg && sourceIsMem) {
    return DST_REG_SRC_MEM;

  } else if (destIsReg && sourceIsImmed) {
    return DST_REG_SRC_IMD;

  } else if (destIsMem && sourceIsReg) {
    return DST_MEM_SRC_REG;

  } else if (destIsMem && sourceIsImmed) {
    return DST_MEM_SRC_IMD;

  } else if (destIsReg && sourceIsAdg) {
    return DST_REG_SRC_ADG;

  } else {
    throw std::runtime_error ("Unknown instruction model");
  }
}

void Instrumenter::instrumentSingleInstruction (InstructionModel model, OPCODE op,
    INS ins, std::string *insAssembly) const {
  switch (model) {
  case DST_REG_SRC_REG:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    REG srcreg = INS_OperandReg (ins, 1);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstRegSrcReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                    IARG_UINT32, srcreg, IARG_REG_VALUE, srcreg,
                    IARG_CONST_CONTEXT,
                    IARG_PTR, insAssembly,
                    IARG_END);
    break;
  }
  case DST_REG_SRC_MEM:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstRegSrcMem,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                    IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_CONST_CONTEXT,
                    IARG_PTR, insAssembly,
                    IARG_END);
    break;
  }
  case DST_REG_SRC_IMD:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstRegSrcImd,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                    IARG_ADDRINT, INS_OperandImmediate (ins, 1),
                    IARG_CONST_CONTEXT,
                    IARG_PTR, insAssembly,
                    IARG_END);
    break;
  }
  case DST_MEM_SRC_REG:
  {
    REG srcreg = INS_OperandReg (ins, 1);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 0,
                    IARG_UINT32, srcreg, IARG_REG_VALUE, srcreg,
                    IARG_MEMORYREAD_SIZE,
                    IARG_CONST_CONTEXT,
                    IARG_PTR, insAssembly,
                    IARG_END);
    break;
  }
  case DST_MEM_SRC_IMD:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcImd,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 0,
                    IARG_ADDRINT, INS_OperandImmediate (ins, 1),
                    IARG_MEMORYREAD_SIZE,
                    IARG_CONST_CONTEXT,
                    IARG_PTR, insAssembly,
                    IARG_END);

    break;
  }
  case DST_STK_SRC_REG:
  {
    REG srcreg = INS_OperandReg (ins, 0);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 0,
                    IARG_UINT32, srcreg, IARG_REG_VALUE, srcreg,
                    IARG_MEMORYREAD_SIZE,
                    IARG_CONST_CONTEXT,
                    IARG_PTR, insAssembly,
                    IARG_END);
    break;
  }
  case DST_STK_SRC_IMD:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcImd,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 0,
                    IARG_ADDRINT, INS_OperandImmediate (ins, 0),
                    IARG_CONST_CONTEXT,
                    IARG_PTR, insAssembly,
                    IARG_END);

    break;
  }
  case DST_STK_SRC_MEM:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcMem,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 1,
                    IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_CONST_CONTEXT,
                    IARG_PTR, insAssembly,
                    IARG_END);
    break;
  }
  case DST_MEM_SRC_STK:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcMem,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 0,
                    IARG_MEMORYOP_EA, 1, IARG_MEMORYREAD_SIZE,
                    IARG_CONST_CONTEXT,
                    IARG_PTR, insAssembly,
                    IARG_END);
    break;
  }
  case JMP_CC_INS_MODELS:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineConditionalBranch,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_BRANCH_TAKEN,
                    IARG_CONST_CONTEXT,
                    IARG_PTR, insAssembly,
                    IARG_END);
    break;
  }
  case DST_REG_SRC_ADG:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    INS_InsertCall (ins, IPOINT_AFTER, (AFUNPTR) analysisRoutineDstRegSrcAdg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                    IARG_CONST_CONTEXT,
                    IARG_PTR, insAssembly,
                    IARG_END);
    break;
  }
  case DST_RSP_SRC_CALL:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineBeforeChangeOfReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, REG_RSP,
                    IARG_CONST_CONTEXT,
                    IARG_PTR, insAssembly,
                    IARG_END);
    break;
  }
  default:
    throw std::runtime_error ("Unknown instruction model");
  }
}

void Instrumenter::printDebugInformation (INS ins) const {
  edu::sharif::twinner::util::Logger debug = edu::sharif::twinner::util::Logger::debug ();
  bool isMemoryRead = INS_IsMemoryRead (ins);
  bool isMemoryWrite = INS_IsMemoryWrite (ins);
  bool isOriginal = INS_IsOriginal (ins);
  UINT32 countOfOperands = INS_OperandCount (ins);
  debug << "Instrumenting assembly instruction: " << INS_Disassemble (ins)
      << "\n\t--> Count of operands: " << countOfOperands
      << "\n\t--> Count of memory operands: " << INS_MemoryOperandCount (ins) << '\n'
      << (isMemoryRead ? "\t--> Reading from memory\n" : "")
      << (isMemoryWrite ? "\t--> Writing to memory\n" : "")
      << (!isOriginal ? "\t--> NON-ORIGINAL instruction!\n" : "");
  for (UINT32 i = 0; i < INS_OperandCount (ins); ++i) {
    bool isReg = INS_OperandIsReg (ins, i);
    bool isImmed = INS_OperandIsImmediate (ins, i);
    bool isMem = INS_OperandIsMemory (ins, i);
    bool isFixedMem = INS_OperandIsFixedMemop (ins, i);
    bool isImplicit = INS_OperandIsImplicit (ins, i);
    bool isBranchDisp = INS_OperandIsBranchDisplacement (ins, i);
    bool isAddrGen = INS_OperandIsAddressGenerator (ins, i);
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
    } else if (isBranchDisp) {
      debug << "branch-displacement";
    } else if (isAddrGen) {
      debug << "address-generator";
    } else {
      debug << "unknown";
    }
    debug << " | " << (isRead ? 'R' : '-') << (isWritten ? 'W' : '-') << '\n';
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
  printInstructionsStatisticsInfo ();
  edu::sharif::twinner::util::Logger::debug () << "Saving trace info...";
  ise->getTrace ()->saveToFile (traceFilePath.c_str ());
  edu::sharif::twinner::util::Logger::debug () << "Done.\n";
}

void Instrumenter::printInstructionsStatisticsInfo () const {
  int countOfIgnoredInstructions = totalCountOfInstructions;
  for (std::map < OPCODE, int >::const_iterator it =
      countOfInstructionsPerOpcode.begin (); it != countOfInstructionsPerOpcode.end ();
      ++it) {
    int op = it->first;
    int c = it->second;
    edu::sharif::twinner::util::Logger::info ()
        << "count of " << OPCODE_StringShort (op) << " instructions: " << c << " ("
        << (c * 100.0 / totalCountOfInstructions) << " %)\n";
    countOfIgnoredInstructions -= c;
  }
  edu::sharif::twinner::util::Logger::info ()
      << "count of ignored instructions: " << countOfIgnoredInstructions << " ("
      << (countOfIgnoredInstructions * 100.0 / totalCountOfInstructions) << " %)\n";
}

VOID instrumentSingleInstruction (INS ins, VOID * v) {
  Instrumenter *im = (Instrumenter *) v;
  im->instrumentSingleInstruction (ins);
}

VOID syscallIsAboutToBeCalled (THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std,
    VOID * v) {
  Instrumenter *im = (Instrumenter *) v;
  im->syscallEntryPoint (threadIndex, ctxt, std);
}

VOID syscallIsReturned (THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std,
    VOID * v) {
  Instrumenter *im = (Instrumenter *) v;
  im->syscallExitPoint (threadIndex, ctxt, std);
}

VOID applicationIsAboutToExit (INT32 code, VOID * v) {
  Instrumenter *im = (Instrumenter *) v;
  im->aboutToExit (code);
}

}
}
}
}
