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

#include "edu/sharif/twinner/trace/RegisterEmergedSymbol.h"
#include "edu/sharif/twinner/trace/MemoryEmergedSymbol.h"
#include "edu/sharif/twinner/trace/Constant.h"
#include "edu/sharif/twinner/trace/ConcreteValue128Bits.h"
#include "edu/sharif/twinner/trace/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/ConcreteValue32Bits.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

ExpressionImp::ExpressionImp (const ExpressionImp &exp) :
Expression (exp) {
}

ExpressionImp::ExpressionImp (REG reg, const ConcreteValue &concreteValue,
    int generationIndex) :
Expression (concreteValue.clone (), false) {
  const REG enclosingReg = REG_FullRegName (reg);
  //TODO: Check whether concreteValue should be casted to 64-bits precision for symbol
  stack.push_back (new RegisterEmergedSymbol
                   (enclosingReg, concreteValue, generationIndex));
  if (enclosingReg != reg) {
    truncate (REG_Size (reg) * 8);
  }
}

ExpressionImp::ExpressionImp (ADDRINT memoryEa, const ConcreteValue &concreteValue,
    int generationIndex, bool isOverwriting) :
Expression (concreteValue.clone (), isOverwriting) {
  if (!isOverwriting) {
    if (memoryEa < 0x7f0000000000ull) { // FIXME: Generalize this code
      // this temporary code assumes that everything out of stack (including heap) is constant
      stack.push_back (new Constant (concreteValue));
      return;
    }
  }
  switch (concreteValue.getSize ()) {
  case 128:
  {
    const ConcreteValue128Bits *cv =
        static_cast<const ConcreteValue128Bits *> (&concreteValue);
    stack.push_back (new MemoryEmergedSymbol
                     (memoryEa + 8 /* little endian*/,
                      ConcreteValue64Bits (cv->getMsb ()), generationIndex));
    stack.push_back (new Constant (64));
    stack.push_back (new Operator (Operator::SHIFT_LEFT));
    stack.push_back (new MemoryEmergedSymbol
                     (memoryEa, ConcreteValue64Bits (cv->getLsb ()), generationIndex));
    stack.push_back (new Operator (Operator::BITWISE_OR));
    break;
  }
  case 64:
    stack.push_back (new MemoryEmergedSymbol (memoryEa, concreteValue, generationIndex));
    break;
  case 32:
    if (memoryEa % 8 == 0) {
      stack.push_back (new MemoryEmergedSymbol
                       (memoryEa, ConcreteValue64Bits (concreteValue), generationIndex));
      stack.push_back (new Constant ((1ull << 32) - 1));
      stack.push_back (new Operator (Operator::BITWISE_AND));
    } else {
      stack.push_back (new MemoryEmergedSymbol
                       (memoryEa - 8,
                        ConcreteValue64Bits (concreteValue.toUint64 () << 32),
                        generationIndex));
      stack.push_back (new Constant (32));
      stack.push_back (new Operator (Operator::SHIFT_RIGHT));
    }
    break;
  default:
    throw std::runtime_error ("ExpressionImp::ExpressionImp (...): "
                              "Unsupported concrete value size");
  }
}

ExpressionImp::ExpressionImp (const ConcreteValue &value) :
Expression (value.clone (), false) {
  stack.push_back (new Constant (value));
}

ExpressionImp::ExpressionImp (ConcreteValue *value) :
Expression (value, false) {
  stack.push_back (new Constant (*value));
}

ExpressionImp::ExpressionImp (UINT64 value) :
Expression (new ConcreteValue64Bits (value), false) {
  stack.push_back (new Constant (new ConcreteValue64Bits (value)));
}

ExpressionImp *ExpressionImp::clone () const {
  return new ExpressionImp (*this);
}

}
}
}
}
