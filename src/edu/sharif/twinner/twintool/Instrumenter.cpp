//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2017 Behnam Momeni
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
#include <fstream>

#include "xed-iclass-enum.h"

#include "InstructionSymbolicExecuter.h"
#include "MemoryResidentExpressionValueProxy.h"

#include "edu/sharif/twinner/trace/MarInfo.h"

#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/syscall/Syscall.h"
#include "edu/sharif/twinner/trace-twintool/FunctionInfo.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/LogStream.h"
#include "edu/sharif/twinner/util/iterationtools.h"
#include "edu/sharif/twinner/util/memory.h"
#include "edu/sharif/twinner/util/MemoryManager.h"

using namespace std;

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

using namespace instructionmodels;

#ifdef TARGET_LINUX
#define MAIN_ROUTINE_NAME "main"
#elif TARGET_WINDOWS
#define MAIN_ROUTINE_NAME "__main"
#else
#error "Unsupported OS"
#endif

inline void read_memory_content_and_add_it_to_map (
    std::map < std::pair < ADDRINT, int >, UINT64 > &map,
    const std::pair < ADDRINT, int > &address);

Instrumenter::Instrumenter (std::ifstream &symbolsFileInStream,
    const string &_traceFilePath, const std::string &_disassemblyFilePath,
    bool _disabled, int _stackOffset,
    ADDRINT _start, ADDRINT _end,
    std::vector<edu::sharif::twinner::trace::FunctionInfo> _safeFunctionsInfo,
    bool _naive, bool measureMode) :
    traceFilePath (_traceFilePath), disassemblyFilePath (_disassemblyFilePath),
    ise (new InstructionSymbolicExecuter (this, symbolsFileInStream,
    _disabled, measureMode)),
    isWithinInitialStateDetectionMode (false),
    disabled (_disabled || _naive),
    withinSafeFunc (false),
    stackOffset (_stackOffset),
    naive (_naive),
    start (_start), end (_end),
    safeFunctionsInfo (_safeFunctionsInfo),
    totalCountOfInstructions (0) {
  initialize ();
}

Instrumenter::Instrumenter (
    const std::set < std::pair < ADDRINT, int > > &_candidateAddresses,
    const std::string &_traceFilePath, const std::string &_disassemblyFilePath,
    bool _disabled, int _stackOffset, ADDRINT _start, ADDRINT _end) :
    traceFilePath (_traceFilePath), disassemblyFilePath (_disassemblyFilePath),
    ise (new InstructionSymbolicExecuter (this, _disabled)),
    candidateAddresses (_candidateAddresses),
    isWithinInitialStateDetectionMode (true),
    disabled (_disabled),
    withinSafeFunc (false),
    stackOffset (_stackOffset),
    start (_start), end (_end),
    totalCountOfInstructions (0) {
  initialize ();
}

Instrumenter::Instrumenter (const string &_traceFilePath,
    const std::string &_disassemblyFilePath, bool _disabled, int _stackOffset,
    ADDRINT _start, ADDRINT _end,
    std::vector<edu::sharif::twinner::trace::FunctionInfo> _safeFunctionsInfo,
    bool _naive) :
    traceFilePath (_traceFilePath), disassemblyFilePath (_disassemblyFilePath),
    ise (new InstructionSymbolicExecuter (this, _disabled)),
    isWithinInitialStateDetectionMode (false),
    disabled (_disabled || _naive),
    withinSafeFunc (false),
    stackOffset (_stackOffset),
    naive (_naive),
    start (_start), end (_end),
    safeFunctionsInfo (_safeFunctionsInfo),
    totalCountOfInstructions (0) {
  initialize ();
}

#ifdef TARGET_LINUX
#define INITIALIZE(MODEL, OPCODES...) \
  initialize<sizeof ((const OPCODE[]) {OPCODES}) / sizeof (OPCODE)> \
    (MODEL, (const OPCODE[]) {OPCODES})
#endif
#ifdef TARGET_WINDOWS
#define Y_TUPLE_SIZE(...) Y_TUPLE_SIZE_II((Y_TUPLE_SIZE_PREFIX_ ## __VA_ARGS__ ## _Y_TUPLE_SIZE_POSTFIX,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0))
#define Y_TUPLE_SIZE_II(__args) Y_TUPLE_SIZE_I __args
#define Y_TUPLE_SIZE_PREFIX__Y_TUPLE_SIZE_POSTFIX ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,0
#define Y_TUPLE_SIZE_I(__p0,__p1,__p2,__p3,__p4,__p5,__p6,__p7,__p8,__p9,__p10,__p11,__p12,__p13,__p14,__p15,__p16,__p17,__p18,__p19,__p20,__p21,__p22,__p23,__p24,__p25,__p26,__p27,__p28,__p29,__p30,__p31,__n,...) __n
#define INITIALIZE(MODEL, ...) \
  do { \
    const OPCODE opcodes[] = {__VA_ARGS__}; \
    initialize<Y_TUPLE_SIZE (__VA_ARGS__)> (MODEL, opcodes); \
  } while (false)
#endif

