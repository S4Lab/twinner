/*********************                                                        */
/*! \file cache.h
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief A generic Cache<> template class for use by functions that
 ** walk the Node DAG and want to cache results for sub-DAGs
 **
 ** A generic Cache<> template class for use by functions that walk
 ** the Node DAG and want to cache results for sub-DAGs.
 **/

#include "cvc4_private.h"

#ifndef __CVC4__CACHE_H
#define __CVC4__CACHE_H

#include <utility>
#include <functional>

namespace CVC4 {

/**
 * A generic implementation of a cache for functions that walk the
 * Node DAG performing a computation and want to cache some or all
 * computations.
 */
template <class T, class U, class Hasher = std::hash<T> >
class Cache {
  typedef std::hash_map<T, U, Hasher> Map;
  Map d_map;
  std::vector<T> d_current;
  typename Map::iterator d_result;

  // disallow copy/assignment
  Cache(const Cache&) CVC4_UNUSED;
  Cache& operator=(const Cache&) CVC4_UNUSED;

public:

  typedef T key_type;
  typedef U value_type;
  typedef Hasher hash_function;

  /**
   * Makes it easy and error-free to use a Cache<> in a function.
   */
  class Scope {
    Cache& d_cache;
    bool d_fired;

  public:
    Scope(Cache<T, U, Hasher>& cache, const T& elt) throw(AssertionException) :
      d_cache(cache),
      d_fired(d_cache.computing(elt)) {
    }

    ~Scope() {
      if(!d_fired) {
        d_cache();// pop stack
      }
    }

    operator bool() throw() {
      return d_fired;
    }

    const U& get() throw(AssertionException) {
      Assert(d_fired, "nothing in cache");
      return d_cache.get();
    }

    U& operator()(U& computed) throw(AssertionException) {
      Assert(!d_fired, "can only cache a computation once");
      d_fired = true;
      return d_cache(computed);
    }
    const U& operator()(const U& computed) throw(AssertionException) {
      Assert(!d_fired, "can only cache a computation once");
      d_fired = true;
      return d_cache(computed);
    }
  };/* class Cache::Scope */

  Cache() {}

  bool computing(const T& elt) {
    d_result = d_map.find(elt);
    bool found = (d_result != d_map.end());
    if(!found) {
      d_current.push_back(elt);
    }
    return found;
  }

  const U& get() {
    Assert(d_result != d_map.end());
    return (*d_result).second;
  }

  // cache nothing (just pop)
  void operator()() {
    d_current.pop_back();
  }

  U& operator()(U& result) {
    d_map.insert(d_current.back(), result);
    d_current.pop_back();
    return result;
  }
  const U& operator()(const U& result) {
    d_map.insert(std::make_pair(d_current.back(), result));
    d_current.pop_back();
    return result;
  }
};/* class Cache<> */


}/* CVC4 namespace */

#endif /* __CVC4__CACHE_H */
