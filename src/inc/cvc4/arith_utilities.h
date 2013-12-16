/*********************                                                        */
/*! \file arith_utilities.h
 ** \verbatim
 ** Original author: Tim King
 ** Major contributors: none
 ** Minor contributors (to current version): Dejan Jovanovic, Morgan Deters
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Arith utilities are common inline functions for dealing with nodes.
 **
 ** Arith utilities are common functions for dealing with nodes.
 **/

#include "cvc4_private.h"

#ifndef __CVC4__THEORY__ARITH__ARITH_UTILITIES_H
#define __CVC4__THEORY__ARITH__ARITH_UTILITIES_H

#include "util/rational.h"
#include "util/integer.h"
#include "util/dense_map.h"
#include "expr/node.h"
#include "theory/arith/delta_rational.h"
#include "theory/arith/arithvar.h"
#include "context/cdhashset.h"
#include <ext/hash_map>
#include <vector>

namespace CVC4 {
namespace theory {
namespace arith {

//Sets of Nodes
typedef __gnu_cxx::hash_set<Node, NodeHashFunction> NodeSet;
typedef __gnu_cxx::hash_set<TNode, TNodeHashFunction> TNodeSet;
typedef context::CDHashSet<Node, NodeHashFunction> CDNodeSet;

//Maps from Nodes -> ArithVars, and vice versa
typedef __gnu_cxx::hash_map<Node, ArithVar, NodeHashFunction> NodeToArithVarMap;
typedef DenseMap<Node> ArithVarToNodeMap;

inline Node mkRationalNode(const Rational& q){
  return NodeManager::currentNM()->mkConst<Rational>(q);
}

inline Node mkBoolNode(bool b){
  return NodeManager::currentNM()->mkConst<bool>(b);
}

inline Node mkIntSkolem(const std::string& name){
  return NodeManager::currentNM()->mkSkolem(name, NodeManager::currentNM()->integerType());
}

inline Node mkRealSkolem(const std::string& name){
  return NodeManager::currentNM()->mkSkolem(name, NodeManager::currentNM()->realType());
}

inline Node skolemFunction(const std::string& name, TypeNode dom, TypeNode range){
  NodeManager* currNM = NodeManager::currentNM();
  TypeNode functionType = currNM->mkFunctionType(dom, range);
  return currNM->mkSkolem(name, functionType);
}

/**
 * (For the moment) the type hierarchy goes as:
 * Integer <: Real
 * The type number of a variable is an integer representing the most specific
 * type of the variable. The possible values of type number are:
 */
enum ArithType {
  ATReal = 0,
  ATInteger = 1
};

inline ArithType nodeToArithType(TNode x) {
  return (x.getType().isInteger() ? ATInteger : ATReal);
}

/** \f$ k \in {LT, LEQ, EQ, GEQ, GT} \f$ */
inline bool isRelationOperator(Kind k){
  using namespace kind;

  switch(k){
  case LT:
  case LEQ:
  case EQUAL:
  case GEQ:
  case GT:
    return true;
  default:
    return false;
  }
}

/**
 * Given a relational kind, k, return the kind k' s.t.
 * swapping the lefthand and righthand side is equivalent.
 *
 * The following equivalence should hold,
 *   (k l r) <=> (k' r l)
 */
inline Kind reverseRelationKind(Kind k){
  using namespace kind;

  switch(k){
  case LT:    return GT;
  case LEQ:   return GEQ;
  case EQUAL: return EQUAL;
  case GEQ:   return LEQ;
  case GT:    return LT;

  default:
    Unreachable();
  }
}

inline bool evaluateConstantPredicate(Kind k, const Rational& left, const Rational& right){
  using namespace kind;

  switch(k){
  case LT:    return left <  right;
  case LEQ:   return left <= right;
  case EQUAL: return left == right;
  case GEQ:   return left >= right;
  case GT:    return left >  right;
  default:
    Unreachable();
    return true;
  }
}

/**
 * Returns the appropriate coefficient for the infinitesimal given the kind
 * for an arithmetic atom inorder to represent strict inequalities as inequalities.
 *   x < c  becomes  x <= c + (-1) * \f$ \delta \f$
 *   x > c  becomes  x >= x + ( 1) * \f$ \delta \f$
 * Non-strict inequalities have a coefficient of zero.
 */
inline int deltaCoeff(Kind k){
  switch(k){
  case kind::LT:
    return -1;
  case kind::GT:
    return 1;
  default:
    return 0;
  }
}

/**
 * Given a literal to TheoryArith return a single kind to
 * to indicate its underlying structure.
 * The function returns the following in each case:
 * - (K left right) -> K where is a wildcard for EQUAL, LT, GT, LEQ, or GEQ:
 * - (NOT (EQUAL left right)) -> DISTINCT
 * - (NOT (LEQ left right))   -> GT
 * - (NOT (GEQ left right))   -> LT
 * - (NOT (LT left right))    -> GEQ
 * - (NOT (GT left right))    -> LEQ
 * If none of these match, it returns UNDEFINED_KIND.
 */
 inline Kind oldSimplifiedKind(TNode literal){
  switch(literal.getKind()){
  case kind::LT:
  case kind::GT:
  case kind::LEQ:
  case kind::GEQ:
  case kind::EQUAL:
    return literal.getKind();
  case  kind::NOT:
    {
      TNode atom = literal[0];
      switch(atom.getKind()){
      case  kind::LEQ: //(not (LEQ x c)) <=> (GT x c)
        return  kind::GT;
      case  kind::GEQ: //(not (GEQ x c)) <=> (LT x c)
        return  kind::LT;
      case  kind::LT: //(not (LT x c)) <=> (GEQ x c)
        return  kind::GEQ;
      case  kind::GT: //(not (GT x c) <=> (LEQ x c)
        return  kind::LEQ;
      case  kind::EQUAL:
        return  kind::DISTINCT;
      default:
        Unreachable();
        return  kind::UNDEFINED_KIND;
      }
    }
  default:
    Unreachable();
    return kind::UNDEFINED_KIND;
  }
}

inline Node negateConjunctionAsClause(TNode conjunction){
  Assert(conjunction.getKind() == kind::AND);
  NodeBuilder<> orBuilder(kind::OR);

  for(TNode::iterator i = conjunction.begin(), end=conjunction.end(); i != end; ++i){
    TNode child = *i;
    Node negatedChild = NodeBuilder<1>(kind::NOT)<<(child);
    orBuilder << negatedChild;
  }
  return orBuilder;
}

inline Node maybeUnaryConvert(NodeBuilder<>& builder){
  Assert(builder.getKind() == kind::OR ||
         builder.getKind() == kind::AND ||
         builder.getKind() == kind::PLUS ||
         builder.getKind() == kind::MULT);
  Assert(builder.getNumChildren() >= 1);
  if(builder.getNumChildren() == 1){
    return builder[0];
  }else{
    return builder;
  }
}

inline void flattenAnd(Node n, std::vector<TNode>& out){
  Assert(n.getKind() == kind::AND);
  for(Node::iterator i=n.begin(), i_end=n.end(); i != i_end; ++i){
    Node curr = *i;
    if(curr.getKind() == kind::AND){
      flattenAnd(curr, out);
    }else{
      out.push_back(curr);
    }
  }
}

inline Node flattenAnd(Node n){
  std::vector<TNode> out;
  flattenAnd(n, out);
  return NodeManager::currentNM()->mkNode(kind::AND, out);
}

}/* CVC4::theory::arith namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */

#endif /* __CVC4__THEORY__ARITH__ARITH_UTILITIES_H */
