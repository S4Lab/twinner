/*********************                                                        */
/*! \file chain.h
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

#include "cvc4_public.h"

#ifndef __CVC4__CHAIN_H
#define __CVC4__CHAIN_H

#include "expr/kind.h"
#include <iostream>

namespace CVC4 {

/** A class to represent a chained, built-in operator. */
class Chain {
  Kind d_kind;
public:
  explicit Chain(Kind k) : d_kind(k) { }
  bool operator==(const Chain& ch) const { return d_kind == ch.d_kind; }
  bool operator!=(const Chain& ch) const { return d_kind != ch.d_kind; }
  Kind getOperator() const { return d_kind; }
};/* class Chain */

inline std::ostream& operator<<(std::ostream& out, const Chain& ch) {
  return out << ch.getOperator();
}

struct ChainHashFunction {
  size_t operator()(const Chain& ch) const {
    return kind::KindHashFunction()(ch.getOperator());
  }
};/* struct ChainHashFunction */

}/* CVC4 namespace */

#endif  /* __CVC4__CHAIN_H */