void Instrumenter::initialize () {
  edu::sharif::twinner::util::Logger::info ()
      << "Instrumenter class created [verboseness level: "
      << edu::sharif::twinner::util::LogStream::getInstance ()
      ->getVerbosenessLevelAsString () << "]\n";
  INITIALIZE (COMMON_INS_MODELS,
              XED_ICLASS_MOV, XED_ICLASS_ADD, XED_ICLASS_ADC,
              XED_ICLASS_SUB, XED_ICLASS_SBB, XED_ICLASS_CMP,
              XED_ICLASS_AND, XED_ICLASS_OR, XED_ICLASS_XOR);
  // Zero extension <=> no changes ; the value which is in 8bits is in 16bits too.
  INITIALIZE (MOV_ZX_AND_SX_INS_MODELS,
              XED_ICLASS_MOVZX, XED_ICLASS_MOVSX, XED_ICLASS_MOVSXD);
  managedInstructions.insert // xmm <- r/m  OR  r/m <- xmm
      (std::make_pair (XED_ICLASS_MOVD, DST_LARGE_REG_SRC_EITHER_REG_OR_MEM_OR_VICE_VERSA));
  INITIALIZE (CMOV_INS_MODELS,
              XED_ICLASS_CMOVB, XED_ICLASS_CMOVBE, XED_ICLASS_CMOVL,
              XED_ICLASS_CMOVLE, XED_ICLASS_CMOVNB, XED_ICLASS_CMOVNBE,
              XED_ICLASS_CMOVNL, XED_ICLASS_CMOVNLE, XED_ICLASS_CMOVNO,
              XED_ICLASS_CMOVNP, XED_ICLASS_CMOVNS, XED_ICLASS_CMOVNZ,
              XED_ICLASS_CMOVO, XED_ICLASS_CMOVP, XED_ICLASS_CMOVS,
              XED_ICLASS_CMOVZ);
  managedInstructions.insert
      (make_pair (XED_ICLASS_CMPXCHG, DST_EITHER_REG_OR_MEM_SRC_REG_AUX_REG));
  INITIALIZE (DST_EITHER_REG_OR_MEM_SRC_MUTABLE_REG,
              XED_ICLASS_XCHG, XED_ICLASS_XADD);
  managedInstructions.insert
      (make_pair (XED_ICLASS_PUSHFD, DST_STK_SRC_IMPLICIT));
  managedInstructions.insert
      (make_pair (XED_ICLASS_PUSH, PUSH_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_POP, POP_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_NOP, NOP_INS_MODELS));
  managedInstructions.insert // Some jumps change RSP (probably PIN is hiding some of instructions)
      (make_pair (XED_ICLASS_JMP, JMP_INS_MODELS));
  INITIALIZE (SYSCALL_INS_MODEL,
              XED_ICLASS_SYSCALL, XED_ICLASS_SYSENTER);
  INITIALIZE (JMP_CC_INS_MODELS,
              XED_ICLASS_JB, XED_ICLASS_JBE, XED_ICLASS_JL, XED_ICLASS_JLE,
              XED_ICLASS_JNB, XED_ICLASS_JNBE, XED_ICLASS_JNL, XED_ICLASS_JNLE,
              XED_ICLASS_JNO, XED_ICLASS_JNP, XED_ICLASS_JNS, XED_ICLASS_JNZ,
              XED_ICLASS_JO, XED_ICLASS_JP, XED_ICLASS_JS, XED_ICLASS_JZ);
  managedInstructions.insert
      (make_pair (XED_ICLASS_LEA, LEA_INS_MODELS));
  INITIALIZE (CALL_INS_MODELS, XED_ICLASS_CALL_NEAR, XED_ICLASS_CALL_FAR);
  INITIALIZE (RET_INS_MODELS, XED_ICLASS_RET_NEAR, XED_ICLASS_RET_FAR);
  managedInstructions.insert
      (make_pair (XED_ICLASS_LEAVE, LEAVE_INS_MODELS));
  INITIALIZE (SHIFT_INS_MODELS,
              XED_ICLASS_TEST, XED_ICLASS_BT,
              XED_ICLASS_BTR,
              XED_ICLASS_SHL, XED_ICLASS_SHR, XED_ICLASS_SAR,
              XED_ICLASS_ROR, XED_ICLASS_ROL);
  managedInstructions.insert
      (make_pair (XED_ICLASS_SHLD, SHIFT_DOUBLE_PRECISION_INS_MODELS));
  INITIALIZE (DST_REG_REG_SRC_EITHER_REG_OR_MEM,
              XED_ICLASS_DIV, XED_ICLASS_MUL);
  /*
   * IMUL is for signed multiplication and has three models:
   * 1. one operand, like IMUL r1/m1, where it does DX:AX <- AX*r1/m1 and a like.
   * In this form, destination size is twice each source operand and so the signed
   * result is different from the unsigned multiplication result.
   * For correct calculation, values can be sign-extended to double size and then
   * perform the unsigned multiplication. As unsigned and signed multiplications have
   * the same result iff the result be truncated to the same bit size.
   * 2. two operands, like IMUL r1, r2/m2, where it does r1 <- r1*r2/m2 and so works like
   * unsigned multiplication.
   * 3. three operands, like IMUL r1, r2/m2, imd1, where it does r1 <- r2/m2*imd1 and so
   * it works like unsigned multiplication.
   */
  managedInstructions.insert
      (make_pair (XED_ICLASS_IMUL, IMUL_INS_MODELS));
  managedInstructions.insert
      (make_pair (XED_ICLASS_IDIV, IDIV_INS_MODELS));
  INITIALIZE (DST_REG_SRC_REG,
              XED_ICLASS_CDQE, XED_ICLASS_CDQ);
  INITIALIZE (OPERAND_LESS,
              XED_ICLASS_RDTSC, // read time-stamp counter
              XED_ICLASS_CLD, // clear direction flag (DF)
              XED_ICLASS_CPUID);
  managedInstructions.insert
      (make_pair (XED_ICLASS_PMOVMSKB, DST_REG_SRC_LARGE_REG)); // packed move mask-byte
  INITIALIZE (PCMPEQX_INS_MODELS,
              XED_ICLASS_PCMPEQB, // packed compare byte
              XED_ICLASS_PXOR);
  INITIALIZE (DST_REG_SRC_EITHER_REG_OR_MEM_ANY_SIZE,
              XED_ICLASS_PMINUB, // packed min of unsigned bytes
              XED_ICLASS_POR);
  INITIALIZE (DST_LARGE_REG_SRC_EITHER_LARGE_REG_OR_MEM,
              XED_ICLASS_PUNPCKLBW, // unpack low data
              XED_ICLASS_PUNPCKLWD);
  managedInstructions.insert
      (make_pair (XED_ICLASS_PSHUFD, DST_LARGE_REG_SRC_EITHER_LARGE_REG_OR_MEM_AUX_IMD));
  INITIALIZE (MOV_INS_WITH_LARGE_REG_INS_MODELS | DST_REG_SRC_REG_ANY_SIZE,
              XED_ICLASS_MOVQ);
  INITIALIZE (MOV_INS_WITH_LARGE_REG_INS_MODELS,
              XED_ICLASS_MOVSD_XMM,
              XED_ICLASS_MOVAPS, XED_ICLASS_MOVDQU, XED_ICLASS_MOVDQA);
  managedInstructions.insert
      (make_pair (XED_ICLASS_LDDQU, DST_LARGE_REG_SRC_MEM));
  managedInstructions.insert
      (make_pair (XED_ICLASS_PSLLDQ, DST_LARGE_REG_SRC_IMD));
  managedInstructions.insert
      (make_pair (XED_ICLASS_BSF, DST_REG_SRC_EITHER_REG_OR_MEM));
  INITIALIZE (DST_EITHER_REG_OR_MEM_SRC_IMPLICIT,
              XED_ICLASS_INC, XED_ICLASS_DEC, XED_ICLASS_NEG,
              XED_ICLASS_SETB, XED_ICLASS_SETBE, XED_ICLASS_SETL, XED_ICLASS_SETLE,
              XED_ICLASS_SETNB, XED_ICLASS_SETNBE, XED_ICLASS_SETNL, XED_ICLASS_SETNLE,
              XED_ICLASS_SETNO, XED_ICLASS_SETNP, XED_ICLASS_SETNS, XED_ICLASS_SETNZ,
              XED_ICLASS_SETO, XED_ICLASS_SETP, XED_ICLASS_SETS, XED_ICLASS_SETZ,
              XED_ICLASS_NOT);
  INITIALIZE (STRING_OPERATION_MEM_REG,
              XED_ICLASS_STOSB, XED_ICLASS_STOSW,
              XED_ICLASS_STOSD, XED_ICLASS_STOSQ);
  INITIALIZE (STRING_OPERATION_REG_MEM,
              XED_ICLASS_SCASB, XED_ICLASS_SCASW);
  INITIALIZE (STRING_OPERATION_MEM_MEM,
              XED_ICLASS_MOVSQ, XED_ICLASS_MOVSD,
              XED_ICLASS_CMPSB, XED_ICLASS_CMPSW, XED_ICLASS_CMPSD, XED_ICLASS_CMPSQ);
  managedInstructions.insert
      (make_pair (XED_ICLASS_LODSD, DST_REG_SRC_MEM_AUX_RSI));
}

