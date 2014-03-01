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

#ifndef INSTRUCTION_SYMBOLIC_EXECUTER_H
#define INSTRUCTION_SYMBOLIC_EXECUTER_H

#include "pin.H"

#include "Flags.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Trace;
class Syscall;
}
namespace twintool {

class ExpressionValueProxy;
class MutableExpressionValueProxy;

class InstructionSymbolicExecuter {

private:

  typedef void (InstructionSymbolicExecuter::*AnalysisRoutine) (
      const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src);
  typedef void (InstructionSymbolicExecuter::*DoubleDestinationsAnalysisRoutine) (
      const MutableExpressionValueProxy &leftDst,
      const MutableExpressionValueProxy &rightDst,
      const ExpressionValueProxy &src);
  typedef void (InstructionSymbolicExecuter::*ConditionalBranchAnalysisRoutine) (
      bool branchTaken);
  typedef void (InstructionSymbolicExecuter::*Hook) (const CONTEXT *context,
      UINT64 value);
  typedef Hook SuddenlyChangedRegAnalysisRoutine;
  typedef void (InstructionSymbolicExecuter::*OperandLessAnalysisRoutine) (
      const CONTEXT *context);

  edu::sharif::twinner::trace::Trace *trace;
  Flags eflags;

  REG trackedReg;
  int operandSize;
  Hook hook;

  bool disabled;

public:
  InstructionSymbolicExecuter (std::ifstream &symbolsFileInputStream, bool disabled);
  InstructionSymbolicExecuter (bool disabled);

  edu::sharif::twinner::trace::Trace *getTrace () const;

  void disable ();
  void enable ();

  void syscallInvoked (const CONTEXT *context, edu::sharif::twinner::trace::Syscall s);

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
      UINT32 memReadBytes,
      std::string *insAssembly);
  void analysisRoutineDstMemSrcImd (AnalysisRoutine routine,
      ADDRINT dstMemoryEa,
      ADDRINT srcImmediateValue,
      UINT32 memReadBytes,
      std::string *insAssembly);
  void analysisRoutineDstMemSrcMem (AnalysisRoutine routine,
      ADDRINT dstMemoryEa,
      ADDRINT srcMemoryEa, UINT32 memReadBytes,
      std::string *insAssembly);
  void analysisRoutineConditionalBranch (ConditionalBranchAnalysisRoutine routine,
      BOOL branchTaken,
      std::string *insAssembly);
  void analysisRoutineDstRegSrcAdg (AnalysisRoutine routine,
      REG dstReg, UINT64 dstRegVal,
      std::string *insAssembly);
  void analysisRoutineBeforeChangeOfReg (SuddenlyChangedRegAnalysisRoutine routine,
      REG reg,
      std::string *insAssembly);
  void analysisRoutineTwoDstRegOneSrcReg (DoubleDestinationsAnalysisRoutine routine,
      REG dstLeftReg, UINT64 dstLeftRegVal,
      REG dstRightReg, UINT64 dstRightRegVal,
      REG srcReg, UINT64 srcRegVal,
      std::string *insAssembly);
  void analysisRoutineAfterOperandLessInstruction (OperandLessAnalysisRoutine routine,
      const CONTEXT *context,
      std::string *insAssembly);
  void analysisRoutineRunHooks (const CONTEXT *context);

private:

  /**
   * Run hooks from last instruction (if any) and reset them afterwards.
   */
  void runHooks (const CONTEXT *context);

