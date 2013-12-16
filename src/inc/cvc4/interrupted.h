/*********************                                                        */
/*! \file interrupted.h
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief An exception signaling that a Theory should immediately
 ** stop performing processing
 **
 ** An exception signaling that a Theory should immediately stop
 ** performing processing and relinquish control to its caller (e.g.,
 ** in a parallel environment).  A Theory might be interrupted if it
 ** calls into its CVC4::theory::OutputChannel, and it should only
 ** catch this exception to perform emergency repair of any invariants
 ** it must re-establish.  Further, if this exception is caught by a
 ** Theory, the Theory should rethrow the same exception (via "throw;"
 ** in the exception block) rather than return, as the Interrupted
 ** instance might contain additional information needed for the
 ** proper management of CVC4 components.
 **/

#include "cvc4_private.h"

#ifndef __CVC4__THEORY__INTERRUPTED_H
#define __CVC4__THEORY__INTERRUPTED_H

#include "util/exception.h"

namespace CVC4 {
namespace theory {

class Interrupted : public CVC4::Exception {
};/* class Interrupted */

}/* CVC4::theory namespace */
}/* CVC4 namespace */

#endif /* __CVC4__THEORY__INTERRUPTED_H */
