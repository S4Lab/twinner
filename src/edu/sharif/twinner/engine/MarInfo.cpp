//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2015  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "MarInfo.h"

#include <fstream>
#include <stdexcept>

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {

MarInfo::MarInfo (int _argc, char **_argv) :
    argc (_argc), argv (_argv) {
}

void MarInfo::saveToFile (const char *path) const {
  std::ofstream out;
  out.open (path, ios_base::out | ios_base::trunc | ios_base::binary);
  if (!out.is_open ()) {
    edu::sharif::twinner::util::Logger::error ()
        << "Can not report main() args: Error in open function: "
        << path << '\n';
    throw std::runtime_error ("Error in reporting main() args during opening file");
  }
  out.write (reinterpret_cast<const char *> (&argc), sizeof (argc));
  out.write (reinterpret_cast<const char *> (&argv), sizeof (argv));
  out.close ();
}

MarInfo *MarInfo::readMarInfoFromFile (const char *path) {
  std::ifstream in;
  in.open (path, ios_base::in | ios_base::binary);
  if (!in.is_open ()) {
    edu::sharif::twinner::util::Logger::error () << "Can not read mar info:"
        " Error in open function: " << path << '\n';
    throw std::runtime_error ("Error in reading MAR info");
  } else {
    int argc;
    char **argv;
    in.read (reinterpret_cast<char *> (&argc), sizeof (argc));
    in.read (reinterpret_cast<char *> (&argv), sizeof (argv));
    in.close ();
    return new MarInfo (argc, argv);
  }
}

}
}
}
}
