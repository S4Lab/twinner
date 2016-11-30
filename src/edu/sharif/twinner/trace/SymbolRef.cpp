//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2016 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "SymbolRef.h"

#include "edu/sharif/twinner/trace/exptoken/Symbol.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

SymbolRef::SymbolRef (
    const edu::sharif::twinner::trace::exptoken::Symbol *_symbol) :
    symbol (*_symbol) {
}

SymbolRef::operator const edu::sharif::twinner::trace::exptoken::Symbol &() {
  return symbol;
}

bool SymbolRef::operator== (const SymbolRef &sr) const {
  return symbol == sr.symbol;
}

bool SymbolRef::operator< (const SymbolRef &sr) const {
  return symbol < sr.symbol;
}

}
}
}
}
