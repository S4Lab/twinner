/*********************                                                        */
/*! \file utility.h
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Some standard STL-related utility functions for CVC4
 **
 ** Some standard STL-related utility functions for CVC4.
 **/

#include "cvc4_private.h"

#ifndef __CVC4__UTILITY_H
#define __CVC4__UTILITY_H

#include <utility>
#include <functional>

namespace CVC4 {


/**
 * Like std::equal_to<>, but tests equality between the first element
 * of a pair and an element.
 */
template <class T, class U>
struct first_equal_to : std::binary_function<std::pair<T, U>, T, bool> {
  bool operator()(const std::pair<T, U>& pr, const T& x) const {
    return pr.first == x;
  }
};/* struct first_equal_to<T> */


/**
 * Like std::equal_to<>, but tests equality between the second element
 * of a pair and an element.
 */
template <class T, class U>
struct second_equal_to : std::binary_function<std::pair<T, U>, U, bool> {
  bool operator()(const std::pair<T, U>& pr, const U& x) const {
    return pr.second == x;
  }
};/* struct first_equal_to<T> */


/**
 * Using std::find_if(), finds the first iterator in [first,last)
 * range that satisfies predicate.  If none, return last; otherwise,
 * search for a second one.  If there IS a second one, return last,
 * otherwise return the first (and unique) iterator satisfying pred().
 */
template <class InputIterator, class Predicate>
inline InputIterator find_if_unique(InputIterator first, InputIterator last, Predicate pred) {
  InputIterator match = std::find_if(first, last, pred);
  if(match == last) {
    return last;
  }

  InputIterator match2 = match;
  match2 = std::find_if(++match2, last, pred);
  return (match2 == last) ? match : last;
}

}/* CVC4 namespace */

#endif /* __CVC4__UTILITY_H */
