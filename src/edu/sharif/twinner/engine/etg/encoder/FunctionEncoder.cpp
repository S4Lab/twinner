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

#include "FunctionEncoder.h"

#include <set>

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {
namespace encoder {
namespace {

int lastFunctionIndex = 0;
}

FunctionEncoder::FunctionEncoder (const AddrToSizeMap &addressToSize) :
    NodeEncoder (addressToSize),
    functionIndex (++lastFunctionIndex),
    firstVisit (true) {
}

FunctionEncoder::~FunctionEncoder () {
}

void FunctionEncoder::finalizeInitialization () {
}

void FunctionEncoder::encode (IndentedStream &body, IndentedStream &preamble,
    int index, bool inMain) {
  NodeEncoder::encode (body, preamble, index, inMain);
}


}
}
}
}
}
}
