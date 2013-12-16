/*********************                                                        */
/*! \file theory_traits_template.h
 ** \verbatim
 ** Original author: Dejan Jovanovic
 ** Major contributors: Morgan Deters
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief A template for the theory_traits.h header, defining various
 ** (static) aspects of theories
 **
 ** This file is a template for the theory_traits.h header, defining
 ** various (static) aspects of theories, combined with the theory
 ** kinds files to produce the final header.
 **/

#include "cvc4_private.h"

#pragma once

#include "theory/theory.h"
#include "theory/options.h"

${theory_includes}

namespace CVC4 {
namespace theory {

template <TheoryId theoryId>
struct TheoryTraits;

${theory_traits}

${theory_for_each_macro}

#line 40 "${template}"

struct TheoryConstructor {
  static void addTheory(TheoryEngine* engine, TheoryId id) {
    switch(id) {

${theory_constructors}

    default:
      Unhandled(id);
    }
  }
};/* struct CVC4::theory::TheoryConstructor */

}/* CVC4::theory namespace */
}/* CVC4 namespace */
