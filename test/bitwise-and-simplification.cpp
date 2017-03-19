#include "edu/sharif/twinner/trace/ExpressionImp.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/MarInfo.h"

#include "edu/sharif/twinner/trace/exptoken/Constant.h"
#include "edu/sharif/twinner/trace/exptoken/RegisterEmergedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/MemoryEmergedSymbol.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue32Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue16Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue8Bits.h"

#include "edu/sharif/twinner/util/Logger.h"


using namespace edu::sharif::twinner::trace;
using namespace edu::sharif::twinner::trace::exptoken;
using namespace edu::sharif::twinner::trace::cv;
using namespace edu::sharif::twinner::util;


void test1 ();
void test2 ();

int main (int argc, char *argv[]) {
  LogStream::init ("loquacious", "out-test-bitwise-and");
  MarInfo (argc, argv);
  test1 ();
  test2 ();
  LogStream::destroy ();
  return 0;
}

void test1 () {
  Logger::info () << "test 1: simplifying (((0x0 - 0x1) & 0xffffffff) ^ 0x100) & 0xffffffff\n";
  Expression *exp = new ExpressionImp (UINT64 (0x0));
  Expression *one = new ExpressionImp (UINT64 (0x1));
  exp->minus (one);
  exp->bitwiseAnd (0xffffffff);
  exp->bitwiseXor (0x100);
  exp->bitwiseAnd (0xffffffff);
  Logger::debug () << "exp: " << exp << '\n';
}

void test2 () {
  Logger::info () << "test 2: simplifying 0x7ffff4be8498 & 0xffffffff\n";
  Expression *orig = new ExpressionImp (UINT64 (0x7ffff4be8498));
  Expression *exp = orig->clone (32);
  Logger::loquacious () << "exp: " << exp->toDetailedString () << '\n';
  exp->truncate (32);
  Logger::debug () << "exp: " << exp->toDetailedString () << '\n';
}

