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

#include "Cvc4SmtSolver.h"

#if 1 // inclusion of CVC4 header
/*
 * Of course, Twinner is not supposed to fix warnings of CVC4. Also it does not like
 * to disable -Werror. So we need to disable warnings in CVC4 temporarily. CVC4 is
 * using deprecated code which defining following macro, will prevent its related warning
 * to be raised while compiling following included header file.
 */
#define _BACKWARD_BACKWARD_WARNING_H 1
#include "inc/cvc4/cvc4.h"
#undef _BACKWARD_BACKWARD_WARNING_H
#endif // end of inclusion of CVC4 header

using namespace CVC4;

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace smt {

Cvc4SmtSolver::Cvc4SmtSolver () :
SmtSolver () {
}

Cvc4SmtSolver::~Cvc4SmtSolver () {
}

void Cvc4SmtSolver::solveConstraints (
    const std::list < const edu::sharif::twinner::trace::Constraint * > &constraints,
    std::set < const edu::sharif::twinner::trace::MemoryEmergedSymbol * > &symbols)
throw (UnsatisfiableConstraintsException) {
  ExprManager em;
  SmtEngine smt (&em);
  smt.setLogic ("QF_BV"); // Set the logic

  Type bitvector32 = em.mkBitVectorType (32);

  Expr x = em.mkVar ("x", bitvector32);
  Expr a = em.mkVar ("a", bitvector32);
  Expr b = em.mkVar ("b", bitvector32);

  Expr x_eq_a = em.mkExpr (kind::EQUAL, x, a);
  Expr x_eq_b = em.mkExpr (kind::EQUAL, x, b);
  Expr assumption = em.mkExpr (kind::OR, x_eq_a, x_eq_b);

  smt.assertFormula (assumption);

  // Introduce a new variable for the new value of x after assignment.
  Expr new_x = em.mkVar ("new_x", bitvector32); // x after executing code (0)
  Expr new_x_ = em.mkVar ("new_x_", bitvector32); // x after executing code (1) or (2)

  // Encoding code (0)
  // new_x = x == a ? b : a;
  Expr ite = em.mkExpr (kind::ITE, x_eq_a, b, a);
  Expr assignment0 = em.mkExpr (kind::EQUAL, new_x, ite);

  smt.assertFormula (assignment0);
  std::cout << "Pushing a new context." << std::endl;
  smt.push ();

  // Encoding code (1)
  // new_x_ = a xor b xor x
  Expr a_xor_b_xor_x = em.mkExpr (kind::BITVECTOR_XOR, a, b, x);
  Expr assignment1 = em.mkExpr (kind::EQUAL, new_x_, a_xor_b_xor_x);

  std::cout << "Asserting " << assignment1 << " to CVC4 " << std::endl;
  smt.assertFormula (assignment1);
  Expr new_x_eq_new_x_ = em.mkExpr (kind::EQUAL, new_x, new_x_);

  std::cout << " Querying: " << new_x_eq_new_x_ << std::endl;
  std::cout << " Expect valid. " << std::endl;
  std::cout << " CVC4: " << smt.query (new_x_eq_new_x_) << std::endl;
  std::cout << " Popping context. " << std::endl;
  smt.pop ();

  // Encoding code (2)
  // new_x_ = a + b - x
  Expr a_plus_b = em.mkExpr (kind::BITVECTOR_PLUS, a, b);
  Expr a_plus_b_minus_x = em.mkExpr (kind::BITVECTOR_SUB, a_plus_b, x);
  Expr assignment2 = em.mkExpr (kind::EQUAL, new_x_, a_plus_b_minus_x);

  std::cout << "Asserting " << assignment2 << " to CVC4 " << std::endl;
  smt.assertFormula (assignment1);

  std::cout << " Querying: " << new_x_eq_new_x_ << std::endl;
  std::cout << " Expect valid. " << std::endl;
  std::cout << " CVC4: " << smt.query (new_x_eq_new_x_) << std::endl;

  throw UnsatisfiableConstraintsException ();
}

}
}
}
}
}
