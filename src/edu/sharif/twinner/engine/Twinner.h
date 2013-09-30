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
namespace engine {

class Twinner {

public:
  void setPinLauncherPath ();

  void setInputBinaryPath ();

  void setTwinBinaryPath ();

  void generateTwinBinary ();

private:
  void calculateSymbolsValuesForCoveringNextPath ();

  void codeTracesIntoTwinBinary ();
};

}
}
}
}
