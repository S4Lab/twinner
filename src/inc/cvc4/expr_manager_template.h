/*********************                                                        */
/*! \file expr_manager_template.h
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: Dejan Jovanovic
 ** Minor contributors (to current version): Andrew Reynolds, Tim King, Christopher L. Conway
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Public-facing expression manager interface
 **
 ** Public-facing expression manager interface.
 **/

#include "cvc4_public.h"

#ifndef __CVC4__EXPR_MANAGER_H
#define __CVC4__EXPR_MANAGER_H

#include <vector>

#include "expr/kind.h"
#include "expr/type.h"
#include "expr/expr.h"
#include "util/subrange_bound.h"
#include "util/statistics.h"
#include "util/sexpr.h"

${includes}

// This is a hack, but an important one: if there's an error, the
// compiler directs the user to the template file instead of the
// generated one.  We don't want the user to modify the generated one,
// since it'll get overwritten on a later build.
#line 38 "${template}"

namespace CVC4 {

class Expr;
class SmtEngine;
class NodeManager;
class Options;
class IntStat;
struct ExprManagerMapCollection;
class StatisticsRegistry;

namespace expr {
  namespace pickle {
    class Pickler;
  }/* CVC4::expr::pickle namespace */
}/* CVC4::expr namespace */

namespace context {
  class Context;
}/* CVC4::context namespace */

namespace stats {
  StatisticsRegistry* getStatisticsRegistry(ExprManager*);
}/* CVC4::stats namespace */

class CVC4_PUBLIC ExprManager {
private:
  /** The context */
  context::Context* d_ctxt;

  /** The internal node manager */
  NodeManager* d_nodeManager;

  /** Counts of expressions and variables created of a given kind */
  IntStat* d_exprStatisticsVars[LAST_TYPE];
  IntStat* d_exprStatistics[kind::LAST_KIND];

  /**
   * Returns the internal node manager.  This should only be used by
   * internal users, i.e. the friend classes.
   */
  NodeManager* getNodeManager() const;

  /**
   * Returns the internal Context.  Used by internal users, i.e. the
   * friend classes.
   */
  context::Context* getContext() const;

  /**
   * Check some things about a newly-created DatatypeType.
   */
  void checkResolvedDatatype(DatatypeType dtt) const;

  /**
   * SmtEngine will use all the internals, so it will grab the
   * NodeManager.
   */
  friend class SmtEngine;

  /** ExprManagerScope reaches in to get the NodeManager */
  friend class ExprManagerScope;

  /** NodeManager reaches in to get the NodeManager */
  friend class NodeManager;

  /** Statistics reach in to get the StatisticsRegistry */
  friend ::CVC4::StatisticsRegistry* ::CVC4::stats::getStatisticsRegistry(ExprManager*);

  /** Get the underlying statistics registry. */
  StatisticsRegistry* getStatisticsRegistry() throw();

  // undefined, private copy constructor and assignment op (disallow copy)
  ExprManager(const ExprManager&) CVC4_UNDEFINED;
  ExprManager& operator=(const ExprManager&) CVC4_UNDEFINED;

public:

  /**
   * Creates an expression manager with default options.
   */
  ExprManager();

  /**
   * Creates an expression manager.
   *
   * @param options the earlyTypeChecking field is used to configure
   * whether to do at Expr creation time.
   */
  explicit ExprManager(const Options& options);

  /**
   * Destroys the expression manager. No will be deallocated at this point, so
   * any expression references that used to be managed by this expression
   * manager and are left-over are bad.
   */
  ~ExprManager() throw();

  /** Get this node manager's options */
  const Options& getOptions() const;

  /** Get the type for booleans */
  BooleanType booleanType() const;

  /** Get the type for strings. */
  StringType stringType() const;

  /** Get the type for reals. */
  RealType realType() const;

  /** Get the type for integers */
  IntegerType integerType() const;

  /**
   * Make a unary expression of a given kind (NOT, BVNOT, ...).
   * @param kind the kind of expression
   * @param child1 kind the kind of expression
   * @return the expression
   */
  Expr mkExpr(Kind kind, Expr child1);

  /**
   * Make a binary expression of a given kind (AND, PLUS, ...).
   * @param kind the kind of expression
   * @param child1 the first child of the new expression
   * @param child2 the second child of the new expression
   * @return the expression
   */
  Expr mkExpr(Kind kind, Expr child1, Expr child2);

  /**
   * Make a 3-ary expression of a given kind (AND, PLUS, ...).
   * @param kind the kind of expression
   * @param child1 the first child of the new expression
   * @param child2 the second child of the new expression
   * @param child3 the third child of the new expression
   * @return the expression
   */
  Expr mkExpr(Kind kind, Expr child1, Expr child2, Expr child3);

