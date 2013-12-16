/*********************                                                        */
/*! \file tls.h.in
 ** \verbatim
 ** Original author: ACSYS
 ** Major contributors: Morgan Deters
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Header to define CVC4_THREAD whether or not TLS is
 ** supported by the compiler/runtime platform
 **
 ** Header to define CVC4_THREAD whether or not TLS is supported by
 ** the compiler/runtime platform.  If not, an implementation based on
 ** pthread_getspecific() / pthread_setspecific() is given.
 **/

#include <cvc4/cvc4_public.h>

#ifndef __CVC4__TLS_H
#define __CVC4__TLS_H

// A bit obnoxious: we have to take varargs to support multi-argument
// template types in the threadlocals.
// E.g. "CVC4_THREADLOCAL(hash_set<type, hasher>*)" fails otherwise,
// due to the embedded comma.
#if 1
#  define CVC4_THREADLOCAL(__type...) __thread __type
#  define CVC4_THREADLOCAL_PUBLIC(__type...) __thread CVC4_PUBLIC __type
#  define CVC4_THREADLOCAL_TYPE(__type...) __type
#else
#  include <pthread.h>
#  define CVC4_THREADLOCAL(__type...) ::CVC4::ThreadLocal< __type >
#  define CVC4_THREADLOCAL_PUBLIC(__type...) CVC4_PUBLIC ::CVC4::ThreadLocal< __type >
#  define CVC4_THREADLOCAL_TYPE(__type...) ::CVC4::ThreadLocal< __type >

namespace CVC4 {

template <class T, bool small>
class ThreadLocalImpl;

template <class T>
class ThreadLocalImpl<T, true> {
  pthread_key_t d_key;

  static void cleanup(void*) {
  }

public:
  ThreadLocalImpl() {
    pthread_key_create(&d_key, ThreadLocalImpl::cleanup);
  }

  ThreadLocalImpl(const T& t) {
    pthread_key_create(&d_key, ThreadLocalImpl::cleanup);
    pthread_setspecific(d_key, const_cast<void*>(reinterpret_cast<const void*>(t)));
  }

  ThreadLocalImpl(const ThreadLocalImpl& tl) {
    pthread_key_create(&d_key, ThreadLocalImpl::cleanup);
    pthread_setspecific(d_key, const_cast<void*>(reinterpret_cast<const void*>(static_cast<const T&>(tl))));
  }

  ThreadLocalImpl& operator=(const T& t) {
    pthread_setspecific(d_key, const_cast<void*>(reinterpret_cast<const void*>(t)));
    return *this;
  }
  ThreadLocalImpl& operator=(const ThreadLocalImpl& tl) {
    pthread_setspecific(d_key, const_cast<void*>(reinterpret_cast<const void*>(static_cast<const T&>(tl))));
    return *this;
  }

  operator T() const {
    return static_cast<T>(reinterpret_cast<size_t>(pthread_getspecific(d_key)));
  }
};/* class ThreadLocalImpl<T, true> */

template <class T>
class ThreadLocalImpl<T*, true> {
  pthread_key_t d_key;

  static void cleanup(void*) {
  }

public:
  ThreadLocalImpl() {
    pthread_key_create(&d_key, ThreadLocalImpl::cleanup);
  }

  ThreadLocalImpl(const T* t) {
    pthread_key_create(&d_key, ThreadLocalImpl::cleanup);
    pthread_setspecific(d_key, const_cast<void*>(reinterpret_cast<const void*>(t)));
  }

  ThreadLocalImpl(const ThreadLocalImpl& tl) {
    pthread_key_create(&d_key, ThreadLocalImpl::cleanup);
    pthread_setspecific(d_key, const_cast<void*>(reinterpret_cast<const void*>(static_cast<const T*>(tl))));
  }

  ThreadLocalImpl& operator=(const T* t) {
    pthread_setspecific(d_key, const_cast<void*>(reinterpret_cast<const void*>(t)));
    return *this;
  }
  ThreadLocalImpl& operator=(const ThreadLocalImpl& tl) {
    pthread_setspecific(d_key, const_cast<void*>(reinterpret_cast<const void*>(static_cast<const T*>(tl))));
    return *this;
  }

  operator T*() const {
    return static_cast<T*>(pthread_getspecific(d_key));
  }

  T operator*() {
    return *static_cast<T*>(pthread_getspecific(d_key));
  }
  T* operator->() {
    return static_cast<T*>(pthread_getspecific(d_key));
  }
};/* class ThreadLocalImpl<T*, true> */

template <class T>
class ThreadLocalImpl<T, false> {
};/* class ThreadLocalImpl<T, false> */

template <class T>
class ThreadLocal : public ThreadLocalImpl<T, sizeof(T) <= sizeof(void*)> {
  typedef ThreadLocalImpl<T, sizeof(T) <= sizeof(void*)> super;

public:
  ThreadLocal() : super() {}
  ThreadLocal(const T& t) : super(t) {}
  ThreadLocal(const ThreadLocal<T>& tl) : super(tl) {}

  ThreadLocal<T>& operator=(const T& t) {
    return static_cast< ThreadLocal<T>& >(super::operator=(t));
  }
  ThreadLocal<T>& operator=(const ThreadLocal<T>& tl) {
    return static_cast< ThreadLocal<T>& >(super::operator=(tl));
  }
};/* class ThreadLocal<T> */

template <class T>
class ThreadLocal<T*> : public ThreadLocalImpl<T*, sizeof(T*) <= sizeof(void*)> {
  typedef ThreadLocalImpl<T*, sizeof(T*) <= sizeof(void*)> super;

public:
  ThreadLocal() : super() {}
  ThreadLocal(T* t) : super(t) {}
  ThreadLocal(const ThreadLocal<T*>& tl) : super(tl) {}

  ThreadLocal<T*>& operator=(T* t) {
    return static_cast< ThreadLocal<T*>& >(super::operator=(t));
  }
  ThreadLocal<T*>& operator=(const ThreadLocal<T*>& tl) {
    return static_cast< ThreadLocal<T*>& >(super::operator=(tl));
  }
  // special operators for pointers
  T& operator*() {
    return *static_cast<T*>(*this);
  }
  const T& operator*() const {
    return *static_cast<const T*>(*this);
  }
  T* operator->() {
    return static_cast<T*>(*this);
  }
  const T* operator->() const {
    return static_cast<const T*>(*this);
  }
  T* operator++() {
    T* p = *this;
    *this = ++p;
    return p;
  }
  T* operator++(int) {
    T* p = *this;
    *this = p + 1;
    return p;
  }
  T* operator--() {
    T* p = *this;
    *this = --p;
    return p;
  }
  T* operator--(int) {
    T* p = *this;
    *this = p - 1;
    return p;
  }
};/* class ThreadLocal<T*> */

}/* CVC4 namespace */

#endif /* 1 */

#endif /* __CVC4__TLS_H */
