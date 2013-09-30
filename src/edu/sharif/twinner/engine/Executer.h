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

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Trace;
}
namespace engine {

class Executer {

public:
  Executer (int pinLauncher, int inputBinary);

  void setSymbolsValues ();

  edu::sharif::twinner::trace::Trace executeSingleTrace ();
};

}
}
}
}
