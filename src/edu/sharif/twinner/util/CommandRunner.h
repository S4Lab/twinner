//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2018 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef COMMAND_RUNNER_H
#define COMMAND_RUNNER_H

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

class CommandRunner {
private:
  static CommandRunner *me;

  enum ExecType {
    SYSTEM = 1,
    PIPE,
  };

  bool isParent;
  int parent2child[2];
  int child2parent[2];

  CommandRunner ();

public:
  ~CommandRunner ();

  bool runCommand (const char *command, int commandSize, int &retval);
  bool runCommand (const char *command, int commandSize,
      char *response, int responseSize);
  bool runCommand (const char *command, int commandSize,
      char *response, int responseSize, int &retval);

  static bool init (char *argv0);
  static CommandRunner *getInstance ();
  static void destroy ();

private:
  bool forkDaemon (char *argv0);
  void waitForCommands ();
  bool executeCommand (ExecType type, const char *command,
      char *response, int responseSize, int &retval);
};

}
}
}
}

#endif /* CommandRunner.h */
