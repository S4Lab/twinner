//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2015  Behnam Momeni
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

namespace cv {

class ConcreteValue;
}
}
namespace twintool {

class ExpressionValueProxy;
class MutableExpressionValueProxy;

class InstructionSymbolicExecuter {
private:
  typedef edu::sharif::twinner::trace::cv::ConcreteValue ConcreteValue;

  typedef void (InstructionSymbolicExecuter::*AnalysisRoutine) (
      const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src);
  typedef void (InstructionSymbolicExecuter::*MutableSourceAnalysisRoutine) (
      const MutableExpressionValueProxy &dst, const MutableExpressionValueProxy &src);
  typedef void (InstructionSymbolicExecuter::*AuxOperandHavingAnalysisRoutine) (
      const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src,
      const MutableExpressionValueProxy &aux);
  typedef void (InstructionSymbolicExecuter::*DoubleDestinationsAnalysisRoutine) (
      const MutableExpressionValueProxy &leftDst,
      const MutableExpressionValueProxy &rightDst,
      const ExpressionValueProxy &src);
  typedef void (InstructionSymbolicExecuter::*DoubleSourcesAnalysisRoutine) (
      const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &leftSrc, const ExpressionValueProxy &rightSrc);
  typedef void (InstructionSymbolicExecuter::*OneToThreeOperandsAnalysisRoutine) (
      const MutableExpressionValueProxy &dstOne,
      const MutableExpressionValueProxy &dstTwo,
      const MutableExpressionValueProxy &dstThree,
      const ExpressionValueProxy &srcOne);
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
  const bool measureMode;

