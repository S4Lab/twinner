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

#include "CommandRunner.h"
#include "Logger.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <sys/prctl.h>


namespace edu {
namespace sharif {
namespace twinner {
namespace util {

CommandRunner *CommandRunner::me = 0;

CommandRunner::CommandRunner () :
    isParent (false) {
}

CommandRunner::~CommandRunner () {
  if (isParent) {
    close (parent2child[1]);
    close (child2parent[0]);
  }
}

bool CommandRunner::forkDaemon (char *argv0) {
  if (pipe (parent2child) == -1) {
    return false;
  }
  if (pipe (child2parent) == -1) {
    close (parent2child[0]);
    close (parent2child[1]);
    return false;
  }
  switch (fork ()) {
  case -1: // error in fork
    close (parent2child[0]);
    close (parent2child[1]);
    close (child2parent[0]);
    close (child2parent[1]);
    return false;

  case 0: // child
    memset (argv0, 0, 30);
    strcpy (argv0, "Commands Runner Daemon");
    close (parent2child[1]); // write-end of parent2child
    close (child2parent[0]); // read-end of child2parent
    std::cerr << "CommandsRunner: daemon is ready!\n";
    waitForCommands ();
    std::cerr << "CommandsRunner: daemon is exiting!\n";
    close (parent2child[0]);
    close (child2parent[1]);
    exit (EXIT_SUCCESS);

  default: // parent
    close (parent2child[0]); // read-end of parent2child
    close (child2parent[1]); // write-end of child2parent
    isParent = true;
    return true;
  }
}

bool CommandRunner::runCommand (const char *command, int commandSize,
    int &retval) {
  const ExecType type = SYSTEM;
  const int bufSize = sizeof (type) + sizeof (commandSize) + commandSize;
  char *buf = new char[bufSize];
  char *ptr = buf;
  memcpy (ptr, &type, sizeof (type));
  ptr += sizeof (type);
  memcpy (ptr, &commandSize, sizeof (commandSize));
  ptr += sizeof (commandSize);
  memcpy (ptr, command, commandSize);
  if (write (parent2child[1], buf, bufSize) != bufSize) {
    return false;
  }
  if (read (child2parent[0], &retval, sizeof (retval)) != sizeof (retval)) {
    return false;
  }
  return true;
}

bool CommandRunner::runCommand (const char *command, int commandSize,
    char *response, int responseSize) {
  int retval;
  return runCommand (command, commandSize, response, responseSize, retval);
}

bool CommandRunner::runCommand (const char *command, int commandSize,
    char *response, int responseSize, int &retval) {
  edu::sharif::twinner::util::Logger::debug ()
      << "runCommand (command=" << command << ", commandSize=" << std::dec
      << commandSize << ", responseSize=" << responseSize << ")\n";
  const ExecType type = PIPE;
  const int bufSize = sizeof (type)
      + sizeof (commandSize) + sizeof (responseSize) + commandSize;
  char *buf = new char[bufSize];
  char *ptr = buf;
  memcpy (ptr, &type, sizeof (type));
  ptr += sizeof (type);
  memcpy (ptr, &commandSize, sizeof (commandSize));
  ptr += sizeof (commandSize);
  memcpy (ptr, &responseSize, sizeof (responseSize));
  ptr += sizeof (responseSize);
  memcpy (ptr, command, commandSize);
  if (write (parent2child[1], buf, bufSize) != bufSize) {
    return false;
  }
  if (read (child2parent[0], response, responseSize) != responseSize) {
    return false;
  }
  if (read (child2parent[0], &retval, sizeof (retval)) != sizeof (retval)) {
    return false;
  }
  return true;
}

void CommandRunner::waitForCommands () {
  for (;;) {
    ExecType type;
    if (read (parent2child[0], &type, sizeof (type)) != sizeof (type)) {
      std::cerr << "CommandsRunner: Warning: cannot read command type!\n";
      return;
    }
    int commandSize;
    if (read (parent2child[0], &commandSize, sizeof (commandSize)) != sizeof (commandSize)) {
      std::cerr << "CommandsRunner: Error: cannot read commandSize!\n";
      return;
    }
    int responseSize = 0;
    if (type == PIPE
        && read (parent2child[0], &responseSize, sizeof (responseSize)) != sizeof (responseSize)) {
      std::cerr << "CommandsRunner: Error: cannot read responseSize!\n";
      return;
    }
    char *command = new char[commandSize];
    if (read (parent2child[0], command, commandSize) != commandSize) {
      std::cerr << "CommandsRunner: Error: cannot read command!\n";
      delete[] command;
      return;
    }
    int retval;
    char *response = new char[responseSize + sizeof (retval)];
    if (!executeCommand (type, command, response, responseSize, retval)) {
      delete[] command;
      delete[] response;
      return;
    }
    delete[] command;
    memcpy (response + responseSize, &retval, sizeof (retval));
    responseSize += sizeof (retval);
    if (write (child2parent[1], response, responseSize) != responseSize) {
      std::cerr << "CommandsRunner: Error: cannot write response!\n";
      delete[] response;
      return;
    }
    delete[] response;
  }
}

bool CommandRunner::executeCommand (ExecType type, const char *command,
    char *response, int responseSize, int &retval) {
  if (type == SYSTEM) {
    retval = system (command);
    return true;
  }
  FILE *fp = popen (command, "r");
  if (fp == NULL) {
    const int e = errno;
    std::cerr << "CommandsRunner: Cannot execute command: " << command << '\n';
    std::cerr << "strerror(errno): " << strerror (e) << '\n';
    return false;
  }
  if (responseSize > 0 && fgets (response, responseSize, fp) == NULL) {
    std::cerr << "CommandsRunner: Error: Cannot read output of the command.\n";
    return false;
  }
  retval = pclose (fp);
  return true;
}

bool CommandRunner::init (char *argv0) {
  if (CommandRunner::me) {
    return false;
  }
  CommandRunner::me = new CommandRunner ();
  if (!CommandRunner::me->forkDaemon (argv0)) {
    destroy ();
    return false;
  }
  return true;
}

CommandRunner *CommandRunner::getInstance () {
  return CommandRunner::me;
}

void CommandRunner::destroy () {
  delete CommandRunner::me;
  CommandRunner::me = 0;
}

}
}
}
}
