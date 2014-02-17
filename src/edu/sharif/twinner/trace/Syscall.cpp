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

#include "Syscall.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

Syscall::Syscall () :
standard (SYSCALL_STANDARD_INVALID) {
}

Syscall::Syscall (SYSCALL_STANDARD _standard) :
standard (_standard) {
}

Syscall::~Syscall () {
}

}
}
}
}
