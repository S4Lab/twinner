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

#ifndef INSTRUCTION_SYMBOLIC_EXECUTER_H
#define INSTRUCTION_SYMBOLIC_EXECUTER_H

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

class InstructionSymbolicExecuter {

public:
  void readMemory ();

  void writeMemory ();

  void branch ();
};

}
}
}
}

#endif /* InstructionSymbolicExecuter.h */
