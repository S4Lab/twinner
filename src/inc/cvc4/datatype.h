/*********************                                                        */
/*! \file datatype.h
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: none
 ** Minor contributors (to current version): Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief A class representing a Datatype definition
 **
 ** A class representing a Datatype definition for the theory of
 ** inductive datatypes.
 **/

#include "cvc4_public.h"

#ifndef __CVC4__DATATYPE_H
#define __CVC4__DATATYPE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace CVC4 {
  // messy; Expr needs Datatype (because it's the payload of a
  // CONSTANT-kinded expression), and Datatype needs Expr.
  class CVC4_PUBLIC Datatype;
}/* CVC4 namespace */

#include "expr/expr.h"
#include "expr/type.h"
#include "util/hash.h"
#include "util/exception.h"

namespace CVC4 {

class CVC4_PUBLIC ExprManager;

/**
 * An exception that is thrown when a datatype resolution fails.
 */
class CVC4_PUBLIC DatatypeResolutionException : public Exception {
public:
  inline DatatypeResolutionException(std::string msg);
};/* class DatatypeResolutionException */

/**
 * A holder type (used in calls to DatatypeConstructor::addArg())
 * to allow a Datatype to refer to itself.  Self-typed fields of
 * Datatypes will be properly typed when a Type is created for the
 * Datatype by the ExprManager (which calls Datatype::resolve()).
 */
class CVC4_PUBLIC DatatypeSelfType {
};/* class DatatypeSelfType */

/**
 * An unresolved type (used in calls to
 * DatatypeConstructor::addArg()) to allow a Datatype to refer to
 * itself or to other mutually-recursive Datatypes.  Unresolved-type
 * fields of Datatypes will be properly typed when a Type is created
 * for the Datatype by the ExprManager (which calls
 * Datatype::resolve()).
 */
class CVC4_PUBLIC DatatypeUnresolvedType {
  std::string d_name;
public:
  inline DatatypeUnresolvedType(std::string name);
  inline std::string getName() const throw();
};/* class DatatypeUnresolvedType */

/**
 * A Datatype constructor argument (i.e., a Datatype field).
 */
class CVC4_PUBLIC DatatypeConstructorArg {

  std::string d_name;
  Expr d_selector;
  /** the constructor associated with this selector */
  Expr d_constructor;
  bool d_resolved;

  DatatypeConstructorArg(std::string name, Expr selector);
  friend class DatatypeConstructor;
  friend class Datatype;

  bool isUnresolvedSelf() const throw();

public:

  /** Get the name of this constructor argument. */
  std::string getName() const throw();

  /**
   * Get the selector for this constructor argument; this call is
   * only permitted after resolution.
   */
  Expr getSelector() const;

  /**
   * Get the associated constructor for this constructor argument;
   * this call is only permitted after resolution.
   */
  Expr getConstructor() const;

  /**
   * Get the type of the selector for this constructor argument;
   * this call is only permitted after resolution.
   */
  SelectorType getType() const;

  /**
   * Get the name of the type of this constructor argument
   * (Datatype field).  Can be used for not-yet-resolved Datatypes
   * (in which case the name of the unresolved type, or "[self]"
   * for a self-referential type is returned).
   */
  std::string getTypeName() const;

  /**
   * Returns true iff this constructor argument has been resolved.
   */
  bool isResolved() const throw();

};/* class DatatypeConstructorArg */

/**
 * A constructor for a Datatype.
 */
class CVC4_PUBLIC DatatypeConstructor {
public:

  /** The type for iterators over constructor arguments. */
  typedef std::vector<DatatypeConstructorArg>::iterator iterator;
  /** The (const) type for iterators over constructor arguments. */
  typedef std::vector<DatatypeConstructorArg>::const_iterator const_iterator;

private:

  std::string d_name;
  Expr d_constructor;
  Expr d_tester;
  std::vector<DatatypeConstructorArg> d_args;

