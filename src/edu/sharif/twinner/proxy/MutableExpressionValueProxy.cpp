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

#include "MutableExpressionValueProxy.h"

#include "edu/sharif/twinner/trace/Expression.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace proxy {

void MutableExpressionValueProxy::setExpression (
    edu::sharif::twinner::trace::Trace *trace,
    const edu::sharif::twinner::trace::Expression *exp,
    edu::sharif::twinner::trace::StateSummary &state) const {
  valueIsChanged
      (trace, setExpressionWithoutChangeNotification (trace, exp), state);
}

void MutableExpressionValueProxy::truncate (
    edu::sharif::twinner::trace::Expression *exp) const {
  exp->truncate (getSize ());
}

}
}
}
}
