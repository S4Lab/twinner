/*********************                                                        */
/** options.h
 **
 ** Copyright 2011-2013  New York University and The University of Iowa,
 ** and as below.
 **
 ** This file automatically generated by:
 **
 **     ../../../src/options/mkoptions ../../../src/options/base_options_template.h ../theory/arith/options.h
 **
 ** for the CVC4 project.
 **/

/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */

/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */
/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT EDIT ! */

/* Edit the template file instead.                     */

/*********************                                                        */
/*! \file base_options_template.h
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Contains code for handling command-line options.
 **
 ** Contains code for handling command-line options
 **/

#include <cvc4/cvc4_public.h>

#ifndef __CVC4__OPTIONS__ARITH_H
#define __CVC4__OPTIONS__ARITH_H

#include <cvc4/options/options.h>

#line 8 "../../../src/options/../theory/arith/options"
#include <cvc4/theory/arith/arith_unate_lemma_mode.h>
#line 11 "../../../src/options/../theory/arith/options"
#include <cvc4/theory/arith/arith_propagation_mode.h>
#line 28 "../../../src/options/../theory/arith/options"
#include <cvc4/theory/arith/arith_heuristic_pivot_rule.h>

#line 26 "../../../src/options/base_options_template.h"

#define CVC4_OPTIONS__ARITH__FOR_OPTION_HOLDER \
  arithUnateLemmaMode__option_t::type arithUnateLemmaMode; \
  bool arithUnateLemmaMode__setByUser__; \
  arithPropagationMode__option_t::type arithPropagationMode; \
  bool arithPropagationMode__setByUser__; \
  arithHeuristicPivots__option_t::type arithHeuristicPivots; \
  bool arithHeuristicPivots__setByUser__; \
  arithStandardCheckVarOrderPivots__option_t::type arithStandardCheckVarOrderPivots; \
  bool arithStandardCheckVarOrderPivots__setByUser__; \
  arithErrorSelectionRule__option_t::type arithErrorSelectionRule; \
  bool arithErrorSelectionRule__setByUser__; \
  arithSimplexCheckPeriod__option_t::type arithSimplexCheckPeriod; \
  bool arithSimplexCheckPeriod__setByUser__; \
  arithPivotThreshold__option_t::type arithPivotThreshold; \
  bool arithPivotThreshold__setByUser__; \
  arithPropagateMaxLength__option_t::type arithPropagateMaxLength; \
  bool arithPropagateMaxLength__setByUser__; \
  arithDioSolver__option_t::type arithDioSolver; \
  bool arithDioSolver__setByUser__; \
  arithRewriteEq__option_t::type arithRewriteEq; \
  bool arithRewriteEq__setByUser__; \
  arithMLTrick__option_t::type arithMLTrick; \
  bool arithMLTrick__setByUser__; \
  arithMLTrickSubstitutions__option_t::type arithMLTrickSubstitutions; \
  bool arithMLTrickSubstitutions__setByUser__; \
  doCutAllBounded__option_t::type doCutAllBounded; \
  bool doCutAllBounded__setByUser__; \
  maxCutsInContext__option_t::type maxCutsInContext; \
  bool maxCutsInContext__setByUser__; \
  revertArithModels__option_t::type revertArithModels; \
  bool revertArithModels__setByUser__; \
  havePenalties__option_t::type havePenalties; \
  bool havePenalties__setByUser__; \
  useFC__option_t::type useFC; \
  bool useFC__setByUser__; \
  useSOI__option_t::type useSOI; \
  bool useSOI__setByUser__; \
  restrictedPivots__option_t::type restrictedPivots; \
  bool restrictedPivots__setByUser__; \
  collectPivots__option_t::type collectPivots; \
  bool collectPivots__setByUser__; \
  fancyFinal__option_t::type fancyFinal; \
  bool fancyFinal__setByUser__; \
  exportDioDecompositions__option_t::type exportDioDecompositions; \
  bool exportDioDecompositions__setByUser__; \
  newProp__option_t::type newProp; \
  bool newProp__setByUser__; \
  arithPropAsLemmaLength__option_t::type arithPropAsLemmaLength; \
  bool arithPropAsLemmaLength__setByUser__; \
  soiQuickExplain__option_t::type soiQuickExplain; \
  bool soiQuickExplain__setByUser__; \
  rewriteDivk__option_t::type rewriteDivk; \
  bool rewriteDivk__setByUser__;

