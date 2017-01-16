//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2017 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef ITERATION_TOOLS_H
#define ITERATION_TOOLS_H

#include <list>
#include <set>
#include <map>

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

template <typename Key, typename Value, typename Aux = void * >
class ForEach {
public:

  template <typename Clazz> struct MemberVisitor {
    typedef void (Clazz::*ItemVisitor) (const Value &item);
    typedef void (Clazz::*PairVisitor) (const Key &key, const Value &value);
  };
  typedef void (*ItemVisitor) (const Value &item);
  typedef void (*ItemVisitorWithAux) (Aux &aux, const Value &item);
  typedef void (*PairVisitor) (const Key &key, const Value &value);
  typedef void (*PairVisitorWithAux) (Aux &aux, const Key &key, const Value &value);

  typedef std::list < Value > ListType;
  typedef std::set < Value > SetType;
  typedef std::map < Key, Value > MapType;

  static inline void iterate (const ListType &list, ItemVisitor visitor) {
    for (typename ListType::const_iterator it = list.begin (); it != list.end (); ++it) {
      const Value &item = *it;
      visitor (item);
    }
  }

  template <typename Clazz>
  static inline void iterate (const ListType &list,
      typename MemberVisitor<Clazz>::ItemVisitor visitor, Clazz *obj) {
    for (typename ListType::const_iterator it = list.begin ();
        it != list.end (); ++it) {
      const Value &item = *it;
      (obj->*visitor) (item);
    }
  }

  static inline void iterate (const ListType &list, ItemVisitorWithAux visitor,
      Aux &aux) {
    for (typename ListType::const_iterator it = list.begin (); it != list.end (); ++it) {
      const Value &item = *it;
      visitor (aux, item);
    }
  }

  static inline void iterate (const SetType &set, ItemVisitor visitor) {
    for (typename SetType::const_iterator it = set.begin (); it != set.end (); ++it) {
      const Value &item = *it;
      visitor (item);
    }
  }

  static inline void iterate (const SetType &set, ItemVisitorWithAux visitor, Aux &aux) {
    for (typename SetType::const_iterator it = set.begin (); it != set.end (); ++it) {
      const Value &item = *it;
      visitor (aux, item);
    }
  }

  static inline void iterate (const MapType &map, PairVisitor visitor) {
    for (typename MapType::const_iterator it = map.begin (); it != map.end (); ++it) {
      const Key &key = it->first;
      const Value &value = it->second;
      visitor (key, value);
    }
  }

  template <typename Clazz>
  static inline void iterate (const MapType &map,
      typename MemberVisitor<Clazz>::PairVisitor visitor, Clazz *obj) {
    for (typename MapType::const_iterator it = map.begin ();
        it != map.end (); ++it) {
      const Key &key = it->first;
      const Value &value = it->second;
      (obj->*visitor) (key, value);
    }
  }

  static inline void iterate (const MapType &map, PairVisitorWithAux visitor, Aux &aux) {
    for (typename MapType::const_iterator it = map.begin (); it != map.end (); ++it) {
      const Key &key = it->first;
      const Value &value = it->second;
      visitor (aux, key, value);
    }
  }
};

#define repeat(N) \
  for (unsigned int nn = 0; nn < N; ++nn)

#define UNUSED_VARIABLE(VAR) (void) VAR

template < typename Key, typename Value, typename Aux >
void foreach (const std::map < Key, Value > &map,
    typename edu::sharif::twinner::util::ForEach < Key, Value, Aux >
    ::PairVisitorWithAux visitor,
    Aux &aux) {
  edu::sharif::twinner::util::ForEach < Key, Value, Aux >::iterate (map, visitor, aux);
}

template < typename Value, typename Aux >
void foreach (const std::list < Value > &list,
    typename edu::sharif::twinner::util::ForEach < int, Value, Aux >
    ::ItemVisitorWithAux visitor,
    Aux &aux) {
  edu::sharif::twinner::util::ForEach < int, Value, Aux >::iterate (list, visitor, aux);
}

template < typename Value, typename Aux >
void foreach (const std::set < Value > &set,
    typename edu::sharif::twinner::util::ForEach < int, Value, Aux >
    ::ItemVisitorWithAux visitor,
    Aux &aux) {
  edu::sharif::twinner::util::ForEach < int, Value, Aux >::iterate (set, visitor, aux);
}

template < typename Key, typename Value >
void foreach (const std::map < Key, Value > &map,
    typename edu::sharif::twinner::util::ForEach < Key, Value >
    ::PairVisitor visitor) {
  edu::sharif::twinner::util::ForEach < Key, Value >::iterate (map, visitor);
}

template < typename Key, typename Value, typename Clazz >
void foreach (const std::map < Key, Value > &map,
    typename edu::sharif::twinner::util::ForEach < Key, Value >
    ::template MemberVisitor<Clazz>::PairVisitor visitor, Clazz *obj) {
  edu::sharif::twinner::util::ForEach < Key, Value >
      ::iterate (map, visitor, obj);
}

template < typename Value >
void foreach (const std::list < Value > &list,
    typename edu::sharif::twinner::util::ForEach < int, Value >
    ::ItemVisitor visitor) {
  edu::sharif::twinner::util::ForEach < int, Value >::iterate (list, visitor);
}

template < typename Value, typename Clazz >
void foreach (const std::list < Value > &list,
    typename edu::sharif::twinner::util::ForEach < int, Value >
    ::template MemberVisitor<Clazz>::ItemVisitor visitor, Clazz *obj) {
  edu::sharif::twinner::util::ForEach < int, Value >
      ::iterate (list, visitor, obj);
}

template < typename Value >
void foreach (const std::set < Value > &set,
    typename edu::sharif::twinner::util::ForEach < int, Value >
    ::ItemVisitor visitor) {
  edu::sharif::twinner::util::ForEach < int, Value >::iterate (set, visitor);
}

}
}
}
}

#endif /* IterationTools.h */
