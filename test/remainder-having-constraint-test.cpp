#include "edu/sharif/twinner/trace/ExpressionImp.h"
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

#include "edu/sharif/twinner/engine/smt/SmtSolver.h"
#include "edu/sharif/twinner/engine/smt/Cvc4SmtSolver.h"

using namespace edu::sharif::twinner::trace;
using namespace edu::sharif::twinner::util;


int main () {
  Logger::setVerbosenessLevel ("loquacious");

  edu::sharif::twinner::engine::smt::SmtSolver::init
    (new edu::sharif::twinner::engine::smt::Cvc4SmtSolver ());

  const Expression *rax_2 = new ExpressionImp (REG_RAX, ConcreteValue64Bits (0x7fffe4869000), 2);
  Expression *rs = rax_2->clone ();
  rs->truncate (64);
  Expression *con = new ExpressionImp (0x7fffe486a000);
  con->minus (rs);
  con->truncate (64);
  rs = con;

  Expression *e1 = rax_2->clone ();
  e1->truncate (64);
  const Expression *e2 = e1->clone ();
  e1->minus (e2);
  e1->truncate (32);
  e2 = e1->clone ();
  e1->binaryOperation (new Operator (Operator::XOR), e2);
  e1->truncate (32);
  e2 = e1->clone ();
  e1->binaryOperation (new Operator (Operator::XOR), e2);
  e1->truncate (32);
  e1->shiftToLeft (64); // 0x40
  e1->bitwiseOr (0x1f);
  e1->binaryOperation (new Operator (Operator::REMAINDER), rs);
  e1->truncate (64);
  con = new ExpressionImp (0x1f);
  con->minus (e1);
  e1 = con;
  Logger::debug () << "Expression: " << e1 << '\n';
  const Constraint *constraint = new Constraint (e1, Constraint::ZERO, 0, false);
  Logger::debug () << "Constraint without additional (divisor!=zero) constraint: " << constraint << '\n';
  Logger::info () << "Simplifying constraint...\n";
  std::list < const edu::sharif::twinner::trace::Constraint * > simplifiedConstraints =
    edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()->simplifyConstraint (constraint);
  Logger out = Logger::debug ();
  out << "Simplified constraint: ";
  bool first = true;
  for (std::list < const edu::sharif::twinner::trace::Constraint * >::const_iterator it =
      simplifiedConstraints.begin (); it != simplifiedConstraints.end (); ++it) {
    const edu::sharif::twinner::trace::Constraint *simplifiedConstraint = *it;
    if (first) {
      out << '(';
      first = false;
    } else {
      out << " && (";
    }
    out << simplifiedConstraint->toString () << ')';
  }

  return 0;
}