#line 30 "../../../src/options/base_options_template.h"

namespace CVC4 {

namespace options {


#line 8 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC arithUnateLemmaMode__option_t { typedef ArithUnateLemmaMode type; type operator()() const; bool wasSetByUser() const; } arithUnateLemmaMode CVC4_PUBLIC;
#line 11 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC arithPropagationMode__option_t { typedef ArithPropagationMode type; type operator()() const; bool wasSetByUser() const; } arithPropagationMode CVC4_PUBLIC;
#line 18 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC arithHeuristicPivots__option_t { typedef int16_t type; type operator()() const; bool wasSetByUser() const; void set(const type& v); } arithHeuristicPivots CVC4_PUBLIC;
#line 25 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC arithStandardCheckVarOrderPivots__option_t { typedef int16_t type; type operator()() const; bool wasSetByUser() const; void set(const type& v); } arithStandardCheckVarOrderPivots CVC4_PUBLIC;
#line 28 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC arithErrorSelectionRule__option_t { typedef ErrorSelectionRule type; type operator()() const; bool wasSetByUser() const; } arithErrorSelectionRule CVC4_PUBLIC;
#line 32 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC arithSimplexCheckPeriod__option_t { typedef uint16_t type; type operator()() const; bool wasSetByUser() const; } arithSimplexCheckPeriod CVC4_PUBLIC;
#line 39 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC arithPivotThreshold__option_t { typedef uint16_t type; type operator()() const; bool wasSetByUser() const; void set(const type& v); } arithPivotThreshold CVC4_PUBLIC;
#line 42 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC arithPropagateMaxLength__option_t { typedef uint16_t type; type operator()() const; bool wasSetByUser() const; } arithPropagateMaxLength CVC4_PUBLIC;
#line 45 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC arithDioSolver__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; } arithDioSolver CVC4_PUBLIC;
#line 50 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC arithRewriteEq__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; void set(const type& v); } arithRewriteEq CVC4_PUBLIC;
#line 55 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC arithMLTrick__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; } arithMLTrick CVC4_PUBLIC;
#line 59 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC arithMLTrickSubstitutions__option_t { typedef unsigned type; type operator()() const; bool wasSetByUser() const; } arithMLTrickSubstitutions CVC4_PUBLIC;
#line 62 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC doCutAllBounded__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; void set(const type& v); } doCutAllBounded CVC4_PUBLIC;
#line 66 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC maxCutsInContext__option_t { typedef unsigned type; type operator()() const; bool wasSetByUser() const; } maxCutsInContext CVC4_PUBLIC;
#line 69 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC revertArithModels__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; } revertArithModels CVC4_PUBLIC;
#line 72 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC havePenalties__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; void set(const type& v); } havePenalties CVC4_PUBLIC;
#line 76 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC useFC__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; void set(const type& v); } useFC CVC4_PUBLIC;
#line 79 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC useSOI__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; void set(const type& v); } useSOI CVC4_PUBLIC;
#line 82 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC restrictedPivots__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; void set(const type& v); } restrictedPivots CVC4_PUBLIC;
#line 85 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC collectPivots__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; void set(const type& v); } collectPivots CVC4_PUBLIC;
#line 88 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC fancyFinal__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; void set(const type& v); } fancyFinal CVC4_PUBLIC;
#line 91 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC exportDioDecompositions__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; void set(const type& v); } exportDioDecompositions CVC4_PUBLIC;
#line 94 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC newProp__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; void set(const type& v); } newProp CVC4_PUBLIC;
#line 97 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC arithPropAsLemmaLength__option_t { typedef uint16_t type; type operator()() const; bool wasSetByUser() const; void set(const type& v); } arithPropAsLemmaLength CVC4_PUBLIC;
#line 100 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC soiQuickExplain__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; void set(const type& v); } soiQuickExplain CVC4_PUBLIC;
#line 103 "../../../src/options/../theory/arith/options"
extern struct CVC4_PUBLIC rewriteDivk__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; } rewriteDivk CVC4_PUBLIC;

#line 38 "../../../src/options/base_options_template.h"

}/* CVC4::options namespace */


