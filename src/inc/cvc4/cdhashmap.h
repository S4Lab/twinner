/*********************                                                        */
/*! \file cdhashmap.h
 ** \verbatim
 ** Original author: Dejan Jovanovic
 ** Major contributors: Morgan Deters
 ** Minor contributors (to current version): Tim King
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Context-dependent map class.
 **
 ** Context-dependent map class.  Generic templated class for a map
 ** which must be saved and restored as contexts are pushed and
 ** popped.  Requires that operator= be defined for the data class,
 ** and operator== for the key class.  For key types that don't have a
 ** __gnu_cxx::hash<>, you should provide an explicit HashFcn.
 **
 ** See also:
 **  CDInsertHashMap : An "insert-once" CD hash map.
 **  CDTrailHashMap : A lightweight CD hash map with poor iteration
 **    characteristics and some quirks in usage.
 **
 ** Internal documentation:
 **
 ** CDHashMap<> is something of a work in progress at present (26 May
 ** 2010), due to some recent discoveries of problems with its
 ** internal state.  Here are some notes on the internal use of
 ** CDOhash_maps that may be useful in figuring out this mess:
 **
 **     So you have a CDHashMap<>.
 **
 **     You insert some (key,value) pairs.  Each allocates a CDOhash_map<>
 **     and goes on a doubly-linked list headed by map.d_first and
 **     threaded via CDOhash_map.{d_prev,d_next}.  CDOhash_maps are constructed
 **     with a NULL d_map pointer, but then immediately call
 **     makeCurrent() and set the d_map pointer back to the map.  At
 **     context level 0, this doesn't lead to anything special.  In
 **     higher context levels, this stores away a CDOhash_map with a NULL
 **     map pointer at level 0, and a non-NULL map pointer in the
 **     current context level.  (Remember that for later.)
 **
 **     When a key is associated to a new value in a CDHashMap, its
 **     associated CDOhash_map calls makeCurrent(), then sets the new
 **     value.  The save object is also a CDOhash_map (allocated in context
 **     memory).
 **
 **     Now, CDOhash_maps disappear in a variety of ways.
 **
 **     First, you might pop beyond a "modification of the value"
 **     scope level, requiring a re-association of the key to an old
 **     value.  This is easy.  CDOhash_map::restore() does the work, and
 **     the context memory of the save object is reclaimed as usual.
 **
 **     Second, you might pop beyond a "insert the key" scope level,
 **     requiring that the key be completely removed from the map and
 **     its CDOhash_map object memory freed.  Here, the CDOhash_map is restored
 **     to a "NULL-map" state (see above), signaling it to remove
 **     itself from the map completely and put itself on a "trash
 **     list" for the map.
 **
 **     Third, you might obliterate() the key.  This calls the CDOhash_map
 **     destructor, which calls destroy(), which does a successive
 **     restore() until level 0.  If the key was in the map since
 **     level 0, the restore() won't remove it, so in that case
 **     obliterate() removes it from the map and frees the CDOhash_map's
 **     memory.
 **
 **     Fourth, you might delete the cdhashmap(calling CDHashMap::~CDHashMap()).
 **     This first calls destroy(), as per ContextObj contract, but
 **     cdhashmapdoesn't save/restore itself, so that does nothing at the
 **     CDHashMap-level.  Then it empties the trash.  Then, for each
 **     element in the map, it marks it as being "part of a complete
 **     map destruction", which essentially short-circuits
 **     CDOhash_map::restore() (see CDOhash_map::restore()), then deallocates
 **     it.  Finally it asserts that the trash is empty (which it
 **     should be, since restore() was short-circuited).
 **
 **     Fifth, you might clear() the CDHashMap.  This does exactly the
 **     same as CDHashMap::~CDHashMap(), except that it doesn't call destroy()
 **     on itself.
 **
 **     CDHashMap::emptyTrash() simply goes through and calls
 **     ->deleteSelf() on all elements in the trash.
 **     ContextObj::deleteSelf() calls the CDOhash_map destructor, then
 **     frees the memory associated to the CDOhash_map.  CDOhash_map::~CDOhash_map()
 **     calls destroy(), which restores as much as possible.  (Note,
 **     though, that since objects placed on the trash have already
 **     restored to the fullest extent possible, it does nothing.)
 **/

#include "cvc4_private.h"

#ifndef __CVC4__CONTEXT__CDHASHMAP_H
#define __CVC4__CONTEXT__CDHASHMAP_H

#include <vector>
#include <iterator>
#include <ext/hash_map>

#include "context/context.h"
#include "util/cvc4_assert.h"
#include "context/cdhashmap_forward.h"

