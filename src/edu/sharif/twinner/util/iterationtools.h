//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information 
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef ITERATION_TOOLS_H
#define ITERATION_TOOLS_H

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

template <typename Key, typename Value, typename Aux = void * >
class ForEach {

public:
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

  static inline void iterate (const MapType &map, PairVisitorWithAux visitor, Aux &aux) {
    for (typename MapType::const_iterator it = map.begin (); it != map.end (); ++it) {
      const Key &key = it->first;
      const Value &value = it->second;
      visitor (aux, key, value);
    }
  }
};

#define repeat(N) \
  for (int nn = 0; nn < N; ++nn)

#define UNUSED_VARIABLE(VAR) (void) VAR

}
}
}
}

#endif	/* IterationTools.h */