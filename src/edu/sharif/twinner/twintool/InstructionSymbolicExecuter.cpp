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

#include "InstructionSymbolicExecuter.h"


#include "RegisterResidentExpressionValueProxy.h"
#include "MemoryResidentExpressionValueProxy.h"
#include "ConstantExpressionValueProxy.h"

#include "edu/sharif/twinner/trace-twintool/TraceImp.h"
#include "edu/sharif/twinner/trace-twintool/ExpressionImp.h"
#include "edu/sharif/twinner/trace/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/ConcreteValue128Bits.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/Syscall.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/memory.h"

#include <stdexcept>

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {


static const int shared_memory_area_capacity = 50 * 1024; // 50 MiB
static char instructions_disassembly_shared_memory_area[shared_memory_area_capacity];
static UINT32 after_last_allocated_byte_index = 0; // [0, this-index) is allocated

UINT32 allocate_memory_from_instructions_disassembly_shared_memory_area (UINT32 size) {
  if (size == 0) {
    throw std::runtime_error
        ("allocate_memory_from_instructions_disassembly_shared_memory_area (...) "
         "function: size must be positive");
  }
  if (after_last_allocated_byte_index + size > shared_memory_area_capacity) {
    // throw std::bad_alloc ();
    throw std::runtime_error
        ("allocate_memory_from_instructions_disassembly_shared_memory_area (...) "
         "function: Out of memory!");
  }
  const UINT32 allocated = after_last_allocated_byte_index;
  after_last_allocated_byte_index += size;
  return allocated;
}

char *get_pointer_to_allocated_memory (UINT32 index) {
  if (index < after_last_allocated_byte_index) {
    return &instructions_disassembly_shared_memory_area[index];
  } else {
    return 0;
  }
}

void deallocate_memory_from_instructions_disassembly_shared_memory_area (UINT32 size) {
  if (size == 0 || size > after_last_allocated_byte_index) {
    throw std::runtime_error
        ("deallocate_memory_from_instructions_disassembly_shared_memory_area (...) "
         "function : size must be positive and smaller than currently allocated area");
  }
  after_last_allocated_byte_index -= size;
}

InstructionSymbolicExecuter::InstructionSymbolicExecuter (
    std::ifstream &symbolsFileInputStream, bool _disabled) :
trace (new edu::sharif::twinner::trace::TraceImp (symbolsFileInputStream)),
trackedReg (REG_INVALID_), operandSize (-1), hook (0),
disabled (_disabled) {
}

InstructionSymbolicExecuter::InstructionSymbolicExecuter (bool _disabled) :
trace (new edu::sharif::twinner::trace::TraceImp ()),
trackedReg (REG_INVALID_), operandSize (-1), hook (0),
disabled (_disabled) {
}

edu::sharif::twinner::trace::Trace *InstructionSymbolicExecuter::getTrace () const {
  return trace;
}

void InstructionSymbolicExecuter::disable () {
  disabled = true;
}

void InstructionSymbolicExecuter::enable () {
  disabled = false;
}

void InstructionSymbolicExecuter::syscallInvoked (const CONTEXT *context,
    edu::sharif::twinner::trace::Syscall s) {
  runHooks (context); // FIXME: Should I call hooks here or syscall instruction is instrumented independently?
  if (disabled) {
    return;
  }
  trace->syscallInvoked (s);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcReg (AnalysisRoutine routine,
    REG dstReg, const ConcreteValue &dstRegVal,
    REG srcReg, const ConcreteValue &srcRegVal,
    const char *insAssembly) {
  if (disabled) {
    return;
  }
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstRegSrcReg(INS: "
      << insAssembly << "): dst reg: " << REG_StringShort (dstReg)
      << ", src reg: " << REG_StringShort (srcReg) << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
      RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcMem (AnalysisRoutine routine,
    REG dstReg, const ConcreteValue &dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    const char *insAssembly) {
  if (disabled) {
    return;
  }
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstRegSrcMem(INS: "
      << insAssembly << "): dst reg: " << REG_StringShort (dstReg)
      << ", src mem addr: " << srcMemoryEa << ", mem read bytes: " << memReadBytes
      << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
      MemoryResidentExpressionValueProxy (srcMemoryEa, (int) memReadBytes));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcImd (AnalysisRoutine routine,
    REG dstReg, const ConcreteValue &dstRegVal,
    const ConcreteValue &srcImmediateValue,
    const char *insAssembly) {
  if (disabled) {
    return;
  }
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstRegSrcImd(INS: "
      << insAssembly << "): dst reg: " << REG_StringShort (dstReg)
      << ", src imd: " << srcImmediateValue << '\n';
  edu::sharif::twinner::trace::Expression *srcexp =
      new edu::sharif::twinner::trace::ExpressionImp (srcImmediateValue);
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
      ConstantExpressionValueProxy (srcexp));
  delete srcexp;
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcReg (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    REG srcReg, const ConcreteValue &srcRegVal,
    UINT32 memReadBytes,
    const char *insAssembly) {
  if (disabled) {
    return;
  }
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstMemSrcReg(INS: "
      << insAssembly << "): dst mem addr: " << dstMemoryEa
      << ", src reg: " << REG_StringShort (srcReg) << '\n';
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
      RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcImd (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    const ConcreteValue &srcImmediateValue,
    UINT32 memReadBytes,
    const char *insAssembly) {
  if (disabled) {
    return;
  }
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  if (insAssembly) {
    logger << "analysisRoutineDstMemSrcImd(INS: "
        << insAssembly << ")";
  } else {
    logger << "analysisRoutineDstMemSrcImd()";
  }
  logger << ": dst mem addr: " << dstMemoryEa
      << ", src imd: " << srcImmediateValue << '\n';
  edu::sharif::twinner::trace::Expression *srcexp =
      new edu::sharif::twinner::trace::ExpressionImp (srcImmediateValue);
  (this->*routine) (MemoryResidentExpressionValueProxy (dstMemoryEa, memReadBytes),
      ConstantExpressionValueProxy (srcexp));
  delete srcexp;
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstMemSrcMem (AnalysisRoutine routine,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    const char *insAssembly) {
  if (disabled) {
    return;
  }
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstMemSrcMem(INS: "
      << insAssembly << "): dst mem addr: " << dstMemoryEa
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
    const char *insAssembly) {
  if (disabled) {
    return;
  }
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineConditionalBranch(INS: "
      << insAssembly << "): branch taken: " << branchTaken << '\n';
  (this->*routine) (branchTaken);
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineDstRegSrcAdg (AnalysisRoutine routine,
    REG dstReg, const ConcreteValue &dstRegVal,
    const char *insAssembly) {
  if (disabled) {
    return;
  }
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineDstRegSrcAdg(INS: "
      << insAssembly << ") [AFTER execution of instruction]: dst reg: "
      << REG_StringShort (dstReg) << ", dst reg value: "
      << dstRegVal << '\n';
  edu::sharif::twinner::trace::Expression *srcexp =
      new edu::sharif::twinner::trace::ExpressionImp (dstRegVal);
  (this->*routine) (RegisterResidentExpressionValueProxy (dstReg, dstRegVal),
      ConstantExpressionValueProxy (srcexp));
  delete srcexp;
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineBeforeChangeOfReg (
    SuddenlyChangedRegAnalysisRoutine routine,
    REG reg,
    const char *insAssembly) {
  if (disabled) {
    return;
  }
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineBeforeChangeOfReg(INS: "
      << insAssembly << ")\n"
      "\tregistering register to be tracked...";
  trackedReg = reg;
  hook = routine;
  logger << "done\n";
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineTwoDstRegOneSrcReg (
    DoubleDestinationsAnalysisRoutine routine,
    REG dstLeftReg, const ConcreteValue &dstLeftRegVal,
    REG dstRightReg, const ConcreteValue &dstRightRegVal,
    REG srcReg, const ConcreteValue &srcRegVal,
    const char *insAssembly) {
  if (disabled) {
    return;
  }
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineTwoDstRegOneSrcReg(INS: "
      << insAssembly << "): left dst reg: " << REG_StringShort (dstLeftReg)
      << ", right dst reg: " << REG_StringShort (dstRightReg)
      << ", src reg: " << REG_StringShort (srcReg) << '\n';
  (this->*routine) (RegisterResidentExpressionValueProxy (dstLeftReg, dstLeftRegVal),
      RegisterResidentExpressionValueProxy (dstRightReg, dstRightRegVal),
      RegisterResidentExpressionValueProxy (srcReg, srcRegVal));
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineAfterOperandLessInstruction (
    OperandLessAnalysisRoutine routine,
    const CONTEXT *context,
    const char *insAssembly) {
  if (disabled) {
    return;
  }
  edu::sharif::twinner::util::Logger logger =
      edu::sharif::twinner::util::Logger::loquacious ();
  logger << "analysisRoutineAfterOperandLessInstruction(INS: "
      << insAssembly << "): [AFTER execution of instruction]: operand-less ins\n";
  (this->*routine) (context);
  logger << "Registers:\n";
  trace->printRegistersValues (logger);
}

void InstructionSymbolicExecuter::analysisRoutineRunHooks (const CONTEXT *context) {
  runHooks (context);
}

void InstructionSymbolicExecuter::runHooks (const CONTEXT *context) {
  if (trackedReg != REG_INVALID_) {
    ConcreteValue *value =
        edu::sharif::twinner::util::readRegisterContent (context, trackedReg);
    (this->*hook) (context, *value);
    delete value;
    trackedReg = REG_INVALID_;

  } else if (operandSize > 0) {
    (this->*hook) (context,
        edu::sharif::twinner::trace::ConcreteValue64Bits (operandSize));
    operandSize = -1;
  }
  hook = 0;
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

void InstructionSymbolicExecuter::movsxAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "movsxAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp = src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious () << "\tsetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.setExpressionWithoutChangeNotification (trace, srcexp);
  // src is either reg or mem. So src is mutable
  const int size = static_cast<const MutableExpressionValueProxy &> (src).getSize ();
  // size is at most 32 bits
  edu::sharif::twinner::trace::Expression *conditionExp = srcexp->clone ();
  conditionExp->minus (1ull << (size - 1));
  edu::sharif::twinner::trace::Constraint *cc;
  if (srcexp->getLastConcreteValue ().isNegative (size)) {
    edu::sharif::twinner::util::Logger::loquacious () << "\tdummy negative condition...";
    cc = new edu::sharif::twinner::trace::Constraint
        (conditionExp, edu::sharif::twinner::trace::Constraint::NON_NEGATIVE);
    edu::sharif::twinner::util::Logger::loquacious () << "\tbinary operations...";
    dstexp->truncate (size);
    dstexp->minus (1ull << size);
    dstexp->truncate (dst.getSize ());
  } else {
    edu::sharif::twinner::util::Logger::loquacious () << "\tdummy positive condition...";
    cc = new edu::sharif::twinner::trace::Constraint
        (conditionExp, edu::sharif::twinner::trace::Constraint::NEGATIVE);
  }
  delete conditionExp; // this is cloned by cc and is not required anymore
  trace->addPathConstraint (cc);
  dst.valueIsChanged (trace, dstexp);
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
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
      = eflags.instantiateConstraintForZeroFlag (!branchTaken);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tadding constraint...";
  trace->addPathConstraint (cc);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::jzAnalysisRoutine (bool branchTaken) {
  edu::sharif::twinner::util::Logger::loquacious () << "jzAnalysisRoutine(...)\n"
      << "\tinstantiating constraint...";
  edu::sharif::twinner::trace::Constraint *cc
      = eflags.instantiateConstraintForZeroFlag (branchTaken);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tadding constraint...";
  trace->addPathConstraint (cc);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::callAnalysisRoutine (const CONTEXT *context,
    const ConcreteValue &rspRegVal) {
  edu::sharif::twinner::util::Logger::loquacious () << "callAnalysisRoutine(...)\n"
      << "\tgetting rsp reg exp...";
  edu::sharif::twinner::trace::Expression *rsp =
      trace->tryToGetSymbolicExpressionByRegister (REG_RSP);
  if (rsp) { // If we are not tracking RSP yet, it's not required to adjust its value
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tadjusting rsp...";
    const ConcreteValue &oldVal = rsp->getLastConcreteValue ();
    if (oldVal > rspRegVal) {
      // some items have been pushed into stack by CALL and so RSP is decremented
      ConcreteValue *cv = oldVal.clone ();
      (*cv) -= rspRegVal;
      rsp->minus (cv);
      // TODO: call valueIsChanged from an expression proxy to address ESP, SP, and SPL

    } else {
      edu::sharif::twinner::util::Logger::warning ()
          << "RSP is not decremented at all after CALL instruction!\n";
    }
  }
  edu::sharif::twinner::util::Logger::loquacious () << "\tdone\n";
}

void InstructionSymbolicExecuter::retAnalysisRoutine (const CONTEXT *context,
    const ConcreteValue &rspRegVal) {
  edu::sharif::twinner::util::Logger::loquacious () << "retAnalysisRoutine(...)\n"
      << "\tgetting rsp reg exp...";
  edu::sharif::twinner::trace::Expression *rsp =
      trace->tryToGetSymbolicExpressionByRegister (REG_RSP);
  if (rsp) { // If we are not tracking RSP yet, it's not required to adjust its value
    edu::sharif::twinner::util::Logger::loquacious ()
        << "\tadjusting rsp...";
    const ConcreteValue &oldVal = rsp->getLastConcreteValue ();
    if (oldVal < rspRegVal) {
      // some items have been popped out from the stack by RET and so RSP is incremented
      ConcreteValue *cv = rspRegVal.clone ();
      (*cv) -= oldVal;
      rsp->add (cv);
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
    dstexp->shiftToLeft (srcexp->getLastConcreteValue ().clone ());
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

void InstructionSymbolicExecuter::shrAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "shrAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tshifting operation...";
  dstexp->shiftToRight (srcexp);
  dst.valueIsChanged (trace, dstexp);
  //TODO: set rflags
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::sarAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "sarAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tgetting dst exp...";
  edu::sharif::twinner::trace::Expression *dstexp =
      dst.getExpression (trace);
  const int size = dst.getSize ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tshifting operation...";
  dstexp->arithmeticShiftToRight (srcexp);
  {//TODO: Remove following code when all sizes have their own concrete value imps.
    const ConcreteValue &srcval = srcexp->getLastConcreteValue ();
    const ConcreteValue &dstval = dstexp->getLastConcreteValue ();
    ConcreteValue *tmp = dstval.clone ();
    ConcreteValue *sizeval = srcval.twosComplement ();
    (*sizeval) += size - 1;
    (*tmp) >>= (*sizeval);
    if ((*tmp) == 1) {
      (*tmp) <<= srcval;
      (*tmp) -= 1;
      (*sizeval) += 1;
      (*tmp) <<= (*sizeval);
      delete sizeval;
      (*tmp) |= dstval;
      dstexp->setLastConcreteValue (tmp);
    } else {
      delete sizeval;
      delete tmp;
    }
  }
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

void InstructionSymbolicExecuter::orAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "orAnalysisRoutine(...)\n"
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
       (edu::sharif::twinner::trace::Operator::BITWISE_OR), srcexp);
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

void InstructionSymbolicExecuter::testAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "testAnalysisRoutine(...)\n"
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
       (edu::sharif::twinner::trace::Operator::BITWISE_AND), srcexp);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting EFLAGS...";
  eflags.setFlags (tmpexp);
  // TODO: Inform eflags about CF and OF which must be zero.
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::pmovmskbAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "pmovmskbAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tpreparing mask-byte(src)...";
  edu::sharif::twinner::trace::Expression *maskbyte =
      new edu::sharif::twinner::trace::ExpressionImp (); // zero-filled
  // src is a reg and is mutable
  const int size = static_cast<const MutableExpressionValueProxy &> (src).getSize ();
  for (int i = 7, loc = 0; i < size; i += 8) {
    edu::sharif::twinner::trace::Expression *ithBit = srcexp->clone ();
    ithBit->shiftToRight (i - loc); // it is (i+1)-th bit in 1-counting mode
    ithBit->bitwiseAnd (1 << loc); // i-th bit in 0-counting
    loc++;
    maskbyte->bitwiseOr (ithBit);
    delete ithBit;
  }
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting dst exp...";
  dst.setExpression (trace, maskbyte);
  delete maskbyte;
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::bsfAnalysisRoutine (
    const MutableExpressionValueProxy &dst, const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "bsfAnalysisRoutine(...)\n"
      << "\tgetting src exp...";
  const edu::sharif::twinner::trace::Expression *srcexp =
      src.getExpression (trace);
  const edu::sharif::twinner::trace::ConcreteValue &cv = srcexp->getLastConcreteValue ();
  UINT64 i = 0;
  for (unsigned int s = cv.getSize (); i < s; ++i) {
    edu::sharif::twinner::trace::ConcreteValue *bit = cv.clone ();
    (*bit) >>= i;
    (*bit) &= 1;
    if ((*bit) == 1) {
      delete bit;
      break;
    }
    delete bit;
  }
  edu::sharif::twinner::trace::Expression *indexexp =
      new edu::sharif::twinner::trace::ExpressionImp (i);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tsetting dst exp...";
  dst.setExpression (trace, indexexp);
  delete indexexp;
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tadding constraint...";
  edu::sharif::twinner::trace::Expression *conditionExp = srcexp->clone ();
  conditionExp->shiftToLeft (i);
  conditionExp->bitwiseAnd (1);
  conditionExp->minus (1);
  edu::sharif::twinner::trace::Constraint *cc =
      new edu::sharif::twinner::trace::Constraint
      (conditionExp, edu::sharif::twinner::trace::Constraint::ZERO);
  delete conditionExp;
  trace->addPathConstraint (cc);
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
  operandSize = leftDst.getSize ();
  leftDstExp->shiftToLeft (operandSize);
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

  hook = &InstructionSymbolicExecuter::adjustDivisionMultiplicationOperands;
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::adjustDivisionMultiplicationOperands (
    const CONTEXT *context, const ConcreteValue &operandSize) {
  edu::sharif::twinner::util::Logger::loquacious ()
      << "adjustDivisionMultiplicationOperands(...) hook...";
  edu::sharif::twinner::trace::ConcreteValue64Bits os = operandSize;
  REG leftReg, rightReg;
  switch (os.getValue ()) {
  case 8:
    leftReg = REG_AH;
    rightReg = REG_AL;
  case 16:
    leftReg = REG_DX;
    rightReg = REG_AX;
    break;
  case 32:
    leftReg = REG_EDX;
    rightReg = REG_EAX;
    break;
  case 64:
    leftReg = REG_RDX;
    rightReg = REG_RAX;
    break;
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "adjustDivisionMultiplicationOperands(...) hook: "
        "unsupported operand size: " << operandSize << '\n';
    throw std::runtime_error
        ("Unsupported operand size in division/multiplication instruction");
  }
  ConcreteValue *leftVal =
      edu::sharif::twinner::util::readRegisterContent (context, leftReg);
  ConcreteValue *rightVal =
      edu::sharif::twinner::util::readRegisterContent (context, rightReg);
  edu::sharif::twinner::trace::Expression *leftExp =
      trace->getSymbolicExpressionByRegister (leftReg);
  edu::sharif::twinner::trace::Expression *rightExp =
      trace->getSymbolicExpressionByRegister (rightReg);
  leftExp->setLastConcreteValue (leftVal);
  rightExp->setLastConcreteValue (rightVal);
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tconcrete values are adjusted...";
  if (os.getValue () == 8) { // AX == AH:AL
    leftExp->shiftToLeft (8);
    leftExp->binaryOperation
        (new edu::sharif::twinner::trace::Operator
         (edu::sharif::twinner::trace::Operator::BITWISE_OR), rightExp);
    const MutableExpressionValueProxy &ax = RegisterResidentExpressionValueProxy (REG_AX);
    ax.setExpression (trace, leftExp); // this deletes unused expressions by itself
  } else {
    const MutableExpressionValueProxy &left =
        RegisterResidentExpressionValueProxy (leftReg, *leftVal);
    const MutableExpressionValueProxy &right =
        RegisterResidentExpressionValueProxy (rightReg, *rightVal);
    left.valueIsChanged (trace, leftExp);
    right.valueIsChanged (trace, rightExp);
  }
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\toverlapping registers are updated.\n";
}

void InstructionSymbolicExecuter::mulAnalysisRoutine (
    const MutableExpressionValueProxy &leftDst,
    const MutableExpressionValueProxy &rightDst,
    const ExpressionValueProxy &src) {
  edu::sharif::twinner::util::Logger::loquacious () << "mulAnalysisRoutine(...)\n"
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
  operandSize = leftDst.getSize ();
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tmultiplying (left-right = right * src; size=0x"
      << std::hex << operandSize << ")...";
  rightDstExp->binaryOperation
      (new edu::sharif::twinner::trace::Operator
       (edu::sharif::twinner::trace::Operator::MULTIPLY), srcexp);
  leftDstExp = leftDst.setExpressionWithoutChangeNotification (trace, rightDstExp);
  leftDstExp->shiftToRight (operandSize);
  rightDstExp->truncate (operandSize);
  // At this point, symbolic multiplication result is calculated correctly.
  // but concrete values are not! So we need to register a hook to synchronize concrete
  // values too (we can also calculate them in assembly, but it's not required).

  hook = &InstructionSymbolicExecuter::adjustDivisionMultiplicationOperands;
  edu::sharif::twinner::util::Logger::loquacious ()
      << "\tdone\n";
}

void InstructionSymbolicExecuter::rdtscAnalysisRoutine (const CONTEXT *context) {
  edu::sharif::twinner::util::Logger::loquacious () << "rdtscAnalysisRoutine(...)\n";
  /**
   * Now, we are right after the RDTSC instruction and time-stamp is loaded in
   * the edx:eax registers. These registers should be loaded as immediate values
   * in symbolic expressions.
   */
  ConcreteValue *edxVal =
      edu::sharif::twinner::util::readRegisterContent (context, REG_EDX);
  ConcreteValue *eaxVal =
      edu::sharif::twinner::util::readRegisterContent (context, REG_EAX);
  edu::sharif::twinner::trace::Expression *edxNewExp =
      new edu::sharif::twinner::trace::ExpressionImp (edxVal);
  edu::sharif::twinner::trace::Expression *eaxNewExp =
      new edu::sharif::twinner::trace::ExpressionImp (eaxVal);

  const MutableExpressionValueProxy &edx =
      RegisterResidentExpressionValueProxy (REG_EDX, *edxVal);
  const MutableExpressionValueProxy &eax =
      RegisterResidentExpressionValueProxy (REG_EAX, *eaxVal);
  edx.setExpression (trace, edxNewExp);
  eax.setExpression (trace, eaxNewExp);
  delete edxNewExp; // expressions are cloned by above setter methods
  delete eaxNewExp;
}

InstructionSymbolicExecuter::AnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToAnalysisRoutine (OPCODE op) const {
  switch (op) {
  case XED_ICLASS_MOV:
  case XED_ICLASS_MOVZX:
    return &InstructionSymbolicExecuter::movAnalysisRoutine;
  case XED_ICLASS_MOVSX:
  case XED_ICLASS_MOVSXD:
    return &InstructionSymbolicExecuter::movsxAnalysisRoutine;
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
  case XED_ICLASS_SHR:
    return &InstructionSymbolicExecuter::shrAnalysisRoutine;
  case XED_ICLASS_SAR:
    return &InstructionSymbolicExecuter::sarAnalysisRoutine;
  case XED_ICLASS_AND:
    return &InstructionSymbolicExecuter::andAnalysisRoutine;
  case XED_ICLASS_OR:
    return &InstructionSymbolicExecuter::orAnalysisRoutine;
  case XED_ICLASS_XOR:
    return &InstructionSymbolicExecuter::xorAnalysisRoutine;
  case XED_ICLASS_TEST:
    return &InstructionSymbolicExecuter::testAnalysisRoutine;
  case XED_ICLASS_PMOVMSKB:
    return &InstructionSymbolicExecuter::pmovmskbAnalysisRoutine;
  case XED_ICLASS_BSF:
    return &InstructionSymbolicExecuter::bsfAnalysisRoutine;
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
  case XED_ICLASS_MUL:
    return &InstructionSymbolicExecuter::mulAnalysisRoutine;
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
  case XED_ICLASS_JZ:
    return &InstructionSymbolicExecuter::jzAnalysisRoutine;
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

InstructionSymbolicExecuter::OperandLessAnalysisRoutine
InstructionSymbolicExecuter::convertOpcodeToOperandLessAnalysisRoutine (
    OPCODE op) const {
  switch (op) {
  case XED_ICLASS_RDTSC:
    return &InstructionSymbolicExecuter::rdtscAnalysisRoutine;
  default:
    edu::sharif::twinner::util::Logger::error () << "Analysis routine: "
        "Operand Less: Unknown opcode: " << OPCODE_StringShort (op) << '\n';
    throw std::runtime_error
        ("Unknown opcode (for operand-less) given to analysis routine");
  }
}

VOID analysisRoutineDstRegSrcReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  const char *insAssemblyStr = get_pointer_to_allocated_memory (insAssembly);
  ise->analysisRoutineDstRegSrcReg
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::ConcreteValue64Bits (dstRegVal),
       (REG) srcReg, edu::sharif::twinner::trace::ConcreteValue64Bits (srcRegVal),
       insAssemblyStr);
}

VOID analysisRoutineDstRegSrcLargeReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 srcReg, const PIN_REGISTER *srcRegVal,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  const char *insAssemblyStr = get_pointer_to_allocated_memory (insAssembly);
  ise->analysisRoutineDstRegSrcReg
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::ConcreteValue64Bits (dstRegVal),
       (REG) srcReg, edu::sharif::twinner::trace::ConcreteValue128Bits (*srcRegVal),
       insAssemblyStr);
}

VOID analysisRoutineDstRegSrcMem (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  const char *insAssemblyStr = get_pointer_to_allocated_memory (insAssembly);
  ise->analysisRoutineDstRegSrcMem
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::ConcreteValue64Bits (dstRegVal),
       srcMemoryEa, memReadBytes,
       insAssemblyStr);
}

VOID analysisRoutineDstRegSrcImd (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    ADDRINT srcImmediateValue,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  const char *insAssemblyStr = get_pointer_to_allocated_memory (insAssembly);
  ise->analysisRoutineDstRegSrcImd
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::ConcreteValue64Bits (dstRegVal),
       edu::sharif::twinner::trace::ConcreteValue64Bits (srcImmediateValue),
       insAssemblyStr);
}

VOID analysisRoutineDstMemSrcReg (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  const char *insAssemblyStr = get_pointer_to_allocated_memory (insAssembly);
  ise->analysisRoutineDstMemSrcReg
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       dstMemoryEa,
       (REG) srcReg, edu::sharif::twinner::trace::ConcreteValue64Bits (srcRegVal),
       memReadBytes,
       insAssemblyStr);
}

VOID analysisRoutineDstMemSrcImd (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcImmediateValue,
    UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  const char *insAssemblyStr = get_pointer_to_allocated_memory (insAssembly);
  ise->analysisRoutineDstMemSrcImd
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       dstMemoryEa,
       edu::sharif::twinner::trace::ConcreteValue64Bits (srcImmediateValue),
       memReadBytes,
       insAssemblyStr);
}

VOID analysisRoutineDstMemSrcMem (VOID *iseptr, UINT32 opcode,
    ADDRINT dstMemoryEa,
    ADDRINT srcMemoryEa, UINT32 memReadBytes,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  const char *insAssemblyStr = get_pointer_to_allocated_memory (insAssembly);
  ise->analysisRoutineDstMemSrcMem
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       dstMemoryEa,
       srcMemoryEa, memReadBytes,
       insAssemblyStr);
}

VOID analysisRoutineConditionalBranch (VOID *iseptr, UINT32 opcode,
    BOOL branchTaken,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  const char *insAssemblyStr = get_pointer_to_allocated_memory (insAssembly);
  ise->analysisRoutineConditionalBranch
      (ise->convertOpcodeToConditionalBranchAnalysisRoutine ((OPCODE) opcode),
       branchTaken,
       insAssemblyStr);
}

VOID analysisRoutineDstRegSrcAdg (VOID *iseptr, UINT32 opcode,
    UINT32 dstReg, ADDRINT dstRegVal,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  const char *insAssemblyStr = get_pointer_to_allocated_memory (insAssembly);
  ise->analysisRoutineDstRegSrcAdg
      (ise->convertOpcodeToAnalysisRoutine ((OPCODE) opcode),
       (REG) dstReg, edu::sharif::twinner::trace::ConcreteValue64Bits (dstRegVal),
       insAssemblyStr);
}

VOID analysisRoutineBeforeChangeOfReg (VOID *iseptr, UINT32 opcode,
    UINT32 reg,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  const char *insAssemblyStr = get_pointer_to_allocated_memory (insAssembly);
  ise->analysisRoutineBeforeChangeOfReg
      (ise->convertOpcodeToSuddenlyChangedRegAnalysisRoutine ((OPCODE) opcode),
       (REG) reg,
       insAssemblyStr);
}

VOID analysisRoutineTwoDstRegOneSrcReg (VOID *iseptr, UINT32 opcode,
    UINT32 dstLeftReg, ADDRINT dstLeftRegVal,
    UINT32 dstRightReg, ADDRINT dstRightRegVal,
    UINT32 srcReg, ADDRINT srcRegVal,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  const char *insAssemblyStr = get_pointer_to_allocated_memory (insAssembly);
  ise->analysisRoutineTwoDstRegOneSrcReg
      (ise->convertOpcodeToDoubleDestinationsAnalysisRoutine ((OPCODE) opcode),
       (REG) dstLeftReg,
       edu::sharif::twinner::trace::ConcreteValue64Bits (dstLeftRegVal),
       (REG) dstRightReg,
       edu::sharif::twinner::trace::ConcreteValue64Bits (dstRightRegVal),
       (REG) srcReg,
       edu::sharif::twinner::trace::ConcreteValue64Bits (srcRegVal),
       insAssemblyStr);
}

VOID analysisRoutineAfterOperandLess (VOID *iseptr, UINT32 opcode,
    const CONTEXT *context,
    UINT32 insAssembly) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  const char *insAssemblyStr = get_pointer_to_allocated_memory (insAssembly);
  ise->analysisRoutineAfterOperandLessInstruction
      (ise->convertOpcodeToOperandLessAnalysisRoutine ((OPCODE) opcode),
       context,
       insAssemblyStr);
}

VOID analysisRoutineRunHooks (VOID *iseptr, const CONTEXT *context) {
  InstructionSymbolicExecuter *ise = (InstructionSymbolicExecuter *) iseptr;
  ise->analysisRoutineRunHooks (context);
}

}
}
}
}
