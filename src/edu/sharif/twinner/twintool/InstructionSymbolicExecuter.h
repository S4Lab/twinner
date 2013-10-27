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

class InstructionSymbolicExecuter {

private:
  edu::sharif::twinner::trace::Trace *trace;
  Flags eflags;

public:
  InstructionSymbolicExecuter ();

  edu::sharif::twinner::trace::Trace *getTrace () const;

private:

  typedef void (InstructionSymbolicExecuter::*AnalysisRoutine) (
      MutableExpressionValueProxy dst, ExpressionValueProxy src);

public:
  void analysisRoutineDstRegSrcReg (AnalysisRoutine routine,
      REG dstReg, UINT64 dstRegVal,
      REG srcReg, UINT64 srcRegVal);
  void analysisRoutineDstRegSrcMem (AnalysisRoutine routine,
      REG dstReg, UINT64 dstRegVal,
      ADDRINT srcMemoryEa);
  void analysisRoutineDstRegSrcImd (AnalysisRoutine routine,
      REG dstReg, UINT64 dstRegVal,
      ADDRINT srcImmediateValue);
  void analysisRoutineDstMemSrcReg (AnalysisRoutine routine,
      ADDRINT dstMemoryEa,
      REG srcReg, UINT64 srcRegVal);
  void analysisRoutineDstMemSrcImd (AnalysisRoutine routine,
      ADDRINT dstMemoryEa,
      ADDRINT srcImmediateValue);
  void analysisRoutineDstMemSrcMem (AnalysisRoutine routine,
      ADDRINT dstMemoryEa,
      ADDRINT srcMemoryEa);

private:

  /**
   * MOV has 5 models
   * r <- r/m/i
   * m <- r/i
   */
  void movAnalysisRoutine (MutableExpressionValueProxy dst, ExpressionValueProxy src);

  /**
   * PUSH has 3 models
   * m <- r/m/i
   */
  void pushAnalysisRoutine (MutableExpressionValueProxy dst, ExpressionValueProxy src);

  /**
   * POP has 2 models
   * r/m <- m
   */
  void popAnalysisRoutine (MutableExpressionValueProxy dst, ExpressionValueProxy src);

  /**
   * ADD has 5 models
   * r += r/m/i
   * m += r/i
   */
  void addAnalysisRoutine (MutableExpressionValueProxy dst, ExpressionValueProxy src);

  /**
   * SUB has 5 models
   * r -= r/m/i
   * m -= r/i
   */
  void subAnalysisRoutine (MutableExpressionValueProxy dst, ExpressionValueProxy src);

  /**
   * CMP is same as SUB else of not modifying dst operand's value
   */
  void cmpAnalysisRoutine (MutableExpressionValueProxy dst, ExpressionValueProxy src);

public:
  AnalysisRoutine convertOpcodeToAnalysisRoutine (OPCODE op) const;

  static UINT64 readMemoryContent (ADDRINT memoryEa);
};

VOID analysisRoutineDstRegSrcReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal);
VOID analysisRoutineDstRegSrcMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcMemoryEa);
VOID analysisRoutineDstRegSrcImd (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcImmediateValue);
VOID analysisRoutineDstMemSrcReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    UINT32 srcReg, ADDRINT srcRegVal);
VOID analysisRoutineDstMemSrcImd (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcImmediateValue);
VOID analysisRoutineDstMemSrcMem (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa);

}
}
}
}

#endif /* InstructionSymbolicExecuter.h */
