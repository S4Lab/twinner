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

#ifndef TWIN_TOOL_H
#define TWIN_TOOL_H

#include "pin.H"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

class Instrumenter;

class TwinTool {

private:
  Instrumenter *im;

public:
  TwinTool ();
  ~TwinTool ();

  INT32 run (int argc, char *argv[]);

private:
  template < typename T >
  INT32 printError (const T &msg) const;
  INT32 printUsage () const;

  bool parseArgumentsAndInitializeTool ();

  void registerInstrumentationRoutines ();
};

}
}
}
}

#endif /* TwinTool.h */
