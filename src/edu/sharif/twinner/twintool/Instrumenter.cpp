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

#include "Instrumenter.h"

#include <stdexcept>
#include <stdlib.h>

#include "xed-iclass-enum.h"

#include "InstructionSymbolicExecuter.h"

#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/Syscall.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"
#include "edu/sharif/twinner/util/memory.h"
#include "edu/sharif/twinner/util/MemoryManager.h"

using namespace std;

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

inline void read_memory_content_and_add_it_to_map (std::map < ADDRINT, UINT64 > &map,
    const ADDRINT &address);

Instrumenter::Instrumenter (std::ifstream &symbolsFileInputStream,
    const string &_traceFilePath, const std::string &_disassemblyFilePath,
    bool _disabled) :
    traceFilePath (_traceFilePath), disassemblyFilePath (_disassemblyFilePath),
    ise (new InstructionSymbolicExecuter (symbolsFileInputStream, _disabled)),
    isWithinInitialStateDetectionMode (false),
    disabled (_disabled),
    totalCountOfInstructions (0) {
  initialize ();
}

Instrumenter::Instrumenter (const std::set < ADDRINT > &_candidateAddresses,
    const std::string &_traceFilePath, const std::string &_disassemblyFilePath,
    bool _disabled) :
    traceFilePath (_traceFilePath), disassemblyFilePath (_disassemblyFilePath),
    ise (new InstructionSymbolicExecuter (_disabled)),
    candidateAddresses (_candidateAddresses),
    isWithinInitialStateDetectionMode (true),
    disabled (_disabled),
    totalCountOfInstructions (0) {
  initialize ();
}

Instrumenter::Instrumenter (const string &_traceFilePath,
    const std::string &_disassemblyFilePath, bool _disabled) :
    traceFilePath (_traceFilePath), disassemblyFilePath (_disassemblyFilePath),
    ise (new InstructionSymbolicExecuter (_disabled)),
    isWithinInitialStateDetectionMode (false),
    disabled (_disabled),
    totalCountOfInstructions (0) {
  initialize ();
}

