//                   In the name of GOD
/*
 * Twinner; An unpacker which utilizes concolic execution.
 * Copyright © 2013-2015  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef TC_UINT128_H
#define TC_UINT128_H

class UINT128 {

public:
  typedef __m128i builtin128;
  typedef __m64 builtin64;

private:
  builtin128 value;

public:

  UINT128 (const builtin128 &v) : value (v) {
  }

  UINT128 (const UINT32 a, const UINT32 b, const UINT32 c, const UINT32 d) {
    value = _mm_set_epi32 (a, b, c, d);
  }

  UINT128 (const UINT64 *v) {

    union {

      UINT32 v32[4];
      UINT64 v64[2];
    } u;
    u.v64[0] = v[0];
    u.v64[1] = v[1];
    value = _mm_set_epi32 (u.v32[3], u.v32[2], u.v32[1], u.v32[0]);
  }

  bool operator== (const UINT128 &v) const {
    union {

      UINT64 v64[2];
      builtin128 v128;
    } u1, u2;
    _mm_store_si128 (&u1.v128, value);
    _mm_store_si128 (&u2.v128, v.value);
    return u1.v64[0] == u2.v64[0] && u1.v64[1] == u2.v64[1];
  }

  bool operator== (const UINT64 &v) const {
    union {

      UINT64 v64[2];
      builtin128 v128;
    } u;
    _mm_store_si128 (&u.v128, value);
    return u.v64[0] == v && u.v64[1] == 0;
  }

  bool operator!= (const UINT128 &v) const {
    return !((*this) == v);
  }

  bool operator!= (const UINT64 &v) const {
    return !((*this) == v);
  }

  bool operator< (const UINT128 &v) const {
    union {

      UINT64 v64[2];
      builtin128 v128;
    } u1, u2;
    _mm_store_si128 (&u1.v128, value);
    _mm_store_si128 (&u2.v128, v.value);
    return u1.v64[1] < u2.v64[1] || (u1.v64[1] == u2.v64[1] && u1.v64[0] < u2.v64[0]);
  }

  bool operator< (const UINT64 &v) const {
    union {

      UINT64 v64[2];
      builtin128 v128;
    } u;
    _mm_store_si128 (&u.v128, value);
    return u.v64[1] == 0 && u.v64[0] < v;
  }

  bool operator> (const UINT128 &v) const {
    return v < (*this);
  }

  bool operator> (const UINT64 &v) const {
    union {

      UINT64 v64[2];
      builtin128 v128;
    } u;
    _mm_store_si128 (&u.v128, value);
    return u.v64[1] > 0 || u.v64[0] > v;
  }
  
  bool operator<= (const UINT128 &v) const {
    union {

      UINT64 v64[2];
      builtin128 v128;
    } u1, u2;
    _mm_store_si128 (&u1.v128, value);
    _mm_store_si128 (&u2.v128, v.value);
    return u1.v64[1] < u2.v64[1] || (u1.v64[1] == u2.v64[1] && u1.v64[0] <= u2.v64[0]);
  }

  bool operator<= (const UINT64 &v) const {
    union {

      UINT64 v64[2];
      builtin128 v128;
    } u;
    _mm_store_si128 (&u.v128, value);
    return u.v64[1] == 0 && u.v64[0] <= v;
  }

  bool operator>= (const UINT128 &v) const {
    return v <= (*this);
  }

  bool operator>= (const UINT64 &v) const {
    union {

      UINT64 v64[2];
      builtin128 v128;
    } u;
    _mm_store_si128 (&u.v128, value);
    return u.v64[1] > 0 || u.v64[0] >= v;
  }
  
  UINT128 &operator= (const builtin128 &v) {
    value = v;
    return *this;
  }

  operator builtin128 () const {
    return value;
  }

  UINT64 toUint64 () const {
    union {

      UINT64 v64[2];
      builtin128 v128;
    } u;
    _mm_store_si128 (&u.v128, value);
    return u.v64[0];
  }

  UINT128 operator^ (const UINT128 &v) const {
    return _mm_xor_si128 (value, v.value);
  }

  UINT128 operator& (const UINT128 &v) const {
    return _mm_and_si128 (value, v.value);
  }

  UINT64 operator& (const UINT64 &v) const {

    union {

      UINT64 v64[2];
      builtin128 v128;
    } u;
    _mm_store_si128 (&u.v128, value);
    return u.v64[0] & v;
  }

  UINT128 operator| (const UINT128 &v) const {
    return _mm_or_si128 (value, v.value);
  }

  UINT128 operator| (const UINT64 &v) const {
    return (value | v);
  }

  UINT128 operator>> (int bits) const {
    if (bits % 8 != 0) {
      throw std::runtime_error ("128-bits shifting is only available in byte multiples");
    }
    return _mm_srli_si128 (value, bits / 8);
  }

  UINT128 operator/ (UINT64 divisor) const {
    return value / divisor;
  }

  UINT128 operator% (const UINT128 &divisor) const {
    return value % divisor.value;
  }

  UINT128 operator% (UINT64 divisor) const {
    return value % divisor;
  }

  UINT128 operator+ (UINT64 addee) const {
    return value + addee;
  }

  UINT128 operator+ (UINT128 addee) const {
    return value + addee.value;
  }

  UINT128 operator- (UINT64 minusee) const {
    return value - minusee;
  }

  UINT128 operator- (UINT128 minusee) const {
    return value - minusee.value;
  }

  UINT128 operator- () const {
    return -value;
  }
};

static inline bool operator== (const UINT64 &a, const UINT128 &b) {
  return b == a;
}

static inline bool operator!= (const UINT64 &a, const UINT128 &b) {
  return b != a;
}

static inline bool operator< (const UINT64 &a, const UINT128 &b) {
  return b > a;
}

static inline bool operator<= (const UINT64 &a, const UINT128 &b) {
  return b >= a;
}

static inline bool operator> (const UINT64 &a, const UINT128 &b) {
  return b < a;
}

static inline bool operator>= (const UINT64 &a, const UINT128 &b) {
  return b <= a;
}

static inline UINT128 operator+ (UINT64 a, UINT128 b) {
  return a + UINT128::builtin128 (b);
}

static inline UINT128 operator- (UINT64 a, UINT128 b) {
  return a - UINT128::builtin128 (b);
}

static inline UINT128 logicalShiftToLeft (const UINT128 &v, const int bits) {
  const UINT128::builtin128 b = v;

  union {

    UINT64 v64[2];
    UINT128::builtin128 v128;
  } u;
  _mm_store_si128 (&u.v128, b);
  u.v64[1] = (u.v64[1] << bits) | (u.v64[0] >> (64 - bits));
  u.v64[0] = u.v64[0] << bits;
  return u.v128;
}

static inline UINT128 logicalShiftToLeft (const UINT64 &v, const int bits) {
  const UINT64 u[] = {v << bits /*lsb*/, v >> (64 - bits)/*msb*/};
  return UINT128 (u);
}

static inline UINT128 arithmeticShiftToRight (const UINT128 &v, int bits) {
  const UINT128::builtin128 b = v;

  union {

    UINT64 v64[2];
    UINT128::builtin128 v128;
  } u;
  _mm_store_si128 (&u.v128, b);
  u.v64[0] = (u.v64[1] << (64 - bits)) | (u.v64[0] >> bits);
  const signed long long int s = u.v64[1];
  u.v64[1] = (s >> bits);
  return u.v128;
}

static inline UINT64 arithmeticShiftToRight (const UINT64 &v, int bits) {
  const signed long long int s = v;
  return (s >> bits);
}

#endif	/* tc-uint128.h */

