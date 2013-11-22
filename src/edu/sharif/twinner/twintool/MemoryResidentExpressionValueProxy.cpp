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
#include "edu/sharif/twinner/trace/Expression.h"

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
  val = InstructionSymbolicExecuter::truncateValue (val, memReadBytes);

  return trace->getSymbolicExpressionByMemoryAddress (memoryEa, val);
}

edu::sharif::twinner::trace::Expression *
MemoryResidentExpressionValueProxy::setExpressionWithoutChangeNotification (
    edu::sharif::twinner::trace::Trace *trace,
    const edu::sharif::twinner::trace::Expression *exp) const {
  return trace->setSymbolicExpressionByMemoryAddress (memoryEa, exp);
}

void MemoryResidentExpressionValueProxy::valueIsChanged (
    edu::sharif::twinner::trace::Trace *trace,
    edu::sharif::twinner::trace::Expression *changedExp) const {
  edu::sharif::twinner::util::Logger::loquacious () << "(memory value is changed to "
      << changedExp << ")\n";
  // TODO: implement
}

void MemoryResidentExpressionValueProxy::truncate (
    edu::sharif::twinner::trace::Expression *exp) const {
  exp->truncate (memReadBytes * 8);
}

}
}
}
}
