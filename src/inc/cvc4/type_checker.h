/*********************                                                        */
/*! \file type_checker.h
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: none
 ** Minor contributors (to current version): Tim King
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief A type checker
 **
 ** A type checker.
 **/

#include "cvc4_private.h"

// ordering dependence
#include "expr/node.h"

#ifndef __CVC4__EXPR__TYPE_CHECKER_H
#define __CVC4__EXPR__TYPE_CHECKER_H

namespace CVC4 {
namespace expr {

class TypeChecker {
public:

  static TypeNode computeType(NodeManager* nodeManager, TNode n, bool check = false)
    throw (TypeCheckingExceptionPrivate, AssertionException);

  static bool computeIsConst(NodeManager* nodeManager, TNode n)
    throw (AssertionException);

  static bool neverIsConst(NodeManager* nodeManager, TNode n)
    throw (AssertionException);

};/* class TypeChecker */

}/* CVC4::expr namespace */
}/* CVC4 namespace */

#endif /* __CVC4__EXPR__TYPE_CHECKER_H */
