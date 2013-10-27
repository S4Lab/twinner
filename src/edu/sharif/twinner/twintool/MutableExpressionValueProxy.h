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

  virtual void setExpression (edu::sharif::twinner::trace::Trace *trace,
      edu::sharif::twinner::trace::Expression *exp) = 0;
};

}
}
}
}

#endif /* MutableExpressionValueProxy.h */
