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

#include "FunctionArgumentInfoImp.h"


#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/StateSummary.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"

#include "edu/sharif/twinner/proxy/RegisterResidentExpressionValueProxy.h"
#include "edu/sharif/twinner/proxy/MemoryResidentExpressionValueProxy.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

#ifdef TARGET_IA32E
static const int STACK_OPERATION_UNIT_SIZE = 8; // bytes
#else
static const int STACK_OPERATION_UNIT_SIZE = 4; // bytes
#endif

FunctionArgumentInfoImp::FunctionArgumentInfoImp (std::string _typeStr,
    bool _requiresStringResolution,
    Expression *_exp,
    std::string _expAsString) :
    FunctionArgumentInfo (_typeStr, _requiresStringResolution,
    _exp, _expAsString) {
}

FunctionArgumentInfoImp::FunctionArgumentInfoImp (std::string _typeStr,
    bool _requiresStringResolution) :
    FunctionArgumentInfo (_typeStr, _requiresStringResolution) {
}

FunctionArgumentInfoImp::~FunctionArgumentInfoImp () {
}

FunctionArgumentInfo *FunctionArgumentInfoImp::resolve (int i, Trace *trace,
    const CONTEXT *context) const {
  Expression *exp = getArgument (i, trace, context);
  std::string expAsString = "";
  if (requiresStringResolution) {
    const ADDRINT stringPointer = exp->getLastConcreteValue ().toUint64 ();
    if (!edu::sharif::twinner::util::readStringFromMemory
        (expAsString, stringPointer)) {
      edu::sharif::twinner::util::Logger::warning ()
          << "Argument #" << std::dec << i
          << " with type ``" << type << "''"
          " cannot be read as a C string";
      expAsString = "";
    }
  }
  return new FunctionArgumentInfoImp (type, requiresStringResolution,
                                      exp, expAsString);
}

FunctionArgumentInfoImp *FunctionArgumentInfoImp::clone () const {
  return new FunctionArgumentInfoImp (type,
                                      requiresStringResolution,
                                      (exp == 0 ? 0 : exp->clone ()),
                                      expAsString);
}

Expression *FunctionArgumentInfoImp::getArgument (int i, Trace *trace,
    const CONTEXT *context) const {
#if defined(TARGET_IA32E) && defined(TARGET_LINUX)
  // args are in RDI, RSI, RDX, RCX, R8, R9, and in the stack respectively
  if (i < 6) {
    const REG regs[] = {
      REG_RDI, REG_RSI, REG_RDX, REG_RCX, REG_R8, REG_R9
    };
    return getArgument (regs[i], trace, context);
  }
  const int offset = (i - 6) * STACK_OPERATION_UNIT_SIZE;
  const ADDRINT topOfStack = PIN_GetContextReg (context, REG_RSP);
#elif defined(TARGET_IA32)
  // args are in the stack respectively (first argument is pushed last)
  const int offset = i * STACK_OPERATION_UNIT_SIZE;
  const ADDRINT topOfStack = PIN_GetContextReg (context, REG_ESP);
#else
#error "Unsupported architecture and/or OS"
#endif
  return getArgument (offset, topOfStack, trace, context);
}
#ifdef TARGET_IA32E

Expression *FunctionArgumentInfoImp::getArgument (REG reg, Trace *trace,
    const CONTEXT *context) const {
  edu::sharif::twinner::proxy::RegisterResidentExpressionValueProxy proxy
      (reg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits
       (PIN_GetContextReg (context, reg)));
  edu::sharif::twinner::trace::StateSummary state;
  edu::sharif::twinner::trace::Expression *exp =
      proxy.getExpression (trace, state);
  if (state.isWrongState ()) {
    edu::sharif::twinner::util::Logger::error () << state.getMessage () << '\n';
    abort ();
  }
  return exp;
}
#endif

Expression *FunctionArgumentInfoImp::getArgument (int offset, ADDRINT topOfStack,
    Trace *trace, const CONTEXT *context) const {
  edu::sharif::twinner::proxy::MemoryResidentExpressionValueProxy proxy
      (topOfStack + offset, STACK_OPERATION_UNIT_SIZE);
  edu::sharif::twinner::trace::StateSummary state;
  edu::sharif::twinner::trace::Expression *exp =
      proxy.getExpression (trace, state);
  if (state.isWrongState ()) {
    edu::sharif::twinner::util::Logger::error () << state.getMessage () << '\n';
    abort ();
  }
  return exp;
}


}
}
}
}