void Instrumenter::initialize () {
  edu::sharif::twinner::util::Logger::info ()
      << "Instrumenter class created [verboseness level: "
      << edu::sharif::twinner::util::Logger::getVerbosenessLevelAsString () << "]\n";
  managedInstructions.insert
      (make_pair (XED_ICLASS_MOV, COMMON_INS_MODELS));
  // TODO: handle more types (data ranges), then add real support for sign/zero extension
  /*
   * Zero extension <=> no changes
   *    | the value which is in 8bits is in 16bits too.
   */
  managedInstructions.insert
      (make_pair (XED_ICLASS_MOVZX, // 2 models (r <- zero-extend(r/m))
                  MOV_ZX_AND_SX_INS_MODELS));
  /*
   * Sign extension <=> add dummy conditions
   *    | small value => without change
   *    | large value => minus half of max value (to become signed)
   */
  managedInstructions.insert
      (make_pair (XED_ICLASS_MOVSX, // 2 models (r <- sign-extend(r/m))
                  MOV_ZX_AND_SX_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_MOVSXD, // 2 models (r <- sign-extend(r/m))
                  MOV_ZX_AND_SX_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_CMOVBE, DST_REG_SRC_EITHER_REG_OR_MEM));
  managedInstructions.insert
      (make_pair (XED_ICLASS_CMOVNBE, DST_REG_SRC_EITHER_REG_OR_MEM));
  managedInstructions.insert
      (make_pair (XED_ICLASS_CMPXCHG, DST_EITHER_REG_OR_MEM_SRC_REG_AUX_REG));
  managedInstructions.insert
      (make_pair (XED_ICLASS_XCHG, DST_EITHER_REG_OR_MEM_SRC_MUTABLE_REG));
  managedInstructions.insert
      (make_pair (XED_ICLASS_PUSH, PUSH_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_POP, POP_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_NOP, NOP_INS_MODELS));
  managedInstructions.insert // Some jumps change RSP (probably PIN is hiding some of instructions)
      (make_pair (XED_ICLASS_JMP, JMP_INS_MODELS));
  managedInstructions.insert // ignoring syscall as it is handled by callback routines
      (make_pair (XED_ICLASS_SYSCALL, NOP_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_ADD, COMMON_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_SUB, COMMON_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_CMP, COMMON_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_JNZ, JMP_CC_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_JZ, JMP_CC_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_JLE, JMP_CC_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_JNLE, JMP_CC_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_JNL, JMP_CC_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_JBE, JMP_CC_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_JNBE, JMP_CC_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_JNB, JMP_CC_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_JS, JMP_CC_INS_MODELS));
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
      (make_pair (XED_ICLASS_SHL, SHIFT_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_SHR, SHIFT_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_SAR, SHIFT_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_AND, COMMON_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_OR, COMMON_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_XOR, COMMON_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_DIV, DST_REG_REG_SRC_REG));
  managedInstructions.insert
      (make_pair (XED_ICLASS_MUL, DST_REG_REG_SRC_REG));
  managedInstructions.insert
      (make_pair (XED_ICLASS_TEST, TEST_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_RDTSC, OPERAND_LESS)); // read time-stamp counter
  managedInstructions.insert
      (make_pair (XED_ICLASS_PMOVMSKB, DST_REG_SRC_LARGE_REG)); // packed move mask-byte
  managedInstructions.insert
      (make_pair (XED_ICLASS_PCMPEQB, PCMPEQX_INS_MODELS)); // packed compare byte
  managedInstructions.insert
      (make_pair (XED_ICLASS_PXOR, PCMPEQX_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_MOVDQU, MOV_INS_WITH_LARGE_REG_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_BSF, DST_REG_SRC_EITHER_REG_OR_MEM));
  managedInstructions.insert
      (make_pair (XED_ICLASS_INC, DST_EITHER_REG_OR_MEM_SRC_IMPLICIT));
  managedInstructions.insert
      (make_pair (XED_ICLASS_DEC, DST_EITHER_REG_OR_MEM_SRC_IMPLICIT));
  managedInstructions.insert
      (make_pair (XED_ICLASS_SETNZ, DST_EITHER_REG_OR_MEM_SRC_IMPLICIT));
  managedInstructions.insert
      (make_pair (XED_ICLASS_SCASB, STRING_OPERATION_REG_MEM));
}

Instrumenter::~Instrumenter () {
  delete ise;
}

void Instrumenter::instrumentSingleInstruction (INS ins) {
  if (disabled) {
    RTN rtn = INS_Rtn (ins);
    if (RTN_Valid (rtn) && RTN_Name (rtn) == "main") {
      enable ();
    } else {
      return;
    }
  }
  std::string insAssemblyStr = INS_Disassemble (ins);
  const int size = insAssemblyStr.length () + 1;
  UINT32 allocatedIndex = ise->getTrace ()->getMemoryManager ()->allocate (size);
  char *allocated = ise->getTrace ()->getMemoryManager ()
      ->getPointerToAllocatedMemory (allocatedIndex);
  char * const insAssembly = allocated;
  //insAssembly = new char[insAssemblyStr.length () + 1];
  for (const char *ptr = insAssemblyStr.c_str (); *ptr; ++ptr) {
    *allocated++ = *ptr;
  }
  *allocated = '\0';

  printDebugInformation (ins, insAssembly);
  if (INS_IsOriginal (ins)) {
    totalCountOfInstructions++;
  }
  OPCODE op = INS_Opcode (ins);
  // Intel has 1024 opcodes. And each opcode has several types/models :)
  std::map < OPCODE, InstructionModel >::iterator it =
      managedInstructions.find (op);
  if (it == managedInstructions.end ()) {
    edu::sharif::twinner::util::Logger::info ()
        << "Ignoring assembly instruction: " << insAssembly << '\n';
  } else {
    edu::sharif::twinner::util::Logger::debug () << "\t--> " << OPCODE_StringShort (op)
        << " instruction\n";
    countOfInstructionsPerOpcode[op]++;
    InstructionModel expectedModels = it->second;
    InstructionModel model = getInstructionModel (op, ins);
    if (model) {
      if (expectedModels & model) {
        instrumentSingleInstruction (model, op, ins, allocatedIndex);
        return;

      } else {
        throw std::runtime_error ("UnExpected instruction model");
      }
    }
  }
  ise->getTrace ()->getMemoryManager ()->deallocate (size);
}

