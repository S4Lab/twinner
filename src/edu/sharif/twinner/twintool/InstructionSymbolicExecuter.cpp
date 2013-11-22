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

#include "InstructionSymbolicExecuter.h"


#include "RegisterResidentExpressionValueProxy.h"
#include "MemoryResidentExpressionValueProxy.h"
#include "ConstantExpressionValueProxy.h"

#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/Constraint.h"

#include "edu/sharif/twinner/util/Logger.h"

#include <stdexcept>

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

InstructionSymbolicExecuter::InstructionSymbolicExecuter () :
trace (new edu::sharif::twinner::trace::Trace ()),
trackedReg (REG_INVALID_), hook (0) {
}

edu::sharif::twinner::trace::Trace *InstructionSymbolicExecuter::getTrace () const {
  return trace;
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcReg (AnalysisRoutine routine,
    REG dstReg, UINT64 dstRegVal,
    REG srcReg, UINT64 srcRegVal,
    const CONTEXT *context,
    std::string *insAssembly) {
  runHooks (context);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstRegSrcReg(INS: "
      << *insAssembly << "): dst reg: " << REG_StringShort (dstReg)
      << ", src reg: " << REG_StringShort (srcReg) << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
      RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcMem (AnalysisRoutine routine,
    REG dstReg, UINT64 dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    const CONTEXT *context,
    std::string *insAssembly) {
  runHooks (context);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstRegSrcMem(INS: "
      << *insAssembly << "): dst reg: " << REG_StringShort (dstReg)
      << ", src mem addr: " << srcMemoryEa << ", mem read bytes: " << memReadBytes
      << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
      MemoryResidentExpressionValueProxy (srcMemoryEa, (int) memReadBytes));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcImd (AnalysisRoutine routine,
    REG dstReg, UINT64 dstRegVal,
    ADDRINT srcImmediateValue,
    const CONTEXT *context,
    std::string *insAssembly) {
  runHooks (context);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstRegSrcImd(INS: "
      << *insAssembly << "): dst reg: " << REG_StringShort (dstReg)
      << ", src imd: " << srcImmediateValue << '\n';
  edu::sharif::twinner::trace::Expression *srcexp =
      new edu::sharif::twinner::trace::Expression (srcImmediateValue);
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
      ConstantExpressionValueProxy (srcexp));
  delete srcexp;
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcReg (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    REG srcReg, UINT64 srcRegVal,
    UINT32 memReadBytes,
    const CONTEXT *context,
    std::string *insAssembly) {
  runHooks (context);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstMemSrcReg(INS: "
      << *insAssembly << "): dst mem addr: " << dstMemoryEa
      << ", src reg: " << REG_StringShort (srcReg) << '\n';
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
      RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcImd (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    ADDRINT srcImmediateValue,
    UINT32 memReadBytes,
    const CONTEXT *context,
    std::string *insAssembly) {
  runHooks (context);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstMemSrcImd(INS: "
      << *insAssembly << "): dst mem addr: " << dstMemoryEa
      << ", src imd: " << srcImmediateValue << '\n';
  edu::sharif::twinner::trace::Expression *srcexp =
      new edu::sharif::twinner::trace::Expression (srcImmediateValue);
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
      ConstantExpressionValueProxy (srcexp));
  delete srcexp;
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcMem (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    const CONTEXT *context,
    std::string *insAssembly) {
  runHooks (context);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstMemSrcMem(INS: "
      << *insAssembly << "): dst mem addr: " << dstMemoryEa
      << ", src mem addr: " << srcMemoryEa << ", mem read bytes: " << memReadBytes
      << '\n';
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa),
      MemoryResidentExpressionValueProxy (srcMemoryEa, (int) memReadBytes));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineConditionalBranch (
    ConditionalBranchAnalysisRoutine routine,
    BOOL branchTaken,
    const CONTEXT *context,
    std::string *insAssembly) {
  runHooks (context);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineConditionalBranch(INS: "
      << *insAssembly << "): branch taken: " << branchTaken << '\n';
  (this->*routine) (branchTaken);
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcAdg (AnalysisRoutine routine,
    REG dstReg, UINT64 dstRegVal,
    const CONTEXT *context,
    std::string *insAssembly) {
  runHooks (context);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstRegSrcAdg(INS: "
      << *insAssembly << ") [AFTER execution of instruction]: dst reg: "
      << REG_StringShort (dstReg) << ", dst reg value: 0x"
      << std::hex << dstRegVal << '\n';
  edu::sharif::twinner::trace::Expression *srcexp =
      new edu::sharif::twinner::trace::Expression (dstRegVal);
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
      ConstantExpressionValueProxy (srcexp));
  delete srcexp;
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineBeforeChangeOfReg (
    SuddenlyChangedRegAnalysisRoutine routine,
    REG reg,
    const CONTEXT *context,
    std::string *insAssembly) {
  runHooks (context);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineBeforeChangeOfReg(INS: "
      << *insAssembly << ")\n"
      "\tregistering register to be tracked...";
  trackedReg = reg;
  hook = routine;
  logger << "done\n";
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineTwoDstRegOneSrcReg (
    DoubleDestinationsAnalysisRoutine routine,
    REG dstLeftReg, UINT64 dstLeftRegVal,
    REG dstRightReg, UINT64 dstRightRegVal,
    REG srcReg, UINT64 srcRegVal,
    const CONTEXT *context,
    std::string *insAssembly) {
  runHooks (context);
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineTwoDstRegOneSrcReg(INS: "
      << *insAssembly << "): left dst reg: " << REG_StringShort (dstLeftReg)
      << ", right dst reg: " << REG_StringShort (dstRightReg)
      << ", src reg: " << REG_StringShort (srcReg) << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstLeftReg, dstLeftRegVal),
      RegisterResidentExpressionValueProxy (dstRightReg, dstRightRegVal),
      RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::runHooks (const CONTEXT *context) {
  if (trackedReg != REG_INVALID_) {
    (this->*hook) (readRegisterContent (context, trackedReg));
    trackedReg = REG_INVALID_;
    hook = 0;
  }
}

void InstructionSymbolicExecuter::movAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "movAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting dst exp...";
  dst.setExpression (trace, srcexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::pushAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "pushAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting dst exp...";
  dst.setExpression (trace, srcexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tadjusting rsp...";
  //TODO: check for current value of RSP too
  edu::sharif::twinner::trace::Expression *rsp =
      trace->tryToGetSymbolicExpressionByRegister (REG_RSP);
  if (rsp) { // If we are not tracking RSP yet, it's not required to adjust its value
    // When check for current value of RSP get added, this condition can be removed.
    rsp->minus (8);
  }
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::popAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "popAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting dst exp...";
  dst.setExpression (trace, srcexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tadjusting rsp...";
  //TODO: check for current value of RSP too
  edu::sharif::twinner::trace::Expression *rsp =
      trace->tryToGetSymbolicExpressionByRegister (REG_RSP);
  if (rsp) { // If we are not tracking RSP yet, it's not required to adjust its value
    // When check for current value of RSP get added, this condition can be removed.
    rsp->add (8);
  }
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::addAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "addAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tbinary operation...";
  dstexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::ADD), srcexp);
  dst.valueIsChanged (trace, dstexp);
  //TODO: set rflags
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::subAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "subAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tbinary operation...";
  dstexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::MINUS), srcexp);
  dst.valueIsChanged (trace, dstexp);
  //TODO: set rflags
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::cmpAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "cmpAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp...";
  const edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);

  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tbinary operation...";
  edu::sharif::twinner::trace::Expression *tmpexp = dstexp->clone ();
  tmpexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::MINUS), srcexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting EFLAGS...";
  eflags.setFlags (tmpexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::leaAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "leaAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting dst exp...";
  dst.setExpression (trace, srcexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::jnzAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::util::Logger::loquacious () << "jnzAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  edu::sharif::twinner::trace::Constraint *cc
      = new edu::sharif::twinner::trace::Constraint
      (eflags.getFlagsUnderlyingExpression (),
       branchTaken ?
       edu::sharif::twinner::trace::Constraint::NON_ZERO :
       edu::sharif::twinner::trace::Constraint::ZERO);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tadding constraint...";
  trace->addPathConstraint (cc);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::callAnalysisRoutine (UINT64 rspRegVal) {
  edu::sharif::twinner::util::Logger::loquacious () << "callAnalysisRoutine(...)\n"
      << "\tgetting rsp reg exp...";
  edu::sharif::twinner::trace::Expression *rsp =
      trace->tryToGetSymbolicExpressionByRegister (REG_RSP);
  if (rsp) { // If we are not tracking RSP yet, it's not required to adjust its value
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tadjusting rsp...";
    UINT64 oldVal = rsp->getLastConcreteValue ();
    if (oldVal > rspRegVal) {
      // some items have been pushed into stack by CALL and so RSP is decremented
      rsp->minus (oldVal - rspRegVal);
      // TODO: call valueIsChanged from an expression proxy to address ESP, SP, and SPL

    } else {
      edu::sharif::twinner::util::Logger::warning ()
          << "RSP is not decremented at all after CALL instruction!\n";
    }
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::retAnalysisRoutine (UINT64 rspRegVal) {
  edu::sharif::twinner::util::Logger::loquacious () << "retAnalysisRoutine(...)\n"
      << "\tgetting rsp reg exp...";
  edu::sharif::twinner::trace::Expression *rsp =
      trace->tryToGetSymbolicExpressionByRegister (REG_RSP);
  if (rsp) { // If we are not tracking RSP yet, it's not required to adjust its value
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tadjusting rsp...";
    UINT64 oldVal = rsp->getLastConcreteValue ();
    if (oldVal < rspRegVal) {
      // some items have been popped out from the stack by RET and so RSP is incremented
      rsp->add (rspRegVal - oldVal);
      // TODO: call valueIsChanged from an expression proxy to address ESP, SP, and SPL

    } else {
      edu::sharif::twinner::util::Logger::warning ()
          << "RSP is not incremented at all after RET instruction!\n";
    }
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::shlAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "shlAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tshifting operation...";
  if (dynamic_cast<const ConstantExpressionValueProxy *> (&src) != 0) {
    // src was an immediate value
    dstexp->shiftToLeft (srcexp->getLastConcreteValue ());
  } else {
    // src was CL register
    dstexp->shiftToLeft (srcexp);
  }
  // truncate bits which are shifted left, outside of dst boundaries
  dst.truncate (dstexp);
  dst.valueIsChanged (trace, dstexp);
  //TODO: set rflags
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::andAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "andAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tbinary operation...";
  dstexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::BITWISE_AND), srcexp);
  dst.valueIsChanged (trace, dstexp);
  //TODO: set rflags
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::xorAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "xorAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tbinary operation...";
  dstexp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::XOR), srcexp);
  dst.valueIsChanged (trace, dstexp);
  //TODO: set rflags
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::divAnalysisRoutine (
    const MutableExpressionValueProxy &leftDst,
    const MutableExpressionValueProxy &rightDst,
    const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "divAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting left dst exp...";
  edu::sharif::twinner::trace::Expression *leftDstExp =
      leftDst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting right dst exp...";
  edu::sharif::twinner::trace::Expression *rightDstExp =
      rightDst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tpreparing left-right in both dst regs...";
  divisionSize = leftDst.getSize ();
  leftDstExp->shiftToLeft (divisionSize);
  leftDstExp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::BITWISE_OR), rightDstExp);
  rightDstExp = rightDst.setExpressionWithoutChangeNotification (trace, leftDstExp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tcalculating quotient (right) and remainder (left) of division...";
  leftDstExp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::REMAINDER), srcexp);
  rightDstExp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::DIVIDE), srcexp);
  // At this point, symbolic quotient and remainder are calculated correctly.
  // but concrete values are not! So we need to register a hook to synchronize concrete
  // values too (we can also calculate them in assembly, but it's not required).

  // TODO: Register hook for updating concrete values of two destination registers
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

InstructionSymbolicExecuter::AnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToAnalysisRoutine (OPCODE op) const {
  switch (op) {
  case XED_ICLASS_MOV:
  case XED_ICLASS_MOVZX:
  case XED_ICLASS_MOVSX:
    return &InstructionSymbolicExecuter::movAnalysisRoutine;
  case XED_ICLASS_PUSH:
    return &InstructionSymbolicExecuter::pushAnalysisRoutine;
  case XED_ICLASS_POP:
    return &InstructionSymbolicExecuter::popAnalysisRoutine;
  case XED_ICLASS_ADD:
    return &InstructionSymbolicExecuter::addAnalysisRoutine;
  case XED_ICLASS_SUB:
    return &InstructionSymbolicExecuter::subAnalysisRoutine;
  case XED_ICLASS_CMP:
    return &InstructionSymbolicExecuter::cmpAnalysisRoutine;
  case XED_ICLASS_LEA:
    return &InstructionSymbolicExecuter::leaAnalysisRoutine;
  case XED_ICLASS_SHL:
    return &InstructionSymbolicExecuter::shlAnalysisRoutine;
  case XED_ICLASS_AND:
    return &InstructionSymbolicExecuter::andAnalysisRoutine;
  case XED_ICLASS_XOR:
    return &InstructionSymbolicExecuter::xorAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: Unknown opcode: "
        << OPCODE_StringShort (op) << '\n';
    throw std::runtime_error ("Unknown opcode given to analysis routine");
  }
}

InstructionSymbolicExecuter::DoubleDestinationsAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToDoubleDestinationsAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_DIV:
    return &InstructionSymbolicExecuter::divAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Double Destinations: Unknown opcode: " << OPCODE_StringShort (op) << '\n';
    throw std::runtime_error
        ("Unknown opcode (for double dests) given to analysis routine");
  }
}

InstructionSymbolicExecuter::ConditionalBranchAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToConditionalBranchAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_JNZ:
    return &InstructionSymbolicExecuter::jnzAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Conditional Branch: Unknown opcode: " << OPCODE_StringShort (op) << '\n';
    throw std::runtime_error ("Unknown opcode (for Jcc) given to analysis routine");
  }
}

InstructionSymbolicExecuter::SuddenlyChangedRegAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToSuddenlyChangedRegAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_CALL_FAR:
  case XED_ICLASS_CALL_NEAR:
    return &InstructionSymbolicExecuter::callAnalysisRoutine;
  case XED_ICLASS_RET_FAR:
  case XED_ICLASS_RET_NEAR:
    return &InstructionSymbolicExecuter::retAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Suddenly Changed Register: Unknown opcode: " << OPCODE_StringShort (op) << '\n';
    throw std::runtime_error
        ("Unknown opcode (for changedReg) given to analysis routine");
  }
}

UINT64 InstructionSymbolicExecuter::readRegisterContent (const CONTEXT *context,
    REG reg) {
  REGVAL regVal;
  PIN_GetContextRegval (context, reg, &regVal);
  UINT64 value;
  PIN_ReadRegvalQWord (&regVal, &value, 0);
  return value;
}

UINT64 InstructionSymbolicExecuter::readMemoryContent (ADDRINT memoryEa) {
  UINT64 currentConcreteValue;
  PIN_SafeCopy (&currentConcreteValue, (const VOID *) (memoryEa), sizeof (UINT64));
  return currentConcreteValue;
}

UINT64 InstructionSymbolicExecuter::truncateValue (UINT64 value, int countOfBytes) {
  switch (countOfBytes) {
  case 8:
    break; // no cast is required in 64-bits mode
  case 4:
    value = (UINT32) value;
    break;
  case 2:
    value = (UINT16) value;
    break;
  case 1:
    value = (UINT8) value;
    break;
  default:
    throw std::runtime_error ("Can not truncate value to given count of bytes");
  }
  return value;
}

VOID analysisRoutineDstRegSrcReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    const CONTEXT *context,
    VOID *insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  std::string *insAssemblyStr = (std::string *) insAssembly;
  ise->analysisRoutineDstRegSrcReg (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    (REG) dstReg, dstRegVal,
                                    (REG) srcReg, srcRegVal,
                                    context,
                                    insAssemblyStr);
}

VOID analysisRoutineDstRegSrcMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    const CONTEXT *context,
    VOID *insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  std::string *insAssemblyStr = (std::string *) insAssembly;
  ise->analysisRoutineDstRegSrcMem (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    (REG) dstReg, dstRegVal,
                                    srcMemoryEa, memReadBytes,
                                    context,
                                    insAssemblyStr);
}

VOID analysisRoutineDstRegSrcImd (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcImmediateValue,
    const CONTEXT *context,
    VOID *insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  std::string *insAssemblyStr = (std::string *) insAssembly;
  ise->analysisRoutineDstRegSrcImd (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    (REG) dstReg, dstRegVal,
                                    srcImmediateValue,
                                    context,
                                    insAssemblyStr);
}

VOID analysisRoutineDstMemSrcReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 memReadBytes,
    const CONTEXT *context,
    VOID *insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  std::string *insAssemblyStr = (std::string *) insAssembly;
  ise->analysisRoutineDstMemSrcReg (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    dstMemoryEa,
                                    (REG) srcReg, srcRegVal,
                                    memReadBytes,
                                    context,
                                    insAssemblyStr);
}

