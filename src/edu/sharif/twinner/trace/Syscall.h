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

#ifndef SYSCALL_H
#define SYSCALL_H

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Syscall {

private:
  SYSCALL_STANDARD standard;

public:
  Syscall ();
  Syscall (SYSCALL_STANDARD standard);
  ~Syscall ();
};

}
}
}
}

#endif /* Syscall.h */