  UINT64 numberOfExecutedInstructions; // used in measure mode

public:
  InstructionSymbolicExecuter (std::ifstream &symbolsFileInputStream, bool disabled,
      bool _measureMode);
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
  void analysisRoutineDstRegSrcMutableReg (MutableSourceAnalysisRoutine routine,
      REG dstReg, const ConcreteValue &dstRegVal,
      REG srcReg, const ConcreteValue &srcRegVal,
      UINT32 insAssembly);
  void analysisRoutineDstRegSrcRegAuxReg (AuxOperandHavingAnalysisRoutine routine,
      REG dstReg, const ConcreteValue &dstRegVal,
      REG srcReg, const ConcreteValue &srcRegVal,
      REG auxReg, const ConcreteValue &auxRegVal,
      UINT32 insAssembly);
  void analysisRoutineDstRegSrcRegAuxImd (DoubleSourcesAnalysisRoutine routine,
      REG dstReg, const ConcreteValue &dstRegVal,
      REG srcReg, const ConcreteValue &srcRegVal,
      const ConcreteValue &auxImmediateValue,
      UINT32 insAssembly);
  void analysisRoutineDstRegSrcMem (AnalysisRoutine routine,
      REG dstReg, const ConcreteValue &dstRegVal,
      ADDRINT srcMemoryEa, UINT32 memReadBytes,
      UINT32 insAssembly);
  void analysisRoutineDstRegSrcMemAuxReg (AuxOperandHavingAnalysisRoutine routine,
      REG dstReg, const ConcreteValue &dstRegVal,
      ADDRINT srcMemoryEa,
      REG auxReg, const ConcreteValue &auxRegVal,
      UINT32 memReadBytes,
      UINT32 insAssembly);
  void analysisRoutineDstRegSrcMemAuxImd (DoubleSourcesAnalysisRoutine routine,
      REG dstReg, const ConcreteValue &dstRegVal,
      ADDRINT srcMemoryEa, UINT32 memReadBytes,
      const ConcreteValue &auxImmediateValue,
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
  void analysisRoutineDstMemSrcMutableReg (MutableSourceAnalysisRoutine routine,
      ADDRINT dstMemoryEa,
      REG srcReg, const ConcreteValue &srcRegVal,
      UINT32 memReadBytes,
      UINT32 insAssembly);
  void analysisRoutineDstMemSrcRegAuxReg (AuxOperandHavingAnalysisRoutine routine,
      ADDRINT dstMemoryEa,
      REG srcReg, const ConcreteValue &srcRegVal,
      REG auxReg, const ConcreteValue &auxRegVal,
      UINT32 memReadBytes,
      UINT32 insAssembly);
  void analysisRoutineDstMemSrcImd (AnalysisRoutine routine,
      ADDRINT dstMemoryEa,
      const ConcreteValue &srcImmediateValue,
      UINT32 memReadBytes,
      UINT32 insAssembly);
  void analysisRoutineDstMemSrcImdAuxReg (AuxOperandHavingAnalysisRoutine routine,
      ADDRINT dstMemoryEa,
      const ConcreteValue &srcImmediateValue,
      REG auxReg, const ConcreteValue &auxRegVal,
      UINT32 memReadBytes,
      UINT32 insAssembly);
  void analysisRoutineDstMemSrcMem (AnalysisRoutine routine,
      ADDRINT dstMemoryEa,
      ADDRINT srcMemoryEa, UINT32 memReadBytes,
      UINT32 insAssembly);
  void analysisRoutineDstMemSrcMemAuxReg (AuxOperandHavingAnalysisRoutine routine,
      ADDRINT dstMemoryEa,
      ADDRINT srcMemoryEa,
      REG auxReg, const ConcreteValue &auxRegVal,
      UINT32 memReadBytes,
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
  void analysisRoutineTwoDstRegOneSrcMem (DoubleDestinationsAnalysisRoutine routine,
      REG dstLeftReg, const ConcreteValue &dstLeftRegVal,
      REG dstRightReg, const ConcreteValue &dstRightRegVal,
      ADDRINT srcMemoryEa, UINT32 memReadBytes,
      UINT32 insAssembly);
  void analysisRoutineTwoRegOneMem (DoubleDestinationsAnalysisRoutine routine,
      REG dstLeftReg, const ConcreteValue &dstLeftRegVal,
      REG dstRightReg, const ConcreteValue &dstRightRegVal,
      ADDRINT srcMemoryEa, UINT32 memReadBytes,
      UINT32 insAssembly);
  void analysisRoutineOneMemTwoReg (DoubleDestinationsAnalysisRoutine routine,
      ADDRINT dstMemoryEa,
      REG dstReg, const ConcreteValue &dstRegVal,
      REG srcReg, const ConcreteValue &srcRegVal,
      UINT32 memReadBytes, UINT32 insAssembly);
  void analysisRoutineTwoRegTwoMem (OneToThreeOperandsAnalysisRoutine routine,
      REG dstLeftReg, const ConcreteValue &dstLeftRegVal,
      REG dstRightReg, const ConcreteValue &dstRightRegVal,
      ADDRINT dstMemoryEa, ADDRINT srcMemoryEa, UINT32 memReadBytes,
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
  void analysisRoutineRepEqualOrRepNotEqualPrefix (REG repReg,
      const ConcreteValue &repRegVal, BOOL executing, BOOL repEqual,
      UINT32 insAssembly);

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
   * accumulator := RAX | EAX | AX | AL
   * if (dst == accumulator)
   *  dst <- src
   * else
   *  accumulator <- dst
   */
  void cmpxchgAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src, const MutableExpressionValueProxy &aux);

  /**
   * PSHUFD is packed shuffle for double words. The 8-bits order argument is consisted
   * of 4 parts of 2-bits index numbers. Each index indicates that which double word
   * from the src operand should be placed in the next double word place of the dst.
   */
  void pshufdAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src, const ExpressionValueProxy &order);

  /**
   * XCHG instruction exchanges values of dst (r/m) and src (r) atomically
   */
  void xchgAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const MutableExpressionValueProxy &src);

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
   * MOV String to String reads from [rsi]/srcMem and moves to [rdi]/dstMem and
   * increments/decrements rdi/rsi registers
   */
  void movsAnalysisRoutine (const MutableExpressionValueProxy &rdi,
      const MutableExpressionValueProxy &rsi,
      const MutableExpressionValueProxy &dstMem,
      const ExpressionValueProxy &srcMem);

