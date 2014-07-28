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
  edu::sharif::twinner::trace::Expression *exp;
  if (memReadBytes < 0) {
    throw std::runtime_error
        ("For getting an expression from memory, "
         "memReadBytes must be provided to the constructor of expression proxy class.");
  } else if (memReadBytes > 8) {
    const UINT64 cvlsb = edu::sharif::twinner::util::readMemoryContent (memoryEa);
    const UINT64 cvmsb = edu::sharif::twinner::util::readMemoryContent (memoryEa + 8);
    const edu::sharif::twinner::trace::Expression *explsb =
        trace->getSymbolicExpressionByMemoryAddress
        (memoryEa,
         edu::sharif::twinner::trace::ConcreteValue64Bits (cvlsb));
    exp = trace->getSymbolicExpressionByMemoryAddress
        (memoryEa + 8,
         edu::sharif::twinner::trace::ConcreteValue64Bits (cvmsb))->clone ();

    exp->shiftToLeft (64);
    exp->bitwiseOr (explsb);
    exp->setLastConcreteValue
        (new edu::sharif::twinner::trace::ConcreteValue128Bits (cvmsb, cvlsb));
  } else {
    const UINT64 cv = edu::sharif::twinner::util::readMemoryContent (memoryEa);
    exp = trace->getSymbolicExpressionByMemoryAddress
        (memoryEa, edu::sharif::twinner::trace::ConcreteValue64Bits (cv))->clone ();
    if (memReadBytes != 8) { // < 8
      exp->truncate (memReadBytes * 8);
    }
  }
  return exp;
}

edu::sharif::twinner::trace::Expression *
MemoryResidentExpressionValueProxy::setExpressionWithoutChangeNotification (
    edu::sharif::twinner::trace::Trace *trace,
    const edu::sharif::twinner::trace::Expression *exp) const {
  if (memReadBytes > 8) {
    edu::sharif::twinner::trace::Expression *expmsb = exp->clone ();
    edu::sharif::twinner::trace::Expression *explsb = exp->clone ();
    expmsb->shiftToRight (64);
    explsb->truncate (64);
    trace->setSymbolicExpressionByMemoryAddress (memoryEa, explsb);
    trace->setSymbolicExpressionByMemoryAddress (memoryEa + 8, expmsb);
    delete expmsb;
    delete explsb;
    return 0; // FIXME: Act like registers (and keep multiple maps to be faster)

  } else if (memReadBytes == 8) {
    return trace->setSymbolicExpressionByMemoryAddress (memoryEa, exp);
  } else {
    const UINT64 cv = edu::sharif::twinner::util::readMemoryContent (memoryEa);
    edu::sharif::twinner::trace::Expression *newexp =
        new edu::sharif::twinner::trace::ExpressionImp
        (memoryEa, edu::sharif::twinner::trace::ConcreteValue64Bits (cv),
         trace->getCurrentSegmentIndex ());
    edu::sharif::twinner::trace::Expression *bigexp =
        trace->getSymbolicExpressionByMemoryAddress (memoryEa, newexp);
    if (bigexp != newexp) {
      delete newexp;
    }
    edu::sharif::twinner::trace::Expression *truncatedExp = exp->clone ();
    truncatedExp->truncate (memReadBytes * 8);
    const UINT64 mask = ~((1ull << (memReadBytes * 8)) - 1);
    bigexp->bitwiseAnd (mask);
    bigexp->bitwiseOr (truncatedExp);
    delete truncatedExp;
    return bigexp;
  }
}

void MemoryResidentExpressionValueProxy::valueIsChanged (
    edu::sharif::twinner::trace::Trace *trace,
    edu::sharif::twinner::trace::Expression *changedExp) const {
  edu::sharif::twinner::util::Logger::loquacious () << "(memory value is changed to "
      << changedExp << ")\n";
  // TODO: implement
  // NOTE: Before implementing this function, the FIXME in
  // the setExpressionWithoutChangeNotification method must be resolved.
}

void MemoryResidentExpressionValueProxy::truncate (
    edu::sharif::twinner::trace::Expression *exp) const {
  exp->truncate (getSize ());
}

int MemoryResidentExpressionValueProxy::getSize () const {
  return memReadBytes * 8;
}

}
}
}
}
