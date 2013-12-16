/*********************                                                        */
/*! \file cvc4_private_library.h
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief #-inclusion of this file marks a header as private and generates a
 ** warning when the file is included improperly
 **
 ** #-inclusion of this file marks a header as private and generates a
 ** warning when the file is included improperly.
 **/

#ifndef __CVC4_PRIVATE_LIBRARY_H
#define __CVC4_PRIVATE_LIBRARY_H

#if ! (defined(__BUILDING_CVC4LIB) || defined(__BUILDING_CVC4LIB_UNIT_TEST) || defined(__BUILDING_CVC4PARSERLIB) || defined(__BUILDING_CVC4PARSERLIB_UNIT_TEST) || defined(__BUILDING_CVC4COMPATLIB) || defined(__BUILDING_CVC4DRIVER))
#  warning A "private library" CVC4 header was included when not building the library, driver, or private unit test code.
#endif /* ! (__BUILDING_CVC4LIB || __BUILDING_CVC4LIB_UNIT_TEST || __BUILDING_CVC4PARSERLIB || __BUILDING_CVC4PARSERLIB_UNIT_TEST || __BUILDING_CVC4DRIVER) */

#include "cvc4_public.h"
#include "cvc4autoconfig.h"

#endif /* __CVC4_PRIVATE_LIBRARY_H */
