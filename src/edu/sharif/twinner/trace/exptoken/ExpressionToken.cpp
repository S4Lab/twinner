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

#include "ExpressionToken.h"
#include "Constant.h"
#include "MemoryEmergedSymbol.h"
#include "RegisterEmergedSymbol.h"
#include "Operator.h"
#include "NamedSymbol.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

ExpressionToken::ExpressionToken () {
}

ExpressionToken::ExpressionToken (const ExpressionToken &et) {
}

ExpressionToken::~ExpressionToken () {
}

ExpressionToken *ExpressionToken::loadFromBinaryStream (std::ifstream &in) {
  char type;
  in.read (&type, 1);
  switch (type) {
  case 'C':
    return Constant::loadFromBinaryStream (in);
  case 'M':
    return MemoryEmergedSymbol::loadFromBinaryStream (in);
  case 'R':
    return RegisterEmergedSymbol::loadFromBinaryStream (in);
  case 'O':
    return Operator::loadFromBinaryStream (in);
  case 'N':
    return NamedSymbol::loadFromBinaryStream (in);
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Unknown expression token's type: " << type << '\n';
    abort ();
  }
}

std::string ExpressionToken::getTechnicalName () const {
  return toString ();
}

}
}
}
}
}
