#include "edu/sharif/twinner/trace/ExpressionImp.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/MarInfo.h"

#include "edu/sharif/twinner/trace/exptoken/Constant.h"
#include "edu/sharif/twinner/trace/exptoken/RegisterEmergedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/MemoryEmergedSymbol.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue32Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue8Bits.h"

#include "edu/sharif/twinner/util/Logger.h"


using namespace edu::sharif::twinner::trace;
using namespace edu::sharif::twinner::trace::exptoken;
using namespace edu::sharif::twinner::trace::cv;
using namespace edu::sharif::twinner::util;


void test1 ();
void test2 ();
void test3 ();
void test4 ();
void test5 ();

int main (int argc, char *argv[]) {
  LogStream::init ("loquacious", "out-test-shift-left-and-bitwise-and-or");
  MarInfo (argc, argv);
  test1 ();
  test2 ();
  test3 ();
  test4 ();
  test5 ();
  LogStream::destroy ();
  return 0;
}

void test1 () {
  Logger::info () << "test 1: simplifying (((((logicalShiftToLeft (((logicalShiftToLeft (((logicalShiftToLeft (((logicalShiftToLeft ((logicalShiftToLeft ((((m7fffffffc950_2 / 0x10000) & 0xffff) >> 0x8), 0x8) | 0x40), 0x10) | 0x0) & 0xffffff), 0x8) | 0x0) & 0xffffffff), 0x18) | 0x0) & 0xffffffff), 0x8) | 0x0) & 0xffffffff) >> 0x8) & 0xffffff) | ((m7fffffffc958_2 >> 0x20) & 0xff000000)) & 0xffff\n";
  const Expression *m7fffffffc950_2 = new ExpressionImp (0x7fffffffc950, ConcreteValue64Bits (0x00400450), 2, true);
  const Expression *m7fffffffc958_2 = new ExpressionImp (0x7fffffffc958, ConcreteValue64Bits (0x10076863), 2, true);

  Expression *exp = m7fffffffc950_2->clone ();
  Expression *right = m7fffffffc950_2->clone ();
  Expression *end = m7fffffffc958_2->clone ();

  end->shiftToRight (0x20);
  end->bitwiseAnd (0xff000000);

  exp->divide (0x10000);
  Logger::info () << "exp: " << exp << '\n';

  right->divide (0x100000000);
  right->bitwiseOr (0x0);
  right->bitwiseAnd (0x0);

  exp->bitwiseAnd (0xffff);
  exp->shiftToRight (0x8);
  exp->shiftToLeft (0x8);
  exp->bitwiseOr (0x40);
  exp->shiftToLeft (0x10);
  exp->bitwiseOr (0x0);
  exp->bitwiseAnd (0xffffff);
  exp->shiftToLeft (0x8);
  exp->bitwiseOr (0x0);
  exp->bitwiseAnd (0xffffffff);
  exp->shiftToLeft (0x18);
  Logger::debug () << "exp: " << exp << '\n';
  exp->bitwiseAnd (0xffffffff);
  Logger::loquacious () << "exp: " << exp << '\n';
  exp->bitwiseOr (right);
  Logger::debug () << "exp: " << exp << '\n';

  exp->bitwiseAnd (0xffffffff);
  exp->shiftToLeft (0x8);
  exp->bitwiseOr (0x0);
  exp->bitwiseAnd (0xffffffff);
  exp->shiftToRight (0x8);
  exp->bitwiseAnd (0xffffff);
  Logger::info () << "exp: " << exp << '\n';

  exp->bitwiseOr (end);
  exp->bitwiseAnd (0xffff);

  Logger::debug () << "exp: " << exp << '\n';
}

void test2 () {
  Logger::info () << "test 2: simplifying (((m220010_1_32 & 0xffffffff) << 0x20) | (signExtend_0x20_0x8 (m22cca5_0_8) & 0xffffffff)) & 0xffffffff\n";
  const Expression *m220010_1_32 = new ExpressionImp (0x220010, ConcreteValue32Bits (0x00400450), 1, true);
  const Expression *m22cca5_0_8 = new ExpressionImp (0x22cca5, ConcreteValue8Bits (0x33), 0, true);
  Expression *exp = m220010_1_32->clone ();
  exp->bitwiseAnd (0xffffffff);
  exp->shiftToLeft (0x20);
  Expression *exp2 = m22cca5_0_8->signExtended (0x20);
  exp2->bitwiseAnd (0xffffffff);
  exp->bitwiseOr (exp2);
  exp->bitwiseAnd (0xffffffff);
  Logger::info () << "exp: " << exp << '\n';
}

void test3 () {
  Logger::info () << "test 3: simplifying (logicalShiftToRight ((((m220010_1_32 & 0xffffffff) << 0x20) | (signExtend_0x20_0x8 (m22cca5_0_8) & 0xffffffff)), 0x20) & 0xffffffff) << 0x20\n";
  const Expression *m220010_1_32 = new ExpressionImp (0x220010, ConcreteValue32Bits (0x00400450), 1, true);
  const Expression *m22cca5_0_8 = new ExpressionImp (0x22cca5, ConcreteValue8Bits (0x33), 0, true);
  Expression *exp = m220010_1_32->clone ();
  exp->bitwiseAnd (0xffffffff);
  exp->shiftToLeft (0x20);
  Expression *exp2 = m22cca5_0_8->signExtended (0x20);
  exp2->bitwiseAnd (0xffffffff);
  exp->bitwiseOr (exp2);
  exp->shiftToRight (0x20);
  exp->bitwiseAnd (0xffffffff);
  exp->shiftToLeft (0x20);
  Logger::info () << "exp: " << exp << '\n';
}

void test4 () {
  Logger::info () << "test 4: simplifying ((m402008_0_32 << 0x28) | 0x15) & 0xffff0015\n";
  const Expression *m402008_0_32 = new ExpressionImp (0x402008, ConcreteValue32Bits (0x00400450), 0, true);
  Expression *exp = m402008_0_32->clone ();
  exp->shiftToLeft (0x28);
  exp->bitwiseOr (0x15);
  exp->bitwiseAnd (0xffff0015);
  Logger::info () << "exp: " << exp << '\n';
}

void test5 () {
  Logger::info () << "test 5: simplifying (((m22cc80_0_32 << 0x8) | 0x3) & 0xffff) & 0xff\n";
  const Expression *m22cc80_0_32 = new ExpressionImp (0x402008, ConcreteValue32Bits (0x00400450), 0, true);
  Expression *exp = m22cc80_0_32->clone ();
  exp->shiftToLeft (0x8);
  exp->bitwiseOr (0x3);
  exp->bitwiseAnd (0xffff);
  exp->bitwiseAnd (0xff);
  Logger::info () << "exp: " << exp << '\n';
}