  void resolve(ExprManager* em, DatatypeType self,
               const std::map<std::string, DatatypeType>& resolutions,
               const std::vector<Type>& placeholders,
               const std::vector<Type>& replacements,
               const std::vector< SortConstructorType >& paramTypes,
               const std::vector< DatatypeType >& paramReplacements)
    throw(IllegalArgumentException, DatatypeResolutionException);
  friend class Datatype;

  /** Helper function for resolving parametric datatypes.
      This replaces instances of the SortConstructorType produced for unresolved
      parametric datatypes, with the corresponding resolved DatatypeType.  For example, take
      the parametric definition of a list, list[T] = cons(car : T, cdr : list[T]) | null.
      If "range" is the unresolved parametric datatype:
        DATATYPE list = cons(car: SORT_TAG_1, cdr: SORT_TAG_2(SORT_TAG_1)) | null END;,
      this function will return the resolved type:
        DATATYPE list = cons(car: SORT_TAG_1, cdr: (list PARAMETERIC_DATATYPE SORT_TAG_1)) | null END;
    */
  Type doParametricSubstitution(Type range,
                                const std::vector< SortConstructorType >& paramTypes,
                                const std::vector< DatatypeType >& paramReplacements);
public:
  /**
   * Create a new Datatype constructor with the given name for the
   * constructor and the same name (prefixed with "is_") for the
   * tester.  The actual constructor and tester (meaning, the Exprs
   * representing operators for these entities) aren't created until
   * resolution time.
   */
  explicit DatatypeConstructor(std::string name);

  /**
   * Create a new Datatype constructor with the given name for the
   * constructor and the given name for the tester.  The actual
   * constructor and tester aren't created until resolution time.
   */
  DatatypeConstructor(std::string name, std::string tester);

  /**
   * Add an argument (i.e., a data field) of the given name and type
   * to this Datatype constructor.  Selector names need not be unique;
   * they are for convenience and pretty-printing only.
   */
  void addArg(std::string selectorName, Type selectorType);

  /**
   * Add an argument (i.e., a data field) of the given name to this
   * Datatype constructor that refers to an as-yet-unresolved
   * Datatype (which may be mutually-recursive).  Selector names need
   * not be unique; they are for convenience and pretty-printing only.
   */
  void addArg(std::string selectorName, DatatypeUnresolvedType selectorType);

  /**
   * Add a self-referential (i.e., a data field) of the given name
   * to this Datatype constructor that refers to the enclosing
   * Datatype.  For example, using the familiar "nat" Datatype, to
   * create the "pred" field for "succ" constructor, one uses
   * succ::addArg("pred", DatatypeSelfType())---the actual Type
   * cannot be passed because the Datatype is still under
   * construction.  Selector names need not be unique; they are for
   * convenience and pretty-printing only.
   *
   * This is a special case of
   * DatatypeConstructor::addArg(std::string, DatatypeUnresolvedType).
   */
  void addArg(std::string selectorName, DatatypeSelfType);

  /** Get the name of this Datatype constructor. */
  std::string getName() const throw();

  /**
   * Get the constructor operator of this Datatype constructor.  The
   * Datatype must be resolved.
   */
  Expr getConstructor() const;

  /**
   * Get the tester operator of this Datatype constructor.  The
   * Datatype must be resolved.
   */
  Expr getTester() const;

  /**
   * Get the tester name for this Datatype constructor.
   */
  std::string getTesterName() const throw();

  /**
   * Get the number of arguments (so far) of this Datatype constructor.
   */
  inline size_t getNumArgs() const throw();

  /**
   * Get the specialized constructor type for a parametric
   * constructor; this call is only permitted after resolution.
   * Given a (concrete) returnType, the constructor's concrete
   * type in this parametric datatype is returned.
   *
   * For instance, if the datatype is list[T], with constructor
   * "cons[T]" of type "T -> list[T] -> list[T]", then calling
   * this function with "list[int]" will return the concrete
   * "cons" constructor type for lists of int---namely,
   * "int -> list[int] -> list[int]".
   */
  Type getSpecializedConstructorType(Type returnType) const;

  /**
   * Return the cardinality of this constructor (the product of the
   * cardinalities of its arguments).
   */
  Cardinality getCardinality() const throw(IllegalArgumentException);

