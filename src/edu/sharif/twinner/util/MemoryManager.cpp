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

#include "MemoryManager.h"

#include <stdexcept>
#include <fstream>

#include "Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

MemoryManager *MemoryManager::me = 0;

MemoryManager::MemoryManager (uint32_t _capacity) :
capacity (_capacity), memory (new char[_capacity]), afterLastAllocatedByteIndex (0) {
}

MemoryManager::MemoryManager (uint32_t _capacity, char *_memory) :
capacity (_capacity), memory (_memory), afterLastAllocatedByteIndex (_capacity) {
}

MemoryManager::~MemoryManager () {
  delete memory;
}

MemoryManager *MemoryManager::allocateInstance () {
  if (MemoryManager::me) {
    throw std::runtime_error ("MemoryManager::allocateInstance (): "
                              "An instance is already allocated.");
  }
  MemoryManager::me = new MemoryManager ();
  return MemoryManager::me;
}

uint32_t MemoryManager::allocate (uint32_t size) {
  if (size == 0) {
    throw std::runtime_error
        ("allocate_memory_from_instructions_disassembly_shared_memory_area (...) "
         "function: size must be positive");
  }
  if (afterLastAllocatedByteIndex + size > capacity) {
    // throw std::bad_alloc ();
    throw std::runtime_error
        ("allocate_memory_from_instructions_disassembly_shared_memory_area (...) "
         "function: Out of memory!");
  }
  const uint32_t allocated = afterLastAllocatedByteIndex;
  afterLastAllocatedByteIndex += size;
  return allocated + 1;
}

void MemoryManager::deallocate (uint32_t size) {
  if (size == 0 || size > afterLastAllocatedByteIndex) {
    throw std::runtime_error
        ("deallocate_memory_from_instructions_disassembly_shared_memory_area (...) "
         "function : size must be positive and smaller than currently allocated area");
  }
  afterLastAllocatedByteIndex -= size;
}

const char *MemoryManager::getPointerToAllocatedMemory (uint32_t index) const {
  if (index == 0 || index > afterLastAllocatedByteIndex) {
    return 0;
  }
  return &memory[index - 1];
}

char *MemoryManager::getPointerToAllocatedMemory (uint32_t index) {
  if (index == 0 || index > afterLastAllocatedByteIndex) {
    return 0;
  }
  return &memory[index - 1];
}

}
}
}
}
