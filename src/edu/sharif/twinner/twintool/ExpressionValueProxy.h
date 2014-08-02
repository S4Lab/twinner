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

#ifndef EXPRESSION_VALUE_PROXY_H
#define EXPRESSION_VALUE_PROXY_H

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Expression;
class Trace;
}
namespace twintool {

/**
 * @interface An interface which allows an expression value to be read.
 * Implementations won't cause any side-effect before reading from them,
 * but upon first read/get side-effects are allowed and possible (for example
 * an implementation which generates new symbol when there was no previous one
 * may cause side-effect upon such read failures).
 */
class ExpressionValueProxy {

private:
  ExpressionValueProxy (const ExpressionValueProxy &evp); /// disable copy-constructor

protected:

  /**
   * Protected constructor prevents direct instantiation of this interface
   */
  ExpressionValueProxy () {
  }

public:

  virtual ~ExpressionValueProxy () {
  }


  /**
   * Returns expression. This call may have side-effects.
   * Changes on the returned expression won't affect the stored one. Indeed a cloned
   * instance is returned which must be deleted by the caller. For changing the underlying
   * instance, the setter methods must be used.
   * @param trace Trace object which this expression will be resolved upon it.
   * @return The cloned expression object.
   */
  virtual edu::sharif::twinner::trace::Expression *getExpression (
      edu::sharif::twinner::trace::Trace *trace) const = 0;

  /**
   * This method should be called whenever the proxied expression undergoes any change.
   * @param trace The trace which changed expression has been kept there.
   * @param changedExp The expression which is changed.
   */
  virtual void valueIsChanged (
      edu::sharif::twinner::trace::Trace *trace,
      const edu::sharif::twinner::trace::Expression &changedExp) const = 0;
};

}
}
}
}

#endif	/* ExpressionValueProxy.h */
