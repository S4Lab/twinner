/*  ArgParser - POSIX/GNU command line argument parser. (C++ version)
 Copyright (C) 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013
 Antonio Diaz Diaz.
 Copyright © 2013  Behnam Momeni

 This library is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this library.  If not, see <http://www.gnu.org/licenses/>.

 As a special exception, you may use this file as part of a free
 software library without restriction.  Specifically, if other files
 instantiate templates or use macros or inline functions from this
 file, or you compile this file and link it with other files to
 produce an executable, this file does not by itself cause the
 resulting executable to be covered by the GNU General Public
 License.  This exception does not however invalidate any other
 reasons why the executable file might be covered by the GNU General
 Public License.
 */

#include <cstring>
#include <string>
#include <vector>

#include "ArgParser.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace cli {

ArgParser::ArgParser (const int argc, const char * const argv[], const Option options[],
    const bool inOrder) {
  if (argc < 2 || !argv || !options) {
    return;
  }

  std::vector < std::string > non_options;  // skipped non-options
  int argind = 1;       // index in argv

  while (argind < argc) {
    const unsigned char ch1 = argv[argind][0];
    const unsigned char ch2 = (ch1 ? argv[argind][1] : 0);

    if (ch1 == '-' && ch2) { // we found an option
      const char * const opt = argv[argind];
      const char * const arg = (argind + 1 < argc) ? argv[argind + 1] : 0;
      if (ch2 == '-') {
        if (!argv[argind][2]) {
          ++argind;
          break;
        } // we found "--"
        else if (!parseLongOption (opt, arg, options, argind)) {
          break;
        }
      } else if (!parseShortOption (opt, arg, options, argind)) {
        break;
      }
    } else {
      if (!inOrder) {
        non_options.push_back (argv[argind++]);
      } else {
        data.push_back (Record ());
        data.back ().argument = argv[argind++];
      }
    }
  }
  if (err.size ()) {
    data.clear ();
  } else {
    for (unsigned i = 0; i < non_options.size (); ++i) {
      data.push_back (Record ());
      data.back ().argument.swap (non_options[i]);
    }
    while (argind < argc) {
      data.push_back (Record ());
      data.back ().argument = argv[argind++];
    }
  }
}

ArgParser::ArgParser (const char * const opt, const char * const arg,
    const Option options[]) {
  if (!opt || !opt[0] || !options) {
    return;
  }

  if (opt[0] == '-' && opt[1]) { // we found an option
    int argind = 1;     // dummy
    if (opt[1] == '-') {
      if (opt[2]) {
        parseLongOption (opt, arg, options, argind);
      }
    } else {
      parseShortOption (opt, arg, options, argind);
    }
    if (err.size ()) {
      data.clear ();
    }
  } else {
    data.push_back (Record ());
    data.back ().argument = opt;
  }
}

bool ArgParser::parseLongOption (const char * const opt, const char * const arg,
    const Option options[], int & argind) {
  unsigned len;
  int index = -1;
  bool exact = false, ambig = false;

  for (len = 0; opt[len + 2] && opt[len + 2] != '='; ++len) {
  }

  // Test all long options for either exact match or abbreviated matches.
  for (int i = 0; options[i].code != 0; ++i) {
    if (options[i].name && std::strncmp (options[i].name, &opt[2], len) == 0) {
      if (std::strlen (options[i].name) == len) { // Exact match found
        index = i;
        exact = true;
        break;
      } else if (index < 0) { // First nonexact match found
        index = i;
      } else if (options[index].code != options[i].code
          || options[index].hasArg != options[i].hasArg) { // Second or later nonexact match found
        ambig = true;
      }
    }
  }

  if (ambig && !exact) {
    err = "option '";
    err += opt;
    err += "' is ambiguous";
    return false;
  }

  if (index < 0) { // nothing found
    err = "unrecognized option '";
    err += opt;
    err += '\'';
    return false;
  }

  ++argind;
  data.push_back (Record (options[index].code));

  if (opt[len + 2]) { // '--<long_option>=<argument>' syntax
    if (options[index].hasArg == NO) {
      err = "option '--";
      err += options[index].name;
      err += "' doesn't allow an argument";
      return false;
    }
    if (options[index].hasArg == YES && !opt[len + 3]) {
      err = "option '--";
      err += options[index].name;
      err += "' requires an argument";
      return false;
    }
    data.back ().argument = &opt[len + 3];
    return true;
  }

  if (options[index].hasArg == YES) {
    if (!arg || !arg[0]) {
      err = "option '--";
      err += options[index].name;
      err += "' requires an argument";
      return false;
    }
    ++argind;
    data.back ().argument = arg;
    return true;
  }

  return true;
}

bool ArgParser::parseShortOption (const char * const opt, const char * const arg,
    const Option options[], int & argind) {
  int cind = 1;			// character index in opt

  while (cind > 0) {
    int index = -1;
    const unsigned char c = opt[cind];

    if (c != 0) {
      for (int i = 0; options[i].code; ++i) {
        if (c == options[i].code) {
          index = i;
          break;
        }
      }
    }

    if (index < 0) {
      err = "invalid option -- ";
      err += c;
      return false;
    }

    data.push_back (Record (c));
    if (opt[++cind] == 0) {
      ++argind;
      cind = 0;
    }	// opt finished

    if (options[index].hasArg != NO && cind > 0 && opt[cind]) {
      data.back ().argument = &opt[cind];
      ++argind;
      cind = 0;
    } else if (options[index].hasArg == YES) {
      if (!arg || !arg[0]) {
        err = "option requires an argument -- ";
        err += c;
        return false;
      }
      data.back ().argument = arg;
      ++argind;
      cind = 0;
    }
  }
  return true;
}

}
}
}
}
