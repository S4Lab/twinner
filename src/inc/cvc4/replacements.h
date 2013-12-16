/*********************                                                        */
/*! \file replacements.h
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Common header for replacement function sources
 **
 ** Common header for replacement function sources.
 **/

#ifndef __CVC4__LIB__REPLACEMENTS_H
#define __CVC4__LIB__REPLACEMENTS_H

#if (defined(__BUILDING_CVC4LIB) || defined(__BUILDING_CVC4LIB_UNIT_TEST)) && !defined(__BUILDING_STATISTICS_FOR_EXPORT)
#  include "cvc4_private.h"
#else
#  if defined(__BUILDING_CVC4PARSERLIB) || defined(__BUILDING_CVC4PARSERLIB_UNIT_TEST)
#    include "cvc4parser_private.h"
#  else
#    if defined(__BUILDING_CVC4DRIVER) || defined(__BUILDING_CVC4_SYSTEM_TEST) || defined(__BUILDING_STATISTICS_FOR_EXPORT)
#      include "cvc4autoconfig.h"
#    else
#      error Must be building libcvc4 or libcvc4parser to use replacement functions.  This is because replacement function headers should never be publicly-depended upon, as they should not be installed on user machines with 'make install'.
#    endif
#  endif
#endif

#endif /* __CVC4__LIB__REPLACEMENTS_H */
