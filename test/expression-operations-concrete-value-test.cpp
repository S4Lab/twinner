#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/Constant.h"
#include "edu/sharif/twinner/trace/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/ConcreteValue32Bits.h"
#include "edu/sharif/twinner/util/Logger.h"

#include "edu/sharif/twinner/trace/RegisterEmergedSymbol.h"
#include "edu/sharif/twinner/trace/MemoryEmergedSymbol.h"
#include "edu/sharif/twinner/trace/Constant.h"
#include "edu/sharif/twinner/trace/ConcreteValue128Bits.h"
#include "edu/sharif/twinner/trace/ConcreteValue64Bits.h"


using namespace edu::sharif::twinner::trace;
using namespace edu::sharif::twinner::util;


class ExpressionTest : public Expression {

public:
ExpressionTest (REG reg, const ConcreteValue &concreteValue,
    int generationIndex) :
    Expression (concreteValue.clone (), false) {
  const REG enclosingReg = (reg);
  //TODO: Check whether concreteValue should be casted to 64-bits precision for symbol
  stack.push_back (new RegisterEmergedSymbol
                   (enclosingReg, concreteValue, generationIndex));
  if (enclosingReg != reg) {
    truncate (REG_Size (reg) * 8);
  }
}

ExpressionTest (ADDRINT memoryEa, const ConcreteValue &concreteValue,
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
                       (memoryEa, ConcreteValue64Bits (concreteValue), generationIndex));
    } else {
      stack.push_back (new MemoryEmergedSymbol
                       (memoryEa - offset,
                        ConcreteValue64Bits (concreteValue.toUint64 () << (offset * 8)),
                        generationIndex));
      stack.push_back (new Constant (offset * 8));
      stack.push_back (new Operator (Operator::SHIFT_RIGHT));
    }
    if (offset < 8 - (cvsize / 8)) {
      stack.push_back (new Constant ((1ull << cvsize) - 1));
      stack.push_back (new Operator (Operator::BITWISE_AND));
    }
    break;
  }
  default:
    throw std::runtime_error ("ExpressionTest (...): "
                              "Unsupported concrete value size");
  }
}

ExpressionTest (const ConcreteValue &value) :
    Expression (value.clone (), false) {
  stack.push_back (new Constant (value));
}

ExpressionTest (ConcreteValue *value) :
    Expression (value, false) {
  stack.push_back (new Constant (*value));
}

ExpressionTest (UINT64 value) :
    Expression (new ConcreteValue64Bits (value), false) {
  stack.push_back (new Constant (new ConcreteValue64Bits (value)));
}

};


int main () {
  Logger::setVerbosenessLevel ("loquacious");

  const Expression *rdi_0 = new ExpressionTest (REG_RDI, ConcreteValue32Bits (0x2), 0);
  const Expression *m7fffffffe028_0 = new ExpressionTest (0x7fffffffe028, ConcreteValue32Bits (0x00400450), 0, false);
  Expression *edi = rdi_0->clone ();
  edi->bitwiseAnd (0xffffffff);
  Expression *exp = edi->clone ();
  exp->truncate (32);
  Logger::debug () << "Mid 1: " << exp << '\n';
  exp = exp->clone (64);
  exp->shiftToLeft (0x20);
  Expression *mem = m7fffffffe028_0->clone ();
  mem->bitwiseAnd (0xffffffff);
  exp->bitwiseOr (mem);
  exp->shiftToRight (0x20);
  exp = exp->clone (32)->signExtended (128);
  Logger::debug () << "Expression: " << exp << '\n';
  bool lessOrEqual;
  const Constraint *con = Constraint::instantiateLessOrEqualConstraint
    (lessOrEqual, exp, new ExpressionTest (new ConcreteValue64Bits (0x1)), 0x1234);
  Logger::debug () << "Constraint: " << con << '\n';
  Logger::debug () << "Type is '<=': " << lessOrEqual << '\n';

  return 0;
}

