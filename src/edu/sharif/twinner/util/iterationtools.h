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

#ifndef ITERATION_TOOLS_H
#define ITERATION_TOOLS_H

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

#define for_each_lst_base(TYPE, CONST_TYPE, LIST, VAR) \
  for (std::list < TYPE * >::const_iterator it = LIST.begin (); it != LIST.end (); ++it) \
    for (CONST_TYPE *VAR = *it; VAR; VAR = 0)

#define for_each_lst_const(TYPE, LIST, VAR) \
  for_each_lst_base (TYPE, TYPE, LIST, VAR)

#define for_each_lst(TYPE, LIST, VAR) \
  for_each_lst_base (TYPE, const TYPE, LIST, VAR)

#define for_each_set_base(TYPE, CONST_TYPE, LIST, VAR) \
  for (std::set < TYPE * >::const_iterator it = LIST.begin (); it != LIST.end (); ++it) \
    for (CONST_TYPE *VAR = *it; VAR; VAR = 0)

#define for_each_set_const(TYPE, LIST, VAR) \
  for_each_set_base (TYPE, TYPE, LIST, VAR)

#define for_each_set(TYPE, LIST, VAR) \
  for_each_set_base (TYPE, const TYPE, LIST, VAR)

#define for_each_map_base(PRE, KEY_TYPE, VAL_TYPE, CONST_VAL_TYPE, MAP, KEY, VAL) \
  for (PRE std::map < KEY_TYPE, VAL_TYPE * >::const_iterator it = MAP.begin (); \
       it != MAP.end (); ++it) \
    for (CONST_VAL_TYPE *VAL = it->second; VAL; VAL = 0) \
      for (const KEY_TYPE &KEY = it->first; VAL; VAL = 0)

#define for_each_map_const(KEY_TYPE, VAL_TYPE, MAP, KEY, VAL) \
  for_each_map_base (, KEY_TYPE, VAL_TYPE, VAL_TYPE, MAP, KEY, VAL)

#define for_each_map(KEY_TYPE, VAL_TYPE, MAP, KEY, VAL) \
  for_each_map_base (, KEY_TYPE, VAL_TYPE, const VAL_TYPE, MAP, KEY, VAL)

#define for_each_map_const_t(KEY_TYPE, VAL_TYPE, MAP, KEY, VAL) \
  for_each_map_base (typename, KEY_TYPE, VAL_TYPE, VAL_TYPE, MAP, KEY, VAL)

#define for_each_map_t(KEY_TYPE, VAL_TYPE, MAP, KEY, VAL) \
  for_each_map_base (typename, KEY_TYPE, VAL_TYPE, const VAL_TYPE, MAP, KEY, VAL)

#define repeat(N) \
  for (int nn = 0; nn < N; ++nn)

#define UNUSED_VARIABLE(VAR) (void) VAR

}
}
}
}

#endif	/* IterationTools.h */
