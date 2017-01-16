//                   In the name of GOD
/*
 * Twinner; An unpacker which utilizes concolic execution.
 * Copyright © 2013-2017 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef TC_SIGN_EXTENSION_H
#define TC_SIGN_EXTENSION_H

static inline UINT128 signExtend_0x80_0x8 (const UINT64 v) {
  const UINT8 source = UINT8 (v);
  if (INT8 (source) < 0) {
    return UINT128 (UINT64 (-1), UINT64 (INT8 (source)));
  } else {
    return UINT128 (UINT64 (0), UINT64 (INT8 (source)));
  }
}

static inline UINT128 signExtend_0x80_0x10 (const UINT64 v) {
  const UINT16 source = UINT16 (v);
  if (INT16 (source) < 0) {
    return UINT128 (UINT64 (-1), UINT64 (INT16 (source)));
  } else {
    return UINT128 (UINT64 (0), UINT64 (INT16 (source)));
  }
}

static inline UINT128 signExtend_0x80_0x20 (const UINT64 v) {
  const UINT32 source = UINT32 (v);
  if (INT32 (source) < 0) {
    return UINT128 (UINT64 (-1), UINT64 (INT32 (source)));
  } else {
    return UINT128 (UINT64 (0), UINT64 (INT32 (source)));
  }
}

static inline UINT128 signExtend_0x80_0x40 (const UINT64 v) {
  if (INT64 (v) < 0) {
    return UINT128 (UINT64 (-1), v);
  } else {
    return UINT128 (UINT64 (0), v);
  }
}

static inline UINT64 signExtend_0x40_0x8 (const UINT64 v) {
  const UINT8 source = UINT8 (v);
  return UINT64 (INT8 (source));
}

static inline UINT64 signExtend_0x40_0x10 (const UINT64 v) {
  const UINT16 source = UINT16 (v);
  return UINT64 (INT16 (source));
}

static inline UINT64 signExtend_0x40_0x20 (const UINT64 v) {
  const UINT32 source = UINT32 (v);
  return UINT64 (INT32 (source));
}

static inline UINT32 signExtend_0x20_0x8 (const UINT64 v) {
  const UINT8 source = UINT8 (v);
  return UINT32 (INT8 (source));
}

static inline UINT32 signExtend_0x20_0x10 (const UINT64 v) {
  const UINT16 source = UINT16 (v);
  return UINT32 (INT16 (source));
}

static inline UINT16 signExtend_0x10_0x8 (const UINT64 v) {
  const UINT8 source = UINT8 (v);
  return UINT16 (INT8 (source));
}

static inline UINT128 signExtend_0x80_0x8 (const UINT128 v) {
  return signExtend_0x80_0x8 (v.toUint64 ());
}

static inline UINT128 signExtend_0x80_0x10 (const UINT128 v) {
  return signExtend_0x80_0x10 (v.toUint64 ());
}

static inline UINT128 signExtend_0x80_0x20 (const UINT128 v) {
  return signExtend_0x80_0x20 (v.toUint64 ());
}

static inline UINT128 signExtend_0x80_0x40 (const UINT128 v) {
  return signExtend_0x80_0x40 (v.toUint64 ());
}

static inline UINT64 signExtend_0x40_0x8 (const UINT128 v) {
  return signExtend_0x40_0x8 (v.toUint64 ());
}

static inline UINT64 signExtend_0x40_0x10 (const UINT128 v) {
  return signExtend_0x40_0x10 (v.toUint64 ());
}

static inline UINT64 signExtend_0x40_0x20 (const UINT128 v) {
  return signExtend_0x40_0x20 (v.toUint64 ());
}

static inline UINT32 signExtend_0x20_0x8 (const UINT128 v) {
  return signExtend_0x20_0x8 (v.toUint64 ());
}

static inline UINT32 signExtend_0x20_0x10 (const UINT128 v) {
  return signExtend_0x20_0x10 (v.toUint64 ());
}

static inline UINT16 signExtend_0x10_0x8 (const UINT128 v) {
  return signExtend_0x10_0x8 (v.toUint64 ());
}

#endif	/* tc-sign-extension.h */

