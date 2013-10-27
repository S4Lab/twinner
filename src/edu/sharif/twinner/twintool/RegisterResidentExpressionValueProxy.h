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

#ifndef REGISTER_RESIDENT_EXPRESSION_VALUE_PROXY_H
#define REGISTER_RESIDENT_EXPRESSION_VALUE_PROXY_H

#include "MutableExpressionValueProxy.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

class RegisterResidentExpressionValueProxy : public MutableExpressionValueProxy {

private:
  REG reg;
  UINT64 regVal;

public:
  RegisterResidentExpressionValueProxy (REG reg, UINT64 regVal);
};

}
}
}
}

#endif /* RegisterResidentExpressionValueProxy.h */