#line 8 "../../../src/options/../theory/arith/options"
template <> const options::arithUnateLemmaMode__option_t::type& Options::operator[](options::arithUnateLemmaMode__option_t) const;
#line 8 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::arithUnateLemmaMode__option_t) const;
#line 8 "../../../src/options/../theory/arith/options"
template <> void Options::assign(options::arithUnateLemmaMode__option_t, std::string option, std::string value, SmtEngine* smt);
#line 11 "../../../src/options/../theory/arith/options"
template <> const options::arithPropagationMode__option_t::type& Options::operator[](options::arithPropagationMode__option_t) const;
#line 11 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::arithPropagationMode__option_t) const;
#line 11 "../../../src/options/../theory/arith/options"
template <> void Options::assign(options::arithPropagationMode__option_t, std::string option, std::string value, SmtEngine* smt);
#line 18 "../../../src/options/../theory/arith/options"
template <> void Options::set(options::arithHeuristicPivots__option_t, const options::arithHeuristicPivots__option_t::type& x);
#line 18 "../../../src/options/../theory/arith/options"
template <> const options::arithHeuristicPivots__option_t::type& Options::operator[](options::arithHeuristicPivots__option_t) const;
#line 18 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::arithHeuristicPivots__option_t) const;
#line 18 "../../../src/options/../theory/arith/options"
template <> void Options::assign(options::arithHeuristicPivots__option_t, std::string option, std::string value, SmtEngine* smt);
#line 25 "../../../src/options/../theory/arith/options"
template <> void Options::set(options::arithStandardCheckVarOrderPivots__option_t, const options::arithStandardCheckVarOrderPivots__option_t::type& x);
#line 25 "../../../src/options/../theory/arith/options"
template <> const options::arithStandardCheckVarOrderPivots__option_t::type& Options::operator[](options::arithStandardCheckVarOrderPivots__option_t) const;
#line 25 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::arithStandardCheckVarOrderPivots__option_t) const;
#line 25 "../../../src/options/../theory/arith/options"
template <> void Options::assign(options::arithStandardCheckVarOrderPivots__option_t, std::string option, std::string value, SmtEngine* smt);
#line 28 "../../../src/options/../theory/arith/options"
template <> const options::arithErrorSelectionRule__option_t::type& Options::operator[](options::arithErrorSelectionRule__option_t) const;
#line 28 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::arithErrorSelectionRule__option_t) const;
#line 28 "../../../src/options/../theory/arith/options"
template <> void Options::assign(options::arithErrorSelectionRule__option_t, std::string option, std::string value, SmtEngine* smt);
#line 32 "../../../src/options/../theory/arith/options"
template <> const options::arithSimplexCheckPeriod__option_t::type& Options::operator[](options::arithSimplexCheckPeriod__option_t) const;
#line 32 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::arithSimplexCheckPeriod__option_t) const;
#line 32 "../../../src/options/../theory/arith/options"
template <> void Options::assign(options::arithSimplexCheckPeriod__option_t, std::string option, std::string value, SmtEngine* smt);
#line 39 "../../../src/options/../theory/arith/options"
template <> void Options::set(options::arithPivotThreshold__option_t, const options::arithPivotThreshold__option_t::type& x);
#line 39 "../../../src/options/../theory/arith/options"
template <> const options::arithPivotThreshold__option_t::type& Options::operator[](options::arithPivotThreshold__option_t) const;
#line 39 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::arithPivotThreshold__option_t) const;
#line 39 "../../../src/options/../theory/arith/options"
template <> void Options::assign(options::arithPivotThreshold__option_t, std::string option, std::string value, SmtEngine* smt);
#line 42 "../../../src/options/../theory/arith/options"
template <> const options::arithPropagateMaxLength__option_t::type& Options::operator[](options::arithPropagateMaxLength__option_t) const;
#line 42 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::arithPropagateMaxLength__option_t) const;
#line 42 "../../../src/options/../theory/arith/options"
template <> void Options::assign(options::arithPropagateMaxLength__option_t, std::string option, std::string value, SmtEngine* smt);
#line 45 "../../../src/options/../theory/arith/options"
template <> const options::arithDioSolver__option_t::type& Options::operator[](options::arithDioSolver__option_t) const;
#line 45 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::arithDioSolver__option_t) const;
#line 45 "../../../src/options/../theory/arith/options"
template <> void Options::assignBool(options::arithDioSolver__option_t, std::string option, bool value, SmtEngine* smt);
#line 50 "../../../src/options/../theory/arith/options"
template <> void Options::set(options::arithRewriteEq__option_t, const options::arithRewriteEq__option_t::type& x);
#line 50 "../../../src/options/../theory/arith/options"
template <> const options::arithRewriteEq__option_t::type& Options::operator[](options::arithRewriteEq__option_t) const;
#line 50 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::arithRewriteEq__option_t) const;
#line 50 "../../../src/options/../theory/arith/options"
template <> void Options::assignBool(options::arithRewriteEq__option_t, std::string option, bool value, SmtEngine* smt);
#line 55 "../../../src/options/../theory/arith/options"
template <> const options::arithMLTrick__option_t::type& Options::operator[](options::arithMLTrick__option_t) const;
#line 55 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::arithMLTrick__option_t) const;
#line 55 "../../../src/options/../theory/arith/options"
template <> void Options::assignBool(options::arithMLTrick__option_t, std::string option, bool value, SmtEngine* smt);
#line 59 "../../../src/options/../theory/arith/options"
template <> const options::arithMLTrickSubstitutions__option_t::type& Options::operator[](options::arithMLTrickSubstitutions__option_t) const;
#line 59 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::arithMLTrickSubstitutions__option_t) const;
#line 59 "../../../src/options/../theory/arith/options"
template <> void Options::assign(options::arithMLTrickSubstitutions__option_t, std::string option, std::string value, SmtEngine* smt);
#line 62 "../../../src/options/../theory/arith/options"
template <> void Options::set(options::doCutAllBounded__option_t, const options::doCutAllBounded__option_t::type& x);
#line 62 "../../../src/options/../theory/arith/options"
template <> const options::doCutAllBounded__option_t::type& Options::operator[](options::doCutAllBounded__option_t) const;
#line 62 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::doCutAllBounded__option_t) const;
#line 62 "../../../src/options/../theory/arith/options"
template <> void Options::assignBool(options::doCutAllBounded__option_t, std::string option, bool value, SmtEngine* smt);
#line 66 "../../../src/options/../theory/arith/options"
template <> const options::maxCutsInContext__option_t::type& Options::operator[](options::maxCutsInContext__option_t) const;
#line 66 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::maxCutsInContext__option_t) const;
#line 66 "../../../src/options/../theory/arith/options"
template <> void Options::assign(options::maxCutsInContext__option_t, std::string option, std::string value, SmtEngine* smt);
#line 69 "../../../src/options/../theory/arith/options"
template <> const options::revertArithModels__option_t::type& Options::operator[](options::revertArithModels__option_t) const;
#line 69 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::revertArithModels__option_t) const;
#line 69 "../../../src/options/../theory/arith/options"
template <> void Options::assignBool(options::revertArithModels__option_t, std::string option, bool value, SmtEngine* smt);
#line 72 "../../../src/options/../theory/arith/options"
template <> void Options::set(options::havePenalties__option_t, const options::havePenalties__option_t::type& x);
#line 72 "../../../src/options/../theory/arith/options"
template <> const options::havePenalties__option_t::type& Options::operator[](options::havePenalties__option_t) const;
#line 72 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::havePenalties__option_t) const;
#line 72 "../../../src/options/../theory/arith/options"
template <> void Options::assignBool(options::havePenalties__option_t, std::string option, bool value, SmtEngine* smt);
#line 76 "../../../src/options/../theory/arith/options"
template <> void Options::set(options::useFC__option_t, const options::useFC__option_t::type& x);
#line 76 "../../../src/options/../theory/arith/options"
template <> const options::useFC__option_t::type& Options::operator[](options::useFC__option_t) const;
#line 76 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::useFC__option_t) const;
#line 76 "../../../src/options/../theory/arith/options"
template <> void Options::assignBool(options::useFC__option_t, std::string option, bool value, SmtEngine* smt);
#line 79 "../../../src/options/../theory/arith/options"
template <> void Options::set(options::useSOI__option_t, const options::useSOI__option_t::type& x);
#line 79 "../../../src/options/../theory/arith/options"
template <> const options::useSOI__option_t::type& Options::operator[](options::useSOI__option_t) const;
#line 79 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::useSOI__option_t) const;
#line 79 "../../../src/options/../theory/arith/options"
template <> void Options::assignBool(options::useSOI__option_t, std::string option, bool value, SmtEngine* smt);
#line 82 "../../../src/options/../theory/arith/options"
template <> void Options::set(options::restrictedPivots__option_t, const options::restrictedPivots__option_t::type& x);
#line 82 "../../../src/options/../theory/arith/options"
template <> const options::restrictedPivots__option_t::type& Options::operator[](options::restrictedPivots__option_t) const;
#line 82 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::restrictedPivots__option_t) const;
#line 82 "../../../src/options/../theory/arith/options"
template <> void Options::assignBool(options::restrictedPivots__option_t, std::string option, bool value, SmtEngine* smt);
#line 85 "../../../src/options/../theory/arith/options"
template <> void Options::set(options::collectPivots__option_t, const options::collectPivots__option_t::type& x);
#line 85 "../../../src/options/../theory/arith/options"
template <> const options::collectPivots__option_t::type& Options::operator[](options::collectPivots__option_t) const;
#line 85 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::collectPivots__option_t) const;
#line 85 "../../../src/options/../theory/arith/options"
template <> void Options::assignBool(options::collectPivots__option_t, std::string option, bool value, SmtEngine* smt);
#line 88 "../../../src/options/../theory/arith/options"
template <> void Options::set(options::fancyFinal__option_t, const options::fancyFinal__option_t::type& x);
#line 88 "../../../src/options/../theory/arith/options"
template <> const options::fancyFinal__option_t::type& Options::operator[](options::fancyFinal__option_t) const;
#line 88 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::fancyFinal__option_t) const;
#line 88 "../../../src/options/../theory/arith/options"
template <> void Options::assignBool(options::fancyFinal__option_t, std::string option, bool value, SmtEngine* smt);
#line 91 "../../../src/options/../theory/arith/options"
template <> void Options::set(options::exportDioDecompositions__option_t, const options::exportDioDecompositions__option_t::type& x);
#line 91 "../../../src/options/../theory/arith/options"
template <> const options::exportDioDecompositions__option_t::type& Options::operator[](options::exportDioDecompositions__option_t) const;
#line 91 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::exportDioDecompositions__option_t) const;
#line 91 "../../../src/options/../theory/arith/options"
template <> void Options::assignBool(options::exportDioDecompositions__option_t, std::string option, bool value, SmtEngine* smt);
#line 94 "../../../src/options/../theory/arith/options"
template <> void Options::set(options::newProp__option_t, const options::newProp__option_t::type& x);
#line 94 "../../../src/options/../theory/arith/options"
template <> const options::newProp__option_t::type& Options::operator[](options::newProp__option_t) const;
#line 94 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::newProp__option_t) const;
#line 94 "../../../src/options/../theory/arith/options"
template <> void Options::assignBool(options::newProp__option_t, std::string option, bool value, SmtEngine* smt);
#line 97 "../../../src/options/../theory/arith/options"
template <> void Options::set(options::arithPropAsLemmaLength__option_t, const options::arithPropAsLemmaLength__option_t::type& x);
#line 97 "../../../src/options/../theory/arith/options"
template <> const options::arithPropAsLemmaLength__option_t::type& Options::operator[](options::arithPropAsLemmaLength__option_t) const;
#line 97 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::arithPropAsLemmaLength__option_t) const;
#line 97 "../../../src/options/../theory/arith/options"
template <> void Options::assign(options::arithPropAsLemmaLength__option_t, std::string option, std::string value, SmtEngine* smt);
#line 100 "../../../src/options/../theory/arith/options"
template <> void Options::set(options::soiQuickExplain__option_t, const options::soiQuickExplain__option_t::type& x);
#line 100 "../../../src/options/../theory/arith/options"
template <> const options::soiQuickExplain__option_t::type& Options::operator[](options::soiQuickExplain__option_t) const;
#line 100 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::soiQuickExplain__option_t) const;
#line 100 "../../../src/options/../theory/arith/options"
template <> void Options::assignBool(options::soiQuickExplain__option_t, std::string option, bool value, SmtEngine* smt);
#line 103 "../../../src/options/../theory/arith/options"
template <> const options::rewriteDivk__option_t::type& Options::operator[](options::rewriteDivk__option_t) const;
#line 103 "../../../src/options/../theory/arith/options"
template <> bool Options::wasSetByUser(options::rewriteDivk__option_t) const;
#line 103 "../../../src/options/../theory/arith/options"
template <> void Options::assignBool(options::rewriteDivk__option_t, std::string option, bool value, SmtEngine* smt);

