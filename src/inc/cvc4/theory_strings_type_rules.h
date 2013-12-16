/*********************                                                        */
/*! \file theory_strings_type_rules.h
 ** \verbatim
 ** Original author: Tianyi Liang
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Typing and cardinality rules for the theory of arrays
 **
 ** Typing and cardinality rules for the theory of arrays.
 **/

#include "cvc4_private.h"

#ifndef __CVC4__THEORY__STRINGS__THEORY_STRINGS_TYPE_RULES_H
#define __CVC4__THEORY__STRINGS__THEORY_STRINGS_TYPE_RULES_H

namespace CVC4 {
namespace theory {
namespace strings {

class StringConstantTypeRule {
public:
  inline static TypeNode computeType(NodeManager* nodeManager, TNode n, bool check)
      throw (TypeCheckingExceptionPrivate, AssertionException) {
    return nodeManager->stringType();
  }
};

class StringConcatTypeRule {
public:
  inline static TypeNode computeType(NodeManager* nodeManager, TNode n, bool check)
      throw (TypeCheckingExceptionPrivate, AssertionException) {
    TNode::iterator it = n.begin();
    TNode::iterator it_end = n.end();
	int size = 0;
    for (; it != it_end; ++ it) {
       TypeNode t = (*it).getType(check);
       if (!t.isString()) {
         throw TypeCheckingExceptionPrivate(n, "expecting string terms in string concat");
       }
	   ++size;
    }
	if(size < 2) {
       throw TypeCheckingExceptionPrivate(n, "expecting at least 2 terms in string concat");
	}
    return nodeManager->stringType();
  }
};

class StringLengthTypeRule {
public:
  inline static TypeNode computeType(NodeManager* nodeManager, TNode n, bool check)
      throw (TypeCheckingExceptionPrivate, AssertionException) {
    if( check ){
        TypeNode t = n[0].getType(check);
        if (!t.isString()) {
          throw TypeCheckingExceptionPrivate(n, "expecting string terms in string length");
        }
    }
    return nodeManager->integerType();
  }
};

class StringSubstrTypeRule {
public:
  inline static TypeNode computeType(NodeManager* nodeManager, TNode n, bool check)
      throw (TypeCheckingExceptionPrivate, AssertionException) {
    if( check ){
        TypeNode t = n[0].getType(check);
        if (!t.isString()) {
          throw TypeCheckingExceptionPrivate(n, "expecting string terms in substr");
        }
		t = n[1].getType(check);
        if (!t.isInteger()) {
          throw TypeCheckingExceptionPrivate(n, "expecting start int terms in substr");
        }
		t = n[2].getType(check);
        if (!t.isInteger()) {
          throw TypeCheckingExceptionPrivate(n, "expecting length int terms in substr");
        }
    }
    return nodeManager->stringType();
  }
};

class RegExpConstantTypeRule {
public:
  inline static TypeNode computeType(NodeManager* nodeManager, TNode n, bool check)
      throw (TypeCheckingExceptionPrivate, AssertionException) {
    return nodeManager->regexpType();
  }
};

class RegExpConcatTypeRule {
public:
  inline static TypeNode computeType(NodeManager* nodeManager, TNode n, bool check)
      throw (TypeCheckingExceptionPrivate, AssertionException) {
    TNode::iterator it = n.begin();
    TNode::iterator it_end = n.end();
	int size = 0;
    for (; it != it_end; ++ it) {
       TypeNode t = (*it).getType(check);
       if (!t.isRegExp()) {
         throw TypeCheckingExceptionPrivate(n, "expecting regexp terms in regexp concat");
       }
	   ++size;
    }
	if(size < 2) {
       throw TypeCheckingExceptionPrivate(n, "expecting at least 2 terms in regexp concat");
	}
    return nodeManager->regexpType();
  }
};

class RegExpOrTypeRule {
public:
  inline static TypeNode computeType(NodeManager* nodeManager, TNode n, bool check)
      throw (TypeCheckingExceptionPrivate, AssertionException) {
    TNode::iterator it = n.begin();
    TNode::iterator it_end = n.end();
    for (; it != it_end; ++ it) {
       TypeNode t = (*it).getType(check);
       if (!t.isRegExp()) {
         throw TypeCheckingExceptionPrivate(n, "expecting regexp terms");
       }
    }
    return nodeManager->regexpType();
  }
};

class RegExpInterTypeRule {
public:
  inline static TypeNode computeType(NodeManager* nodeManager, TNode n, bool check)
      throw (TypeCheckingExceptionPrivate, AssertionException) {
    TNode::iterator it = n.begin();
    TNode::iterator it_end = n.end();
    for (; it != it_end; ++ it) {
       TypeNode t = (*it).getType(check);
       if (!t.isRegExp()) {
         throw TypeCheckingExceptionPrivate(n, "expecting regexp terms");
       }
    }
    return nodeManager->regexpType();
  }
};

class RegExpStarTypeRule {
public:
  inline static TypeNode computeType(NodeManager* nodeManager, TNode n, bool check)
      throw (TypeCheckingExceptionPrivate, AssertionException) {
    TNode::iterator it = n.begin();
    TNode::iterator it_end = n.end();
    TypeNode t = (*it).getType(check);
    if (!t.isRegExp()) {
      throw TypeCheckingExceptionPrivate(n, "expecting regexp terms");
    }
    if(++it != it_end) {
      throw TypeCheckingExceptionPrivate(n, "too many regexp");
    }

    return nodeManager->regexpType();
  }
};

class RegExpPlusTypeRule {
public:
  inline static TypeNode computeType(NodeManager* nodeManager, TNode n, bool check)
      throw (TypeCheckingExceptionPrivate, AssertionException) {
    TNode::iterator it = n.begin();
    TNode::iterator it_end = n.end();
    TypeNode t = (*it).getType(check);
    if (!t.isRegExp()) {
      throw TypeCheckingExceptionPrivate(n, "expecting regexp terms");
    }
    if(++it != it_end) {
      throw TypeCheckingExceptionPrivate(n, "too many regexp");
    }

    return nodeManager->regexpType();
  }
};

class RegExpOptTypeRule {
public:
  inline static TypeNode computeType(NodeManager* nodeManager, TNode n, bool check)
      throw (TypeCheckingExceptionPrivate, AssertionException) {
    TNode::iterator it = n.begin();
    TNode::iterator it_end = n.end();
    TypeNode t = (*it).getType(check);
    if (!t.isRegExp()) {
      throw TypeCheckingExceptionPrivate(n, "expecting regexp terms");
    }
    if(++it != it_end) {
      throw TypeCheckingExceptionPrivate(n, "too many regexp");
    }

    return nodeManager->regexpType();
  }
};

class RegExpRangeTypeRule {
public:
  inline static TypeNode computeType(NodeManager* nodeManager, TNode n, bool check)
      throw (TypeCheckingExceptionPrivate, AssertionException) {
    TNode::iterator it = n.begin();
    TNode::iterator it_end = n.end();
	char ch[2];

	for(int i=0; i<2; ++i) {
		TypeNode t = (*it).getType(check);
		if (!t.isString()) {
		  throw TypeCheckingExceptionPrivate(n, "expecting a string term in regexp range");
		}
		if( (*it).getKind() != kind::CONST_STRING ) {
		  throw TypeCheckingExceptionPrivate(n, "expecting a constant string term in regexp range");
		}
		if( (*it).getConst<String>().size() != 1 ) {
		  throw TypeCheckingExceptionPrivate(n, "expecting a single constant string term in regexp range");
		}
		ch[i] = (*it).getConst<String>().getFirstChar();
		++it;
	}
	if(ch[0] > ch[1]) {
		throw TypeCheckingExceptionPrivate(n, "expecting the first constant is less or equal to the second one in regexp range");
	}

    if( it != it_end ) {
      throw TypeCheckingExceptionPrivate(n, "too many terms in regexp range");
    }

    return nodeManager->regexpType();
  }
};

class StringToRegExpTypeRule {
public:
  inline static TypeNode computeType(NodeManager* nodeManager, TNode n, bool check)
      throw (TypeCheckingExceptionPrivate, AssertionException) {
    TNode::iterator it = n.begin();
    TNode::iterator it_end = n.end();
    TypeNode t = (*it).getType(check);
    if (!t.isString()) {
      throw TypeCheckingExceptionPrivate(n, "expecting string terms");
    }
    //if( (*it).getKind() != kind::CONST_STRING ) {
    //  throw TypeCheckingExceptionPrivate(n, "expecting constant string terms");
    //}
    if(++it != it_end) {
      throw TypeCheckingExceptionPrivate(n, "too many terms");
    }

    return nodeManager->regexpType();
  }
};

class StringInRegExpTypeRule {
public:
  inline static TypeNode computeType(NodeManager* nodeManager, TNode n, bool check)
      throw (TypeCheckingExceptionPrivate, AssertionException) {
    TNode::iterator it = n.begin();
    TNode::iterator it_end = n.end();
    TypeNode t = (*it).getType(check);
    if (!t.isString()) {
      throw TypeCheckingExceptionPrivate(n, "expecting string terms");
    }
    ++it;
    t = (*it).getType(check);
    if (!t.isRegExp()) {
      throw TypeCheckingExceptionPrivate(n, "expecting regexp terms");
    }
    if(++it != it_end) {
      throw TypeCheckingExceptionPrivate(n, "too many terms");
    }

    return nodeManager->booleanType();
  }
};


}/* CVC4::theory::strings namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */

#endif /* __CVC4__THEORY__STRINGS__THEORY_STRINGS_TYPE_RULES_H */
