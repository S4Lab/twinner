/*********************                                                        */
/** options.h
 **
 ** Copyright 2011-2013  New York University and The University of Iowa,
 ** and as below.
 **
 ** This file automatically generated by:
 **
 **     ../../../src/options/mkoptions ../../../src/options/base_options_template.h ../expr/options.h
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

#ifndef __CVC4__OPTIONS__EXPR_H
#define __CVC4__OPTIONS__EXPR_H

#include <cvc4/options/options.h>


#line 26 "../../../src/options/base_options_template.h"

#define CVC4_OPTIONS__EXPR__FOR_OPTION_HOLDER \
  defaultExprDepth__option_t::type defaultExprDepth; \
  bool defaultExprDepth__setByUser__; \
  defaultDagThresh__option_t::type defaultDagThresh; \
  bool defaultDagThresh__setByUser__; \
  earlyTypeChecking__option_t::type earlyTypeChecking; \
  bool earlyTypeChecking__setByUser__; \
  typeChecking__option_t::type typeChecking; \
  bool typeChecking__setByUser__; \
  biasedITERemoval__option_t::type biasedITERemoval; \
  bool biasedITERemoval__setByUser__;

#line 30 "../../../src/options/base_options_template.h"

namespace CVC4 {

namespace options {


#line 8 "../../../src/options/../expr/options"
extern struct CVC4_PUBLIC defaultExprDepth__option_t { typedef int type; type operator()() const; bool wasSetByUser() const; } defaultExprDepth CVC4_PUBLIC;
#line 10 "../../../src/options/../expr/options"
extern struct CVC4_PUBLIC defaultDagThresh__option_t { typedef int type; type operator()() const; bool wasSetByUser() const; } defaultDagThresh CVC4_PUBLIC;
#line 15 "../../../src/options/../expr/options"
extern struct CVC4_PUBLIC earlyTypeChecking__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; } earlyTypeChecking CVC4_PUBLIC;
#line 21 "../../../src/options/../expr/options"
extern struct CVC4_PUBLIC typeChecking__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; } typeChecking CVC4_PUBLIC;
#line 24 "../../../src/options/../expr/options"
extern struct CVC4_PUBLIC biasedITERemoval__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; } biasedITERemoval CVC4_PUBLIC;

#line 38 "../../../src/options/base_options_template.h"

}/* CVC4::options namespace */


#line 8 "../../../src/options/../expr/options"
template <> const options::defaultExprDepth__option_t::type& Options::operator[](options::defaultExprDepth__option_t) const;
#line 8 "../../../src/options/../expr/options"
template <> bool Options::wasSetByUser(options::defaultExprDepth__option_t) const;
#line 8 "../../../src/options/../expr/options"
template <> void Options::assign(options::defaultExprDepth__option_t, std::string option, std::string value, SmtEngine* smt);
#line 10 "../../../src/options/../expr/options"
template <> const options::defaultDagThresh__option_t::type& Options::operator[](options::defaultDagThresh__option_t) const;
#line 10 "../../../src/options/../expr/options"
template <> bool Options::wasSetByUser(options::defaultDagThresh__option_t) const;
#line 10 "../../../src/options/../expr/options"
template <> void Options::assign(options::defaultDagThresh__option_t, std::string option, std::string value, SmtEngine* smt);
#line 15 "../../../src/options/../expr/options"
template <> const options::earlyTypeChecking__option_t::type& Options::operator[](options::earlyTypeChecking__option_t) const;
#line 15 "../../../src/options/../expr/options"
template <> bool Options::wasSetByUser(options::earlyTypeChecking__option_t) const;
#line 15 "../../../src/options/../expr/options"
template <> void Options::assignBool(options::earlyTypeChecking__option_t, std::string option, bool value, SmtEngine* smt);
#line 21 "../../../src/options/../expr/options"
template <> const options::typeChecking__option_t::type& Options::operator[](options::typeChecking__option_t) const;
#line 21 "../../../src/options/../expr/options"
template <> bool Options::wasSetByUser(options::typeChecking__option_t) const;
#line 21 "../../../src/options/../expr/options"
template <> void Options::assignBool(options::typeChecking__option_t, std::string option, bool value, SmtEngine* smt);
#line 24 "../../../src/options/../expr/options"
template <> const options::biasedITERemoval__option_t::type& Options::operator[](options::biasedITERemoval__option_t) const;
#line 24 "../../../src/options/../expr/options"
template <> bool Options::wasSetByUser(options::biasedITERemoval__option_t) const;
#line 24 "../../../src/options/../expr/options"
template <> void Options::assignBool(options::biasedITERemoval__option_t, std::string option, bool value, SmtEngine* smt);

#line 44 "../../../src/options/base_options_template.h"

namespace options {


#line 8 "../../../src/options/../expr/options"
inline defaultExprDepth__option_t::type defaultExprDepth__option_t::operator()() const { return Options::current()[*this]; }
#line 8 "../../../src/options/../expr/options"
inline bool defaultExprDepth__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 10 "../../../src/options/../expr/options"
inline defaultDagThresh__option_t::type defaultDagThresh__option_t::operator()() const { return Options::current()[*this]; }
#line 10 "../../../src/options/../expr/options"
inline bool defaultDagThresh__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 15 "../../../src/options/../expr/options"
inline earlyTypeChecking__option_t::type earlyTypeChecking__option_t::operator()() const { return Options::current()[*this]; }
#line 15 "../../../src/options/../expr/options"
inline bool earlyTypeChecking__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 21 "../../../src/options/../expr/options"
inline typeChecking__option_t::type typeChecking__option_t::operator()() const { return Options::current()[*this]; }
#line 21 "../../../src/options/../expr/options"
inline bool typeChecking__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 24 "../../../src/options/../expr/options"
inline biasedITERemoval__option_t::type biasedITERemoval__option_t::operator()() const { return Options::current()[*this]; }
#line 24 "../../../src/options/../expr/options"
inline bool biasedITERemoval__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 50 "../../../src/options/base_options_template.h"

}/* CVC4::options namespace */

}/* CVC4 namespace */

#endif /* __CVC4__OPTIONS__EXPR_H */