  /**
   * CMPSB / CMPSW / CMPSD / CMPSQ compare string with 1/2/4/8 bytes sizes.
   * Operands are read from [rsi]/srcMem and [rdi]/dstMem and
   * increments/decrements rdi/rsi registers.
   */
  void cmpsAnalysisRoutine (const MutableExpressionValueProxy &rdi,
      const MutableExpressionValueProxy &rsi,
      const MutableExpressionValueProxy &dstMem,
      const ExpressionValueProxy &srcMem);

  /**
   * PUSH has 3 models
   * m <- r/m/i
   */
  void pushAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src,
      const MutableExpressionValueProxy &aux);

  /**
   * POP has 2 models
   * r/m <- m
   */
  void popAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src,
      const MutableExpressionValueProxy &aux);

  /**
   * LODSD is load string double word
   * eax/dst-reg <- [rsi]
   */
  void lodsdAnalysisRoutine (const MutableExpressionValueProxy &dstReg,
      const ExpressionValueProxy &srcMem,
      const MutableExpressionValueProxy &rsi);

  /**
   * ADD has 5 models
   * r += r/m/i
   * m += r/i
   */
  void addAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * ADC has 5 models. It is Add with carry.
   * r += r/m/i
   * m += r/i
   */
  void adcAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * SUB has 5 models
   * r -= r/m/i
   * m -= r/i
   */
  void subAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * SBB is subtract with borrow
   * dst = dst - (src + CF) where CF is the carry of the previous operation
   */
  void sbbAnalysisRoutine (const MutableExpressionValueProxy &dst,
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
   * JNLE jumps if ZF=0 and SF=OF which means that corresponding expression was > 0
   */
  void jnleAnalysisRoutine (bool branchTaken);

  /**
   * JL jumps if SF!=OF which means that corresponding expression was < 0
   */
  void jlAnalysisRoutine (bool branchTaken);

  /**
   * JNL jumps if SF=OF which means that corresponding expression was > 0
   */
  void jnlAnalysisRoutine (bool branchTaken);

  /**
   * JBE jumps if ZF=1 or CF=1 which means that corresponding expression was <= 0
   */
  void jbeAnalysisRoutine (bool branchTaken);

  /**
   * JNBE jumps if ZF=0 and CF=0 which means that corresponding expression was > 0
   */
  void jnbeAnalysisRoutine (bool branchTaken);

  /**
   * JNB jumps if CF=0 which means that corresponding expression was > 0
   */
  void jnbAnalysisRoutine (bool branchTaken);

  /**
   * JB jumps if CF=1 (jump below)
   */
  void jbAnalysisRoutine (bool branchTaken);

  /**
   * JS jumps if SF=1 which means that corresponding expression was < 0
   */
  void jsAnalysisRoutine (bool branchTaken);

  /**
   * JNS jumps if SF=0 which means that corresponding expression was >= 0
   */
  void jnsAnalysisRoutine (bool branchTaken);

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
   * JMP instruction performs an unconditional jump.
   * Normally we do not need to track jumps. However due to a bug in PIN, some
   * instructions coming after some JMP instructions are not get instrumented.
   * So JMP may change value of RSP without any notice.
   * This hook is for maintaining the value of RSP.
   */
  void jmpAnalysisRoutine (const CONTEXT *context, const ConcreteValue &rspRegVal);

  void repAnalysisRoutine (const MutableExpressionValueProxy &dst,
      bool executing, bool repEqual);

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
   * ROR rotates right the dst as much as indicated by src.
   * Also the LSB of src (which will be moved to the new MSB) will be set in CF.
   */
  void rorAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * ROL rotates left the dst as much as indicated by src.
   * Also the MSB of src (which will be moved to the new LSB) will be set in CF.
   */
  void rolAnalysisRoutine (const MutableExpressionValueProxy &dst,
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
   * BT is bit test instruction. It finds the bitoffset-th bit from the bitstring and
   * set it as the CF.
   */
  void btAnalysisRoutine (const MutableExpressionValueProxy &bitstring,
      const ExpressionValueProxy &bitoffset);

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
   * are compared together byte-by-byte and those bytes which are/aren't equal will be
   * filed with 1 (0xFF) / 0 (0x00) in the dst reg.
   */
  void pcmpeqbAnalysisRoutine (
      const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src);

  /**
   * PMINUB is a packed minimum finding for unsigned bytes.
   * Packed unsigned bytes which are stored in dst and src wil be compared to find their
   * minimum values. Minimum values will be stored in the dst.
   */
  void pminubAnalysisRoutine (
      const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src);

  /**
   * PUNPCKLBW is a packed operation which "unpacks" low-data from src-dst and interleaves
   * them and put the result in the dst.
   *  -- byte to word
   */
  void punpcklbwAnalysisRoutine (
      const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src);

  /**
   * PUNPCKLWD is a packed operation which "unpacks" low-data from src-dst and interleaves
   * them and put the result in the dst.
   *  -- word to double-word
   */
  void punpcklwdAnalysisRoutine (
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
   * IMUL is signed multiply and has three models.
   * This method implements the two operands model.
   */
  void imulAnalysisRoutine (const MutableExpressionValueProxy &dst,
      const ExpressionValueProxy &src);

  /**
   * SCAS instruction compares AL/AX/EAX/RAX (the dstReg) and a given srcMem value
   * which is pointed to by the DI/EDI/RDI (the srcReg) and sets the EFLAGS based on
   * the comparison result.
   */
  void scasAnalysisRoutine (const MutableExpressionValueProxy &dstReg,
      const MutableExpressionValueProxy &srcReg,
      const ExpressionValueProxy &srcMem);

  /**
   * Store String stores the srcReg into dstMem==[rdi] location and moves rdi accordingly.
   */
  void stosAnalysisRoutine (const MutableExpressionValueProxy &dstMem,
      const MutableExpressionValueProxy &rdireg,
      const ExpressionValueProxy &srcReg);

  /**
   * LEAVE instruction:
   *   spReg <- fpReg
   *   fpReg <- pop-from-stack
   */
  void leaveAnalysisRoutine (const MutableExpressionValueProxy &fpReg,
      const MutableExpressionValueProxy &spReg,
      const ExpressionValueProxy &srcMem);

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
   * Clears the direction flags (DF)
   */
  void cldAnalysisRoutine (const CONTEXT *context);

  /**
   * CPUID == CPU Identification
   */
  void cpuidAnalysisRoutine (const CONTEXT *context);

  /**
   * INC increments the opr reg/mem operand.
   */
  void incAnalysisRoutine (const MutableExpressionValueProxy &opr);

  /**
   * DEC decrements the opr reg/mem operand.
   */
  void decAnalysisRoutine (const MutableExpressionValueProxy &opr);

  /**
   * NEG two's complements the opr (which is reg or mem).
   */
  void negAnalysisRoutine (const MutableExpressionValueProxy &opr);

  /**
   * SETNZ sets opr to 1 iff ZF=0 (and sets it to 0 otherwise).
   */
  void setnzAnalysisRoutine (const MutableExpressionValueProxy &opr);

  /**
   * SETZ sets opr to 1 iff ZF=1 (and sets it to 0 otherwise).
   */
  void setzAnalysisRoutine (const MutableExpressionValueProxy &opr);

  /**
   * SETLE sets opr to 1 iff ZF=1 or SF != OF (and sets it to 0 otherwise).
   */
  void setleAnalysisRoutine (const MutableExpressionValueProxy &opr);

  /**
   * SETL sets opr to 1 iff SF != OF (and sets it to 0 otherwise).
   */
  void setlAnalysisRoutine (const MutableExpressionValueProxy &opr);

  /**
   * SETBE sets opr to 1 iff ZF=1 or CF=1 (and sets it to 0 otherwise).
   */
  void setbeAnalysisRoutine (const MutableExpressionValueProxy &opr);

  /**
   * SETNBE sets opr to 1 iff ZF=0 and CF=0 (and sets it to 0 otherwise).
   */
  void setnbeAnalysisRoutine (const MutableExpressionValueProxy &opr);

  /**
   * NOT one's complements the opr.
   * opr <- NOT(opr)
   */
  void notAnalysisRoutine (const MutableExpressionValueProxy &opr);

  void adjustRsiRdiRegisters (int size,
      const MutableExpressionValueProxy &rdi, const MutableExpressionValueProxy &rsi);

public:
  AnalysisRoutine convertOpcodeToAnalysisRoutine (OPCODE op) const;
  MutableSourceAnalysisRoutine convertOpcodeToMutableSourceAnalysisRoutine (
      OPCODE op) const;
  AuxOperandHavingAnalysisRoutine convertOpcodeToAuxOperandHavingAnalysisRoutine (
      OPCODE op) const;
  DoubleDestinationsAnalysisRoutine convertOpcodeToDoubleDestinationsAnalysisRoutine (
      OPCODE op) const;
  DoubleSourcesAnalysisRoutine convertOpcodeToDoubleSourcesAnalysisRoutine (
      OPCODE op) const;
  OneToThreeOperandsAnalysisRoutine convertOpcodeToOneToThreeOperandsAnalysisRoutine (
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
VOID analysisRoutineDstRegSrcMutableReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 insAssembly);
VOID analysisRoutineDstRegSrcRegAuxReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 auxReg, ADDRINT auxRegVal,
    UINT32 insAssembly);
VOID analysisRoutineDstRegSrcLargeReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, const PIN_REGISTER *srcRegVal,
    UINT32 insAssembly);
VOID analysisRoutineDstLargeRegSrcReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, const PIN_REGISTER *dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 insAssembly);
VOID analysisRoutineDstLargeRegSrcLargeReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, const PIN_REGISTER *dstRegVal,
    UINT32 srcReg, const PIN_REGISTER *srcRegVal,
    UINT32 insAssembly);