  /**
   * Make a 4-ary expression of a given kind (AND, PLUS, ...).
   * @param kind the kind of expression
   * @param child1 the first child of the new expression
   * @param child2 the second child of the new expression
   * @param child3 the third child of the new expression
   * @param child4 the fourth child of the new expression
   * @return the expression
   */
  Expr mkExpr(Kind kind, Expr child1, Expr child2, Expr child3, Expr child4);

  /**
   * Make a 5-ary expression of a given kind (AND, PLUS, ...).
   * @param kind the kind of expression
   * @param child1 the first child of the new expression
   * @param child2 the second child of the new expression
   * @param child3 the third child of the new expression
   * @param child4 the fourth child of the new expression
   * @param child5 the fifth child of the new expression
   * @return the expression
   */
  Expr mkExpr(Kind kind, Expr child1, Expr child2, Expr child3, Expr child4,
              Expr child5);

  /**
   * Make an n-ary expression of given kind given a vector of its
   * children
   *
   * @param kind the kind of expression to build
   * @param children the subexpressions
   * @return the n-ary expression
   */
  Expr mkExpr(Kind kind, const std::vector<Expr>& children);

  /**
   * Make an n-ary expression of given kind given a first arg and
   * a vector of its remaining children (this can be useful where the
   * kind is parameterized operator, so the first arg is really an
   * arg to the kind to construct an operator)
   *
   * @param kind the kind of expression to build
   * @param child1 the first subexpression
   * @param otherChildren the remaining subexpressions
   * @return the n-ary expression
   */
  Expr mkExpr(Kind kind, Expr child1, const std::vector<Expr>& otherChildren);

  /**
   * Make a nullary parameterized expression with the given operator.
   *
   * @param opExpr the operator expression
   * @return the nullary expression
   */
  Expr mkExpr(Expr opExpr);

  /**
   * Make a unary parameterized expression with the given operator.
   *
   * @param opExpr the operator expression
   * @param child1 the subexpression
   * @return the unary expression
   */
  Expr mkExpr(Expr opExpr, Expr child1);

  /**
   * Make a binary parameterized expression with the given operator.
   *
   * @param opExpr the operator expression
   * @param child1 the first subexpression
   * @param child2 the second subexpression
   * @return the binary expression
   */
  Expr mkExpr(Expr opExpr, Expr child1, Expr child2);

  /**
   * Make a ternary parameterized expression with the given operator.
   *
   * @param opExpr the operator expression
   * @param child1 the first subexpression
   * @param child2 the second subexpression
   * @param child3 the third subexpression
   * @return the ternary expression
   */
  Expr mkExpr(Expr opExpr, Expr child1, Expr child2, Expr child3);

  /**
   * Make a quaternary parameterized expression with the given operator.
   *
   * @param opExpr the operator expression
   * @param child1 the first subexpression
   * @param child2 the second subexpression
   * @param child3 the third subexpression
   * @param child4 the fourth subexpression
   * @return the quaternary expression
   */
  Expr mkExpr(Expr opExpr, Expr child1, Expr child2, Expr child3, Expr child4);

  /**
   * Make a quinary parameterized expression with the given operator.
   *
   * @param opExpr the operator expression
   * @param child1 the first subexpression
   * @param child2 the second subexpression
   * @param child3 the third subexpression
   * @param child4 the fourth subexpression
   * @param child5 the fifth subexpression
   * @return the quinary expression
   */
  Expr mkExpr(Expr opExpr, Expr child1, Expr child2, Expr child3, Expr child4,
              Expr child5);

  /**
   * Make an n-ary expression of given operator to apply and a vector
   * of its children
   *
   * @param opExpr the operator expression
   * @param children the subexpressions
   * @return the n-ary expression
   */
  Expr mkExpr(Expr opExpr, const std::vector<Expr>& children);

  /** Create a constant of type T */
  template <class T>
  Expr mkConst(const T&);

  /**
   * Create an Expr by applying an associative operator to the children.
   * If <code>children.size()</code> is greater than the max arity for
   * <code>kind</code>, then the expression will be broken up into
   * suitably-sized chunks, taking advantage of the associativity of
   * <code>kind</code>. For example, if kind <code>FOO</code> has max arity
   * 2, then calling <code>mkAssociative(FOO,a,b,c)</code> will return
   * <code>(FOO (FOO a b) c)</code> or <code>(FOO a (FOO b c))</code>.
   * The order of the arguments will be preserved in a left-to-right
   * traversal of the resulting tree.
   */
  Expr mkAssociative(Kind kind, const std::vector<Expr>& children);

  /**
   * Determine whether Exprs of a particular Kind have operators.
   * @returns true if Exprs of Kind k have operators.
   */
  static bool hasOperator(Kind k);

  /**
   * Get the (singleton) operator of an OPERATOR-kinded kind.  The
   * returned Expr e will have kind BUILTIN, and calling
   * e.getConst<CVC4::Kind>() will yield k.
   */
  Expr operatorOf(Kind k);

