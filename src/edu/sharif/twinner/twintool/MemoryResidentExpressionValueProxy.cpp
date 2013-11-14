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

#include "MemoryResidentExpressionValueProxy.h"

#include "InstructionSymbolicExecuter.h"

#include "edu/sharif/twinner/trace/Trace.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

MemoryResidentExpressionValueProxy::MemoryResidentExpressionValueProxy (
    ADDRINT _memoryEa) :
memoryEa (_memoryEa) {
}

edu::sharif::twinner::trace::Expression *
MemoryResidentExpressionValueProxy::getExpression (
    edu::sharif::twinner::trace::Trace *trace) const {
  return trace->getSymbolicExpressionByMemoryAddress
      (memoryEa, InstructionSymbolicExecuter::readMemoryContent (memoryEa));
}

void MemoryResidentExpressionValueProxy::setExpression (
    edu::sharif::twinner::trace::Trace *trace,
    const edu::sharif::twinner::trace::Expression *exp) const {
  edu::sharif::twinner::trace::Expression *newExp =
      trace->setSymbolicExpressionByMemoryAddress (memoryEa, exp);
  valueIsChanged (trace, newExp);
}

void MemoryResidentExpressionValueProxy::valueIsChanged (
    edu::sharif::twinner::trace::Trace *trace,
    edu::sharif::twinner::trace::Expression *changedExp) const {
  // TODO: implement
}

}
}
}
}