  /**
   * Return true iff this constructor is finite (it is nullary or
   * each of its argument types are finite).  This function can
   * only be called for resolved constructors.
   */
  bool isFinite() const throw(IllegalArgumentException);

  /**
   * Return true iff this constructor is well-founded (there exist
   * ground terms).  The constructor must be resolved or an
   * exception is thrown.
   */
  bool isWellFounded() const throw(IllegalArgumentException);

  /**
   * Construct and return a ground term of this constructor.  The
   * constructor must be both resolved and well-founded, or else an
   * exception is thrown.
   */
  Expr mkGroundTerm( Type t ) const throw(IllegalArgumentException);

  /**
   * Returns true iff this Datatype constructor has already been
   * resolved.
   */
  inline bool isResolved() const throw();

  /** Get the beginning iterator over DatatypeConstructor args. */
  inline iterator begin() throw();
  /** Get the ending iterator over DatatypeConstructor args. */
  inline iterator end() throw();
  /** Get the beginning const_iterator over DatatypeConstructor args. */
  inline const_iterator begin() const throw();
  /** Get the ending const_iterator over DatatypeConstructor args. */
  inline const_iterator end() const throw();

  /** Get the ith DatatypeConstructor arg. */
  const DatatypeConstructorArg& operator[](size_t index) const;

  /**
   * Get the DatatypeConstructor arg named.  This is a linear search
   * through the arguments, so in the case of multiple,
   * similarly-named arguments, the first is returned.
   */
  const DatatypeConstructorArg& operator[](std::string name) const;

  /**
   * Get the selector named.  This is a linear search
   * through the arguments, so in the case of multiple,
   * similarly-named arguments, the selector for the first
   * is returned.
   */
  Expr getSelector(std::string name) const;

  /**
   * Get whether this datatype involves an external type.  If so,
   * then we will pose additional requirements for sharing.
   */
  bool involvesExternalType() const;

};/* class DatatypeConstructor */

/**
 * The representation of an inductive datatype.
 *
 * This is far more complicated than it first seems.  Consider this
 * datatype definition:
 *
 *   DATATYPE nat =
 *     succ(pred: nat)
 *   | zero
 *   END;
 *
 * You cannot define "nat" until you have a Type for it, but you
 * cannot have a Type for it until you fill in the type of the "pred"
 * selector, which needs the Type.  So we have a chicken-and-egg
 * problem.  It's even more complicated when we have mutual recursion
 * between datatypes, since the CVC presentation language does not
 * require forward-declarations.  Here, we define trees of lists that
 * contain trees of lists (etc):
 *
 *   DATATYPE
 *     tree = node(left: tree, right: tree) | leaf(list),
 *     list = cons(car: tree, cdr: list) | nil
 *   END;
 *
 * Note that while parsing the "tree" definition, we have to take it
 * on faith that "list" is a valid type.  We build Datatype objects to
 * describe "tree" and "list", and their constructors and constructor
 * arguments, but leave any unknown types (including self-references)
 * in an "unresolved" state.  After parsing the whole DATATYPE block,
 * we create a DatatypeType through
 * ExprManager::mkMutualDatatypeTypes().  The ExprManager creates a
 * DatatypeType for each, but before "releasing" this type into the
 * wild, it does a round of in-place "resolution" on each Datatype by
 * calling Datatype::resolve() with a map of string -> DatatypeType to
 * allow the datatype to construct the necessary testers and
 * selectors.
 *
 * An additional point to make is that we want to ease the burden on
 * both the parser AND the users of the CVC4 API, so this class takes
 * on the task of generating its own selectors and testers, for
 * instance.  That means that, after reifying the Datatype with the
 * ExprManager, the parser needs to go through the (now-resolved)
 * Datatype and request the constructor, selector, and tester terms.
 * See src/parser/parser.cpp for how this is done.  For API usage
 * ideas, see test/unit/util/datatype_black.h.
 *
 * Datatypes may also be defined parametrically, such as this example:
 *
 *  DATATYPE
 *    list[T] = cons(car : T, cdr : list[T]) | null,
 *    tree = node(children : list[tree]) | leaf
 *  END;
 *
 * Here, the definition of the parametric datatype list, where T is a type variable.
 * In other words, this defines a family of types list[C] where C is any concrete
 * type.  Datatypes can be parameterized over multiple type variables using the
 * syntax sym[ T1, ..., Tn ] = ...,
 *
 */
class CVC4_PUBLIC Datatype {
public:
  /**
   * Get the datatype of a constructor, selector, or tester operator.
   */
  static const Datatype& datatypeOf(Expr item) CVC4_PUBLIC;

