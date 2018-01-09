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

#if 1

#include "edu/sharif/twinner/pin-wrapper.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/CommandRunner.h"

#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <fstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {

struct Measurement {
  int ret; // full return value including termination and exit codes
  uint64_t cputime; // including user/system/kernel times in microseconds
  uint64_t mss; // maximum segment size (in kilobytes)
};

/**
 * Indicating name of the temp file, being used to communicate
 * the overhead measurements from the forked children process (which runs the
 * Twintool) to its parent (i.e. Twinner).
 */
static const char *OVERHEAD_MEASUREMENT_COMMUNICATION_TEMP_FILE =
    "/tmp/twinner/measurements.dat";

Measurement measureCurrentState (int ret);

bool executeAndMeasure (std::string command, Measurement &m) {
  // TODO: remove this fork() and move it to CommandRunner
  pid_t childPid = fork ();
  if (childPid < 0) {
    edu::sharif::twinner::util::Logger::error ()
        << "executeAndMeasure (...): Cannot fork!\n";
    return false;
  } else if (childPid == 0) { // executed in the child process
    int ret;
    if (!edu::sharif::twinner::util::CommandRunner::getInstance ()
        ->runCommand (command.c_str (), command.size () + 1, ret)) {
      edu::sharif::twinner::util::Logger::error ()
          << "Error in command execution [command=" << command << "]\n";
      abort ();
    }
    Measurement measurement = measureCurrentState (ret);
    std::ofstream out;
    const char *path = OVERHEAD_MEASUREMENT_COMMUNICATION_TEMP_FILE;
    out.open (path, ios_base::out | ios_base::trunc | ios_base::binary);
    if (!out.is_open ()) {
      edu::sharif::twinner::util::Logger::error ()
          << "Can not write measurement info:"
          " Error in open function: " << path << '\n';
    } else {
      out.write ((const char *) &measurement, sizeof (measurement));
      out.close ();
    }
    exit (ret);

  } else { // executed in the parent process
    int status;
    if (childPid != waitpid (childPid, &status, 0)) {
      edu::sharif::twinner::util::Logger::warning ()
          << "Error while waiting for child\n";
    }
    std::ifstream in;
    const char *path = OVERHEAD_MEASUREMENT_COMMUNICATION_TEMP_FILE;
    in.open (path, ios_base::in | ios_base::binary);
    if (!in.is_open ()) {
      edu::sharif::twinner::util::Logger::error ()
          << "Can not read measurement info:"
          " Error in open function: " << path << '\n';
    } else {
      in.read ((char *) &m, sizeof (m));
      in.close ();
      const int cmdret = WIFEXITED (status) ? WEXITSTATUS (status) : -1 /*signaled*/;
      if (WIFEXITED (status) && m.ret != WEXITSTATUS (status)) {
        edu::sharif::twinner::util::Logger::error ()
            << "Measurement info are inconsistent (cmd ret: "
            << std::dec << cmdret << ", measurement ret: " << m.ret << ")\n";
      }
    }
  }
  return true;
}

UINT64 operator+ (struct timeval a, struct timeval b) {
  return (a.tv_sec + b.tv_sec) * 1000 * 1000 + (a.tv_usec + b.tv_usec);
}

Measurement measureCurrentState (int ret) {
  Measurement measurement;
  measurement.ret = ret;
  struct rusage usage;
  if (getrusage (RUSAGE_CHILDREN, &usage) != 0) {
    edu::sharif::twinner::util::Logger::error () << "Error in getrusage()\n";
  } else {
    measurement.cputime = usage.ru_utime + usage.ru_stime;
    measurement.mss = usage.ru_maxrss;
  }
  return measurement;
}

}
}
}
}

#endif
