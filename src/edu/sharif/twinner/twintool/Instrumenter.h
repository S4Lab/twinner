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

#ifndef INSTRUMENTER_H
#define INSTRUMENTER_H

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

class Instrumenter {

public:
  void instrumentSingleInstruction ();

  void syscallEntry ();
};

}
}
}
}

#endif /* Instrumenter.h */
