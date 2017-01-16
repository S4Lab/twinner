//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2017 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef MEMORY_H
#define MEMORY_H

#include "edu/sharif/twinner/pin-wrapper.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

/**
 * Caller is owner of the returned concrete value object.
 */
edu::sharif::twinner::trace::cv::ConcreteValue *readRegisterContent (
    const CONTEXT *context, LEVEL_BASE::REG reg);

BOOL readMemoryContent (UINT64 &outValue, ADDRINT memoryEa, size_t size);
BOOL readStringFromMemory (std::string &outValue, const ADDRINT memoryEa);
BOOL writeMemoryContent (ADDRINT memoryEa, const UINT8 *value, size_t size);
BOOL writeRegisterContent (CONTEXT *context,
    LEVEL_BASE::REG reg, const UINT8 *value);

}
}
}
}

#endif /* memory.h */
