/*********************                                                        */
/*! \file cdinsert_hashmap_forward.h
 ** \verbatim
 ** Original author: Tim King
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief This is a forward declaration header to declare the CDInsertHashMap<>
 ** template
 **
 ** This is a forward declaration header to declare the CDInsertHashMap<>
 ** template.  It's useful if you want to forward-declare CDInsertHashMap<>
 ** without including the full cdinsert_hashmap.h header, for example, in a
 ** public header context.
 **
 ** For CDInsertHashMap<> in particular, it's difficult to forward-declare it
 ** yourself, because it has a default template argument.
 **/

#include "cvc4_public.h"

#pragma once

namespace __gnu_cxx {
  template <class Key> struct hash;
}/* __gnu_cxx namespace */

namespace CVC4 {
  namespace context {
    template <class Key, class Data, class HashFcn = __gnu_cxx::hash<Key> >
    class CDInsertHashMap;
  }/* CVC4::context namespace */
}/* CVC4 namespace */

