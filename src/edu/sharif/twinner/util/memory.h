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

#ifndef MEMORY_H
#define	MEMORY_H

#include "pin.H"

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

UINT64 readRegisterContent (const CONTEXT *context, REG reg);
UINT64 readMemoryContent (ADDRINT memoryEa);
VOID writeMemoryContent (ADDRINT memoryEa, UINT64 value);

inline UINT64 truncateValue (UINT64 value, int countOfBytes) {
  switch (countOfBytes) {
  case 8:
    break; // no cast is required in 64-bits mode
  case 4:
    value = (UINT32) value;
    break;
  case 2:
    value = (UINT16) value;
    break;
  case 1:
    value = (UINT8) value;
    break;
  default:
    throw std::runtime_error ("Can not truncate value to given count of bytes");
  }
  return value;
}


}
}
}
}

#endif	/* memory.h */