template<int size>
void Instrumenter::initialize (InstructionModel model, const OPCODE opcodes[]) {
  for (unsigned int i = 0; i < size; ++i) {
    managedInstructions.insert (make_pair (opcodes[i], model));
  }
}

Instrumenter::~Instrumenter () {
  delete ise;
}

void Instrumenter::registerInstrumentationRoutines () {
  if (disabled) { // this includes main and endpoints scenarios
    /**
     * This is required for -main option. That option commands instrumentation
     * to start from the main() routine instead of the RTLD start point.
     * Finding the main() routine requires symbols (so it's not reliable and
     * is not recommended for real malwares).
     * Other approach (start/end based scenario) always works.
     */
    PIN_InitSymbols ();

    IMG_AddInstrumentFunction ((IMAGECALLBACK) imageIsLoaded, this);
  }
  if (!safeFunctionsInfo.empty ()) {
    if (!disabled) {
      PIN_InitSymbols ();
    }
    IMG_AddInstrumentFunction ((IMAGECALLBACK) instrumentSafeFuncs, this);
  }
  //TODO: Consider instrumenting at higher granularity for more performance
  INS_AddInstrumentFunction (instrumentSingleInst, this);

  PIN_AddSyscallEntryFunction (syscallIsAboutToBeCalled, this);
  PIN_AddSyscallExitFunction (syscallIsReturned, this);

  PIN_AddFiniFunction (applicationIsAboutToExit, this);
}

void Instrumenter::instrumentSafeFunctions (IMG img) {
  for (std::vector<edu::sharif::twinner::trace::FunctionInfo>
      ::const_iterator it = safeFunctionsInfo.begin ();
      it != safeFunctionsInfo.end (); ++it) {
    const std::string name = it->getName ();
    RTN safeRoutine = RTN_FindByName (img, name.c_str ());
    if (RTN_Valid (safeRoutine)) {
      edu::sharif::twinner::util::Logger::debug ()
          << "Instrumenter::instrumentSafeFunctions (img):"
          " instrumenting ret from the ``" << name << "'' safe routine\n";
      RTN_Open (safeRoutine);
      RTN_InsertCall (safeRoutine, IPOINT_AFTER, (AFUNPTR) afterSafeFunc,
                      IARG_PTR, this,
                      IARG_CONTEXT,
                      IARG_END);
      RTN_Close (safeRoutine);
    }
  }
}

bool Instrumenter::instrumentSafeFunctions (INS ins, UINT32 insAssembly) const {
  if (INS_IsDirectCall (ins)) {
    const ADDRINT target = INS_DirectBranchOrCallTargetAddress (ins);
    for (std::vector<edu::sharif::twinner::trace::FunctionInfo>
        ::const_iterator it = safeFunctionsInfo.begin ();
        it != safeFunctionsInfo.end (); ++it) {
      const edu::sharif::twinner::trace::FunctionInfo &fi = *it;
      if (fi.getAddress () == target) {
        const std::string name = fi.getName ();
        edu::sharif::twinner::util::Logger::debug ()
            << "Instrumenter::instrumentSafeFunctions (ins):"
            " instrumenting call to the ``" << name << "'' safe routine\n";
        INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) beforeSafeFunc,
                        IARG_PTR, this,
                        IARG_ADDRINT, INS_Address (ins) + INS_Size (ins),
                        IARG_PTR, &fi,
                        IARG_UINT32, insAssembly,
                        IARG_CONST_CONTEXT,
                        IARG_END);
        return true;
      }
    }
  }
  return false;
}

void Instrumenter::setMainArgsReportingFilePath (const std::string &_marFilePath) {
  marFilePath = _marFilePath;
}

bool Instrumenter::instrumentSingleInstruction (INS ins) {
  std::stringstream ss;
  ss << INS_Disassemble (ins) << " @" << std::hex << INS_Address (ins);
  const std::string insAssemblyStr = ss.str ();
  const int size = insAssemblyStr.length () + 1;
  UINT32 allocatedIndex;
  if (!ise->getTraceMemoryManager ()->allocate (allocatedIndex, size)) {
    edu::sharif::twinner::util::Logger::error ()
        << "Instrumenter::instrumentSingleInstruction (...):"
        " Memory allocation failed\n";
    return false;
  }
  char *allocated = ise->getTraceMemoryManager ()
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
        << "[opcode: " << std::dec << op << "] "
        << "Ignoring assembly instruction: " << insAssembly << '\n';
  } else if (naive) {
    edu::sharif::twinner::util::Logger::info ()
        << "[opcode: " << std::dec << op << "] [NAIVE MODE] "
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
        return true;

      } else {
        edu::sharif::twinner::util::Logger::error ()
            << "Instrumenter::instrumentSingleInstruction (...):"
            " Unexpected instruction model"
            " (expectedModels = " << expectedModels
            << ", actualModel = " << model << ")\n";
        return false;
      }
    }
  }
  ise->getTraceMemoryManager ()->deallocate (size);
  return true;
}

