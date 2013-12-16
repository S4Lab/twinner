/*********************                                                        */
/** options.h
 **
 ** Copyright 2011-2013  New York University and The University of Iowa,
 ** and as below.
 **
 ** This file automatically generated by:
 **
 **     ../../../src/options/mkoptions ../../../src/options/base_options_template.h ../theory/rewriterules/options.h
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

#ifndef __CVC4__OPTIONS__REWRITE_RULES_H
#define __CVC4__OPTIONS__REWRITE_RULES_H

#include <cvc4/options/options.h>


#line 26 "../../../src/options/base_options_template.h"

#define CVC4_OPTIONS__REWRITE_RULES__FOR_OPTION_HOLDER \
  efficientEMatching__option_t::type efficientEMatching; \
  bool efficientEMatching__setByUser__; \
  rewriteRulesAsAxioms__option_t::type rewriteRulesAsAxioms; \
  bool rewriteRulesAsAxioms__setByUser__;

#line 30 "../../../src/options/base_options_template.h"

namespace CVC4 {

namespace options {


#line 8 "../../../src/options/../theory/rewriterules/options"
extern struct CVC4_PUBLIC efficientEMatching__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; } efficientEMatching CVC4_PUBLIC;
#line 11 "../../../src/options/../theory/rewriterules/options"
extern struct CVC4_PUBLIC rewriteRulesAsAxioms__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; } rewriteRulesAsAxioms CVC4_PUBLIC;

#line 38 "../../../src/options/base_options_template.h"

}/* CVC4::options namespace */


#line 8 "../../../src/options/../theory/rewriterules/options"
template <> const options::efficientEMatching__option_t::type& Options::operator[](options::efficientEMatching__option_t) const;
#line 8 "../../../src/options/../theory/rewriterules/options"
template <> bool Options::wasSetByUser(options::efficientEMatching__option_t) const;
#line 8 "../../../src/options/../theory/rewriterules/options"
template <> void Options::assignBool(options::efficientEMatching__option_t, std::string option, bool value, SmtEngine* smt);
#line 11 "../../../src/options/../theory/rewriterules/options"
template <> const options::rewriteRulesAsAxioms__option_t::type& Options::operator[](options::rewriteRulesAsAxioms__option_t) const;
#line 11 "../../../src/options/../theory/rewriterules/options"
template <> bool Options::wasSetByUser(options::rewriteRulesAsAxioms__option_t) const;
#line 11 "../../../src/options/../theory/rewriterules/options"
template <> void Options::assignBool(options::rewriteRulesAsAxioms__option_t, std::string option, bool value, SmtEngine* smt);

#line 44 "../../../src/options/base_options_template.h"

namespace options {


#line 8 "../../../src/options/../theory/rewriterules/options"
inline efficientEMatching__option_t::type efficientEMatching__option_t::operator()() const { return Options::current()[*this]; }
#line 8 "../../../src/options/../theory/rewriterules/options"
inline bool efficientEMatching__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 11 "../../../src/options/../theory/rewriterules/options"
inline rewriteRulesAsAxioms__option_t::type rewriteRulesAsAxioms__option_t::operator()() const { return Options::current()[*this]; }
#line 11 "../../../src/options/../theory/rewriterules/options"
inline bool rewriteRulesAsAxioms__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 50 "../../../src/options/base_options_template.h"

}/* CVC4::options namespace */

}/* CVC4 namespace */

#endif /* __CVC4__OPTIONS__REWRITE_RULES_H */