#line 44 "../../../src/options/base_options_template.h"

namespace options {


#line 8 "../../../src/options/../theory/arith/options"
inline arithUnateLemmaMode__option_t::type arithUnateLemmaMode__option_t::operator()() const { return Options::current()[*this]; }
#line 8 "../../../src/options/../theory/arith/options"
inline bool arithUnateLemmaMode__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 11 "../../../src/options/../theory/arith/options"
inline arithPropagationMode__option_t::type arithPropagationMode__option_t::operator()() const { return Options::current()[*this]; }
#line 11 "../../../src/options/../theory/arith/options"
inline bool arithPropagationMode__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 18 "../../../src/options/../theory/arith/options"
inline arithHeuristicPivots__option_t::type arithHeuristicPivots__option_t::operator()() const { return Options::current()[*this]; }
#line 18 "../../../src/options/../theory/arith/options"
inline bool arithHeuristicPivots__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }
#line 18 "../../../src/options/../theory/arith/options"
inline void arithHeuristicPivots__option_t::set(const arithHeuristicPivots__option_t::type& v) { Options::current().set(*this, v); }

#line 25 "../../../src/options/../theory/arith/options"
inline arithStandardCheckVarOrderPivots__option_t::type arithStandardCheckVarOrderPivots__option_t::operator()() const { return Options::current()[*this]; }
#line 25 "../../../src/options/../theory/arith/options"
inline bool arithStandardCheckVarOrderPivots__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }
#line 25 "../../../src/options/../theory/arith/options"
inline void arithStandardCheckVarOrderPivots__option_t::set(const arithStandardCheckVarOrderPivots__option_t::type& v) { Options::current().set(*this, v); }

