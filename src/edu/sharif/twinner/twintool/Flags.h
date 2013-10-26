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

#ifndef FLAGS_H
#define FLAGS_H

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Expression;
}
namespace twintool {

class Flags {

private:
  edu::sharif::twinner::trace::Expression *exp;

public:
  Flags ();
  ~Flags ();

};

}
}
}
}

#endif /* Flags.h */
