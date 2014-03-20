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
#include "edu/sharif/twinner/trace/ConcreteValue64Bits.h"

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
  stack.push_back (new RegisterEmergedSymbol
                   (enclosingReg, concreteValue, generationIndex));
  if (enclosingReg != reg) {
    truncate (REG_Size (reg) * 8);
  }
}

ExpressionImp::ExpressionImp (ADDRINT memoryEa, const ConcreteValue &concreteValue,
    int generationIndex, bool isOverwriting) :
Expression (concreteValue.clone (), isOverwriting) {
  stack.push_back (new MemoryEmergedSymbol (memoryEa, concreteValue, generationIndex));
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
