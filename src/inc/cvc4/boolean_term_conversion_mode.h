/*********************                                                        */
/*! \file boolean_term_conversion_mode.h
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief [[ Add one-line brief description here ]]
 **
 ** [[ Add lengthier description here ]]
 ** \todo document this file
 **/

#include "cvc4_private.h"

#ifndef __CVC4__THEORY__BOOLEANS__BOOLEAN_TERM_CONVERSION_MODE_H
#define __CVC4__THEORY__BOOLEANS__BOOLEAN_TERM_CONVERSION_MODE_H

#include <iostream>

namespace CVC4 {
namespace theory {
namespace booleans {

typedef enum {
  /**
   * Convert Boolean terms to bitvectors of size 1.
   */
  BOOLEAN_TERM_CONVERT_TO_BITVECTORS,
  /**
   * Convert Boolean terms to enumerations in the Datatypes theory.
   */
  BOOLEAN_TERM_CONVERT_TO_DATATYPES,
  /**
   * Convert Boolean terms to enumerations in the Datatypes theory IF
   * used in a datatypes context, otherwise to a bitvector of size 1.
   */
  BOOLEAN_TERM_CONVERT_NATIVE

} BooleanTermConversionMode;

}/* CVC4::theory::booleans namespace */
}/* CVC4::theory namespace */

std::ostream& operator<<(std::ostream& out, theory::booleans::BooleanTermConversionMode mode) CVC4_PUBLIC;

}/* CVC4 namespace */

#endif /* __CVC4__THEORY__BOOLEANS__BOOLEAN_TERM_CONVERSION_MODE_H */