namespace CVC4 {
namespace context {

// Auxiliary class: almost the same as CDO (see cdo.h)

template <class Key, class Data, class HashFcn = __gnu_cxx::hash<Key> >
class CDOhash_map : public ContextObj {
  friend class CDHashMap<Key, Data, HashFcn>;

  Key d_key;
  Data d_data;
  CDHashMap<Key, Data, HashFcn>* d_map;

  /** never put this cdhashmapelement on the trash */
  bool d_noTrash;

  // Doubly-linked list for keeping track of elements in order of insertion
  CDOhash_map* d_prev;
  CDOhash_map* d_next;

  virtual ContextObj* save(ContextMemoryManager* pCMM) {
    return new(pCMM) CDOhash_map(*this);
  }

  virtual void restore(ContextObj* data) {
    if(d_map != NULL) {
      CDOhash_map* p = static_cast<CDOhash_map*>(data);
      if(p->d_map == NULL) {
        Assert(d_map->d_map.find(d_key) != d_map->d_map.end() &&
               (*d_map->d_map.find(d_key)).second == this);
        // no longer in map (popped beyond first level in which it was)
        d_map->d_map.erase(d_key);
        // If we call deleteSelf() here, it re-enters restore().  So,
        // put it on a "trash heap" instead, for later deletion.
        //
        // FIXME multithreading
        if(d_map->d_first == this) {
          Debug("gc") << "remove first-elem " << this << " from map " << d_map << " with next-elem " << d_next << std::endl;
          if(d_next == this) {
            Assert(d_prev == this);
            d_map->d_first = NULL;
          } else {
            d_map->d_first = d_next;
          }
        } else {
          Debug("gc") << "remove nonfirst-elem " << this << " from map " << d_map << std::endl;
        }
        d_next->d_prev = d_prev;
        d_prev->d_next = d_next;
        if(d_noTrash) {
          Debug("gc") << "CDHashMap<> no-trash " << this << std::endl;
        } else {
          Debug("gc") << "CDHashMap<> trash push_back " << this << std::endl;
          //this->deleteSelf();
          d_map->d_trash.push_back(this);
        }
      } else {
        d_data = p->d_data;
      }
    }
  }

  /** ensure copy ctor is only called by us */
  CDOhash_map(const CDOhash_map& other) :
    ContextObj(other),
    d_key(other.d_key),
    d_data(other.d_data),
    d_map(other.d_map),
    d_prev(NULL),
    d_next(NULL) {
  }

public:

  CDOhash_map(Context* context,
         CDHashMap<Key, Data, HashFcn>* map,
	 const Key& key,
         const Data& data,
         bool atLevelZero = false,
         bool allocatedInCMM = false) :
    ContextObj(allocatedInCMM, context),
    d_key(key),
    d_map(NULL),
    d_noTrash(allocatedInCMM) {

    // untested, probably unsafe.
    Assert(!(atLevelZero && allocatedInCMM));

    if(atLevelZero) {
      // "Initializing" map insertion: this entry will never be
      // removed from the map, it's inserted at level 0 as an
      // "initializing" element.  See
      // CDHashMap<>::insertAtContextLevelZero().
      d_data = data;
    } else {
      // Normal map insertion: first makeCurrent(), then set the data
      // and then, later, the map.  Order is important; we can't
      // initialize d_map in the constructor init list above, because
      // we want the restore of d_map to NULL to signal us to remove
      // the element from the map.

      if(allocatedInCMM) {
        // Force a save/restore point, even though the object is
        // allocated here.  This is so that we can detect when the
        // object falls out of the map (otherwise we wouldn't get it).
        makeSaveRestorePoint();
      }

      set(data);
    }
    d_map = map;

    CDOhash_map*& first = d_map->d_first;
    if(first == NULL) {
      first = d_next = d_prev = this;
      Debug("gc") << "add first-elem " << this << " to map " << d_map << std::endl;
    } else {
      Debug("gc") << "add nonfirst-elem " << this << " to map " << d_map << " with first-elem " << first << "[" << first->d_prev << " " << first->d_next << std::endl;
      d_prev = first->d_prev;
      d_next = first;
      d_prev->d_next = this;
      first->d_prev = this;
    }
  }

  ~CDOhash_map() throw(AssertionException) {
    destroy();
  }

  void set(const Data& data) {
    makeCurrent();
    d_data = data;
  }

  const Key& getKey() const {
    return d_key;
  }

  const Data& get() const {
    return d_data;
  }

  operator Data() {
    return get();
  }

  const Data& operator=(const Data& data) {
    set(data);
    return data;
  }

