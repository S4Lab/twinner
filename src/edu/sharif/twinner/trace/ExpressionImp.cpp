//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2018 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "ExpressionImp.h"

#include "edu/sharif/twinner/trace/exptoken/RegisterEmergedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/MemoryEmergedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/Constant.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue128Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "MarInfo.h"
#include "Snapshot.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

ExpressionImp::ExpressionImp (const ExpressionImp &exp) :
    Expression (exp) {
}

ExpressionImp::ExpressionImp (REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue,
    int generationIndex, int snapshotIndex) :
    Expression (concreteValue.clone (), false) {
  if (concreteValue.getSize () < 64) {
    /*
     * The reg and concreteValue must always have the same precision but when the reg is
     * promoted to its enclosing register by caller.
     * Also this constructor should be called with 128 or 64 bits regs and so smaller
     * concrete values is a sign of reg promotion by the caller.
     * And as no reg promotion is implemented for 128 bits registers, the reg have to
     * be a 64 bits register.
     */
    const edu::sharif::twinner::trace::cv::ConcreteValue64Bits cv (concreteValue);
    stack.push_back (new edu::sharif::twinner::trace::exptoken::RegisterEmergedSymbol
                     (reg, cv, generationIndex, snapshotIndex));
  } else {
    stack.push_back (new edu::sharif::twinner::trace::exptoken::RegisterEmergedSymbol
                     (reg, concreteValue, generationIndex, snapshotIndex));
  }
}

edu::sharif::twinner::trace::exptoken::ExpressionToken *
ExpressionImp::instantiateMemorySymbol (ADDRINT memoryEa,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue,
    int generationIndex, bool isOverwriting, int snapshotIndex) const {
  if (isOverwriting) {
    return new edu::sharif::twinner::trace::exptoken::MemoryEmergedSymbol
        (memoryEa, concreteValue, generationIndex, snapshotIndex);
  } else {
    const int size = concreteValue.getSize () / 8;
    UINT64 currentConcreteValue = 0;
    if (!edu::sharif::twinner::util::readMemoryContent
        (currentConcreteValue, memoryEa, size)) {
      edu::sharif::twinner::util::Logger::warning ()
          << "memory is not readable;"
          " address: 0x" << std::hex << memoryEa << '\n';
      abort ();
    }
    if (edu::sharif::twinner::util::writeMemoryContent
        (memoryEa, (const UINT8 *) &currentConcreteValue, size)) {
      return new edu::sharif::twinner::trace::exptoken::MemoryEmergedSymbol
          (memoryEa, concreteValue, generationIndex, snapshotIndex);
    } else {
      edu::sharif::twinner::util::Logger::warning ()
          << "memory is not writable;"
          " address: 0x" << std::hex << memoryEa << '\n';
      return new edu::sharif::twinner::trace::exptoken::Constant (concreteValue);
    }
  }
}

ExpressionImp::ExpressionImp (ADDRINT memoryEa,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue,
    int generationIndex, bool isOverwriting, int snapshotIndex) :
    Expression (concreteValue.clone (), isOverwriting) {
  if (!isOverwriting && snapshotIndex == -1) {
    //    if (memoryEa < 0x1f0000) { // 32 bits
    //    if (memoryEa < 0x7f0000000000ull) { // 64 bits
    const ADDRINT argv = ADDRINT (MarInfo::getInitialArgv ());
    if (memoryEa < ((argv >> 12) << 12)) { // FIXME: Generalize this code
      // this temporary code assumes that everything out of stack (including heap) is constant
      stack.push_back (new edu::sharif::twinner::trace::exptoken::Constant (concreteValue));
      return;
    }
  }
  if (concreteValue.getSize () == 128) {
    const edu::sharif::twinner::trace::cv::ConcreteValue128Bits *cv =
        static_cast<const edu::sharif::twinner::trace::cv::ConcreteValue128Bits *>
        (&concreteValue);
    stack.push_back (instantiateMemorySymbol
                     (memoryEa + 8, // little endian
                      edu::sharif::twinner::trace::cv::ConcreteValue64Bits (cv->getMsb ()),
                      generationIndex, isOverwriting, snapshotIndex));
    stack.push_back (new edu::sharif::twinner::trace::exptoken::Constant (64));
    stack.push_back (Operator::instantiateOperator (Operator::SHIFT_LEFT));
    stack.push_back
        (instantiateMemorySymbol
         (memoryEa,
          edu::sharif::twinner::trace::cv::ConcreteValue64Bits (cv->getLsb ()),
          generationIndex, isOverwriting, snapshotIndex));
    stack.push_back (Operator::instantiateOperator (Operator::BITWISE_OR));
  } else {
    stack.push_back (instantiateMemorySymbol
                     (memoryEa, concreteValue, generationIndex, isOverwriting,
                      snapshotIndex));
  }
}

ExpressionImp::ExpressionImp (edu::sharif::twinner::trace::exptoken::Symbol *symbol) :
    Expression (symbol->getValue ().clone (), false) {
  stack.push_back (symbol);
}

ExpressionImp::ExpressionImp (
    const edu::sharif::twinner::trace::cv::ConcreteValue &value) :
    Expression (value.clone (), false) {
  stack.push_back (new edu::sharif::twinner::trace::exptoken::Constant (value));
}

ExpressionImp::ExpressionImp (edu::sharif::twinner::trace::cv::ConcreteValue *value) :
    Expression (value, false) {
  stack.push_back (new edu::sharif::twinner::trace::exptoken::Constant (*value));
}

ExpressionImp::ExpressionImp (UINT64 value) :
    Expression (new edu::sharif::twinner::trace::cv::ConcreteValue64Bits (value), false) {
  stack.push_back (new edu::sharif::twinner::trace::exptoken::Constant
                   (new edu::sharif::twinner::trace::cv::ConcreteValue64Bits (value)));
}

ExpressionImp *ExpressionImp::clone () const {
  return new ExpressionImp (*this);
}

}
}
}
}
