/*********************                                                        */
/*! \file strtok_r.h
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Replacement for strtok_r() for systems without it (like Win32)
 **
 ** Replacement for strtok_r() for systems without it (like Win32).
 **/

#include "cvc4_public.h"

#ifndef __CVC4__LIB__STRTOK_R_H
#define __CVC4__LIB__STRTOK_R_H

#ifdef HAVE_STRTOK_R

// available in string.h
#include <string.h>

#else /* ! HAVE_STRTOK_R */

#include "lib/replacements.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

char* strtok_r(char *str, const char *delim, char **saveptr);

#ifdef __cplusplus
}/* extern "C" */
#endif /* __cplusplus */

#endif /* HAVE_STRTOK_R */
#endif /* __CVC4__LIB__STRTOK_R_H */