#line 28 "../../../src/options/../theory/arith/options"
inline arithErrorSelectionRule__option_t::type arithErrorSelectionRule__option_t::operator()() const { return Options::current()[*this]; }
#line 28 "../../../src/options/../theory/arith/options"
inline bool arithErrorSelectionRule__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 32 "../../../src/options/../theory/arith/options"
inline arithSimplexCheckPeriod__option_t::type arithSimplexCheckPeriod__option_t::operator()() const { return Options::current()[*this]; }
#line 32 "../../../src/options/../theory/arith/options"
inline bool arithSimplexCheckPeriod__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 39 "../../../src/options/../theory/arith/options"
inline arithPivotThreshold__option_t::type arithPivotThreshold__option_t::operator()() const { return Options::current()[*this]; }
#line 39 "../../../src/options/../theory/arith/options"
inline bool arithPivotThreshold__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }
#line 39 "../../../src/options/../theory/arith/options"
inline void arithPivotThreshold__option_t::set(const arithPivotThreshold__option_t::type& v) { Options::current().set(*this, v); }

#line 42 "../../../src/options/../theory/arith/options"
inline arithPropagateMaxLength__option_t::type arithPropagateMaxLength__option_t::operator()() const { return Options::current()[*this]; }
#line 42 "../../../src/options/../theory/arith/options"
inline bool arithPropagateMaxLength__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 45 "../../../src/options/../theory/arith/options"
inline arithDioSolver__option_t::type arithDioSolver__option_t::operator()() const { return Options::current()[*this]; }
#line 45 "../../../src/options/../theory/arith/options"
inline bool arithDioSolver__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 50 "../../../src/options/../theory/arith/options"
inline arithRewriteEq__option_t::type arithRewriteEq__option_t::operator()() const { return Options::current()[*this]; }
#line 50 "../../../src/options/../theory/arith/options"
inline bool arithRewriteEq__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }
#line 50 "../../../src/options/../theory/arith/options"
inline void arithRewriteEq__option_t::set(const arithRewriteEq__option_t::type& v) { Options::current().set(*this, v); }

