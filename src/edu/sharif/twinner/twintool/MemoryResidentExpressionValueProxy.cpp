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

#include <stdexcept>

#include "MemoryResidentExpressionValueProxy.h"

#include "InstructionSymbolicExecuter.h"

#include "edu/sharif/twinner/trace/Trace.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

MemoryResidentExpressionValueProxy::MemoryResidentExpressionValueProxy (
    ADDRINT _memoryEa, int _memReadBytes) :
memoryEa (_memoryEa), memReadBytes (_memReadBytes) {
}

edu::sharif::twinner::trace::Expression *
MemoryResidentExpressionValueProxy::getExpression (
    edu::sharif::twinner::trace::Trace *trace) const {
  if (memReadBytes < 0) {
    throw std::runtime_error
        ("For getting an expression from memory, "
         "memReadBytes must be provided to the constructor of expression proxy class.");
  }
  UINT64 val = InstructionSymbolicExecuter::readMemoryContent (memoryEa);
  switch (memReadBytes) {
  case 8:
    break; // no cast is required in 64-bits mode
  case 4:
    val = (UINT32) val;
    break;
  case 2:
    val = (UINT16) val;
    break;
  case 1:
    val = (UINT8) val;
    break;
  default:
    throw std::runtime_error ("Invalid mem read bytes size in expression proxy class");
  }
  return trace->getSymbolicExpressionByMemoryAddress (memoryEa, val);
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
  edu::sharif::twinner::util::Logger::loquacious () << "(memory value is changed to "
      << changedExp << ")\n";
  // TODO: implement
}

}
}
}
}
