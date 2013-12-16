/*********************                                                        */
/*! \file cdo.h
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: none
 ** Minor contributors (to current version): Clark Barrett, Francois Bobot
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief A context-dependent object.
 **
 ** A context-dependent object.
 **/

#include "cvc4_private.h"

#ifndef __CVC4__CONTEXT__CDO_H
#define __CVC4__CONTEXT__CDO_H

#include "context/context.h"
#include "util/cvc4_assert.h"

namespace CVC4 {
namespace context {

/**
 * Most basic template for context-dependent objects.  Simply makes a copy
 * (using the copy constructor) of class T when saving, and copies it back
 * (using operator=) during restore.
 */
template <class T>
class CDO : public ContextObj {

  /**
   * The data of type T being stored in this context-dependent object.
   */
  T d_data;

protected:

  /**
   * Copy constructor - it's private to ensure it is only used by save().
   * Basic CDO objects, cannot be copied-they have to be unique.
   */
  CDO(const CDO<T>& cdo) : ContextObj(cdo), d_data(cdo.d_data) {}

  /**
   * operator= for CDO is private to ensure CDO object is not copied.
   */
  CDO<T>& operator=(const CDO<T>& cdo) CVC4_UNUSED;

  /**
   * Implementation of mandatory ContextObj method save: simply copies the
   * current data to a copy using the copy constructor.  Memory is allocated
   * using the ContextMemoryManager.
   */
  virtual ContextObj* save(ContextMemoryManager* pCMM) {
    Debug("context") << "save cdo " << this;
    ContextObj* p = new(pCMM) CDO<T>(*this);
    Debug("context") << " to " << p << std::endl;
    return p;
  }

  /**
   * Implementation of mandatory ContextObj method restore: simply copies the
   * saved data back from the saved copy using operator= for T.
   */
  virtual void restore(ContextObj* pContextObj) {
    //Debug("context") << "restore cdo " << this;
    d_data = ((CDO<T>*) pContextObj)->d_data;
    //Debug("context") << " to " << get() << std::endl;
  }

public:

  /**
   * Main constructor - uses default constructor for T to create the initial
   * value of d_data.
   */
  CDO(Context* context) :
    ContextObj(context),
    d_data(T()) {
  }

  /**
   * Main constructor - uses default constructor for T to create the
   * initial value of d_data.
   *
   * This version takes an argument that specifies whether this CDO<>
   * was itself allocated in context memory.  If it was, it is linked
   * with the current scope rather than scope 0.
   *
   * WARNING: Read the notes in src/context/context.h on "Gotchas when
   * allocating contextual objects with non-standard allocators."
   */
  CDO(bool allocatedInCMM, Context* context) :
    ContextObj(allocatedInCMM, context),
    d_data(T()) {
  }

  /**
   * Constructor from object of type T.  Creates a ContextObj and sets the data
   * to the given data value.  Note that this value is only valid in the
   * current Scope.  If the Scope is popped, the value will revert to whatever
   * is assigned by the default constructor for T
   */
  CDO(Context* context, const T& data) :
    ContextObj(context),
    d_data(T()) {
    makeCurrent();
    d_data = data;
  }

  /**
   * Constructor from object of type T.  Creates a ContextObj and sets the data
   * to the given data value.  Note that this value is only valid in the
   * current Scope.  If the Scope is popped, the value will revert to whatever
   * is assigned by the default constructor for T.
   *
   * This version takes an argument that specifies whether this CDO<>
   * was itself allocated in context memory.  If it was, it is linked
   * with the current scope rather than scope 0.
   *
   * WARNING: Read the notes in src/context/context.h on "Gotchas when
   * allocating contextual objects with non-standard allocators."
   */
  CDO(bool allocatedInCMM, Context* context, const T& data) :
    ContextObj(allocatedInCMM, context),
    d_data(T()) {
    makeCurrent();
    d_data = data;
  }

  /**
   * Destructor - call destroy() method
   */
  ~CDO() throw(AssertionException) { destroy(); }

  /**
   * Set the data in the CDO.  First call makeCurrent.
   */
  void set(const T& data) {
    makeCurrent();
    d_data = data;
  }

  /**
   * Get the current data from the CDO.  Read-only.
   */
  const T& get() const { return d_data; }

  /**
   * For convenience, define operator T() to be the same as get().
   */
  operator T() { return get(); }

  /**
   * For convenience, define operator const T() to be the same as get().
   */
  operator const T() const { return get(); }

  /**
   * For convenience, define operator= that takes an object of type T.
   */
  CDO<T>& operator=(const T& data) {
    set(data);
    return *this;
  }

};/* class CDO */

}/* CVC4::context namespace */
}/* CVC4 namespace */

#endif /* __CVC4__CONTEXT__CDO_H */
