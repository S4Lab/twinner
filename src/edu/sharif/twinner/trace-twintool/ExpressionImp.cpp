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

#include "ExpressionImp.h"

#include "edu/sharif/twinner/trace/RegisterEmergedSymbol.h"
#include "edu/sharif/twinner/trace/MemoryEmergedSymbol.h"
#include "edu/sharif/twinner/trace/Constant.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

ExpressionImp::ExpressionImp (const ExpressionImp &exp) :
Expression (exp) {
}

ExpressionImp::ExpressionImp (REG reg, UINT64 concreteValue, int generationIndex) :
Expression (concreteValue, false) {
  stack.push_back (new RegisterEmergedSymbol (reg, concreteValue, generationIndex));
}

ExpressionImp::ExpressionImp (ADDRINT memoryEa, UINT64 concreteValue, int generationIndex,
    bool isOverwriting) :
Expression (concreteValue, isOverwriting) {
  stack.push_back (new MemoryEmergedSymbol (memoryEa, concreteValue, generationIndex));
}

ExpressionImp::ExpressionImp (UINT64 value) :
Expression (value, false) {
  stack.push_back (new Constant (value));
}

ExpressionImp *ExpressionImp::clone () const {
  return new ExpressionImp (*this);
}

}
}
}
}
