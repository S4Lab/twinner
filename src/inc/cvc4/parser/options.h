/*********************                                                        */
/** options.h
 **
 ** Copyright 2011-2013  New York University and The University of Iowa,
 ** and as below.
 **
 ** This file automatically generated by:
 **
 **     ../../../src/options/mkoptions ../../../src/options/base_options_template.h ../parser/options.h
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

#ifndef __CVC4__OPTIONS__PARSER_H
#define __CVC4__OPTIONS__PARSER_H

#include <cvc4/options/options.h>


#line 26 "../../../src/options/base_options_template.h"

#define CVC4_OPTIONS__PARSER__FOR_OPTION_HOLDER \
  strictParsing__option_t::type strictParsing; \
  bool strictParsing__setByUser__; \
  memoryMap__option_t::type memoryMap; \
  bool memoryMap__setByUser__; \
  semanticChecks__option_t::type semanticChecks; \
  bool semanticChecks__setByUser__; \
  canIncludeFile__option_t::type canIncludeFile; \
  bool canIncludeFile__setByUser__;

#line 30 "../../../src/options/base_options_template.h"

namespace CVC4 {

namespace options {


#line 8 "../../../src/options/../parser/options"
extern struct CVC4_PUBLIC strictParsing__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; } strictParsing CVC4_PUBLIC;
#line 11 "../../../src/options/../parser/options"
extern struct CVC4_PUBLIC memoryMap__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; } memoryMap CVC4_PUBLIC;
#line 14 "../../../src/options/../parser/options"
extern struct CVC4_PUBLIC semanticChecks__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; } semanticChecks CVC4_PUBLIC;
#line 19 "../../../src/options/../parser/options"
extern struct CVC4_PUBLIC canIncludeFile__option_t { typedef bool type; type operator()() const; bool wasSetByUser() const; } canIncludeFile CVC4_PUBLIC;

#line 38 "../../../src/options/base_options_template.h"

}/* CVC4::options namespace */


#line 8 "../../../src/options/../parser/options"
template <> const options::strictParsing__option_t::type& Options::operator[](options::strictParsing__option_t) const;
#line 8 "../../../src/options/../parser/options"
template <> bool Options::wasSetByUser(options::strictParsing__option_t) const;
#line 8 "../../../src/options/../parser/options"
template <> void Options::assignBool(options::strictParsing__option_t, std::string option, bool value, SmtEngine* smt);
#line 11 "../../../src/options/../parser/options"
template <> const options::memoryMap__option_t::type& Options::operator[](options::memoryMap__option_t) const;
#line 11 "../../../src/options/../parser/options"
template <> bool Options::wasSetByUser(options::memoryMap__option_t) const;
#line 11 "../../../src/options/../parser/options"
template <> void Options::assignBool(options::memoryMap__option_t, std::string option, bool value, SmtEngine* smt);
#line 14 "../../../src/options/../parser/options"
template <> const options::semanticChecks__option_t::type& Options::operator[](options::semanticChecks__option_t) const;
#line 14 "../../../src/options/../parser/options"
template <> bool Options::wasSetByUser(options::semanticChecks__option_t) const;
#line 14 "../../../src/options/../parser/options"
template <> void Options::assignBool(options::semanticChecks__option_t, std::string option, bool value, SmtEngine* smt);
#line 19 "../../../src/options/../parser/options"
template <> const options::canIncludeFile__option_t::type& Options::operator[](options::canIncludeFile__option_t) const;
#line 19 "../../../src/options/../parser/options"
template <> bool Options::wasSetByUser(options::canIncludeFile__option_t) const;
#line 19 "../../../src/options/../parser/options"
template <> void Options::assignBool(options::canIncludeFile__option_t, std::string option, bool value, SmtEngine* smt);

#line 44 "../../../src/options/base_options_template.h"

namespace options {


#line 8 "../../../src/options/../parser/options"
inline strictParsing__option_t::type strictParsing__option_t::operator()() const { return Options::current()[*this]; }
#line 8 "../../../src/options/../parser/options"
inline bool strictParsing__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 11 "../../../src/options/../parser/options"
inline memoryMap__option_t::type memoryMap__option_t::operator()() const { return Options::current()[*this]; }
#line 11 "../../../src/options/../parser/options"
inline bool memoryMap__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 14 "../../../src/options/../parser/options"
inline semanticChecks__option_t::type semanticChecks__option_t::operator()() const { return Options::current()[*this]; }
#line 14 "../../../src/options/../parser/options"
inline bool semanticChecks__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 19 "../../../src/options/../parser/options"
inline canIncludeFile__option_t::type canIncludeFile__option_t::operator()() const { return Options::current()[*this]; }
#line 19 "../../../src/options/../parser/options"
inline bool canIncludeFile__option_t::wasSetByUser() const { return Options::current().wasSetByUser(*this); }

#line 50 "../../../src/options/base_options_template.h"

}/* CVC4::options namespace */

}/* CVC4 namespace */

#endif /* __CVC4__OPTIONS__PARSER_H */
