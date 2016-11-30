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

#ifndef SYMBOL_REF_H
#define SYMBOL_REF_H

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

class Symbol;
}

class SymbolRef {
private:
  const edu::sharif::twinner::trace::exptoken::Symbol &symbol;

public:
  SymbolRef (const edu::sharif::twinner::trace::exptoken::Symbol *symbol);

  operator const edu::sharif::twinner::trace::exptoken::Symbol &();
  bool operator== (const SymbolRef &sr) const;
  bool operator< (const SymbolRef &sr) const;
};

}
}
}
}

#endif /* SymbolRef.h */
