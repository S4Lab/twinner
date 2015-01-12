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

#include "ExpressionImp.h"

#include "RegisterEmergedSymbol.h"
#include "MemoryEmergedSymbol.h"
#include "Constant.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue128Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

ExpressionImp::ExpressionImp (const ExpressionImp &exp) :
    Expression (exp) {
}

ExpressionImp::ExpressionImp (REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue,
    int generationIndex) :
    Expression (concreteValue.clone (), false) {
  //TODO: Check whether concreteValue should be casted to 64-bits precision for symbol
  stack.push_back (new RegisterEmergedSymbol (reg, concreteValue, generationIndex));
}

ExpressionImp::ExpressionImp (ADDRINT memoryEa,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue,
    int generationIndex, bool isOverwriting) :
    Expression (concreteValue.clone (), isOverwriting) {
  if (!isOverwriting) {
    if (memoryEa < 0x7f0000000000ull) { // FIXME: Generalize this code
      // this temporary code assumes that everything out of stack (including heap) is constant
      stack.push_back (new Constant (concreteValue));
      return;
    }
  }
  switch (const int cvsize = concreteValue.getSize ()) {
  case 128:
  {
    const edu::sharif::twinner::trace::cv::ConcreteValue128Bits *cv =
        static_cast<const edu::sharif::twinner::trace::cv::ConcreteValue128Bits *>
        (&concreteValue);
    stack.push_back (new MemoryEmergedSymbol
                     (memoryEa + 8, // little endian
                      edu::sharif::twinner::trace::cv::ConcreteValue64Bits (cv->getMsb ()),
                      generationIndex));
    stack.push_back (new Constant (64));
    stack.push_back (new Operator (Operator::SHIFT_LEFT));
    stack.push_back (new MemoryEmergedSymbol
                     (memoryEa,
                      edu::sharif::twinner::trace::cv::ConcreteValue64Bits (cv->getLsb ()),
                      generationIndex));
    stack.push_back (new Operator (Operator::BITWISE_OR));
    break;
  }
  case 64:
    stack.push_back (new MemoryEmergedSymbol (memoryEa, concreteValue, generationIndex));
    break;
  case 32:
  case 16:
  case 8:
  {
    const int offset = memoryEa % 8;
    if (offset == 0) {
      stack.push_back (new MemoryEmergedSymbol
                       (memoryEa,
                        edu::sharif::twinner::trace::cv::ConcreteValue64Bits (concreteValue),
                        generationIndex));
    } else {
      stack.push_back (new MemoryEmergedSymbol
                       (memoryEa - offset,
                        edu::sharif::twinner::trace::cv::ConcreteValue64Bits
                        (concreteValue.toUint64 () << (offset * 8)),
                        generationIndex));
      // Using division instead of shift-to-right to match with Expression implementation
      stack.push_back (new Constant (1ull << (offset * 8)));
      stack.push_back (new Operator (Operator::DIVIDE));
    }
    if (offset < 8 - (cvsize / 8)) {
      stack.push_back (new Constant ((1ull << cvsize) - 1));
      stack.push_back (new Operator (Operator::BITWISE_AND));
    }
    break;
  }
  default:
    throw std::runtime_error ("ExpressionImp::ExpressionImp (...): "
                              "Unsupported concrete value size");
  }
}

ExpressionImp::ExpressionImp (Symbol *symbol) :
    Expression (symbol->getValue ().clone (), false) {
  stack.push_back (symbol);
}

ExpressionImp::ExpressionImp (const edu::sharif::twinner::trace::cv::ConcreteValue &value) :
    Expression (value.clone (), false) {
  stack.push_back (new Constant (value));
}

ExpressionImp::ExpressionImp (edu::sharif::twinner::trace::cv::ConcreteValue *value) :
    Expression (value, false) {
  stack.push_back (new Constant (*value));
}

ExpressionImp::ExpressionImp (UINT64 value) :
    Expression (new edu::sharif::twinner::trace::cv::ConcreteValue64Bits (value), false) {
  stack.push_back
      (new Constant (new edu::sharif::twinner::trace::cv::ConcreteValue64Bits (value)));
}

ExpressionImp *ExpressionImp::clone () const {
  return new ExpressionImp (*this);
}

}
}
}
}
