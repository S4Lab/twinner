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

#ifndef TWINNER_H
#define TWINNER_H

#include <string>

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {

class Twinner {

public:
  void setInputBinaryPath (std::string input);
  void setPinLauncherPath (std::string pin);
  void setTwinBinaryPath (std::string twin);

  void generateTwinBinary ();

private:
  void calculateSymbolsValuesForCoveringNextPath ();

  void codeTracesIntoTwinBinary ();
};

}
}
}
}

#endif /* Twinner.h */
