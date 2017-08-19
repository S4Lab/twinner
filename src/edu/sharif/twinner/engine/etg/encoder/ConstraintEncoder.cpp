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

#include "ConstraintEncoder.h"

#include <list>
#include <set>
#include <map>

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {
namespace encoder {

ConstraintEncoder::ConstraintEncoder (
    edu::sharif::twinner::engine::etg::ConstraintEdge *edge,
    const AddrToSizeMap &addressToSize, bool bypassConstraint) :
    Encoder (addressToSize) {
}

ConstraintEncoder::~ConstraintEncoder () {
}


}
}
}
}
}
}
