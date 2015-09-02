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

#ifndef MAR_INFO_H
#define MAR_INFO_H

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {

class MarInfo {
private:
  int argc;
  char **argv;

public:
  MarInfo (int argc, char **argv);

  void saveToFile (const char *path) const;
  static MarInfo *readMarInfoFromFile (const char *path);
};

}
}
}
}

#endif /* MarInfo.h */
