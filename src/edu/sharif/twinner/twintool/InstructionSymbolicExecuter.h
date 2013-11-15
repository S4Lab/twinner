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

#ifndef INSTRUCTION_SYMBOLIC_EXECUTER_H
#define INSTRUCTION_SYMBOLIC_EXECUTER_H

#include "pin.H"

#include "Flags.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Trace;
}
namespace twintool {

class ExpressionValueProxy;
class MutableExpressionValueProxy;

class InstructionSymbolicExecuter {

private:
  edu::sharif::twinner::trace::Trace *trace;
  Flags eflags;

public:
  InstructionSymbolicExecuter ();

  edu::sharif::twinner::trace::Trace *getTrace () const;

private:

  typedef void (InstructionSymbolicExecuter::*AnalysisRoutine) (
      const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src);
  typedef void (InstructionSymbolicExecuter::*ConditionalBranchAnalysisRoutine) (
      bool branchTaken);

public:
  void analysisRoutineDstRegSrcReg (AnalysisRoutine routine,
      REG dstReg, UINT64 dstRegVal,
      REG srcReg, UINT64 srcRegVal,
      std::string *insAssembly);
  void analysisRoutineDstRegSrcMem (AnalysisRoutine routine,
      REG dstReg, UINT64 dstRegVal,
      ADDRINT srcMemoryEa, UINT32 memReadBytes,
      std::string *insAssembly);
  void analysisRoutineDstRegSrcImd (AnalysisRoutine routine,
      REG dstReg, UINT64 dstRegVal,
      ADDRINT srcImmediateValue,
      std::string *insAssembly);
  void analysisRoutineDstMemSrcReg (AnalysisRoutine routine,
      ADDRINT dstMemoryEa,
      REG srcReg, UINT64 srcRegVal,
      std::string *insAssembly);
  void analysisRoutineDstMemSrcImd (AnalysisRoutine routine,
      ADDRINT dstMemoryEa,
      ADDRINT srcImmediateValue,
      std::string *insAssembly);
  void analysisRoutineDstMemSrcMem (AnalysisRoutine routine,
      ADDRINT dstMemoryEa,
      ADDRINT srcMemoryEa, UINT32 memReadBytes,
      std::string *insAssembly);
  void analysisRoutineConditionalBranch (ConditionalBranchAnalysisRoutine routine,
      BOOL branchTaken,
      std::string *insAssembly);

private:

  /**
   * MOV has 5 models
   * r <- r/m/i
   * m <- r/i
   */
  void movAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * PUSH has 3 models
   * m <- r/m/i
   */
  void pushAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * POP has 2 models
   * r/m <- m
   */
  void popAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * ADD has 5 models
   * r += r/m/i
   * m += r/i
   */
  void addAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * SUB has 5 models
   * r -= r/m/i
   * m -= r/i
   */
  void subAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * CMP is same as SUB else of not modifying dst operand's value
   */
  void cmpAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * JNZ jumps if ZF=0 which means that corresponding expression was not zero
   */
  void jnzAnalysisRoutine (bool branchTaken);

public:
  AnalysisRoutine convertOpcodeToAnalysisRoutine (OPCODE op) const;
  ConditionalBranchAnalysisRoutine convertOpcodeToConditionalBranchAnalysisRoutine (
      OPCODE op) const;

  static UINT64 readMemoryContent (ADDRINT memoryEa);
};

VOID analysisRoutineDstRegSrcReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    VOID *insAssembly);
VOID analysisRoutineDstRegSrcMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    VOID *insAssembly);
VOID analysisRoutineDstRegSrcImd (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcImmediateValue,
    VOID *insAssembly);
VOID analysisRoutineDstMemSrcReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    UINT32 srcReg, ADDRINT srcRegVal,
    VOID *insAssembly);
VOID analysisRoutineDstMemSrcImd (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcImmediateValue,
    VOID *insAssembly);
VOID analysisRoutineDstMemSrcMem (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    VOID *insAssembly);
VOID analysisRoutineConditionalBranch (VOID *iseptr, UINT32 opcode,
    BOOL branchTaken,
    VOID *insAssembly);

}
}
}
}

#endif /* InstructionSymbolicExecuter.h */