Instrumenter::InstructionModel Instrumenter::getInstructionModel (OPCODE op,
    INS ins) const {
  switch (op) {
  case XED_ICLASS_PUSH:
  case XED_ICLASS_PUSHFD:
    return getInstructionModelForPushInstruction (ins);
  case XED_ICLASS_POP:
    return getInstructionModelForPopInstruction (ins);
  case XED_ICLASS_NOP:
    return NOP_INS_MODELS;
  case XED_ICLASS_SYSCALL:
  case XED_ICLASS_SYSENTER:
    return SYSCALL_INS_MODEL;
  case XED_ICLASS_RDTSC:
  case XED_ICLASS_CLD:
  case XED_ICLASS_CPUID:
    return OPERAND_LESS;
  case XED_ICLASS_DIV:
  case XED_ICLASS_IDIV:
  case XED_ICLASS_MUL:
    return INS_OperandIsReg (ins, 0) ? DST_REG_REG_SRC_REG
        : DST_REG_REG_SRC_MEM;
  case XED_ICLASS_IMUL:
    switch (INS_OperandCount (ins)) {
    case 3:
      // two operands
      return INS_OperandIsReg (ins, 1) ? DST_REG_SRC_REG : DST_REG_SRC_MEM;
    case 4:
      if (INS_OperandIsImmediate (ins, 2)) {
        // three operands
        return INS_OperandIsReg (ins, 1) ?
            DST_REG_SRC_REG_AUX_IMD : DST_REG_SRC_MEM_AUX_IMD;
      }
      // one operand
      return INS_OperandIsReg (ins, 0) ? DST_REG_REG_SRC_REG : DST_REG_REG_SRC_MEM;
    }
    edu::sharif::twinner::util::Logger::error ()
        << "Instrumenter::getInstructionModel (op=" << op << ", ins=...): "
        "Unknown IMUL model\n";
    abort ();
  case XED_ICLASS_INC:
  case XED_ICLASS_DEC:
  case XED_ICLASS_NEG:
  case XED_ICLASS_SETO:
  case XED_ICLASS_SETNP:
  case XED_ICLASS_SETNS:
  case XED_ICLASS_SETNZ:
  case XED_ICLASS_SETZ:
  case XED_ICLASS_SETLE:
  case XED_ICLASS_SETNLE:
  case XED_ICLASS_SETL:
  case XED_ICLASS_SETNL:
  case XED_ICLASS_SETB:
  case XED_ICLASS_SETBE:
  case XED_ICLASS_SETNBE:
  case XED_ICLASS_SETNB:
  case XED_ICLASS_NOT:
    return INS_OperandIsReg (ins, 0) ? DST_REG_SRC_IMPLICIT : DST_MEM_SRC_IMPLICIT;
  case XED_ICLASS_LEAVE:
    return LEAVE_INS_MODELS;
  case XED_ICLASS_LODSD:
    return DST_REG_SRC_MEM_AUX_RSI;
  default:
    switch (INS_Category (ins)) {
    case XED_CATEGORY_COND_BR:
      return JMP_CC_INS_MODELS;
    case XED_CATEGORY_CMOV:
      return CMOV_INS_MODELS;
    case XED_CATEGORY_RET:
      if (INS_OperandIsImmediate (ins, 0)) {
        return RET_INS_WITH_ARG_MODEL;
      }
    case XED_CATEGORY_CALL:
    case XED_CATEGORY_UNCOND_BR:
      return DST_RSP_SRC_CALL;
    case XED_CATEGORY_STRINGOP:
      if (INS_OperandIsReg (ins, 0)) {
        return STRING_OPERATION_REG_MEM;
      } else if (INS_OperandIsReg (ins, 2)) {
        return STRING_OPERATION_MEM_REG;
      } else {
        return STRING_OPERATION_MEM_MEM;
      }
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
  bool sourceIsImplicit = INS_OperandIsImplicit (ins, 0);
  if (sourceIsReg) {
    return DST_STK_SRC_REG;
  } else if (sourceIsMem) {
    return DST_STK_SRC_MEM;
  } else if (sourceIsImmed) {
    return DST_STK_SRC_IMD;
  } else if (sourceIsImplicit) {
    return DST_STK_SRC_IMPLICIT;
  } else {
    edu::sharif::twinner::util::Logger::error ()
        << "Instrumenter::getInstructionModelForPushInstruction (...): "
        "Unknown PUSH instruction model\n";
    abort ();
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
    edu::sharif::twinner::util::Logger::error ()
        << "Instrumenter::getInstructionModelForPopInstruction (...): "
        "Unknown POP instruction model\n";
    abort ();
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
      if (INS_OperandCount (ins) > 2 && INS_OperandIsImmediate (ins, 2)) {
        return DST_LARGE_REG_SRC_LARGE_REG_AUX_IMD;
      } else {
        return DST_LARGE_REG_SRC_LARGE_REG;
      }
    } else if (!destRegIsXmm && sourceRegIsXmm) {
      return DST_REG_SRC_LARGE_REG;
    } else if (destRegIsXmm && !sourceRegIsXmm) {
      return DST_LARGE_REG_SRC_REG;
    } else {
      if (INS_OperandCount (ins) > 2) {
        if (INS_OperandIsReg (ins, 2)
            && REG_is_gr_any_size (INS_OperandReg (ins, 2))) {
          return DST_REG_SRC_REG_AUX_REG;
        } else if (INS_OperandIsImmediate (ins, 2)) {
          return DST_REG_SRC_REG_AUX_IMD;
        }
      }
      if (INS_OperandWritten (ins, 1)) {
        return DST_REG_SRC_MUTABLE_REG;
      } else {
        return DST_REG_SRC_REG;
      }
    }

  } else if (destIsReg && sourceIsMem) {
    const bool destRegIsXmm = REG_is_xmm (INS_OperandReg (ins, 0));
    if (destRegIsXmm) {
      if (INS_OperandCount (ins) > 2 && INS_OperandIsImmediate (ins, 2)) {
        return DST_LARGE_REG_SRC_MEM_AUX_IMD;
      } else {
        return DST_LARGE_REG_SRC_MEM;
      }
    } else {
      return DST_REG_SRC_MEM;
    }

  } else if (destIsReg && sourceIsImmed) {
    const bool destRegIsXmm = REG_is_xmm (INS_OperandReg (ins, 0));
    if (destRegIsXmm) {
      return DST_LARGE_REG_SRC_IMD;
    } else {
      return DST_REG_SRC_IMD;
    }

  } else if (destIsMem && sourceIsReg) {
    const bool sourceRegIsXmm = REG_is_xmm (INS_OperandReg (ins, 1));
    if (sourceRegIsXmm) {
      return DST_MEM_SRC_LARGE_REG;
    } else {
      if (INS_OperandCount (ins) > 2) {
        if (INS_OperandIsReg (ins, 2)
            && REG_is_gr_any_size (INS_OperandReg (ins, 2))) {
          return DST_MEM_SRC_REG_AUX_REG;

        } else if (INS_OperandIsImmediate (ins, 2)) {
          return DST_MEM_SRC_REG_AUX_IMD;
        }
      }
      if (INS_OperandWritten (ins, 1)) {
        return DST_MEM_SRC_MUTABLE_REG;
      } else {
        return DST_MEM_SRC_REG;
      }
    }

  } else if (destIsMem && sourceIsImmed) {
    return DST_MEM_SRC_IMD;

  } else if (destIsReg && sourceIsAdg) {
    return DST_REG_SRC_ADG;

  } else if (destIsMem && sourceIsMem) {
    return DST_MEM_SRC_MEM;

  }
  edu::sharif::twinner::util::Logger::error ()
      << "Instrumenter::getInstructionModelForNormalInstruction (...): "
      "Unknown instruction model\n";
  abort ();
}

inline void Instrumenter::checkForInitialState (INS ins) const {
  static bool firstTime = true;
  if (firstTime && !disabled) {
    // initializing registers, right before execution of first instrumented instruction
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineInitializeRegisters,
                    IARG_PTR, ise,
                    IARG_CONTEXT,
                    IARG_END);
  }
  firstTime = false;
}

