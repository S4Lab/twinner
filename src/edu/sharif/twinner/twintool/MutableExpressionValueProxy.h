//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2014  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef MUTABLE_EXPRESSION_VALUE_PROXY_H
#define MUTABLE_EXPRESSION_VALUE_PROXY_H

#include "ExpressionValueProxy.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

/**
 * @interface An interface which describes mutable expression value proxies. These
 * expression values can be read same as ExpressionValueProxy instances and also
 * can be set without being read. Setting without any previous get call, guarantees
 * that there is no side-effect.
 */
class MutableExpressionValueProxy : public ExpressionValueProxy {

protected:

  MutableExpressionValueProxy () {
  }

public:

  virtual ~MutableExpressionValueProxy () {
  }

  /**
   * Same as setExpressionWithoutChangeNotification() method, but also calls the
   * valueIsChanged() method to notify about changes.
   * @param trace The execution trace which changing expression will be saved there.
   * @param exp The new expression which should be cloned and set over the proxied
   * expression.
   * 
   * @return The newly cloned expression which is set by this method.
   */
  virtual edu::sharif::twinner::trace::Expression *setExpression (
      edu::sharif::twinner::trace::Trace *trace,
      const edu::sharif::twinner::trace::Expression *exp) const {
    edu::sharif::twinner::trace::Expression *newExp =
        setExpressionWithoutChangeNotification (trace, exp);
    valueIsChanged (trace, newExp);
    return newExp;
  }

  /**
   * Sets the proxied expression as the given exp value within the given execution trace.
   * @param trace The execution trace which changing expression will be saved there.
   * @param exp The new expression which should be cloned and set over the proxied
   * expression.
   * 
   * @return The newly cloned expression which is set by this method.
   */
  virtual edu::sharif::twinner::trace::Expression *
  setExpressionWithoutChangeNotification (
      edu::sharif::twinner::trace::Trace *trace,
      const edu::sharif::twinner::trace::Expression *exp) const = 0;

  /**
   * Truncates the given expression, so it can be fitted in the underlying expression.
   * Underlying expression won't be touched at all.
   * @param exp The expression who should be truncated to be fitted in this expression.
   */
  virtual void truncate (edu::sharif::twinner::trace::Expression *exp) const = 0;

  /**
   * Finds size of the underlying expression container. For example memory read/write
   * bits or register's size in bits.
   * @return Size of the container of the underlying expression (mem or reg) in bits.
   */
  virtual int getSize () const = 0;
};

}
}
}
}

#endif /* MutableExpressionValueProxy.h */
