//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef SAVABLE_H
#define	SAVABLE_H

#include <list>
#include <fstream>
#include <string.h>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

/**
 * @interface This interface indicates ability to be saved into an output stream in
 * binary format.
 */
class Savable {

protected:

  Savable () {
  }

public:

  virtual ~Savable () {
  }


  /**
   * Saves all information of the object in the given output stream in binary format.
   * @param out The output stream which this object should be saved on it.
   */
  virtual void saveToBinaryStream (std::ofstream &out) const = 0;
};

/**
 * Saves a list of Savable objects into the binary output stream. The SAVABLE class
 * have to implement the Savable interface.
 * @param out The output stream.
 * @param magicString The magic string which indicates list's type.
 * @param list List of items which should be saved into output stream.
 */
template < typename SAVABLE >
void saveListToBinaryStream (std::ofstream &out,
    const char *magicString, const std::list< SAVABLE * > list) {
  out.write (magicString, strlen (magicString));

  typename std::list< SAVABLE * >::size_type s = list.size ();
  out.write ((const char *) &s, sizeof (s));
  for (typename std::list< SAVABLE * >::const_iterator it = list.begin ();
      it != list.end (); ++it) {
    const Savable *con = *it;
    con->saveToBinaryStream (out);
  }
}

}
}
}
}

#endif	/* Savable.h */