void Instrumenter::instrumentSingleInstruction (InstructionModel model,
    OPCODE op, INS ins, UINT32 insAssembly) const {
  checkForInitialState (ins);
  INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineRunHooks,
                  IARG_PTR, ise,
                  IARG_CONST_CONTEXT,
                  IARG_END);
  instrumentMemoryRegisterCorrespondence (model, op, ins, insAssembly);
  switch (model) {
  case SYSCALL_INS_MODEL:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineSyscall,
                    IARG_PTR, ise,
                    IARG_SYSCALL_NUMBER,
                    IARG_SYSARG_VALUE, 0,
                    IARG_SYSARG_VALUE, 1,
                    IARG_SYSARG_VALUE, 2,
                    IARG_SYSARG_VALUE, 3,
                    IARG_SYSARG_VALUE, 4,
                    IARG_SYSARG_VALUE, 5,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
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
  case DST_LARGE_REG_SRC_REG:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    REG srcreg = INS_OperandReg (ins, 1);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstLargeRegSrcReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_CONST_REFERENCE, dstreg,
                    IARG_UINT32, srcreg, IARG_REG_VALUE, srcreg,
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
  case DST_LARGE_REG_SRC_LARGE_REG_AUX_IMD:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    REG srcreg = INS_OperandReg (ins, 1);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstLargeRegSrcLargeRegAuxImd,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_CONST_REFERENCE, dstreg,
                    IARG_UINT32, srcreg, IARG_REG_CONST_REFERENCE, srcreg,
                    IARG_ADDRINT, ADDRINT (INS_OperandImmediate (ins, 2)),
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_REG_SRC_REG_AUX_IMD:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    REG srcreg = INS_OperandReg (ins, 1);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstRegSrcRegAuxImd,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                    IARG_UINT32, srcreg, IARG_REG_VALUE, srcreg,
                    IARG_ADDRINT, ADDRINT (INS_OperandImmediate (ins, 2)),
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_LARGE_REG_SRC_MEM:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // src mem
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstLargeRegSrcMem,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_CONST_REFERENCE, dstreg,
                    IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_LARGE_REG_SRC_IMD:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstLargeRegSrcImd,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_CONST_REFERENCE, dstreg,
                    IARG_ADDRINT, ADDRINT (INS_OperandImmediate (ins, 1)),
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_LARGE_REG_SRC_MEM_AUX_IMD:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // src mem
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstLargeRegSrcMemAuxImd,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_CONST_REFERENCE, dstreg,
                    IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_ADDRINT, ADDRINT (INS_OperandImmediate (ins, 2)),
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_REG_SRC_MEM_AUX_IMD:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // src mem
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstRegSrcMemAuxImd,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                    IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_ADDRINT, ADDRINT (INS_OperandImmediate (ins, 2)),
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_REG_SRC_MEM:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // src mem
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstRegSrcMem,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                    IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_REG_SRC_MEM_AUX_RSI:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // src mem
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstRegSrcMemAuxReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                    IARG_MEMORYOP_EA, 0,
#ifdef TARGET_IA32E
        IARG_UINT32, REG_RSI, IARG_REG_VALUE, REG_RSI,
#else
        IARG_UINT32, REG_ESI, IARG_REG_VALUE, REG_ESI,
#endif
        IARG_MEMORYREAD_SIZE,
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
                    IARG_ADDRINT, ADDRINT (INS_OperandImmediate (ins, 1)),
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_MEM_SRC_REG:
  {
    REG srcreg = INS_OperandReg (ins, 1);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // dst mem
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
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // dst mem
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
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // dst mem
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
  case DST_MEM_SRC_REG_AUX_IMD:
  {
    REG srcreg = INS_OperandReg (ins, 1);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // dst mem
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcRegAuxImd,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 0,
                    IARG_UINT32, srcreg, IARG_REG_VALUE, srcreg,
                    IARG_ADDRINT, ADDRINT (INS_OperandImmediate (ins, 2)),
                    IARG_MEMORYREAD_SIZE,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_MEM_SRC_LARGE_REG:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // dst mem
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
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // dst mem
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcImd,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 0,
                    IARG_ADDRINT, ADDRINT (INS_OperandImmediate (ins, 1)),
                    IARG_MEMORYREAD_SIZE,
                    IARG_UINT32, insAssembly,
                    IARG_END);

    break;
  }
  case DST_STK_SRC_REG:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // dst mem
    REG srcreg = INS_OperandReg (ins, 0);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcRegAuxReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 0,
                    IARG_UINT32, srcreg, IARG_REG_VALUE, srcreg,
#ifdef TARGET_IA32E
        IARG_UINT32, REG_RSP, IARG_REG_VALUE, REG_RSP,
#else
        IARG_UINT32, REG_ESP, IARG_REG_VALUE, REG_ESP,
#endif
        IARG_MEMORYREAD_SIZE,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_STK_SRC_IMPLICIT:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // dst mem
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcRegAuxReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 0,
#ifdef TARGET_IA32E
        IARG_UINT32, REG_RFLAGS, IARG_REG_VALUE, REG_RFLAGS,
                    IARG_UINT32, REG_RSP, IARG_REG_VALUE, REG_RSP,
#else
        IARG_UINT32, REG_EFLAGS, IARG_REG_VALUE, REG_EFLAGS,
                    IARG_UINT32, REG_ESP, IARG_REG_VALUE, REG_ESP,
#endif
        IARG_MEMORYREAD_SIZE,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_STK_SRC_IMD:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // dst mem
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcImdAuxReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 0,
                    IARG_ADDRINT, ADDRINT (INS_OperandImmediate (ins, 0)),
#ifdef TARGET_IA32E
        IARG_UINT32, REG_RSP, IARG_REG_VALUE, REG_RSP,
#else
        IARG_UINT32, REG_ESP, IARG_REG_VALUE, REG_ESP,
#endif
        IARG_MEMORYREAD_SIZE,
                    IARG_UINT32, insAssembly,
                    IARG_END);

    break;
  }
  case DST_STK_SRC_MEM:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 1, IARG_MEMORYREAD_SIZE,
                    IARG_END); // dst mem
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // src mem
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcMemAuxReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 1,
                    IARG_MEMORYOP_EA, 0,
#ifdef TARGET_IA32E
        IARG_UINT32, REG_RSP, IARG_REG_VALUE, REG_RSP,
#else
        IARG_UINT32, REG_ESP, IARG_REG_VALUE, REG_ESP,
#endif
        IARG_MEMORYREAD_SIZE,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_REG_SRC_STK:
  {
    REG dstreg = INS_OperandReg (ins, 0);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // src mem
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstRegSrcMemAuxReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                    IARG_MEMORYOP_EA, 0,
#ifdef TARGET_IA32E
        IARG_UINT32, REG_RSP, IARG_REG_VALUE, REG_RSP,
#else
        IARG_UINT32, REG_ESP, IARG_REG_VALUE, REG_ESP,
#endif
        IARG_MEMORYREAD_SIZE,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_MEM_SRC_STK:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // dst mem
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 1, IARG_MEMORYREAD_SIZE,
                    IARG_END); // src mem
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcMemAuxReg,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 0,
                    IARG_MEMORYOP_EA, 1,
#ifdef TARGET_IA32E
        IARG_UINT32, REG_RSP, IARG_REG_VALUE, REG_RSP,
