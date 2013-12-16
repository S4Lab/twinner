/*********************                                                        */
/*! \file registrar.h
 ** \verbatim
 ** Original author: Liana Hadarean
 ** Major contributors: Tim King, Morgan Deters
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Class to encapsulate preregistration duties
 **
 ** Class to encapsulate preregistration duties.  This class permits the
 ** CNF stream implementation to reach into the theory engine to
 ** preregister only those terms with an associated SAT literal (at the
 ** point when they get the SAT literal), without having to refer to the
 ** TheoryEngine class directly.
 **/

#include "cvc4_private.h"

#ifndef __CVC4__PROP__REGISTRAR_H
#define __CVC4__PROP__REGISTRAR_H

namespace CVC4 {
namespace prop {

class Registrar {
public:
  virtual void preRegister(Node n) = 0;

};/* class Registrar */

class NullRegistrar: public Registrar {
public:
  void preRegister(Node n) {};

};/* class Registrar */



}/* CVC4::prop namespace */
}/* CVC4 namespace */

#endif /* __CVC4__PROP__REGISTRAR_H */
