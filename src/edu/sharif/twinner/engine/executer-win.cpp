//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2015  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#if 0

#include "edu/sharif/twinner/pin-wrapper.h"

#include <string>
#include <windows.h>
#include <psapi.h>

#include "inc/unistd-win.h"

#define UINT8 uint8_t
#define UINT16 uint16_t
#define UINT32 uint32_t
#define UINT64 uint64_t

typedef UINT16 OPCODE;

#include "edu/sharif/twinner/util/Logger.h"


namespace edu {
namespace sharif {
namespace twinner {
namespace engine {

struct Measurement {
  int ret; // full return value including termination and exit codes
  uint64_t cputime; // including user/system/kernel times in microseconds
  uint64_t mss; // maximum segment size (in kilobytes)
};

Measurement measureCurrentState (HANDLE hProcess);

bool executeAndMeasure (std::string command, Measurement &m) {
  STARTUPINFO si;
  ZeroMemory (&si, sizeof (si));
  si.cb = sizeof (si);

  PROCESS_INFORMATION pi;
  ZeroMemory (&pi, sizeof (pi));
  const int size = command.length () + 1; // size including the null terminator
  char *cmd = new char[size];
  memcpy (cmd, command.c_str (), size);
  // Start the child process.
  if (!CreateProcess (NULL, // No module name (use command line)
                      cmd, // Command line
                      NULL, // Process handle not inheritable
                      NULL, // Thread handle not inheritable
                      FALSE, // Set handle inheritance to FALSE
                      0, // No creation flags
                      NULL, // Use parent's environment block
                      NULL, // Use parent's starting directory
                      &si, // Pointer to STARTUPINFO structure
                      &pi) // Pointer to PROCESS_INFORMATION structure
      ) {
    edu::sharif::twinner::util::Logger::error ()
        << "Executer::executeAndMeasure (...): "
        "Error in the CreateProcess function (error code="
        << GetLastError () << ")\n";
    delete[] cmd;
    return false;
  }
  delete[] cmd;
  WaitForSingleObject (pi.hProcess, INFINITE);

  m = measureCurrentState (pi.hProcess);

  CloseHandle (pi.hProcess);
  CloseHandle (pi.hThread);
  return true;
}

UINT64 operator+ (SYSTEMTIME a, SYSTEMTIME b) {
  return UINT64 (a.wHour + b.wHour) * 60 * 60 * 1000 * 1000 +
      UINT64 (a.wMinute + b.wMinute) * 60.0 * 1000 * 1000 +
      UINT64 (a.wSecond + b.wSecond) * 1000 * 1000 +
      UINT64 (a.wMilliseconds + b.wMilliseconds) * 1000;
}

Measurement measureCurrentState (HANDLE hProcess) {
  Measurement measurement;

  DWORD status = 0;
  GetExitCodeProcess (hProcess, &status);

  measurement.ret = status;

  FILETIME createTime;
  FILETIME exitTime;
  FILETIME kernelTime;
  FILETIME userTime;
  if (GetProcessTimes
      (hProcess, &createTime, &exitTime, &kernelTime, &userTime) == -1) {
    edu::sharif::twinner::util::Logger::error ()
        << "Error in GetProcessTimes (...) function\n";
    return measurement;
  }
  SYSTEMTIME userSystemTime;
  if (FileTimeToSystemTime (&userTime, &userSystemTime) == -1) {
    edu::sharif::twinner::util::Logger::error ()
        << "Error in FileTimeToSystemTime (...) function\n";
    return measurement;
  }
  SYSTEMTIME kernelSystemTime;
  if (FileTimeToSystemTime (&kernelTime, &kernelSystemTime) == -1) {
    edu::sharif::twinner::util::Logger::error ()
        << "Error in FileTimeToSystemTime (...) function\n";
    return measurement;
  }
  measurement.cputime = userSystemTime + kernelSystemTime;

  PROCESS_MEMORY_COUNTERS pmc;
  if (GetProcessMemoryInfo (hProcess, &pmc, sizeof (pmc)) == 0) {
    edu::sharif::twinner::util::Logger::error ()
        << "Error in GetProcessMemoryInfo (...) function\n";
    return measurement;
  }
  measurement.mss = pmc.PeakWorkingSetSize / 1024; // in kilobytes

  return measurement;
}

}
}
}
}

#endif
