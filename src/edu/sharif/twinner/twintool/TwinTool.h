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

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

class TwinTool {
public:
  /**
   * Indicating name of the temp file, being used to communicate with Executer about the last execution trace.
   *
   * This must be kept in sync with edu::sharif::twinner::engine::Executer::EXECUTION_TRACE_COMMUNICATION_TEMP_FILE field.
   * As this (TwinTool package) and the Engine package are not linked together in anyway, this code should be written
   * redundantly and be kept in sync manually.
   */
  static const char *EXECUTION_TRACE_COMMUNICATION_TEMP_FILE;

  void run ();

private:
  void registerInstrumentationRoutines ();
};

}
}
}
}

#endif /* TwinTool.h */