#else
        IARG_UINT32, REG_ESP, IARG_REG_VALUE, REG_ESP,
#endif
        IARG_MEMORYREAD_SIZE,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case DST_MEM_SRC_MEM:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // dst mem
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 1, IARG_MEMORYREAD_SIZE,
                    IARG_END); // src mem
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstMemSrcMem,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_MEMORYOP_EA, 0,
                    IARG_MEMORYOP_EA, 1,
                    IARG_MEMORYREAD_SIZE,
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
  case CMOV_INS_MODELS:
  {
    // instruction type: DST_REG_SRC_EITHER_REG_OR_MEM
    edu::sharif::twinner::util::Logger::debug () << "Instrumenting conditional move "
        "as a conditional jump followed by a normal move\n";
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineConditionalBranch,
                    IARG_PTR, ise, IARG_UINT32, convertConditionalMoveToJumpOpcode (op),
                    IARG_EXECUTING,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    REG dstreg = INS_OperandReg (ins, 0);
    if (INS_OperandIsReg (ins, 1)) {
      REG srcreg = INS_OperandReg (ins, 1);
      INS_InsertPredicatedCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstRegSrcReg,
                                IARG_PTR, ise, IARG_UINT32, XED_ICLASS_MOV,
                                IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                                IARG_UINT32, srcreg, IARG_REG_VALUE, srcreg,
                                IARG_UINT32, insAssembly,
                                IARG_END);
    } else {
      INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                      IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                      IARG_END); // src mem
      INS_InsertPredicatedCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineDstRegSrcMem,
                                IARG_PTR, ise, IARG_UINT32, XED_ICLASS_MOV,
                                IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                                IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                                IARG_UINT32, insAssembly,
                                IARG_END);
    }
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
    if (instrumentSafeFunctions (ins, insAssembly)) {
      break; // ins is handled
    }
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineBeforeChangeOfReg,
                    IARG_PTR, ise, IARG_UINT32, op,
#ifdef TARGET_IA32E
        IARG_UINT32, REG_RSP,
#else
        IARG_UINT32, REG_ESP,
#endif
        IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  case RET_INS_WITH_ARG_MODEL:
  {
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineBeforeChangeOfRegWithArg,
                    IARG_PTR, ise, IARG_UINT32, op,
#ifdef TARGET_IA32E
        IARG_UINT32, REG_RSP,
#else
        IARG_UINT32, REG_ESP,
#endif
        IARG_ADDRINT, ADDRINT (INS_OperandImmediate (ins, 0)),
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
  case DST_REG_REG_SRC_MEM:
  {
    REG dstRightReg = INS_OperandReg (ins, 1);
    REG dstLeftReg = INS_OperandReg (ins, 2);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // src mem
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineTwoDstRegOneSrcMem,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, dstLeftReg, IARG_REG_VALUE, dstLeftReg,
                    IARG_UINT32, dstRightReg, IARG_REG_VALUE, dstRightReg,
                    IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
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
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // dst mem
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
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // src mem
    INS_InsertPredicatedCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineStrOpRegMem,
                              IARG_PTR, ise, IARG_UINT32, op,
                              IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                              IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
#ifdef TARGET_IA32E
        IARG_UINT32, REG_RDI, IARG_REG_VALUE, REG_RDI,
#else
        IARG_UINT32, REG_EDI, IARG_REG_VALUE, REG_EDI,
#endif
        IARG_UINT32, insAssembly,
                              IARG_END);
    instrumentRepPrefix (op, ins, insAssembly);
    break;
  }
  case STRING_OPERATION_MEM_REG:
  {
    const REG srcreg = INS_OperandReg (ins, 2);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // dst mem
    INS_InsertPredicatedCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineStrOpMemReg,
                              IARG_PTR, ise, IARG_UINT32, op,
                              IARG_MEMORYOP_EA, 0,
#ifdef TARGET_IA32E
        IARG_UINT32, REG_RDI, IARG_REG_VALUE, REG_RDI,
#else
        IARG_UINT32, REG_EDI, IARG_REG_VALUE, REG_EDI,
#endif
        IARG_UINT32, srcreg, IARG_REG_VALUE, srcreg,
                              IARG_MEMORYREAD_SIZE,
                              IARG_UINT32, insAssembly,
                              IARG_END);
    instrumentRepPrefix (op, ins, insAssembly);
    break;
  }
  case STRING_OPERATION_MEM_MEM:
  {
#ifdef TARGET_IA32E
    const REG dstreg = REG_RDI;
    const REG srcreg = REG_RSI;
#else
    const REG dstreg = REG_EDI;
    const REG srcreg = REG_ESI;
#endif
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // dst mem
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 1, IARG_MEMORYREAD_SIZE,
                    IARG_END); // src mem
    INS_InsertPredicatedCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineStrOpMemMem,
                              IARG_PTR, ise, IARG_UINT32, op,
                              IARG_UINT32, dstreg, IARG_REG_VALUE, dstreg,
                              IARG_MEMORYOP_EA, 0,
                              IARG_UINT32, srcreg, IARG_REG_VALUE, srcreg,
                              IARG_MEMORYOP_EA, 1,
                              IARG_MEMORYREAD_SIZE,
                              IARG_UINT32, insAssembly,
                              IARG_END);
    instrumentRepPrefix (op, ins, insAssembly);
    break;
  }
  case LEAVE_INS_MODELS:
  {
    const REG fpreg = INS_OperandReg (ins, 2);
    const REG spreg = INS_OperandReg (ins, 3);
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutinePrefetchMem,
                    IARG_PTR, ise, IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_END); // src mem
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineTwoDstRegOneSrcMem,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, fpreg, IARG_REG_VALUE, fpreg,
                    IARG_UINT32, spreg, IARG_REG_VALUE, spreg,
                    IARG_MEMORYOP_EA, 0, IARG_MEMORYREAD_SIZE,
                    IARG_UINT32, insAssembly,
                    IARG_END);
    break;
  }
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Instrumenter::instrumentSingleInstruction (model="
        << std::dec << model << ", ...): Unknown instruction model\n";
    abort ();
  }
}