#line 55 "../../../src/options/../theory/arith/options"
inline arithMLTrick__option_t::type arithMLTrick__option_t::operator()() const { return Options::current()[*this]; }
#line 55 "../../../src/options/../theory/arith/options"
inline bool arithMLTrick__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 59 "../../../src/options/../theory/arith/options"
inline arithMLTrickSubstitutions__option_t::type arithMLTrickSubstitutions__option_t::operator()() const { return Options::current()[*this]; }
#line 59 "../../../src/options/../theory/arith/options"
inline bool arithMLTrickSubstitutions__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 62 "../../../src/options/../theory/arith/options"
inline doCutAllBounded__option_t::type doCutAllBounded__option_t::operator()() const { return Options::current()[*this]; }
#line 62 "../../../src/options/../theory/arith/options"
inline bool doCutAllBounded__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }
#line 62 "../../../src/options/../theory/arith/options"
inline void doCutAllBounded__option_t::set(const doCutAllBounded__option_t::type& v) { Options::current().set(*this, v); }

#line 66 "../../../src/options/../theory/arith/options"
inline maxCutsInContext__option_t::type maxCutsInContext__option_t::operator()() const { return Options::current()[*this]; }
#line 66 "../../../src/options/../theory/arith/options"
inline bool maxCutsInContext__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 69 "../../../src/options/../theory/arith/options"
inline revertArithModels__option_t::type revertArithModels__option_t::operator()() const { return Options::current()[*this]; }
#line 69 "../../../src/options/../theory/arith/options"
inline bool revertArithModels__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 72 "../../../src/options/../theory/arith/options"
inline havePenalties__option_t::type havePenalties__option_t::operator()() const { return Options::current()[*this]; }
#line 72 "../../../src/options/../theory/arith/options"
inline bool havePenalties__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }
#line 72 "../../../src/options/../theory/arith/options"
inline void havePenalties__option_t::set(const havePenalties__option_t::type& v) { Options::current().set(*this, v); }