  /**
   * Get the index of a constructor or tester in its datatype, or the
   * index of a selector in its constructor.  (Zero is always the
   * first index.)
   */
  static size_t indexOf(Expr item) CVC4_PUBLIC;

  /** The type for iterators over constructors. */
  typedef std::vector<DatatypeConstructor>::iterator iterator;
  /** The (const) type for iterators over constructors. */
  typedef std::vector<DatatypeConstructor>::const_iterator const_iterator;

private:
  std::string d_name;
  std::vector<Type> d_params;
  std::vector<DatatypeConstructor> d_constructors;
  bool d_resolved;
  Type d_self;

  // "mutable" because computing the cardinality can be expensive,
  // and so it's computed just once, on demand---this is the cache
  mutable Cardinality d_card;

  /**
   * Datatypes refer to themselves, recursively, and we have a
   * chicken-and-egg problem.  The DatatypeType around the Datatype
   * cannot exist until the Datatype is finalized, and the Datatype
   * cannot refer to the DatatypeType representing itself until it
   * exists.  resolve() is called by the ExprManager when a Type is
   * ultimately requested of the Datatype specification (that is, when
   * ExprManager::mkDatatypeType() or ExprManager::mkMutualDatatypeTypes()
   * is called).  Has the effect of freezing the object, too; that is,
   * addConstructor() will fail after a call to resolve().
   *
   * The basic goal of resolution is to assign constructors, selectors,
   * and testers.  To do this, any UnresolvedType/SelfType references
   * must be cleared up.  This is the purpose of the "resolutions" map;
   * it includes any mutually-recursive datatypes that are currently
   * under resolution.  The four vectors come in two pairs (so, really
   * they are two maps).  placeholders->replacements give type variables
   * that should be resolved in the case of parametric datatypes.
   *
   * @param em the ExprManager at play
   * @param resolutions a map of strings to DatatypeTypes currently under resolution
   * @param placeholders the types in these Datatypes under resolution that must be replaced
   * @param replacements the corresponding replacements
   * @param paramTypes the sort constructors in these Datatypes under resolution that must be replaced
   * @param paramReplacements the corresponding (parametric) DatatypeTypes
   */
  void resolve(ExprManager* em,
               const std::map<std::string, DatatypeType>& resolutions,
               const std::vector<Type>& placeholders,
               const std::vector<Type>& replacements,
               const std::vector< SortConstructorType >& paramTypes,
               const std::vector< DatatypeType >& paramReplacements)
    throw(IllegalArgumentException, DatatypeResolutionException);
  friend class ExprManager;// for access to resolve()

public:

  /** Create a new Datatype of the given name. */
  inline explicit Datatype(std::string name);

  /**
   * Create a new Datatype of the given name, with the given
   * parameterization.
   */
  inline Datatype(std::string name, const std::vector<Type>& params);

  /**
   * Add a constructor to this Datatype.  Constructor names need not
   * be unique; they are for convenience and pretty-printing only.
   */
  void addConstructor(const DatatypeConstructor& c);

  /** Get the name of this Datatype. */
  inline std::string getName() const throw();

  /** Get the number of constructors (so far) for this Datatype. */
  inline size_t getNumConstructors() const throw();

  /** Is this datatype parametric? */
  inline bool isParametric() const throw();

  /** Get the nubmer of type parameters */
  inline size_t getNumParameters() const throw();

  /** Get parameter */
  inline Type getParameter( unsigned int i ) const;

  /** Get parameters */
  inline std::vector<Type> getParameters() const;

