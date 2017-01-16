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

#ifndef MAX_H
#define MAX_H

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

template<typename Type>
inline const Type &max (const Type &a, const Type &b) {
  if (a < b) {
    return b;
  } else {
    return a;
  }
}

template<typename Type>
inline const Type &max (const Type &a, const Type &b, const Type &c) {
  if (a < b || a < c) {
    return max (b, c);
  } else {
    return a;
  }
}

}
}
}
}

#endif /* MAX_H */
