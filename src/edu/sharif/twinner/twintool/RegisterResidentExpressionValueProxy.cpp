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

#include "RegisterResidentExpressionValueProxy.h"

#include "edu/sharif/twinner/trace/Trace.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

RegisterResidentExpressionValueProxy::RegisterResidentExpressionValueProxy (
    REG _reg, UINT64 _regVal) :
reg (_reg), regVal (_regVal) {
}

edu::sharif::twinner::trace::Expression *
RegisterResidentExpressionValueProxy::getExpression (
    edu::sharif::twinner::trace::Trace *trace) const {
  return trace->getSymbolicExpressionByRegister (reg, regVal);
}

void RegisterResidentExpressionValueProxy::setExpression (
    edu::sharif::twinner::trace::Trace *trace,
    const edu::sharif::twinner::trace::Expression *exp) const {
  trace->setSymbolicExpressionByRegister (reg, exp);
}

}
}
}
}