  /**
   * Return the cardinality of this datatype (the sum of the
   * cardinalities of its constructors).  The Datatype must be
   * resolved.
   */
  Cardinality getCardinality() const throw(IllegalArgumentException);

  /**
   * Return  true iff this  Datatype is  finite (all  constructors are
   * finite,  i.e., there  are finitely  many ground  terms).   If the
   * datatype is  not well-founded, this function  returns false.  The
   * Datatype must be resolved or an exception is thrown.
   */
  bool isFinite() const throw(IllegalArgumentException);

  /**
   * Return true iff this datatype is well-founded (there exist ground
   * terms).  The Datatype must be resolved or an exception is thrown.
   */
  bool isWellFounded() const throw(IllegalArgumentException);

  /**
   * Construct and return a ground term of this Datatype.  The
   * Datatype must be both resolved and well-founded, or else an
   * exception is thrown.
   */
  Expr mkGroundTerm( Type t ) const throw(IllegalArgumentException);

  /**
   * Get the DatatypeType associated to this Datatype.  Can only be
   * called post-resolution.
   */
  DatatypeType getDatatypeType() const throw(IllegalArgumentException);

  /**
   * Get the DatatypeType associated to this (parameterized) Datatype.  Can only be
   * called post-resolution.
   */
  DatatypeType getDatatypeType(const std::vector<Type>& params) const throw(IllegalArgumentException);

  /**
   * Return true iff the two Datatypes are the same.
   *
   * We need == for mkConst(Datatype) to properly work---since if the
   * Datatype Expr requested is the same as an already-existing one,
   * we need to return that one.  For that, we have a hash and
   * operator==.  We provide != for symmetry.  We don't provide
   * operator<() etc. because given two Datatype Exprs, you could
   * simply compare those rather than the (bare) Datatypes.  This
   * means, though, that Datatype cannot be stored in a sorted list or
   * RB tree directly, so maybe we can consider adding these
   * comparison operators later on.
   */
  bool operator==(const Datatype& other) const throw();
  /** Return true iff the two Datatypes are not the same. */
  inline bool operator!=(const Datatype& other) const throw();

  /** Return true iff this Datatype has already been resolved. */
  inline bool isResolved() const throw();

  /** Get the beginning iterator over DatatypeConstructors. */
  inline std::vector<DatatypeConstructor>::iterator begin() throw();
  /** Get the ending iterator over DatatypeConstructors. */
  inline std::vector<DatatypeConstructor>::iterator end() throw();
  /** Get the beginning const_iterator over DatatypeConstructors. */
  inline std::vector<DatatypeConstructor>::const_iterator begin() const throw();
  /** Get the ending const_iterator over DatatypeConstructors. */
  inline std::vector<DatatypeConstructor>::const_iterator end() const throw();

  /** Get the ith DatatypeConstructor. */
  const DatatypeConstructor& operator[](size_t index) const;

  /**
   * Get the DatatypeConstructor named.  This is a linear search
   * through the constructors, so in the case of multiple,
   * similarly-named constructors, the first is returned.
   */
  const DatatypeConstructor& operator[](std::string name) const;

  /**
   * Get the constructor operator for the named constructor.
   * This is a linear search through the constructors, so in
   * the case of multiple, similarly-named constructors, the
   * first is returned.
   *
   * This Datatype must be resolved.
   */
  Expr getConstructor(std::string name) const;