  /** Make a function type from domain to range. */
  FunctionType mkFunctionType(Type domain, Type range);

  /**
   * Make a function type with input types from argTypes.
   * <code>argTypes</code> must have at least one element.
   */
  FunctionType mkFunctionType(const std::vector<Type>& argTypes, Type range);

  /**
   * Make a function type with input types from
   * <code>sorts[0..sorts.size()-2]</code> and result type
   * <code>sorts[sorts.size()-1]</code>. <code>sorts</code> must have
   * at least 2 elements.
   */
  FunctionType mkFunctionType(const std::vector<Type>& sorts);

  /**
   * Make a predicate type with input types from
   * <code>sorts</code>. The result with be a function type with range
   * <code>BOOLEAN</code>. <code>sorts</code> must have at least one
   * element.
   */
  FunctionType mkPredicateType(const std::vector<Type>& sorts);

  /**
   * Make a tuple type with types from
   * <code>types[0..types.size()-1]</code>.  <code>types</code> must
   * have at least one element.
   */
  TupleType mkTupleType(const std::vector<Type>& types);

  /**
   * Make a record type with types from the rec parameter.
   */
  RecordType mkRecordType(const Record& rec);

  /**
   * Make a symbolic expressiontype with types from
   * <code>types[0..types.size()-1]</code>.  <code>types</code> may
   * have any number of elements.
   */
  SExprType mkSExprType(const std::vector<Type>& types);

  /** Make a type representing a bit-vector of the given size. */
  BitVectorType mkBitVectorType(unsigned size) const;

  /** Make the type of arrays with the given parameterization. */
  ArrayType mkArrayType(Type indexType, Type constituentType) const;

  /** Make a type representing the given datatype. */
  DatatypeType mkDatatypeType(const Datatype& datatype);

  /**
   * Make a set of types representing the given datatypes, which may be
   * mutually recursive.
   */
  std::vector<DatatypeType>
  mkMutualDatatypeTypes(const std::vector<Datatype>& datatypes);

  /**
   * Make a set of types representing the given datatypes, which may
   * be mutually recursive.  unresolvedTypes is a set of SortTypes
   * that were used as placeholders in the Datatypes for the Datatypes
   * of the same name.  This is just a more complicated version of the
   * above mkMutualDatatypeTypes() function, but is required to handle
   * complex types.
   *
   * For example, unresolvedTypes might contain the single sort "list"
   * (with that name reported from SortType::getName()).  The
   * datatypes list might have the single datatype
   *
   *   DATATYPE
   *     list = cons(car:ARRAY INT OF list, cdr:list) | nil;
   *   END;
   *
   * To represent the Type of the array, the user had to create a
   * placeholder type (an uninterpreted sort) to stand for "list" in
   * the type of "car".  It is this placeholder sort that should be
   * passed in unresolvedTypes.  If the datatype was of the simpler
   * form:
   *
   *   DATATYPE
   *     list = cons(car:list, cdr:list) | nil;
   *   END;
   *
   * then no complicated Type needs to be created, and the above,
   * simpler form of mkMutualDatatypeTypes() is enough.
   */
  std::vector<DatatypeType>
  mkMutualDatatypeTypes(const std::vector<Datatype>& datatypes,
                        const std::set<Type>& unresolvedTypes);

  /**
   * Make a type representing a constructor with the given parameterization.
   */
  ConstructorType mkConstructorType(const DatatypeConstructor& constructor, Type range) const;

  /** Make a type representing a selector with the given parameterization. */
  SelectorType mkSelectorType(Type domain, Type range) const;

  /** Make a type representing a tester with the given parameterization. */
  TesterType mkTesterType(Type domain) const;

  /** Bits for use in mkSort() flags. */
  enum {
    SORT_FLAG_NONE = 0,
    SORT_FLAG_PLACEHOLDER = 1
  };/* enum */

  /** Make a new sort with the given name. */
  SortType mkSort(const std::string& name, uint32_t flags = SORT_FLAG_NONE) const;

  /** Make a sort constructor from a name and arity. */
  SortConstructorType mkSortConstructor(const std::string& name,
                                        size_t arity) const;

  /**
   * Make a predicate subtype type defined by the given LAMBDA
   * expression.  A TypeCheckingException can be thrown if lambda is
   * not a LAMBDA, or is ill-typed, or if CVC4 fails at proving that
   * the resulting predicate subtype is inhabited.
   */
  // not in release 1.0
  //Type mkPredicateSubtype(Expr lambda)
  //  throw(TypeCheckingException);