  /**
   * MOV has 5 models
   * r <- r/m/i
   * m <- r/i
   */
  void movAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * MOV with Sign extension
   * r <- sign-extend (r/m)
   */
  void movsxAnalysisRoutine (const MutableExpressionValueProxy &dst,
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
   * LEA loads an address into a register. This analysis routine is called after execution
   * of the instruction. The dst parameter should be set, without getting its value, since
   * value of register has been changed by the instruction and we must synch its symbolic
   * value (as a constant value) now.
   */
  void leaAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * JNZ jumps if ZF=0 which means that corresponding expression was not zero
   */
  void jnzAnalysisRoutine (bool branchTaken);

  /**
   * JZ jumps if ZF=1 which means that corresponding expression was zero
   */
  void jzAnalysisRoutine (bool branchTaken);

  /**
   * CALL instruction is executed and RSP is changed. This method will synchronize its
   * symbolic value with its concrete value.
   */
  void callAnalysisRoutine (const CONTEXT *context, UINT64 rspRegVal);

  /**
   * RET instruction is executed and RSP is changed. This method will synchronize its
   * symbolic value with its concrete value.
   */
  void retAnalysisRoutine (const CONTEXT *context, UINT64 rspRegVal);

  /**
   * SHL shifts dst to left as much as indicated by src.
   */
  void shlAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * SHR shifts dst to right as much as indicated by src.
   */
  void shrAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * AND bitwise ands dst with src as its mask.
   */
  void andAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * OR bitwise ores dst with src as its complement.
   */
  void orAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * XOR calculates exclusive or of dst with src.
   */
  void xorAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * TEST performs AND between arguments, temporarily, and sets ZF, SF, and PF based
   * on result. Also CF and OF are set to zero. AF is undefined.
   */
  void testAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * DIV unsigned divide left-right regs by src reg putting quotient in right, remainder
   * in left. This method only calculates symbolic values of operands (concrete values
   * will be wrong) and also ignores propagating new values to overlapping registers.
   * Instead, it registers a hook to adjust concrete values and propagate to overlapping
   * registers at the beginning of next executed instruction.
   */
  void divAnalysisRoutine (const MutableExpressionValueProxy &leftDst,
      const MutableExpressionValueProxy &rightDst,
      const ExpressionValueProxy &src);

  /**
   * MUL unsigned multiply right reg by src and puts result in left-right regs.
   * This method only calculates symbolic values of operands (concrete values
   * will be wrong) and also ignores propagating new values to overlapping registers.
   * Instead, it registers a hook to adjust concrete values and propagate to overlapping
   * registers at the beginning of next executed instruction.
   */
  void mulAnalysisRoutine (const MutableExpressionValueProxy &leftDst,
      const MutableExpressionValueProxy &rightDst,
      const ExpressionValueProxy &src);

  /**
   * This hook adjusts concrete values of division/multiplication operands
   * and also propagates their values to overlapping registers.
   */
  void adjustDivisionMultiplicationOperands (const CONTEXT *context, UINT64 operandSize);

  /**
   * read time-stamp counter and put it in EDX:EAX
   */
  void rdtscAnalysisRoutine (const CONTEXT *context);

public:
  AnalysisRoutine convertOpcodeToAnalysisRoutine (OPCODE op) const;
  DoubleDestinationsAnalysisRoutine convertOpcodeToDoubleDestinationsAnalysisRoutine (
      OPCODE op) const;
  ConditionalBranchAnalysisRoutine convertOpcodeToConditionalBranchAnalysisRoutine (
      OPCODE op) const;
  SuddenlyChangedRegAnalysisRoutine convertOpcodeToSuddenlyChangedRegAnalysisRoutine (
      OPCODE op) const;
  OperandLessAnalysisRoutine convertOpcodeToOperandLessAnalysisRoutine (
      OPCODE op) const;
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
    UINT32 memReadBytes,
    VOID *insAssembly);
VOID analysisRoutineDstMemSrcImd (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcImmediateValue,
    UINT32 memReadBytes,
    VOID *insAssembly);
VOID analysisRoutineDstMemSrcMem (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    VOID *insAssembly);
VOID analysisRoutineConditionalBranch (VOID *iseptr, UINT32 opcode,
    BOOL branchTaken,
    VOID *insAssembly);
VOID analysisRoutineDstRegSrcAdg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    VOID *insAssembly);
VOID analysisRoutineBeforeChangeOfReg (VOID *iseptr, UINT32 opcode,
    UINT32 reg,
    VOID *insAssembly);
VOID analysisRoutineTwoDstRegOneSrcReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstLeftReg, ADDRINT dstLeftRegVal,
    UINT32 dstRightReg, ADDRINT dstRightRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    VOID *insAssembly);
VOID analysisRoutineAfterOperandLess (VOID *iseptr, UINT32 opcode,
    const CONTEXT *context,
    VOID *insAssembly);
VOID analysisRoutineRunHooks (VOID *iseptr, const CONTEXT *context);

}
}
}
}

#endif /* InstructionSymbolicExecuter.h */
