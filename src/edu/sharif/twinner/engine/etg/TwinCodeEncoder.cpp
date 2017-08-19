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

#include "TwinCodeEncoder.h"

#include "edu/sharif/twinner/util/Logger.h"

#include "InstructionNode.h"
#include "AddressAggregator.h"

#include <fstream>
#include <stdexcept>

#include "edu/sharif/twinner/engine/etg/encoder/NodeEncoder.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

TwinCodeEncoder::TwinCodeEncoder (const std::list < ConstTrace * > &traces,
    const MemoryValueMap &_initialValues,
    InstructionNode *_root) :
    initialValues (_initialValues), root (_root) {
  AddressAggregator (traces, addressToSize).gatherSymbolsAddressToSize ();
}

void TwinCodeEncoder::encodeToFile (std::ofstream &code) {
  code << '\n';
  code << "#include \"twincode.h\"\n";
  code << '\n';

  edu::sharif::twinner::util::IndentedStringStream out;
  edu::sharif::twinner::util::IndentedStringStream conout;
  edu::sharif::twinner::engine::etg::encoder::NodeEncoder *encoder =
      root->getEncoder (addressToSize);
  encoder->initializeSecondPass ();
  encoder->encodeMain (out, conout, initialValues);

  code << conout.str () << out.str ();
}

}
}
}
}
}
