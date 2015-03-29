#include "edu/sharif/twinner/trace/ExpressionImp.h"
#include "edu/sharif/twinner/trace/Constraint.h"

#include "edu/sharif/twinner/trace/exptoken/Constant.h"
#include "edu/sharif/twinner/trace/exptoken/RegisterEmergedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/MemoryEmergedSymbol.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue128Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue32Bits.h"

#include "edu/sharif/twinner/util/Logger.h"


using namespace edu::sharif::twinner::trace;
using namespace edu::sharif::twinner::trace::exptoken;
using namespace edu::sharif::twinner::trace::cv;
using namespace edu::sharif::twinner::util;


void test1 ();

int main () {
  Logger::setVerbosenessLevel ("loquacious");
  test1 ();
  return 0;
}

void test1 () {
  Logger::info () << "test 1: simplifying (((((((((((((m7fffe0655030_12 / 0x100000000) & 0xff) + 0x2b5a5) & 0xffffffffffffffff) + (logicalShiftToLeft ((((m7fffe0655030_12 / 0x100000000) & 0xff) + 0x2b5a5), 0x5) & UINT128 (0x0, 0x0, 0xffffffff, 0xffffffe0))) & 0xffffffffffffffff) + ((m7fffe0655030_12 / 0x10000000000) & 0xff)) & 0xffffffffffffffff) + (logicalShiftToLeft ((((((((m7fffe0655030_12 / 0x100000000) & 0xff) + 0x2b5a5) & 0xffffffffffffffff) + (logicalShiftToLeft ((((m7fffe0655030_12 / 0x100000000) & 0xff) + 0x2b5a5), 0x5) & UINT128 (0x0, 0x0, 0xffffffff, 0xffffffe0))) & 0xffffffffffffffff) + ((m7fffe0655030_12 / 0x10000000000) & 0xff)), 0x5) & UINT128 (0x0, 0x0, 0xffffffff, 0xffffffe0))) & 0xffffffffffffffff) + ((m7fffe0655030_12 / 0x1000000000000) & 0xff)) & 0xffffffffffffffff) + (logicalShiftToLeft ((((((((((((m7fffe0655030_12 / 0x100000000) & 0xff) + 0x2b5a5) & 0xffffffffffffffff) + (logicalShiftToLeft ((((m7fffe0655030_12 / 0x100000000) & 0xff) + 0x2b5a5), 0x5) & UINT128 (0x0, 0x0, 0xffffffff, 0xffffffe0))) & 0xffffffffffffffff) + ((m7fffe0655030_12 / 0x10000000000) & 0xff)) & 0xffffffffffffffff) + (logicalShiftToLeft ((((((((m7fffe0655030_12 / 0x100000000) & 0xff) + 0x2b5a5) & 0xffffffffffffffff) + (logicalShiftToLeft ((((m7fffe0655030_12 / 0x100000000) & 0xff) + 0x2b5a5), 0x5) & UINT128 (0x0, 0x0, 0xffffffff, 0xffffffe0))) & 0xffffffffffffffff) + ((m7fffe0655030_12 / 0x10000000000) & 0xff)), 0x5) & UINT128 (0x0, 0x0, 0xffffffff, 0xffffffe0))) & 0xffffffffffffffff) + ((m7fffe0655030_12 / 0x1000000000000) & 0xff)), 0x5) & UINT128 (0x0, 0x0, 0xffffffff, 0xffffffe0))) & 0xffffffffffffffff\n";
  const Expression *m7fffe0655030_12 = new ExpressionImp (0x7fffe0655030, ConcreteValue64Bits (0x00400450), 0x12, false);
  const ConcreteValue128Bits *cv = new ConcreteValue128Bits (0x0, 0xffffffffffffffe0ull);
  Expression *exp = m7fffe0655030_12->clone ();
  exp->divide (0x100000000);
  exp->bitwiseAnd (0xff);
  exp->add (0x2b5a5);
  Expression *exp2 = exp->clone ();
  exp->bitwiseAnd (0xffffffffffffffff);
  exp2->shiftToLeft (5);
  exp2->bitwiseAnd (cv->clone ());
  exp->add (exp2);
  exp->bitwiseAnd (0xffffffffffffffff);
  Expression *exp3 = m7fffe0655030_12->clone ();
  exp3->divide (0x10000000000);
  exp3->bitwiseAnd (0xff);
  exp->add (exp3);
  exp->bitwiseAnd (0xffffffffffffffff);
  Expression *exp4 = m7fffe0655030_12->clone ();
  exp4->divide (0x100000000);
  exp4->bitwiseAnd (0xff);
  exp4->add (0x2b5a5);
  Expression *exp5 = exp4->clone ();
  exp4->bitwiseAnd (0xffffffffffffffff);
  exp5->shiftToLeft (5);
  exp5->bitwiseAnd (cv->clone ());
  exp4->add (exp5);
  exp4->bitwiseAnd (0xffffffffffffffff);
  Expression *exp6 = m7fffe0655030_12->clone ();
  exp6->divide (0x10000000000);
  exp6->bitwiseAnd (0xff);
  exp4->add (exp6);
  exp4->shiftToLeft (5);
  exp4->bitwiseAnd (cv->clone ());
  exp->add (exp4);
  exp->bitwiseAnd (0xffffffffffffffff);
  Expression *exp7 = m7fffe0655030_12->clone ();
  exp7->divide (0x1000000000000);
  exp7->bitwiseAnd (0xff);
  exp->add (exp7);
  Expression *exp8 = exp->clone ();
  exp->bitwiseAnd (0xffffffffffffffff);
  exp8->shiftToLeft (5);
  exp8->bitwiseAnd (cv->clone ());
  exp->add (exp8);
  exp->bitwiseAnd (0xffffffffffffffff);

  Logger::info () << "exp: " << exp << '\n';
}