#line 76 "../../../src/options/../theory/arith/options"
inline useFC__option_t::type useFC__option_t::operator()() const { return Options::current()[*this]; }
#line 76 "../../../src/options/../theory/arith/options"
inline bool useFC__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }
#line 76 "../../../src/options/../theory/arith/options"
inline void useFC__option_t::set(const useFC__option_t::type& v) { Options::current().set(*this, v); }

#line 79 "../../../src/options/../theory/arith/options"
inline useSOI__option_t::type useSOI__option_t::operator()() const { return Options::current()[*this]; }
#line 79 "../../../src/options/../theory/arith/options"
inline bool useSOI__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }
#line 79 "../../../src/options/../theory/arith/options"
inline void useSOI__option_t::set(const useSOI__option_t::type& v) { Options::current().set(*this, v); }

#line 82 "../../../src/options/../theory/arith/options"
inline restrictedPivots__option_t::type restrictedPivots__option_t::operator()() const { return Options::current()[*this]; }
#line 82 "../../../src/options/../theory/arith/options"
inline bool restrictedPivots__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }
#line 82 "../../../src/options/../theory/arith/options"
inline void restrictedPivots__option_t::set(const restrictedPivots__option_t::type& v) { Options::current().set(*this, v); }

#line 85 "../../../src/options/../theory/arith/options"
inline collectPivots__option_t::type collectPivots__option_t::operator()() const { return Options::current()[*this]; }
#line 85 "../../../src/options/../theory/arith/options"
inline bool collectPivots__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }
#line 85 "../../../src/options/../theory/arith/options"
inline void collectPivots__option_t::set(const collectPivots__option_t::type& v) { Options::current().set(*this, v); }