VOID analysisRoutineDstMemSrcImd (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcImmediateValue,
    UINT32 memReadBytes,
    const CONTEXT *context,
    VOID *insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  std::string *insAssemblyStr = (std::string *) insAssembly;
  ise->analysisRoutineDstMemSrcImd (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    dstMemoryEa,
                                    srcImmediateValue,
                                    memReadBytes,
                                    context,
                                    insAssemblyStr);
}

VOID analysisRoutineDstMemSrcMem (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    const CONTEXT *context,
    VOID *insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  std::string *insAssemblyStr = (std::string *) insAssembly;
  ise->analysisRoutineDstMemSrcMem (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    dstMemoryEa,
                                    srcMemoryEa, memReadBytes,
                                    context,
                                    insAssemblyStr);
}

VOID analysisRoutineConditionalBranch (VOID *iseptr, UINT32 opcode,
    BOOL branchTaken,
    const CONTEXT *context,
    VOID *insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  std::string *insAssemblyStr = (std::string *) insAssembly;
  ise->analysisRoutineConditionalBranch
      (ise->convertOpcodeToConditionalBranchAnalysisRoutine ((OPCODE) opcode),
       branchTaken,
       context,
       insAssemblyStr);
}

VOID analysisRoutineDstRegSrcAdg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    const CONTEXT *context,
    VOID *insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  std::string *insAssemblyStr = (std::string *) insAssembly;
  ise->analysisRoutineDstRegSrcAdg (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
                                    (REG) dstReg, dstRegVal,
                                    context,
                                    insAssemblyStr);
}

VOID analysisRoutineBeforeChangeOfReg (VOID *iseptr, UINT32 opcode,
    UINT32 reg,
    const CONTEXT *context,
    VOID *insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  std::string *insAssemblyStr = (std::string *) insAssembly;
  ise->analysisRoutineBeforeChangeOfReg
      (ise->convertOpcodeToSuddenlyChangedRegAnalysisRoutine ((OPCODE) opcode),
       (REG) reg,
       context,
       insAssemblyStr);
}

VOID analysisRoutineTwoDstRegOneSrcReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstLeftReg, ADDRINT dstLeftRegVal,
    UINT32 dstRightReg, ADDRINT dstRightRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    const CONTEXT *context,
    VOID *insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  std::string *insAssemblyStr = (std::string *) insAssembly;
  ise->analysisRoutineTwoDstRegOneSrcReg
      (ise->convertOpcodeToDoubleDestinationsAnalysisRoutine ((OPCODE) opcode),
       (REG) dstLeftReg, dstLeftRegVal,
       (REG) dstRightReg, dstRightRegVal,
       (REG) srcReg, srcRegVal,
       context,
       insAssemblyStr);
}

}
}
}
}