  CDOhash_map* next() const {
    if(d_next == d_map->d_first) {
      return NULL;
    } else {
      return d_next;
    }
  }
};/* class CDOhash_map<> */


/**
 * Generic templated class for a map which must be saved and restored
 * as contexts are pushed and popped.  Requires that operator= be
 * defined for the data class, and operator== for the key class.
 */
template <class Key, class Data, class HashFcn>
class CDHashMap : public ContextObj {

  typedef CDOhash_map<Key, Data, HashFcn> Element;
  typedef __gnu_cxx::hash_map<Key, Element*, HashFcn> table_type;

  friend class CDOhash_map<Key, Data, HashFcn>;

  table_type d_map;

  Element* d_first;
  Context* d_context;

  std::vector<Element*> d_trash;

  // Nothing to save; the elements take care of themselves
  virtual ContextObj* save(ContextMemoryManager* pCMM) {
    Unreachable();
  }

  // Similarly, nothing to restore
  virtual void restore(ContextObj* data) {
    Unreachable();
  }

  void emptyTrash() {
    //FIXME multithreading
    for(typename std::vector<Element*>::iterator i = d_trash.begin();
        i != d_trash.end();
        ++i) {
      Debug("gc") << "emptyTrash(): " << *i << std::endl;
      (*i)->deleteSelf();
    }
    d_trash.clear();
  }

public:

  CDHashMap(Context* context) :
    ContextObj(context),
    d_map(),
    d_first(NULL),
    d_context(context),
    d_trash() {
  }

  ~CDHashMap() throw(AssertionException) {
    Debug("gc") << "cdhashmap" << this
                << " disappearing, destroying..." << std::endl;
    destroy();
    Debug("gc") << "cdhashmap" << this
                << " disappearing, done destroying" << std::endl;

    Debug("gc") << "cdhashmap" << this << " gone, emptying trash" << std::endl;
    emptyTrash();
    Debug("gc") << "done emptying trash for " << this << std::endl;

    for(typename table_type::iterator i = d_map.begin();
        i != d_map.end();
        ++i) {
      // mark it as being a destruction (short-circuit restore())
      (*i).second->d_map = NULL;
      if(!(*i).second->d_noTrash) {
        (*i).second->deleteSelf();
      }
    }
    d_map.clear();
    d_first = NULL;

    Assert(d_trash.empty());
  }

  void clear() throw(AssertionException) {
    Debug("gc") << "clearing cdhashmap" << this << ", emptying trash" << std::endl;
    emptyTrash();
    Debug("gc") << "done emptying trash for " << this << std::endl;

    for(typename table_type::iterator i = d_map.begin();
        i != d_map.end();
        ++i) {
      // mark it as being a destruction (short-circuit restore())
      (*i).second->d_map = NULL;
      if(!(*i).second->d_noTrash) {
        (*i).second->deleteSelf();
      }
    }
    d_map.clear();
    d_first = NULL;

    Assert(d_trash.empty());
  }

  // The usual operators of map

  size_t size() const {
    return d_map.size();
  }

  bool empty() const {
    return d_map.empty();
  }

  size_t count(const Key& k) const {
    return d_map.count(k);
  }

  // If a key is not present, a new object is created and inserted
  Element& operator[](const Key& k) {
    emptyTrash();

    typename table_type::iterator i = d_map.find(k);

    Element* obj;
    if(i == d_map.end()) {// create new object
      obj = new(true) Element(d_context, this, k, Data());
      d_map[k] = obj;
    } else {
      obj = (*i).second;
    }
    return *obj;
  }

  bool insert(const Key& k, const Data& d) {
    emptyTrash();

    typename table_type::iterator i = d_map.find(k);

    if(i == d_map.end()) {// create new object
      Element* obj = new(true) Element(d_context, this, k, d);
      d_map[k] = obj;
      return true;
    } else {
      (*i).second->set(d);
      return false;
    }
  }

  // Use this for pointer data d allocated in context memory at this
  // level.  THIS IS HIGHLY EXPERIMENTAL.  It seems to work if ALL
  // your data objects are allocated from context memory.
  void insertDataFromContextMemory(const Key& k, const Data& d) {
    emptyTrash();

    AlwaysAssert(d_map.find(k) == d_map.end());

    Element* obj = new(d_context->getCMM()) Element(d_context, this, k, d,
                                                    false /* atLevelZero */,
                                                    true /* allocatedInCMM */);

    d_map[k] = obj;
  }

