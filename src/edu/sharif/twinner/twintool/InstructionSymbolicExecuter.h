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
class ConcreteValue;
}
namespace twintool {

class ExpressionValueProxy;
class MutableExpressionValueProxy;

class InstructionSymbolicExecuter {

private:
  typedef edu::sharif::twinner::trace::ConcreteValue ConcreteValue;

  typedef void (InstructionSymbolicExecuter::*AnalysisRoutine) (
      const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src);
  typedef void (InstructionSymbolicExecuter::*DoubleDestinationsAnalysisRoutine) (
      const MutableExpressionValueProxy &leftDst,
      const MutableExpressionValueProxy &rightDst,
      const ExpressionValueProxy &src);
  typedef void (InstructionSymbolicExecuter::*ConditionalBranchAnalysisRoutine) (
      bool branchTaken);
  typedef void (InstructionSymbolicExecuter::*Hook) (const CONTEXT *context,
      const ConcreteValue &value);
  typedef Hook SuddenlyChangedRegAnalysisRoutine;
  typedef void (InstructionSymbolicExecuter::*OperandLessAnalysisRoutine) (
      const CONTEXT *context);
  typedef void (InstructionSymbolicExecuter::*SingleOperandAnalysisRoutine) (
      const MutableExpressionValueProxy &opr);

  edu::sharif::twinner::trace::Trace *trace;
  Flags eflags;

  REG trackedReg;
  int operandSize;
  Hook hook;

  UINT32 disassembledInstruction;

  bool disabled;

public:
  InstructionSymbolicExecuter (std::ifstream &symbolsFileInputStream, bool disabled);
  InstructionSymbolicExecuter (bool disabled);

  edu::sharif::twinner::trace::Trace *getTrace () const;

  void disable ();
  void enable ();

  void syscallInvoked (const CONTEXT *context, edu::sharif::twinner::trace::Syscall s);
  void syscallReturned (CONTEXT *context) const;

public:
  void analysisRoutineDstRegSrcReg (AnalysisRoutine routine,
      REG dstReg, const ConcreteValue &dstRegVal,
      REG srcReg, const ConcreteValue &srcRegVal,
      UINT32 insAssembly);
  void analysisRoutineDstRegSrcMem (AnalysisRoutine routine,
      REG dstReg, const ConcreteValue &dstRegVal,
      ADDRINT srcMemoryEa, UINT32 memReadBytes,
      UINT32 insAssembly);
  void analysisRoutineDstRegSrcImd (AnalysisRoutine routine,
      REG dstReg, const ConcreteValue &dstRegVal,
      const ConcreteValue &srcImmediateValue,
      UINT32 insAssembly);
  void analysisRoutineDstMemSrcReg (AnalysisRoutine routine,
      ADDRINT dstMemoryEa,
      REG srcReg, const ConcreteValue &srcRegVal,
      UINT32 memReadBytes,
      UINT32 insAssembly);
  void analysisRoutineDstMemSrcImd (AnalysisRoutine routine,
      ADDRINT dstMemoryEa,
      const ConcreteValue &srcImmediateValue,
      UINT32 memReadBytes,
      UINT32 insAssembly);
  void analysisRoutineDstMemSrcMem (AnalysisRoutine routine,
      ADDRINT dstMemoryEa,
      ADDRINT srcMemoryEa, UINT32 memReadBytes,
      UINT32 insAssembly);
  void analysisRoutineConditionalBranch (ConditionalBranchAnalysisRoutine routine,
      BOOL branchTaken,
      UINT32 insAssembly);
  void analysisRoutineDstRegSrcAdg (AnalysisRoutine routine,
      REG dstReg, const ConcreteValue &dstRegVal,
      UINT32 insAssembly);
  void analysisRoutineBeforeChangeOfReg (SuddenlyChangedRegAnalysisRoutine routine,
      REG reg,
      UINT32 insAssembly);
  void analysisRoutineTwoDstRegOneSrcReg (DoubleDestinationsAnalysisRoutine routine,
      REG dstLeftReg, const ConcreteValue &dstLeftRegVal,
      REG dstRightReg, const ConcreteValue &dstRightRegVal,
      REG srcReg, const ConcreteValue &srcRegVal,
      UINT32 insAssembly);
  void analysisRoutineAfterOperandLessInstruction (OperandLessAnalysisRoutine routine,
      const CONTEXT *context,
      UINT32 insAssembly);
  void analysisRoutineDstRegSrcImplicit (SingleOperandAnalysisRoutine routine,
      REG dstReg, const ConcreteValue &dstRegVal,
      UINT32 insAssembly);
  void analysisRoutineDstMemSrcImplicit (SingleOperandAnalysisRoutine routine,
      ADDRINT srcMemoryEa, UINT32 memReadBytes,
      UINT32 insAssembly);
  void analysisRoutineRunHooks (const CONTEXT *context);
  /// This call does not return to caller
  void analysisRoutineInitializeRegisters (CONTEXT *context) const;

private:

  /**
   * Run hooks from last instruction (if any) and reset them afterwards.
   */
  void runHooks (const CONTEXT *context);

  /**
   * CMOVBE (Conditional Move) moves src to dst iff (CF=1 || ZF=1).
   */
  void cmovbeAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * CMOVNBE (Conditional Move) moves src to dst iff (CF=0 && ZF=0).
   */
  void cmovnbeAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

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
   * JLE jumps if ZF=1 or SF!=OF which means that corresponding expression was <= 0
   */
  void jleAnalysisRoutine (bool branchTaken);

