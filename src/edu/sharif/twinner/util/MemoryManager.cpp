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

#include "edu/sharif/twinner/pin-wrapper.h"

#include "MemoryManager.h"

#include <fstream>
#include <stdlib.h>

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

MemoryManager *MemoryManager::getInstance () {
  if (MemoryManager::me == 0) {
    MemoryManager::me = new MemoryManager ();
  }
  return MemoryManager::me;
}

MemoryManager *MemoryManager::loadFromFile (const char *path) {
  std::ifstream in;
  in.open (path, ios_base::in | ios_base::binary);
  if (!in.is_open ()) {
    edu::sharif::twinner::util::Logger::error ()
        << "MemoryManager::loadFromFile (...): Can not read memory data:"
        " Error in open function: " << path << '\n';
    return 0;
  }
  uint32_t after_last_allocated_byte_index;
  in.read ((char *) &after_last_allocated_byte_index,
           sizeof (after_last_allocated_byte_index));
  char *memory_area = new char[after_last_allocated_byte_index];
  in.read (memory_area, after_last_allocated_byte_index);
  in.close ();
  return new MemoryManager (after_last_allocated_byte_index, memory_area);
}

bool MemoryManager::saveToFile (const char *path) const {
  std::ofstream out;
  out.open (path, ios_base::out | ios_base::trunc | ios_base::binary);
  if (!out.is_open ()) {
    edu::sharif::twinner::util::Logger::error ()
        << "MemoryManager::saveToFile (...): Can not write memory data:"
        " Error in open function: " << path << '\n';
    return false;
  }
  out.write ((const char *) &afterLastAllocatedByteIndex,
             sizeof (afterLastAllocatedByteIndex));
  out.write (memory, afterLastAllocatedByteIndex);
  out.close ();
  return true;
}

bool MemoryManager::allocate (uint32_t &index, uint32_t size) {
  if (size == 0) {
    edu::sharif::twinner::util::Logger::error ()
        << "MemoryManager::allocate (size=" << size << "): "
        "size must be positive\n";
    return false;
  }
  if (afterLastAllocatedByteIndex + size > capacity) {
    edu::sharif::twinner::util::Logger::error ()
        << "MemoryManager::allocate (size=" << size << "): "
        "Out of memory!\n";
    return false;
  }
  const uint32_t allocated = afterLastAllocatedByteIndex;
  afterLastAllocatedByteIndex += size;
  index = allocated + 1;
  return true;
}

void MemoryManager::deallocate (uint32_t size) {
  if (size == 0 || size > afterLastAllocatedByteIndex) {
    edu::sharif::twinner::util::Logger::error ()
        << "MemoryManager::deallocate (...): "
        "deallocate_memory_from_instructions_disassembly_shared_memory_area "
        "function: size must be positive"
        " and smaller than currently allocated area\n";
    abort ();
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
