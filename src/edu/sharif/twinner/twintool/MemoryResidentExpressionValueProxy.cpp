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

#include <stdexcept>

#include "MemoryResidentExpressionValueProxy.h"

#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/ConcreteValue128Bits.h"
#include "edu/sharif/twinner/trace-twintool/ExpressionImp.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/memory.h"

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
  if (!isMemoryEaAligned ()) {
    //TODO: Get two aligned exp and merge them in response
    throw std::runtime_error ("Unaligned memory read is not implemented");
  }
  edu::sharif::twinner::trace::ConcreteValue *cv;
  if (memReadBytes == 16) {
    const UINT64 cvlsb = edu::sharif::twinner::util::readMemoryContent (memoryEa);
    const UINT64 cvmsb = edu::sharif::twinner::util::readMemoryContent (memoryEa + 8);
    cv = new edu::sharif::twinner::trace::ConcreteValue128Bits (cvmsb, cvlsb);
  } else {
    const UINT64 cvval = edu::sharif::twinner::util::readMemoryContent (memoryEa);
    cv = edu::sharif::twinner::trace::ConcreteValue64Bits (cvval).clone (getSize ());
  }
  edu::sharif::twinner::trace::Expression *exp =
      trace->getSymbolicExpressionByMemoryAddress (getSize (), memoryEa, *cv)->clone ();
  delete cv;
  if (memReadBytes < 4) { // FIXME: Remove after supporting 16/8-bits concrete values
    exp->truncate (memReadBytes * 8);
  }
  return exp;
}

edu::sharif::twinner::trace::Expression
MemoryResidentExpressionValueProxy::setExpressionWithoutChangeNotification (
    edu::sharif::twinner::trace::Trace *trace,
    const edu::sharif::twinner::trace::Expression *exp) const {
  if (isMemoryEaAligned ()) {
    edu::sharif::twinner::trace::Expression *exp =
        trace->setSymbolicExpressionByMemoryAddress (getSize (), memoryEa, exp);
    truncate (exp);
    return *exp;
  } else {
    //TODO: Divide exp into two expressions and set them at two aligned addresses
    throw std::runtime_error ("Unaligned memory write is not implemented");
  }
}

bool MemoryResidentExpressionValueProxy::isMemoryEaAligned () const {
  return (memoryEa % memReadBytes) == 0;
}

void MemoryResidentExpressionValueProxy::valueIsChanged (
    edu::sharif::twinner::trace::Trace *trace,
    const edu::sharif::twinner::trace::Expression &changedExp) const {
  edu::sharif::twinner::util::Logger::loquacious () << "(memory value is changed to "
      << changedExp << ")\n";
  // ASSERT: changedExp was returned from setExpressionWithoutChangeNotification () method
  if (isMemoryEaAligned ()) {
    propagateChangeUpwards (getSize (), memoryEa, trace, changedExp);
    propagateChangeDownwards (getSize (), memoryEa, trace, changedExp);
  } else {
    throw std::runtime_error ("Unaligned memory write is not implemented");
  }
}

void MemoryResidentExpressionValueProxy::propagateChangeDownwards (int size,
    ADDRINT memoryEa, edu::sharif::twinner::trace::Trace *trace,
    const edu::sharif::twinner::trace::Expression &changedExp) const {
  size /= 2;
  if (size >= 32) {
    edu::sharif::twinner::trace::Expression *exp = changedExp.clone ();
    exp->truncate (size);
    trace->setSymbolicExpressionByMemoryAddress (size, memoryEa + size / 8, exp);
    propagateChangeDownwards (size, memoryEa + size / 8, exp);
    delete exp;
    exp = changedExp.clone ();
    exp->shiftToRight (size);
    trace->setSymbolicExpressionByMemoryAddress (size, memoryEa, exp);
    propagateChangeDownwards (size, memoryEa, exp);
    delete exp;
  }
}

void MemoryResidentExpressionValueProxy::propagateChangeUpwards (int size,
    ADDRINT memoryEa, edu::sharif::twinner::trace::Trace *trace,
    const edu::sharif::twinner::trace::Expression &changedExp) const {
  if (size <= 64) {
    edu::sharif::twinner::trace::Expression *exp;
    if (memoryEa % (size / 4) == 0) { // (2*size)-bits aligned
      const UINT64 cv =
          edu::sharif::twinner::util::readMemoryContent (memoryEa + size / 8);
      edu::sharif::twinner::trace::ConcreteValue *cvObj =
          edu::sharif::twinner::trace::ConcreteValue64Bits (cv).clone (size);
      const edu::sharif::twinner::trace::Expression *neighbor =
          trace->getSymbolicExpressionByMemoryAddress (size, memoryEa + size / 8, *cvObj);
      delete cvObj;
      exp = changedExp.clone (2 * size);
      exp->shiftToRight (size);
      exp->bitwiseOr (neighbor); // neighbor will be cloned internally
    } else {
      const UINT64 cv =
          edu::sharif::twinner::util::readMemoryContent (memoryEa - size / 8);
      edu::sharif::twinner::trace::ConcreteValue *cvObj =
          edu::sharif::twinner::trace::ConcreteValue64Bits (cv).clone (size);
      const edu::sharif::twinner::trace::Expression *neighbor =
          trace->getSymbolicExpressionByMemoryAddress (size, memoryEa - size / 8, *cvObj);
      delete cvObj;
      exp = neighbor.clone (2 * size);
      exp->shiftToRight (size);
      exp->bitwiseOr (&changedExp); // changedExp will be cloned internally
      memoryEa -= size / 8;
    }
    size *= 2;
    trace->setSymbolicExpressionByMemoryAddress (size, memoryEa, exp);
    propagateChangeUpwards (size, memoryEa, trace, *exp);
    delete exp;
  }
}

int MemoryResidentExpressionValueProxy::getSize () const {
  if (memReadBytes < 0) {
    throw std::runtime_error
        ("memReadBytes is not provided to the constructor of expression proxy class.");
  }
  return memReadBytes * 8;
}

}
}
}
}
