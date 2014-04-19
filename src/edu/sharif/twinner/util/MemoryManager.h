//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2014  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdint.h>

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

class MemoryManager {

private:
  static MemoryManager *me;

  const uint32_t capacity;
  char * const memory;
  uint32_t afterLastAllocatedByteIndex; // [0, this-index) is allocated

  MemoryManager (uint32_t capacity = 50 * 1024 /* 50 MiB */);
  MemoryManager (uint32_t capacity, char *memory);

public:
  ~MemoryManager ();

  static MemoryManager *allocateInstance ();
  static MemoryManager *loadFromFile (const char *path);

  void saveToFile (const char *path) const;

  uint32_t allocate (uint32_t size);
  void deallocate (uint32_t size);

  const char *getPointerToAllocatedMemory (uint32_t index) const;
  char *getPointerToAllocatedMemory (uint32_t index);
};

}
}
}
}

#endif /* MemoryManager.h */
