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
  if (memReadBytes == 16) {
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
  } else if (0 <= memReadBytes && memReadBytes <= 8) {
    const UINT64 cv = edu::sharif::twinner::util::readMemoryContent (memoryEa);
    exp = trace->getSymbolicExpressionByMemoryAddress
        (memoryEa, edu::sharif::twinner::trace::ConcreteValue64Bits (cv))->clone ();
    if (memReadBytes != 8) { // < 8
      exp->truncate (memReadBytes * 8);
    }
  } else {
    throw std::runtime_error
        ("For getting an expression from memory, "
         "memReadBytes must be provided to the constructor of expression proxy class.");
  }
  return exp;
}

edu::sharif::twinner::trace::Expression
MemoryResidentExpressionValueProxy::setExpressionWithoutChangeNotification (
    edu::sharif::twinner::trace::Trace *trace,
    const edu::sharif::twinner::trace::Expression *exp) const {
  if (isMemoryEaAligned ()) {
    switch (memReadBytes) {
    case 16:
      return *trace->setSymbolic128BitsExpressionByMemoryAddress (memoryEa, exp);
    case 8:
      return *trace->setSymbolic64BitsExpressionByMemoryAddress (memoryEa, exp);
    case 4:
      return *trace->setSymbolic32BitsExpressionByMemoryAddress (memoryEa, exp);
    default:
      throw std::runtime_error ("Memory setting size is not supported");
    }
  } else {
    //TODO: Divide exp into two expressions and set them at two aligned addresses
    throw std::runtime_error ("Unaligned memory write is not implemented");
  }
  /*
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
   */
}

bool MemoryResidentExpressionValueProxy::isMemoryEaAligned () const {
  return (memoryEa % memReadBytes) == 0;
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