Instrumenter::InstructionModel Instrumenter::getInstructionModel (OPCODE op,
    INS ins) const {
  switch (op) {
  case XED_ICLASS_PUSH:
    return getInstructionModelForPushInstruction (ins);
  case XED_ICLASS_POP:
    return getInstructionModelForPopInstruction (ins);
  case XED_ICLASS_NOP:
  case XED_ICLASS_SYSCALL:
    return NOP_INS_MODELS;
  case XED_ICLASS_RDTSC:
    return OPERAND_LESS;
  case XED_ICLASS_DIV:
  case XED_ICLASS_MUL:
    return DST_REG_REG_SRC_REG;
  case XED_ICLASS_INC:
  case XED_ICLASS_DEC:
  case XED_ICLASS_SETNZ:
    return INS_OperandIsReg (ins, 0) ? DST_REG_SRC_IMPLICIT : DST_MEM_SRC_IMPLICIT;
  default:
    switch (INS_Category (ins)) {
    case XED_CATEGORY_COND_BR:
      return JMP_CC_INS_MODELS;
    case XED_CATEGORY_CALL:
    case XED_CATEGORY_RET:
    case XED_CATEGORY_UNCOND_BR:
      return DST_RSP_SRC_CALL;
    case XED_CATEGORY_STRINGOP:
      return INS_OperandIsReg (ins, 0) ?
          STRING_OPERATION_REG_MEM : STRING_OPERATION_MEM_MEM;
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
  const bool destIsReg = INS_OperandIsReg (ins, 0);
  const bool destIsMem = INS_OperandIsMemory (ins, 0);
  const bool sourceIsReg = INS_OperandIsReg (ins, 1);
  const bool sourceIsMem = INS_OperandIsMemory (ins, 1);
  const bool sourceIsImmed = INS_OperandIsImmediate (ins, 1);
  const bool sourceIsAdg = INS_OperandIsAddressGenerator (ins, 1);

  if (destIsReg && sourceIsReg) {
    const bool destRegIsXmm = REG_is_xmm (INS_OperandReg (ins, 0));
    const bool sourceRegIsXmm = REG_is_xmm (INS_OperandReg (ins, 1));
    if (destRegIsXmm && sourceRegIsXmm) {
      return DST_LARGE_REG_SRC_LARGE_REG;
    } else if (!destRegIsXmm && sourceRegIsXmm) {
      return DST_REG_SRC_LARGE_REG;
    } else {
      if (INS_OperandCount (ins) > 2 && INS_OperandIsReg (ins, 2)
          && REG_is_gr_any_size (INS_OperandReg (ins, 2))) {
        return DST_REG_SRC_REG_AUX_REG;
      } else {
        if (INS_OperandWritten (ins, 1)) {
          return DST_REG_SRC_MUTABLE_REG;
        } else {
          return DST_REG_SRC_REG;
        }
      }
    }

  } else if (destIsReg && sourceIsMem) {
    const bool destRegIsXmm = REG_is_xmm (INS_OperandReg (ins, 0));
    if (destRegIsXmm) {
      return DST_LARGE_REG_SRC_MEM;
    } else {
      return DST_REG_SRC_MEM;
    }

  } else if (destIsReg && sourceIsImmed) {
    return DST_REG_SRC_IMD;

  } else if (destIsMem && sourceIsReg) {
    const bool sourceRegIsXmm = REG_is_xmm (INS_OperandReg (ins, 1));
    if (sourceRegIsXmm) {
      return DST_MEM_SRC_LARGE_REG;
    } else {
      if (INS_OperandCount (ins) > 2 && INS_OperandIsReg (ins, 2)
          && REG_is_gr_any_size (INS_OperandReg (ins, 2))) {
        return DST_MEM_SRC_REG_AUX_REG;
      } else {
        if (INS_OperandWritten (ins, 1)) {
          return DST_MEM_SRC_MUTABLE_REG;
        } else {
          return DST_MEM_SRC_REG;
        }
      }
    }

  } else if (destIsMem && sourceIsImmed) {
    return DST_MEM_SRC_IMD;

  } else if (destIsReg && sourceIsAdg) {
    return DST_REG_SRC_ADG;

  } else {
    throw std::runtime_error ("Unknown instruction model");
  }
}

inline void Instrumenter::checkForInitialState (INS ins) const {
  static bool firstTime = true;
  if (firstTime) {
    firstTime = false;
    // initializing registers, right before execution of first instrumented instruction
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineInitializeRegisters,
                    IARG_PTR, ise,
                    IARG_CONTEXT,
                    IARG_END);
  }
}

void Instrumenter::instrumentSingleInstruction (InstructionModel model, OPCODE op,
    INS ins, UINT32 insAssembly) const {
  checkForInitialState (ins);
  INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineRunHooks,
                  IARG_PTR, ise,
                  IARG_CONST_CONTEXT,
                  IARG_END);
  switch (model) {
  case DST_REG_SRC_REG:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    REG srcreg = INS_OperandReg (ins, 1);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstRegSrcReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                    IARG_UINT32, srcreg, IARG_REG_VALUE, srcreg,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_REG_SRC_MUTABLE_REG:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    REG srcreg = INS_OperandReg (ins, 1);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstRegSrcMutableReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                    IARG_UINT32, srcreg, IARG_REG_VALUE, srcreg,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_REG_SRC_REG_AUX_REG:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    REG srcreg = INS_OperandReg (ins, 1);
    REG auxreg = INS_OperandReg (ins, 2);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstRegSrcRegAuxReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                    IARG_UINT32, srcreg, IARG_REG_VALUE, srcreg,
                    IARG_UINT32, auxreg, IARG_REG_VALUE, auxreg,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_REG_SRC_LARGE_REG:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    REG srcreg = INS_OperandReg (ins, 1);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstRegSrcLargeReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                    IARG_UINT32, srcreg, IARG_REG_CONST_REFERENCE, srcreg,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_LARGE_REG_SRC_LARGE_REG:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    REG srcreg = INS_OperandReg (ins, 1);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstLargeRegSrcLargeReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_CONST_REFERENCE, dstreg,
                    IARG_UINT32, srcreg, IARG_REG_CONST_REFERENCE, srcreg,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_LARGE_REG_SRC_MEM:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstLargeRegSrcMem,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_CONST_REFERENCE, dstreg,
                    IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_UINT32, insAssembly,
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
                    IARG_UINT32, insAssembly,
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
                    IARG_UINT32, insAssembly,
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
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_MEM_SRC_MUTABLE_REG:
  {
    REG srcreg = INS_OperandReg (ins, 1);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcMutableReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 0,
                    IARG_UINT32, srcreg, IARG_REG_VALUE, srcreg,
                    IARG_MEMORYREAD_SIZE,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_MEM_SRC_REG_AUX_REG:
  {
    REG srcreg = INS_OperandReg (ins, 1);
    REG auxreg = INS_OperandReg (ins, 2);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcRegAuxReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 0,
                    IARG_UINT32, srcreg, IARG_REG_VALUE, srcreg,
                    IARG_UINT32, auxreg, IARG_REG_VALUE, auxreg,
                    IARG_MEMORYREAD_SIZE,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_MEM_SRC_LARGE_REG:
  {
    REG srcreg = INS_OperandReg (ins, 1);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcLargeReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 0,
                    IARG_UINT32, srcreg, IARG_REG_CONST_REFERENCE, srcreg,
                    IARG_MEMORYREAD_SIZE,
                    IARG_UINT32, insAssembly,
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
                    IARG_UINT32, insAssembly,
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
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_STK_SRC_IMD:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcImd,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 0,
                    IARG_ADDRINT, INS_OperandImmediate (ins, 0),
                    IARG_UINT32, insAssembly,
                    IARG_END);

    break;
  }
  case DST_STK_SRC_MEM:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcMem,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 1,
                    IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_MEM_SRC_STK:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcMem,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 0,
                    IARG_MEMORYOP_EA, 1, IARG_MEMORYREAD_SIZE,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case JMP_CC_INS_MODELS:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineConditionalBranch,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_BRANCH_TAKEN,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_REG_SRC_ADG:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    INS_InsertCall (ins, IPOINT_AFTER, (AFUNPTR) analysisRoutineDstRegSrcAdg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_RSP_SRC_CALL:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineBeforeChangeOfReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, REG_RSP,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_REG_REG_SRC_REG:
  {
    REG srcreg = INS_OperandReg (ins, 0);
    REG dstRightReg = INS_OperandReg (ins, 1);
    REG dstLeftReg = INS_OperandReg (ins, 2);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineTwoDstRegOneSrcReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstLeftReg, IARG_REG_VALUE, dstLeftReg,
                    IARG_UINT32, dstRightReg, IARG_REG_VALUE, dstRightReg,
                    IARG_UINT32, srcreg, IARG_REG_VALUE, srcreg,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case OPERAND_LESS:
  {
    INS_InsertCall (ins, IPOINT_AFTER, (AFUNPTR) analysisRoutineAfterOperandLess,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_CONST_CONTEXT,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_REG_SRC_IMPLICIT:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstRegSrcImplicit,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_MEM_SRC_IMPLICIT:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcImplicit,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case STRING_OPERATION_REG_MEM:
  {
    const REG dstreg = INS_OperandReg (ins, 0);
    INS_InsertPredicatedCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineStrOpRegMem,
                              IARG_PTR, ise, IARG_UINT32, op,
                              IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                              IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                              IARG_UINT32, REG_RDI, IARG_REG_VALUE, REG_RDI,
                              IARG_UINT32, insAssembly,
                              IARG_END);
    const BOOL repe = INS_RepPrefix (ins);
    const BOOL repne = INS_RepnePrefix (ins);
    if (repe || repne) {
      REG repreg = INS_RepCountRegister (ins);
      if (repreg == REG_INVALID ()) {
        throw std::runtime_error ("INS_Rep(ne)Prefix conflicts INS_RepCountRegister");
      }
      INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineRepPrefix,
                      IARG_PTR, ise, IARG_UINT32, op,
                      IARG_UINT32, repreg, IARG_REG_VALUE, repreg,
                      IARG_EXECUTING,
                      IARG_UINT32, repe,
                      IARG_UINT32, insAssembly,
                      IARG_END);
    }
    break;
  }
  default:
    throw std::runtime_error ("Unknown instruction model");
  }
}

void Instrumenter::printDebugInformation (INS ins, const char *insAssembly) const {
  edu::sharif::twinner::util::Logger debug = edu::sharif::twinner::util::Logger::debug ();
  bool isMemoryRead = INS_IsMemoryRead (ins);
  bool isMemoryWrite = INS_IsMemoryWrite (ins);
  bool isOriginal = INS_IsOriginal (ins);
  UINT32 countOfOperands = INS_OperandCount (ins);
  debug << "Instrumenting assembly instruction: " << insAssembly
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
  UNUSED_VARIABLE (threadIndex);
  edu::sharif::twinner::util::Logger::loquacious () << "***** syscallEntryPoint *****\n";
  ise->syscallInvoked (ctxt, edu::sharif::twinner::trace::Syscall (std));

  if (isWithinInitialStateDetectionMode) {
    edu::sharif::twinner::util::Logger::debug () << "Gathering initial contents of"
        " requested memory addresses, right before first syscall\n";
    saveMemoryContentsToFile (traceFilePath.c_str ());
    edu::sharif::twinner::util::Logger::debug () << "Done.\tExiting...\n";
    exit (0); // think about probably acquired locks of application
  }
}

void Instrumenter::syscallExitPoint (THREADID threadIndex, CONTEXT *ctxt,
    SYSCALL_STANDARD std) {
  UNUSED_VARIABLE (threadIndex);
  UNUSED_VARIABLE (std);
  edu::sharif::twinner::util::Logger::loquacious () << "*** syscallExitPoint ***\n";
  ise->syscallReturned (ctxt);
}

void Instrumenter::saveMemoryContentsToFile (const char *path) const {
  std::map < ADDRINT, UINT64 > map;
  edu::sharif::twinner::util::foreach (candidateAddresses,
                                       &read_memory_content_and_add_it_to_map, map);
  if (!ise->getTrace ()->saveAddressToValueMapToFile (map, path)) {
    throw std::runtime_error ("Can not save address-to-value map into binary file");
  }
}

void read_memory_content_and_add_it_to_map (std::map < ADDRINT, UINT64 > &map,
    const ADDRINT &address) {
  //TODO: Communicate mem-read size and read 64bits or 128bits values accordingly
  map.insert (make_pair (address,
                         edu::sharif::twinner::util::readMemoryContent (address)));
}

void Instrumenter::aboutToExit (INT32 code) {
  printInstructionsStatisticsInfo ();
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::debug ();
  logger << "Saving trace info...";
  if (ise->getTrace ()->saveToFile
      (traceFilePath.c_str (), disassemblyFilePath.c_str ())) {
    logger << "Done.\n";
  } else {
    logger << "Failed.\n";
  }
}

void Instrumenter::disable () {
  disabled = true;
  ise->disable ();
}

void Instrumenter::enable () {
  disabled = false;
  ise->enable ();
}

void Instrumenter::printInstructionsStatisticsInfo () const {
  int countOfIgnoredInstructions = totalCountOfInstructions;
  for (std::map < OPCODE, int >::const_iterator it =
      countOfInstructionsPerOpcode.begin (); it != countOfInstructionsPerOpcode.end ();
      ++it) {
    int op = it->first;
    int c = it->second;
    edu::sharif::twinner::util::Logger::info ()
        << "count of " << OPCODE_StringShort (op) << " instructions: "
        << std::dec << c << " (" << (c * 100.0 / totalCountOfInstructions) << " %)\n";
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

VOID imageIsLoaded (IMG img, VOID *v) {
  edu::sharif::twinner::util::Logger log = edu::sharif::twinner::util::Logger::debug ();
  log << "Instrumenting image...";
  for (SEC section = IMG_SecHead (img); SEC_Valid (section);
      section = SEC_Next (section)) {
    for (RTN routine = SEC_RtnHead (section); RTN_Valid (routine);
        routine = RTN_Next (routine)) {
      std::string name = RTN_Name (routine);
      if (name == "main") {
        log << " routine: " << name;
        RTN_Open (routine);
        /*
         * All instructions before main() routine are owned by RTLD. So we should start
         * instrumenting instructions after when main() routine is called. Also
         * instructions after returning from main() are owned by RTLD and so, instrumenter
         * and analysis routines should be disabled when main() returns.
         */
        RTN_InsertCall (routine, IPOINT_BEFORE, (AFUNPTR) startAnalysis,
                        IARG_PTR, v,
                        IARG_END);
        RTN_InsertCall (routine, IPOINT_AFTER, (AFUNPTR) terminateAnalysis,
                        IARG_PTR, v,
                        IARG_END);
        RTN_Close (routine);
        log << '\n';
        return;
      }
    }
  }
  log << '\n';
}

VOID startAnalysis (VOID *v) {
  edu::sharif::twinner::util::Logger::loquacious ()
      << "********** startAnalysis(...) **********\n";
  Instrumenter *im = (Instrumenter *) v;
  im->enable ();
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

VOID terminateAnalysis (VOID *imptr) {
  edu::sharif::twinner::util::Logger::loquacious ()
      << "********** terminateAnalysis(...) **********\n";
  Instrumenter *im = (Instrumenter *) imptr;
  im->disable ();
}

}
}
}
}