  /**
   * Version of insert() for CDHashMap<> that inserts data value d at
   * context level zero.  This is a special escape hatch for inserting
   * "initializing" data into the map.  Imagine something happens at a
   * deep context level L that causes insertion into a map, such that
   * the object should have an "initializing" value v1 below context
   * level L, and a "current" value v2 at context level L.  Then you
   * can (assuming key k):
   *
   *   map.insertAtContextLevelZero(k, v1);
   *   map.insert(k, v2);
   *
   * The justification for this "escape hatch" has to do with
   * variables and assignments in theories (e.g., in arithmetic).
   * Let's say you introduce a new variable x at some deep decision
   * level (thanks to lazy registration, or a splitting lemma, or
   * whatever).  x might be mapped to something, but for theory
   * implementation simplicity shouldn't disappear from the map on
   * backjump; rather, it can take another (legal) value, or a special
   * value to indicate it needs to be recomputed.
   *
   * It is an error (checked via AlwaysAssert()) to
   * insertAtContextLevelZero() a key that already is in the map.
   */
  void insertAtContextLevelZero(const Key& k, const Data& d) {
    emptyTrash();

    AlwaysAssert(d_map.find(k) == d_map.end());

    Element* obj = new(true) Element(d_context, this, k, d,
                                     true /* atLevelZero */);
    d_map[k] = obj;
  }

  // FIXME: no erase(), too much hassle to implement efficiently...

  /**
   * "Obliterating" is kind of like erasing, except it's not
   * backtrackable; the key is permanently removed from the map.
   * (Naturally, it can be re-added as a new element.)
   */
  void obliterate(const Key& k) {
    typename table_type::iterator i = d_map.find(k);
    if(i != d_map.end()) {
      Debug("gc") << "key " << k << " obliterated" << std::endl;
      // We can't call ->deleteSelf() here, because it calls the
      // ContextObj destructor, which calls CDOhash_map::destroy(), which
      // restore()'s, which puts the CDOhash_map on the trash, which causes
      // a double-delete.
      (*i).second->~Element();
      // Writing ...->~CDOhash_map() in the above is legal (?) but breaks
      // g++ 4.1, though later versions have no problem.

      typename table_type::iterator j = d_map.find(k);
      // This if() succeeds for objects inserted when in the
      // zero-scope: they were never save()'d there, so restore()
      // never gets a NULL map and so they leak.
      if(j != d_map.end()) {
        Element* elt = (*j).second;
        if(d_first == elt) {
          if(elt->d_next == elt) {
            Assert(elt->d_prev == elt);
            d_first = NULL;
          } else {
            d_first = elt->d_next;
          }
        } else {
          elt->d_prev->d_next = elt->d_next;
          elt->d_next->d_prev = elt->d_prev;
        }
        d_map.erase(j);//FIXME multithreading
        Debug("gc") << "key " << k << " obliterated zero-scope: " << elt << std::endl;
        // was already destructed, so don't call ->deleteSelf()
        if(!elt->d_noTrash) {
          ::operator delete(elt);
        }
      }
    }
  }

  class iterator {
    const Element* d_it;

  public:

    iterator(const Element* p) : d_it(p) {}
    iterator(const iterator& i) : d_it(i.d_it) {}

    // Default constructor
    iterator() {}

    // (Dis)equality
    bool operator==(const iterator& i) const {
      return d_it == i.d_it;
    }
    bool operator!=(const iterator& i) const {
      return d_it != i.d_it;
    }

    // Dereference operators.
    std::pair<const Key, const Data> operator*() const {
      return std::pair<const Key, const Data>(d_it->getKey(), d_it->get());
    }

    // Prefix increment
    iterator& operator++() {
      d_it = d_it->next();
      return *this;
    }

    // Postfix increment: requires a Proxy object to hold the
    // intermediate value for dereferencing
    class Proxy {
      const std::pair<const Key, Data>* d_pair;

    public:

      Proxy(const std::pair<const Key, Data>& p) : d_pair(&p) {}

      const std::pair<const Key, Data>& operator*() const {
        return *d_pair;
      }
    };/* class CDHashMap<>::iterator::Proxy */

    // Actual postfix increment: returns Proxy with the old value.
    // Now, an expression like *i++ will return the current *i, and
    // then advance the iterator.  However, don't try to use
    // Proxy for anything else.
    const Proxy operator++(int) {
      Proxy e(*(*this));
      ++(*this);
      return e;
    }
  };/* class CDHashMap<>::iterator */

  typedef iterator const_iterator;

  iterator begin() const {
    return iterator(d_first);
  }

  iterator end() const {
    return iterator(NULL);
  }

  iterator find(const Key& k) const {
    typename table_type::const_iterator i = d_map.find(k);

    if(i == d_map.end()) {
      return end();
    } else {
      return iterator((*i).second);
    }
  }

  iterator find(const Key& k) {
    emptyTrash();
    return const_cast<const CDHashMap*>(this)->find(k);
  }

};/* class CDHashMap<> */

}/* CVC4::context namespace */
}/* CVC4 namespace */

#endif /* __CVC4__CONTEXT__CDHashMAP_H */
