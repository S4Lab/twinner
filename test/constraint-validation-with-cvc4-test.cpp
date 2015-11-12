#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/util/Logger.h"

#include "edu/sharif/twinner/trace/exptoken/RegisterEmergedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/MemoryEmergedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/Constant.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue32Bits.h"

#include "edu/sharif/twinner/engine/smt/SmtSolver.h"
#include "edu/sharif/twinner/engine/smt/Cvc4SmtSolver.h"

using namespace edu::sharif::twinner::trace;
using namespace edu::sharif::twinner::trace::cv;
using namespace edu::sharif::twinner::trace::exptoken;
using namespace edu::sharif::twinner::util;

class ExpressionTest : public Expression {

ExpressionToken *instantiateMemorySymbol (ADDRINT memoryEa,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue,
    int generationIndex, bool isOverwriting) const {
  return new edu::sharif::twinner::trace::exptoken::MemoryEmergedSymbol
    (memoryEa, concreteValue, generationIndex);
}

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

ExpressionTest (ADDRINT memoryEa,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue,
    int generationIndex, bool isOverwriting) :
    Expression (concreteValue.clone (), isOverwriting) {
  if (!isOverwriting) {
    if (memoryEa < 0x7f0000000000ull) { // FIXME: Generalize this code
      // this temporary code assumes that everything out of stack (including heap) is constant
      stack.push_back (new edu::sharif::twinner::trace::exptoken::Constant (concreteValue));
      return;
    }
  }
  stack.push_back (instantiateMemorySymbol
      (memoryEa, concreteValue, generationIndex, isOverwriting));
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

  edu::sharif::twinner::engine::smt::SmtSolver::init
    (new edu::sharif::twinner::engine::smt::Cvc4SmtSolver ());

//(rsp_0 - 0x7fffffffdf98) & 0xffffffffffffffff == 0
  const Expression *rsp_0 = new ExpressionTest (REG_RSP, ConcreteValue64Bits (0x7fffffffdf98), 0);
  Expression *exp1 = rsp_0->clone ();
  exp1->minus (0x7fffffffdf98);
  exp1->bitwiseAnd (0xffffffffffffffff);
  bool equal;
  const Constraint *con1 = Constraint::instantiateEqualConstraint
    (equal, exp1, new ExpressionTest (new ConcreteValue64Bits (0x0)), 0x1235);
  Logger::debug () << "Constraint: " << con1 << '\n';
  edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()->checkValidity (con1);
  edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()->assertConstraint (con1);

//signExtend_0x80_0x20 ((logicalShiftToRight ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffdf78_0_32), 0x20) & 0xffffffff)) >= UINT128 (0x0, 0x0, 0x0, 0x2)
  const Expression *rdi_0 = new ExpressionTest (REG_RDI, ConcreteValue64Bits (0x2), 0);
  Expression *exp = rdi_0->clone ();
  exp->bitwiseAnd (0xffffffff);
  exp->shiftToLeft (0x20);
  const Expression *m7fffffffdf78_0_32 = new ExpressionTest (0x7fffffffdf78, ConcreteValue32Bits (0x00400450), 0, false);
  exp->bitwiseOr (m7fffffffdf78_0_32);
  exp->shiftToRight (0x20);
  exp->bitwiseAnd (0xffffffff);
  bool less;
  const Constraint *con = Constraint::instantiateLessConstraint
    (less, exp, new ExpressionTest (new ConcreteValue32Bits (0x2)), 0x1234);
  Logger::debug () << "Constraint: " << con << '\n';

  edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()->checkValidity (con);

  return 0;
}