VOID analysisRoutineDstLargeRegSrcLargeRegAuxImd (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, const PIN_REGISTER *dstRegVal,
    UINT32 srcReg, const PIN_REGISTER *srcRegVal,
    ADDRINT auxImmediateValue,
    UINT32 insAssembly);
VOID analysisRoutineDstLargeRegSrcMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, const PIN_REGISTER *dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly);
VOID analysisRoutineDstLargeRegSrcMemAuxImd (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, const PIN_REGISTER *dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    ADDRINT auxImmediateValue,
    UINT32 insAssembly);
VOID analysisRoutineDstRegSrcMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly);
VOID analysisRoutineDstRegSrcMemAuxReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcMemoryEa,
    UINT32 auxReg, ADDRINT auxRegVal,
    UINT32 memReadBytes,
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
VOID analysisRoutineDstMemSrcMutableReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly);
VOID analysisRoutineDstMemSrcRegAuxReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 auxReg, ADDRINT auxRegVal,
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
VOID analysisRoutineDstMemSrcImdAuxReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcImmediateValue,
    UINT32 auxReg, ADDRINT auxRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly);
VOID analysisRoutineDstMemSrcMem (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly);
VOID analysisRoutineDstMemSrcMemAuxReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa,
    UINT32 auxReg, ADDRINT auxRegVal,
    UINT32 memReadBytes,
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
VOID analysisRoutineTwoDstRegOneSrcMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstLeftReg, ADDRINT dstLeftRegVal,
    UINT32 dstRightReg, ADDRINT dstRightRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
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
VOID analysisRoutineStrOpRegMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 insAssembly);
VOID analysisRoutineStrOpMemReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly);
VOID analysisRoutineStrOpMemMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT dstMemoryEa,
    UINT32 srcReg, ADDRINT srcRegVal,
    ADDRINT srcMemoryEa,
    UINT32 memReadBytes,
    UINT32 insAssembly);
VOID analysisRoutineRepPrefix (VOID *iseptr, UINT32 opcode,
    UINT32 repReg, ADDRINT repRegVal,
    UINT32 executing, UINT32 repEqual,
    UINT32 insAssembly);

}
}
}
}

#endif /* InstructionSymbolicExecuter.h */