  /**
   * Make a predicate subtype type defined by the given LAMBDA
   * expression and whose non-emptiness is witnessed by the given
   * witness.  A TypeCheckingException can be thrown if lambda is not
   * a LAMBDA, or is ill-typed, or if the witness is not a witness or
   * ill-typed.
   */
  // not in release 1.0
  //Type mkPredicateSubtype(Expr lambda, Expr witness)
  //  throw(TypeCheckingException);

  /**
   * Make an integer subrange type as defined by the argument.
   */
  Type mkSubrangeType(const SubrangeBounds& bounds)
    throw(TypeCheckingException);

  /** Get the type of an expression */
  Type getType(Expr e, bool check = false)
    throw(TypeCheckingException);

  /** Bits for use in mkVar() flags. */
  enum {
    VAR_FLAG_NONE = 0,
    VAR_FLAG_GLOBAL = 1,
    VAR_FLAG_DEFINED = 2
  };/* enum */

  /**
   * Create a new, fresh variable.  This variable is guaranteed to be
   * distinct from every variable thus far in the ExprManager, even
   * if it shares a name with another; this is to support any kind of
   * scoping policy on top of ExprManager.  The SymbolTable class
   * can be used to store and lookup symbols by name, if desired.
   *
   * @param name a name to associate to the fresh new variable
   * @param type the type for the new variable
   * @param flags - VAR_FLAG_NONE - no flags;
   * VAR_FLAG_GLOBAL - whether this variable is to be
   * considered "global" or not.  Note that this information isn't
   * used by the ExprManager, but is passed on to the ExprManager's
   * event subscribers like the model-building service; if isGlobal
   * is true, this newly-created variable will still available in
   * models generated after an intervening pop.
   * VAR_FLAG_DEFINED - if this is to be a "defined" symbol, e.g., for
   * use with SmtEngine::defineFunction().  This keeps a declaration
   * from being emitted in API dumps (since a subsequent definition is
   * expected to be dumped instead).
   */
  Expr mkVar(const std::string& name, Type type, uint32_t flags = VAR_FLAG_NONE);

  /**
   * Create a (nameless) new, fresh variable.  This variable is guaranteed
   * to be distinct from every variable thus far in the ExprManager.
   *
   * @param type the type for the new variable
   * @param flags - VAR_FLAG_GLOBAL - whether this variable is to be considered "global"
   * or not.  Note that this information isn't used by the ExprManager,
   * but is passed on to the ExprManager's event subscribers like the
   * model-building service; if isGlobal is true, this newly-created
   * variable will still available in models generated after an
   * intervening pop.
   */
  Expr mkVar(Type type, uint32_t flags = VAR_FLAG_NONE);

  /**
   * Create a new, fresh variable for use in a binder expression
   * (the BOUND_VAR_LIST of a FORALL, EXISTS, or LAMBDA).  It is
   * an error for this bound variable to exist outside of a binder,
   * and it should also only be used in a single binder expression.
   * That is, two distinct FORALL expressions should use entirely
   * disjoint sets of bound variables (however, a single FORALL
   * expression can be used in multiple places in a formula without
   * a problem).  This newly-created bound variable is guaranteed to
   * be distinct from every variable thus far in the ExprManager, even
   * if it shares a name with another; this is to support any kind of
   * scoping policy on top of ExprManager.  The SymbolTable class
   * can be used to store and lookup symbols by name, if desired.
   *
   * @param name a name to associate to the fresh new bound variable
   * @param type the type for the new bound variable
   */
  Expr mkBoundVar(const std::string& name, Type type);

  /**
   * Create a (nameless) new, fresh variable for use in a binder
   * expression (the BOUND_VAR_LIST of a FORALL, EXISTS, or LAMBDA).
   * It is an error for this bound variable to exist outside of a
   * binder, and it should also only be used in a single binder
   * expression.  That is, two distinct FORALL expressions should use
   * entirely disjoint sets of bound variables (however, a single FORALL
   * expression can be used in multiple places in a formula without
   * a problem).  This newly-created bound variable is guaranteed to
   * be distinct from every variable thus far in the ExprManager.
   *
   * @param type the type for the new bound variable
   */
  Expr mkBoundVar(Type type);

  /** Get a reference to the statistics registry for this ExprManager */
  Statistics getStatistics() const throw();

  /** Get a reference to the statistics registry for this ExprManager */
  SExpr getStatistic(const std::string& name) const throw();

  /** Export an expr to a different ExprManager */
  //static Expr exportExpr(const Expr& e, ExprManager* em);
  /** Export a type to a different ExprManager */
  static Type exportType(const Type& t, ExprManager* em, ExprManagerMapCollection& vmap);

  /** Returns the minimum arity of the given kind. */
  static unsigned minArity(Kind kind);

  /** Returns the maximum arity of the given kind. */
  static unsigned maxArity(Kind kind);

};/* class ExprManager */

${mkConst_instantiations}

}/* CVC4 namespace */

#endif /* __CVC4__EXPR_MANAGER_H */
