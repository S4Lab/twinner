/*********************                                                        */
/*! \file logic_exception.h
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief An exception that is thrown when a feature is used outside
 ** the logic that CVC4 is currently using
 **
 ** \brief An exception that is thrown when a feature is used outside
 ** the logic that CVC4 is currently using (for example, a quantifier
 ** is used while running in a quantifier-free logic).
 **/

#include "cvc4_public.h"

#ifndef __CVC4__SMT__LOGIC_EXCEPTION_H
#define __CVC4__SMT__LOGIC_EXCEPTION_H

#include "util/exception.h"

namespace CVC4 {

class CVC4_PUBLIC LogicException : public CVC4::Exception {
public:
  LogicException() :
    Exception("Feature used while operating in "
              "incorrect state") {
  }

  LogicException(const std::string& msg) :
    Exception(msg) {
  }

  LogicException(const char* msg) :
    Exception(msg) {
  }
};/* class LogicException */

}/* CVC4 namespace */

#endif /* __CVC4__SMT__LOGIC_EXCEPTION_H */
