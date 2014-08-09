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
    /**
     * Example for 64-bits (each character is showing one byte):
     * Memory state:           0 1 2 3 4 5 6 7 8 9 a b c d e f
     *                               X Y Z W P Q R T
     * Expected expression (little-endian): T R Q P W Z Y X
     * reading two 64-bits from memory:    P W Z Y X 2 1 0      f e d c b T R Q
     * After shift & truncation:           - - - P W Z Y X      T R Q - - - - -
     * After bitwise or:                   T R Q P W Z Y X
     */
    MemoryResidentExpressionValueProxy leftProxy
        (memoryEa - (memoryEa % memReadBytes), memReadBytes);
    MemoryResidentExpressionValueProxy rightProxy
        (memoryEa - (memoryEa % memReadBytes) + memReadBytes, memReadBytes);
    edu::sharif::twinner::trace::Expression *leftExp = leftProxy.getExpression (trace);
    edu::sharif::twinner::trace::Expression *rightExp = rightProxy.getExpression (trace);
    leftExp->shiftToRight (8 * (memoryEa % memReadBytes));
    rightExp->shiftToLeft (8 * (8 - (memoryEa % memReadBytes)));
    rightExp->truncate (memReadBytes * 8);
    leftExp->bitwiseOr (rightExp);
    delete rightExp;
    return leftExp;
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
  return exp;
}

edu::sharif::twinner::trace::Expression
MemoryResidentExpressionValueProxy::setExpressionWithoutChangeNotification (
    edu::sharif::twinner::trace::Trace *trace,
    const edu::sharif::twinner::trace::Expression *exp) const {
  if (isMemoryEaAligned ()) {
    edu::sharif::twinner::trace::Expression *newExp =
        trace->setSymbolicExpressionByMemoryAddress (getSize (), memoryEa, exp);
    truncate (newExp);
    return *newExp;
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
      << &changedExp << ")\n";
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
    exp->truncate (size); // LSB (left-side in little-endian)
    trace->setSymbolicExpressionByMemoryAddress (size, memoryEa, exp);
    propagateChangeDownwards (size, memoryEa, trace, *exp);
    delete exp;
    exp = changedExp.clone ();
    exp->shiftToRight (size); // MSB (right-side in little-endian)
    trace->setSymbolicExpressionByMemoryAddress (size, memoryEa + size / 8, exp);
    propagateChangeDownwards (size, memoryEa + size / 8, trace, *exp);
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
      exp = neighbor->clone (2 * size); // MSB
      exp->shiftToLeft (size);
      exp->bitwiseOr (&changedExp); // changedExp will be cloned internally
      //XXX: Following downwards check is only necessary when neighbor is newly created
      propagateChangeDownwards (size, memoryEa + size / 8, trace, *neighbor);
    } else { // changedExp is right-side (i.e. MSB in little-endian)
      const UINT64 cv =
          edu::sharif::twinner::util::readMemoryContent (memoryEa - size / 8);
      edu::sharif::twinner::trace::ConcreteValue *cvObj =
          edu::sharif::twinner::trace::ConcreteValue64Bits (cv).clone (size);
      const edu::sharif::twinner::trace::Expression *neighbor =
          trace->getSymbolicExpressionByMemoryAddress (size, memoryEa - size / 8, *cvObj);
      delete cvObj;
      exp = changedExp.clone (2 * size); // MSB
      exp->shiftToLeft (size);
      exp->bitwiseOr (neighbor); // neighbor will be cloned internally
      memoryEa -= size / 8;
      //XXX: Following downwards check is only necessary when neighbor is newly created
      propagateChangeDownwards (size, memoryEa, trace, *neighbor);
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
