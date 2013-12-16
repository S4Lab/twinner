/*********************                                                        */
/*! \file clock_gettime.h
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Replacement for clock_gettime() for systems without it (like Mac OS X)
 **
 ** Replacement for clock_gettime() for systems without it (like Mac OS X).
 **/

#include "cvc4_public.h"

#ifndef __CVC4__LIB__CLOCK_GETTIME_H
#define __CVC4__LIB__CLOCK_GETTIME_H

#include "lib/replacements.h"

#ifdef HAVE_CLOCK_GETTIME

/* it should be available from <time.h> */
#include <time.h>

#else /* HAVE_CLOCK_GETTIME */

/* otherwise, we have to define it */

#if defined(__WIN32__) && !defined(__WIN64__)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct timespec {
  uint64_t tv_sec;
  int32_t tv_nsec;
};/* struct timespec */

#ifdef __cplusplus
}/* extern "C" */
#endif /* __cplusplus */

#else /* !__WIN32__ || __WIN64__ */

/* get timespec from <time.h> */
#include <time.h>

#endif /* __WIN32__ && !__WIN64__ */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct timespec;

typedef enum {
  CLOCK_REALTIME,
  CLOCK_MONOTONIC,
  CLOCK_REALTIME_HR,
  CLOCK_MONOTONIC_HR
} clockid_t;

long clock_gettime(clockid_t which_clock, struct timespec* tp);

#ifdef __cplusplus
}/* extern "C" */
#endif /* __cplusplus */

#endif /* HAVE_CLOCK_GETTIME */
#endif /*__CVC4__LIB__CLOCK_GETTIME_H */
