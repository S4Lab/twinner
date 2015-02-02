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
//(((((logicalShiftToLeft (((logicalShiftToLeft (((logicalShiftToLeft (((logicalShiftToLeft ((logicalShiftToLeft ((((m7fffffffc950_2 / 0x10000) & 0xffff) >> 0x8), 0x8) | 0x40), 0x10) | 0x0) & 0xffffff), 0x8) | 0x0) & 0xffffffff), 0x18) | 0x0) & 0xffffffff), 0x8) | 0x0) & 0xffffffff) >> 0x8) & 0xffffff) | ((m7fffffffc958_2 >> 0x20) & 0xff000000)) & 0xffff
//(((((logicalShiftToLeft ((((logicalShiftToLeft (((logicalShiftToLeft (((logicalShiftToLeft ((logicalShiftToLeft ((((m7fffffffc950_2 / 0x10000) & 0xffff) >> 0x8), 0x8) | 0x40), 0x10) | 0x0) & 0xffffff), 0x8) | 0x0) & 0xffffffff), 0x18) & 0xffffffff) | (((m7fffffffc950_2 / 0x100000000) | 0x0) & 0x0)) & 0xffffffff), 0x8) | 0x0) & 0xffffffff) >> 0x8) & 0xffffff) | ((m7fffffffc958_2 >> 0x20) & 0xff000000)) & 0xffff
  const Expression *m7fffffffc950_2 = new ExpressionImp (0x7fffffffc950, ConcreteValue64Bits (0x00400450), 2, false);
  const Expression *m7fffffffc958_2 = new ExpressionImp (0x7fffffffc958, ConcreteValue64Bits (0x10076863), 2, false);

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

  return 0;
}