  /**
   * JBE jumps if ZF=1 or CF=1 which means that corresponding expression was <= 0
   */
  void jbeAnalysisRoutine (bool branchTaken);

  /**
   * JNBE jumps if ZF=0 and CF=0 which means that corresponding expression was > 0
   */
  void jnbeAnalysisRoutine (bool branchTaken);

  /**
   * JS jumps if SF=1 which means that corresponding expression was < 0
   */
  void jsAnalysisRoutine (bool branchTaken);

  /**
   * CALL instruction is executed and RSP is changed. This method will synchronize its
   * symbolic value with its concrete value.
   */
  void callAnalysisRoutine (const CONTEXT *context, const ConcreteValue &rspRegVal);

  /**
   * RET instruction is executed and RSP is changed. This method will synchronize its
   * symbolic value with its concrete value.
   */
  void retAnalysisRoutine (const CONTEXT *context, const ConcreteValue &rspRegVal);

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
   * SAR arithmetic shifts dst to right as much as indicated by src (signed division).
   */
  void sarAnalysisRoutine (const MutableExpressionValueProxy &dst,
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
   * PMOVMSKB is a packed-move instruction which moves mask-byte of src reg to dat reg.
   * Mask-byte: read MSB of each byte of a reg and put those bits together. A 128-bits reg
   * has 16 bytes and its mask-byte has 16-bits or 2 bytes. Remaining bits in left-side of
   * the dst reg will be filled with zero.
   * TODO: Currently only 128-bit XMM registers are supported which should be expanded with proxy objects
   */
  void pmovmskbAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * PCMPEQB is a packed compare equality check which works byte-wise. The src and dst
   * are compared together byte-by-byte and those byte which are/aren't equal will be
   * filed with 1 (0xFF) / 0 (0x00) in the dst reg.
   */
  void pcmpeqbAnalysisRoutine (
      const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src);

  /**
   * BSF is bit scan forward instruction which searches for the least significant 1 bit
   * in the src and sets its index in the dst. The index is placed as a constant in dst
   * and a constraint is added to indicate that the noted bit was set.
   */
  void bsfAnalysisRoutine (const MutableExpressionValueProxy &dst,
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
  void adjustDivisionMultiplicationOperands (const CONTEXT *context,
      const ConcreteValue &operandSize);

  /**
   * read time-stamp counter and put it in EDX:EAX
   */
  void rdtscAnalysisRoutine (const CONTEXT *context);

  /**
   * INC increments the opr reg/mem operand.
   */
  void incAnalysisRoutine (const MutableExpressionValueProxy &opr);

  /**
   * SETNZ sets opr to 1 iff ZF=0 (and set it to 0 otherwise).
   */
  void setnzAnalysisRoutine (const MutableExpressionValueProxy &opr);

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
  SingleOperandAnalysisRoutine convertOpcodeToSingleOperandAnalysisRoutine (
      OPCODE op) const;
};

VOID analysisRoutineDstRegSrcReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 insAssembly);
VOID analysisRoutineDstRegSrcLargeReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, const PIN_REGISTER *srcRegVal,
    UINT32 insAssembly);
VOID analysisRoutineDstLargeRegSrcLargeReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, const PIN_REGISTER *dstRegVal,
    UINT32 srcReg, const PIN_REGISTER *srcRegVal,
    UINT32 insAssembly);
VOID analysisRoutineDstLargeRegSrcMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, const PIN_REGISTER *dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly);
VOID analysisRoutineDstRegSrcMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly);
VOID analysisRoutineDstRegSrcImd (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcImmediateValue,
    UINT32 insAssembly);
VOID analysisRoutineDstMemSrcReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly);
VOID analysisRoutineDstMemSrcLargeReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    UINT32 srcReg, const PIN_REGISTER *srcRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly);
VOID analysisRoutineDstMemSrcImd (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcImmediateValue,
    UINT32 memReadBytes,
    UINT32 insAssembly);
VOID analysisRoutineDstMemSrcMem (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly);
VOID analysisRoutineConditionalBranch (VOID *iseptr, UINT32 opcode,
    BOOL branchTaken,
    UINT32 insAssembly);
VOID analysisRoutineDstRegSrcAdg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 insAssembly);
VOID analysisRoutineBeforeChangeOfReg (VOID *iseptr, UINT32 opcode,
    UINT32 reg,
    UINT32 insAssembly);
VOID analysisRoutineTwoDstRegOneSrcReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstLeftReg, ADDRINT dstLeftRegVal,
    UINT32 dstRightReg, ADDRINT dstRightRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 insAssembly);
VOID analysisRoutineAfterOperandLess (VOID *iseptr, UINT32 opcode,
    const CONTEXT *context,
    UINT32 insAssembly);
VOID analysisRoutineRunHooks (VOID *iseptr, const CONTEXT *context);
VOID analysisRoutineDstRegSrcImplicit (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 insAssembly);
VOID analysisRoutineDstMemSrcImplicit (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly);
VOID analysisRoutineInitializeRegisters (VOID *iseptr, CONTEXT *context);

}
}
}
}

#endif /* InstructionSymbolicExecuter.h */