void Instrumenter::instrumentMemoryRegisterCorrespondence (
    InstructionModel model, OPCODE op, INS ins, UINT32 insAssembly) const {
  const UINT32 countOfOperands = INS_OperandCount (ins);
  for (UINT32 i = 0; i < countOfOperands; ++i) {
    if (INS_OperandIsMemory (ins, i)) {
      REG baseReg = INS_OperandMemoryBaseReg (ins, i);
      if (baseReg != REG_INVALID ()) {
        ADDRDELTA displacement = INS_OperandMemoryDisplacement (ins, i);
        if (model == DST_RSP_SRC_CALL
            || model == DST_STK_SRC_IMPLICIT
            || model == DST_STK_SRC_REG
            || model == DST_STK_SRC_IMD
            || model == DST_STK_SRC_MEM) {
          if (op != XED_ICLASS_RET_NEAR
              && op != XED_ICLASS_RET_FAR) {
            displacement = -STACK_OPERATION_UNIT_SIZE;
          }
        }
        REG indexReg = INS_OperandMemoryIndexReg (ins, i);
        if (indexReg != REG_INVALID ()) {
          UINT32 scale = INS_OperandMemoryScale (ins, i);
          // EA = displacement + baseReg + indexReg*scale
          INS_InsertCall (ins, IPOINT_BEFORE,
                          (AFUNPTR) analysisRoutineMemoryIndexedRegisterCorrespondence,
                          IARG_PTR, ise,
                          IARG_UINT32, baseReg, IARG_REG_VALUE, baseReg,
                          IARG_ADDRINT, ADDRINT (displacement),
                          IARG_UINT32, indexReg, IARG_REG_VALUE, indexReg,
                          IARG_ADDRINT, ADDRINT (scale),
                          IARG_MEMORYOP_EA, 0,
                          IARG_UINT32, insAssembly,
                          IARG_END);
        } else {
          // EA = displacement + baseReg
          INS_InsertCall (ins, IPOINT_BEFORE,
                          (AFUNPTR) analysisRoutineMemoryRegisterCorrespondence,
                          IARG_PTR, ise,
                          IARG_UINT32, baseReg, IARG_REG_VALUE, baseReg,
                          IARG_ADDRINT, ADDRINT (displacement),
                          IARG_MEMORYOP_EA, 0,
                          IARG_UINT32, insAssembly,
                          IARG_END);
        }
      }
      // TODO: Handle instructions with two memory operands
      break;
    }
  }
}

void Instrumenter::instrumentRepPrefix (OPCODE op, INS ins, UINT32 insAssembly) const {
  const BOOL repe = INS_RepPrefix (ins);
  const BOOL repne = INS_RepnePrefix (ins);
  if (repe || repne) {
    REG repreg = INS_RepCountRegister (ins);
    if (repreg == REG_INVALID ()) {
      edu::sharif::twinner::util::Logger::error ()
          << "Instrumenter::instrumentRepPrefix (...): "
          "INS_Rep(ne)Prefix conflicts INS_RepCountRegister\n";
      abort ();
    }
    INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) analysisRoutineRepPrefix,
                    IARG_PTR, ise, IARG_UINT32, op,
                    IARG_UINT32, repreg, IARG_REG_VALUE, repreg,
                    IARG_EXECUTING,
                    IARG_UINT32, repe,
                    IARG_UINT32, insAssembly,
                    IARG_END);
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
  for (UINT32 i = 0; i < countOfOperands; ++i) {
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
      debug << "immediate (" << ADDRINT (INS_OperandImmediate (ins, i)) << ')';
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
  ise->syscallInvoked (ctxt, edu::sharif::twinner::trace::syscall::Syscall (std));
  if (isWithinInitialStateDetectionMode && !disabled) {
    edu::sharif::twinner::util::Logger::debug ()
        << "Gathering initial contents of requested memory addresses,"
        " right before first syscall\n";
    saveMemoryContentsToFile (traceFilePath.c_str ());
    edu::sharif::twinner::util::Logger::debug () << "Done.\tExiting...\n";
    PIN_ExitProcess (0); // think about probably acquired locks of application
  }
}

void Instrumenter::syscallExitPoint (THREADID threadIndex, CONTEXT *ctxt,
    SYSCALL_STANDARD std) {
  UNUSED_VARIABLE (threadIndex);
  UNUSED_VARIABLE (std);
  edu::sharif::twinner::util::Logger::loquacious () << "*** syscallExitPoint ***\n";
  if (disabled) {
    return;
  }
  ise->startNewTraceSegment (ctxt);
}

void Instrumenter::saveMemoryContentsToFile (const char *path) const {
  std::map < std::pair < ADDRINT, int >, UINT64 > map;
  edu::sharif::twinner::util::foreach (candidateAddresses,
                                       &read_memory_content_and_add_it_to_map, map);
  if (!ise->getTrace ()->saveAddressToValueMapToFile (map, path)) {
    edu::sharif::twinner::util::Logger::error ()
        << "Instrumenter::saveMemoryContentsToFile (path=" << path << "): "
        "Can not save address-to-value map into binary file\n";
    abort ();
  }
}

void read_memory_content_and_add_it_to_map (
    std::map < std::pair < ADDRINT, int >, UINT64 > &map,
    const std::pair < ADDRINT, int > &address) {
  const ADDRINT symbolAddress = address.first;
  const int symbolSize = address.second;
  UINT64 value;
  if (!edu::sharif::twinner::util::readMemoryContent
      (value, symbolAddress, symbolSize / 8)) {
    edu::sharif::twinner::util::Logger::warning ()
        << "read_memory_content_and_add_it_to_map (...) function:"
        " error reading memory value\n";
    return;
  }
  map.insert (make_pair (make_pair (symbolAddress, symbolSize), value));
}

void Instrumenter::aboutToExit (INT32 code) {
  printInstructionsStatisticsInfo ();
  saveAll ();
  edu::sharif::twinner::util::LogStream::destroy ();
}