  /**
   * Get whether this datatype involves an external type.  If so,
   * then we will pose additional requirements for sharing.
   */
  bool involvesExternalType() const;

};/* class Datatype */

/**
 * A hash function for Datatypes.  Needed to store them in hash sets
 * and hash maps.
 */
struct CVC4_PUBLIC DatatypeHashFunction {
  inline size_t operator()(const Datatype& dt) const {
    return StringHashFunction()(dt.getName());
  }
  inline size_t operator()(const Datatype* dt) const {
    return StringHashFunction()(dt->getName());
  }
  inline size_t operator()(const DatatypeConstructor& dtc) const {
    return StringHashFunction()(dtc.getName());
  }
  inline size_t operator()(const DatatypeConstructor* dtc) const {
    return StringHashFunction()(dtc->getName());
  }
};/* struct DatatypeHashFunction */

// FUNCTION DECLARATIONS FOR OUTPUT STREAMS

std::ostream& operator<<(std::ostream& os, const Datatype& dt) CVC4_PUBLIC;
std::ostream& operator<<(std::ostream& os, const DatatypeConstructor& ctor) CVC4_PUBLIC;
std::ostream& operator<<(std::ostream& os, const DatatypeConstructorArg& arg) CVC4_PUBLIC;

// INLINE FUNCTIONS

inline DatatypeResolutionException::DatatypeResolutionException(std::string msg) :
  Exception(msg) {
}

inline DatatypeUnresolvedType::DatatypeUnresolvedType(std::string name) :
  d_name(name) {
}

inline std::string DatatypeUnresolvedType::getName() const throw() {
  return d_name;
}

inline Datatype::Datatype(std::string name) :
  d_name(name),
  d_params(),
  d_constructors(),
  d_resolved(false),
  d_self(),
  d_card(CardinalityUnknown()) {
}

inline Datatype::Datatype(std::string name, const std::vector<Type>& params) :
  d_name(name),
  d_params(params),
  d_constructors(),
  d_resolved(false),
  d_self(),
  d_card(CardinalityUnknown()) {
}

inline std::string Datatype::getName() const throw() {
  return d_name;
}

inline size_t Datatype::getNumConstructors() const throw() {
  return d_constructors.size();
}

inline bool Datatype::isParametric() const throw() {
  return d_params.size() > 0;
}

inline size_t Datatype::getNumParameters() const throw() {
  return d_params.size();
}

inline Type Datatype::getParameter( unsigned int i ) const {
  CheckArgument(isParametric(), this, "cannot get type parameter of a non-parametric datatype");
  CheckArgument(i < d_params.size(), i, "type parameter index out of range for datatype");
  return d_params[i];
}

inline std::vector<Type> Datatype::getParameters() const {
  CheckArgument(isParametric(), this, "cannot get type parameters of a non-parametric datatype");
  return d_params;
}

inline bool Datatype::operator!=(const Datatype& other) const throw() {
  return !(*this == other);
}

inline bool Datatype::isResolved() const throw() {
  return d_resolved;
}

inline Datatype::iterator Datatype::begin() throw() {
  return d_constructors.begin();
}

inline Datatype::iterator Datatype::end() throw() {
  return d_constructors.end();
}

inline Datatype::const_iterator Datatype::begin() const throw() {
  return d_constructors.begin();
}

inline Datatype::const_iterator Datatype::end() const throw() {
  return d_constructors.end();
}

inline bool DatatypeConstructor::isResolved() const throw() {
  return !d_tester.isNull();
}

inline size_t DatatypeConstructor::getNumArgs() const throw() {
  return d_args.size();
}

inline bool DatatypeConstructorArg::isResolved() const throw() {
  // We could just write:
  //
  //   return !d_selector.isNull() && d_selector.getType().isSelector();
  //
  // HOWEVER, this causes problems in ExprManager tear-down, because
  // the attributes are removed before the pool is purged.  When the
  // pool is purged, this triggers an equality test between Datatypes,
  // and this triggers a call to isResolved(), which breaks because
  // d_selector has no type after attributes are stripped.
  //
  // This problem, coupled with the fact that this function is called
  // _often_, means we should just use a boolean flag.
  //
  return d_resolved;
}

inline DatatypeConstructor::iterator DatatypeConstructor::begin() throw() {
  return d_args.begin();
}

inline DatatypeConstructor::iterator DatatypeConstructor::end() throw() {
  return d_args.end();
}

inline DatatypeConstructor::const_iterator DatatypeConstructor::begin() const throw() {
  return d_args.begin();
}

inline DatatypeConstructor::const_iterator DatatypeConstructor::end() const throw() {
  return d_args.end();
}

}/* CVC4 namespace */

#endif /* __CVC4__DATATYPE_H */
