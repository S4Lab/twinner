#include "edu/sharif/twinner/trace/ExpressionImp.h"
#include "edu/sharif/twinner/trace/Constraint.h"

#include "edu/sharif/twinner/trace/exptoken/Constant.h"
#include "edu/sharif/twinner/trace/exptoken/RegisterEmergedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/MemoryEmergedSymbol.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue32Bits.h"

#include "edu/sharif/twinner/util/Logger.h"


using namespace edu::sharif::twinner::trace;
using namespace edu::sharif::twinner::trace::exptoken;
using namespace edu::sharif::twinner::trace::cv;
using namespace edu::sharif::twinner::util;


int main () {
  Logger::setVerbosenessLevel ("loquacious");
//(((((((((((logicalShiftToLeft ((m7fffffffe108_1 / 0x100000000), 0x20) | ((m7fffffffe108_1 + 0x400000) & 0xffffffff)) - 0xffffffffffffffdd) & 0xffffffffffffffff) - 0xfffffffffffffffc) >> 0x40) & 0x1) + 0x1b7d85fbe) * 0x4) & 0xffffffff) * 0x2) >> 0x20)
  const Expression *m7fffffffe108_1 = new ExpressionImp (0x7fffffffe108, ConcreteValue64Bits (0x00400450), 1, false);
  Expression *exp = m7fffffffe108_1->clone ();
  exp->divide (0x100000000);
  exp->shiftToLeft (0x20);

  Expression *right = m7fffffffe108_1->clone ();
  right->add (0x400000);
  right->bitwiseAnd (0xffffffff);
  exp->bitwiseOr (right);
  exp->minus (0xffffffffffffffdd);
  exp->bitwiseAnd (0xffffffffffffffff);
  exp->minus (0xfffffffffffffffc);
  exp->shiftToRight (0x40);
  exp->bitwiseAnd (0x1);
  exp->add (0x1b7d85fbe);
  exp->multiply (0x4);
  exp->bitwiseAnd (0xffffffff);
  exp->multiply (0x2);
  exp->shiftToRight (0x20);
  Logger::info () << "exp: " << exp << '\n';


  return 0;
}

