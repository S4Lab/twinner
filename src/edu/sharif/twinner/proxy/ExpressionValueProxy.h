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

#ifndef EXPRESSION_VALUE_PROXY_H
#define EXPRESSION_VALUE_PROXY_H

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Expression;
class Trace;
class StateSummary;

namespace cv {

class ConcreteValue;
}
}
namespace proxy {

/**
 * @interface An interface which allows an expression value to be read.
 * Implementations won't cause any side-effect before reading from them,
 * but upon first read/get side-effects are allowed and possible (for example
 * an implementation which generates new symbol when there was no previous one
 * may cause side-effect upon such read failures).
 */
class ExpressionValueProxy {
private:
  ExpressionValueProxy (const edu::sharif::twinner::proxy::ExpressionValueProxy &evp); /// disable copy-constructor

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
   * @param state Indicates whether the last seen concrete value differs
   * from the expected value.
   *
   * @return The cloned expression object.
   */
  virtual edu::sharif::twinner::trace::Expression *getExpression (
      edu::sharif::twinner::trace::Trace *trace,
      edu::sharif::twinner::trace::StateSummary &state) const = 0;

  /**
   * Returns the concrete value which the underlying expression is supposed
   * to have. The returned object is owned by the caller.
   *
   * @return The cloned concrete value of the supposed underlying expression.
   */
  virtual edu::sharif::twinner::trace::cv::ConcreteValue *getConcreteValue () const = 0;

  /**
   * This method should be called whenever the proxied expression undergoes any change.
   * @param trace The trace which changed expression has been kept there.
   * @param changedExp The expression which is changed.
   * @param state Indicates whether the last seen concrete value differs
   * from the expected value (this may happen due to the overlapping locations).
   */
  virtual void valueIsChanged (
      edu::sharif::twinner::trace::Trace *trace,
      const edu::sharif::twinner::trace::Expression &changedExp,
      edu::sharif::twinner::trace::StateSummary &state) const = 0;
};

}
}
}
}

#endif /* ExpressionValueProxy.h */
