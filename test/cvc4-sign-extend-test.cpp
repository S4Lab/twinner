
#include <iostream>

#if 1 // inclusion of CVC4 header
#define _BACKWARD_BACKWARD_WARNING_H 1
#include "inc/cvc4/cvc4.h"
#undef _BACKWARD_BACKWARD_WARNING_H
#endif // end of inclusion of CVC4 header

using namespace CVC4;

using namespace std;

int main () {
	ExprManager em;
	SmtEngine smt (&em);
	smt.setLogic ("QF_BV");
	smt.setOption ("produce-models", true);
	Type type = em.mkBitVectorType (128);
	Expr cc = em.mkConst (BitVector (128, Integer ("4", 16)));
	cout << cc << '\n';
	cout << "--------------------------\n";
	Expr dd = em.mkExpr (kind::BITVECTOR_EXTRACT,
			em.mkConst (BitVectorExtract (2, 0)),
			cc);
	cout << dd << '\n';
	cout << "--------------------------\n";
	Expr ee = em.mkExpr (kind::BITVECTOR_SIGN_EXTEND,
			em.mkConst (BitVectorSignExtend (3)),
			dd);
	cout << ee << '\n';
	cout << "--------------------------\n";
	Expr ss = smt.simplify (ee);
	cout << ss << '\n';
	return 0;
}