void Instrumenter::saveAll () {
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::debug ();
  logger << "Saving trace info...";
  if (ise->getTrace ()->saveToFile
      (traceFilePath.c_str (), disassemblyFilePath.c_str ())) {
    logger << "Done.\n";
    if (marFilePath.empty ()) {
      return;
    }
    logger << "Saving MarInfo...";
    std::ofstream out;
    out.open (marFilePath.c_str (),
              ios_base::out | ios_base::trunc | ios_base::binary);
    if (!out.is_open ()) {
      edu::sharif::twinner::util::Logger::error ()
          << "Instrumenter::aboutToExit (path=" << marFilePath << "): "
          << "Can not report main() args due to error in open function\n";
      abort ();
    }
    out << marCache;
    out.close ();
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
  if (isWithinInitialStateDetectionMode) {
    edu::sharif::twinner::util::Logger::debug ()
        << "Instrumenter::enable (): "
        "Gathering initial contents of requested memory addresses,"
        " right before first syscall\n";
    saveMemoryContentsToFile (traceFilePath.c_str ());
    edu::sharif::twinner::util::Logger::debug () << "Done.\tExiting...\n";
    PIN_ExitProcess (0); // think about probably acquired locks of application
  }
}

void Instrumenter::beforeSafeFunction (ADDRINT retAddress,
    const edu::sharif::twinner::trace::FunctionInfo &fi, UINT32 insAssembly,
    const CONTEXT *context) {
  edu::sharif::twinner::util::Logger::loquacious ()
      << "Instrumenter::beforeSafeFunction ()\n";
  ise->analysisRoutineBeforeCallingSafeFunction
      (retAddress, fi, insAssembly, context);
  disable ();
  withinSafeFunc = true;
}

void Instrumenter::afterSafeFunction (const CONTEXT *context) {
  if (!withinSafeFunc) {
    return;
  }
  CONTEXT mutableContext;
  PIN_SaveContext (context, &mutableContext);
  afterSafeFunction (&mutableContext);
}

void Instrumenter::afterSafeFunction (CONTEXT *context) {
  if (!withinSafeFunc) {
    return;
  }
  edu::sharif::twinner::util::Logger::loquacious ()
      << "Instrumenter::afterSafeFunction ()\n";
  enable ();
  ise->startNewTraceSegment (context);
  withinSafeFunc = false;
}

void Instrumenter::reportMainArguments (int argc, char **argv) {
  static bool calledOnce = false;
  if (disabled || calledOnce) {
    return;
  }
  calledOnce = true;
  std::stringstream ss;
  edu::sharif::twinner::trace::MarInfo (argc, argv).saveToOutputStream (ss);
  marCache = ss.str ();
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

OPCODE Instrumenter::convertConditionalMoveToJumpOpcode (OPCODE cmovcc) const {
  if (cmovcc < XED_ICLASS_CMOVNB) {
    return cmovcc + XED_ICLASS_JB - XED_ICLASS_CMOVB;
  } else if (cmovcc < XED_ICLASS_CMOVS) {
    return cmovcc + XED_ICLASS_JNB - XED_ICLASS_CMOVNB;
  } else {
    return cmovcc + XED_ICLASS_JS - XED_ICLASS_CMOVS;
  }
}

void Instrumenter::instrumentImage (IMG img) {
  edu::sharif::twinner::util::Logger log =
      edu::sharif::twinner::util::Logger::debug ();
  log << "Instrumenter::instrumentImage (img)\n";
  int state = 0;
  const bool shouldSaveMainArgs = !marFilePath.empty ();
  if (start != end) {
    for (SEC section = IMG_SecHead (img);
        SEC_Valid (section); section = SEC_Next (section)) {
      for (RTN routine = SEC_RtnHead (section);
          RTN_Valid (routine); routine = RTN_Next (routine)) {
        RTN_Open (routine);
        for (INS ins = RTN_InsHead (routine);
            INS_Valid (ins); ins = INS_Next (ins)) {
          const ADDRINT addr = INS_Address (ins);
          if (addr == start) {
            INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) startAnalysis,
                            IARG_PTR, this,
                            IARG_END);
            if (shouldSaveMainArgs) {
              INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) reportMainArgs,
                              IARG_PTR, this,
                              IARG_FUNCARG_ENTRYPOINT_REFERENCE, 0 + stackOffset,
                              IARG_FUNCARG_ENTRYPOINT_REFERENCE, 1 + stackOffset,
                              IARG_END);
            }
            INS_InsertCall (ins, IPOINT_BEFORE,
                            (AFUNPTR) analysisRoutineInitializeRegisters,
                            IARG_PTR, ise,
                            IARG_CONTEXT,
                            IARG_END);
            ++state;
          } else if (addr == end) {
            INS_InsertCall (ins, IPOINT_BEFORE, (AFUNPTR) terminateAnalysis,
                            IARG_PTR, this,
                            IARG_END);
            ++state;
          }
          if (state == 2) {
            RTN_Close (routine);
            return;
          }
        }
        RTN_Close (routine);
      }
    }
  } else {
    RTN mainRoutine = RTN_FindByName (img, MAIN_ROUTINE_NAME);
    if (RTN_Valid (mainRoutine)) {
      log << " routine: " MAIN_ROUTINE_NAME "\n";
      RTN_Open (mainRoutine);
      /*
       * All instructions before main() routine are owned by RTLD.
       * So we should start instrumenting instructions thereafter.
       * Also instructions after returning from main() are owned by RTLD and
       * so instrumenter and analysis routines should be disabled afterwards.
       */
      RTN_InsertCall (mainRoutine, IPOINT_BEFORE, (AFUNPTR) startAnalysis,
                      IARG_PTR, this,
                      IARG_END);
      if (shouldSaveMainArgs) {
        RTN_InsertCall (mainRoutine, IPOINT_BEFORE, (AFUNPTR) reportMainArgs,
                        IARG_PTR, this,
                        IARG_FUNCARG_ENTRYPOINT_REFERENCE, 0 + stackOffset,
                        IARG_FUNCARG_ENTRYPOINT_REFERENCE, 1 + stackOffset,
                        IARG_END);
      }
      RTN_InsertCall (mainRoutine, IPOINT_BEFORE,
                      (AFUNPTR) analysisRoutineInitializeRegisters,
                      IARG_PTR, ise,
                      IARG_CONTEXT,
                      IARG_END);
      RTN_InsertCall (mainRoutine, IPOINT_AFTER, (AFUNPTR) terminateAnalysis,
                      IARG_PTR, this,
                      IARG_END);
      RTN_Close (mainRoutine);
    }
  }
}

VOID instrumentSingleInst (INS ins, VOID * v) {
  Instrumenter *im = (Instrumenter *) v;
  if (!im->instrumentSingleInstruction (ins)) {
    abort ();
  }
}

VOID imageIsLoaded (IMG img, VOID *v) {
  Instrumenter *im = (Instrumenter *) v;
  im->instrumentImage (img);
}

VOID instrumentSafeFuncs (IMG img, VOID *v) {
  Instrumenter *im = (Instrumenter *) v;
  im->instrumentSafeFunctions (img);
}

VOID beforeSafeFunc (VOID *v, ADDRINT retAddress, VOID *p, UINT32 insAssembly,
    const CONTEXT *context) {
  Instrumenter *im = (Instrumenter *) v;
  const edu::sharif::twinner::trace::FunctionInfo *fi =
      (const edu::sharif::twinner::trace::FunctionInfo *) p;
  im->beforeSafeFunction (retAddress, *fi, insAssembly, context);
}

VOID afterSafeFunc (VOID *v, CONTEXT *context) {
  Instrumenter *im = (Instrumenter *) v;
  im->afterSafeFunction (context);
}

VOID startAnalysis (VOID *v) {
  edu::sharif::twinner::util::Logger::loquacious ()
      << "********** startAnalysis(...) **********\n";
  Instrumenter *im = (Instrumenter *) v;
  im->enable ();
}

VOID reportMainArgs (VOID *v, ADDRINT *arg0, ADDRINT *arg1) {
  edu::sharif::twinner::util::Logger::loquacious ()
      << "reportMainArgs (...) function is called\n";
  Instrumenter *im = (Instrumenter *) v;
  int argc = *reinterpret_cast<int *> (arg0);
  char **argv = *reinterpret_cast<char ***> (arg1);
  im->reportMainArguments (argc, argv);
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
  static bool called = false;
  if (called) {
    // Logger is not available here
    std::cerr << "applicationIsAboutToExit (0x" << std::hex << code
        << ", 0x" << ADDRINT (v) << ") is called more than once!"
        " Ignoring the duplicate call\n";
    return;
  }
  called = true;
  edu::sharif::twinner::util::Logger::loquacious ()
      << "********** applicationIsAboutToExit(...) **********\n";
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