#line 88 "../../../src/options/../theory/arith/options"
inline fancyFinal__option_t::type fancyFinal__option_t::operator()() const { return Options::current()[*this]; }
#line 88 "../../../src/options/../theory/arith/options"
inline bool fancyFinal__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }
#line 88 "../../../src/options/../theory/arith/options"
inline void fancyFinal__option_t::set(const fancyFinal__option_t::type& v) { Options::current().set(*this, v); }

#line 91 "../../../src/options/../theory/arith/options"
inline exportDioDecompositions__option_t::type exportDioDecompositions__option_t::operator()() const { return Options::current()[*this]; }
#line 91 "../../../src/options/../theory/arith/options"
inline bool exportDioDecompositions__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }
#line 91 "../../../src/options/../theory/arith/options"
inline void exportDioDecompositions__option_t::set(const exportDioDecompositions__option_t::type& v) { Options::current().set(*this, v); }

#line 94 "../../../src/options/../theory/arith/options"
inline newProp__option_t::type newProp__option_t::operator()() const { return Options::current()[*this]; }
#line 94 "../../../src/options/../theory/arith/options"
inline bool newProp__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }
#line 94 "../../../src/options/../theory/arith/options"
inline void newProp__option_t::set(const newProp__option_t::type& v) { Options::current().set(*this, v); }

#line 97 "../../../src/options/../theory/arith/options"
inline arithPropAsLemmaLength__option_t::type arithPropAsLemmaLength__option_t::operator()() const { return Options::current()[*this]; }
#line 97 "../../../src/options/../theory/arith/options"
inline bool arithPropAsLemmaLength__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }
#line 97 "../../../src/options/../theory/arith/options"
inline void arithPropAsLemmaLength__option_t::set(const arithPropAsLemmaLength__option_t::type& v) { Options::current().set(*this, v); }

#line 100 "../../../src/options/../theory/arith/options"
inline soiQuickExplain__option_t::type soiQuickExplain__option_t::operator()() const { return Options::current()[*this]; }
#line 100 "../../../src/options/../theory/arith/options"
inline bool soiQuickExplain__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }
#line 100 "../../../src/options/../theory/arith/options"
inline void soiQuickExplain__option_t::set(const soiQuickExplain__option_t::type& v) { Options::current().set(*this, v); }

#line 103 "../../../src/options/../theory/arith/options"
inline rewriteDivk__option_t::type rewriteDivk__option_t::operator()() const { return Options::current()[*this]; }
#line 103 "../../../src/options/../theory/arith/options"
inline bool rewriteDivk__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 50 "../../../src/options/base_options_template.h"

}/* CVC4::options namespace */

}/* CVC4 namespace */

#endif /* __CVC4__OPTIONS__ARITH_H */
