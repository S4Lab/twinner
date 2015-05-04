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

private:
  UINT64 high, low;

public:

  constexpr UINT128 (UINT64 _high, UINT64 _low)
      : high (_high), low (_low) {
  }

  constexpr UINT128 (const UINT32 a, const UINT32 b, const UINT32 c, const UINT32 d)
      : high ((UINT64 (a) << 32) | b), low ((UINT64 (c) << 32) | d) {
  }

  constexpr UINT128 (const UINT64 *v)
      : low (v[0]), high (v[1]) {
  }

  bool operator== (const UINT128 &v) const {
    return high == v.high && low == v.low;
  }

  bool operator== (const UINT64 &v) const {
    return high == 0 && low == v;
  }

  bool operator!= (const UINT128 &v) const {
    return !((*this) == v);
  }

  bool operator!= (const UINT64 &v) const {
    return !((*this) == v);
  }

  bool operator< (const UINT128 &v) const {
    return high < v.high || (high == v.high && low < v.low);
  }

  bool operator< (const UINT64 &v) const {
    return high == 0 && low < v;
  }

  bool operator<= (const UINT128 &v) const {
    return high < v.high || (high == v.high && low <= v.low);
  }

  bool operator<= (const UINT64 &v) const {
    return high == 0 && low <= v;
  }

  bool operator> (const UINT128 &v) const {
    return !((*this) <= v);
  }

  bool operator> (const UINT64 &v) const {
    return !((*this) <= v);
  }

  bool operator>= (const UINT128 &v) const {
    return !((*this) < v);
  }

  bool operator>= (const UINT64 &v) const {
    return !((*this) < v);
  }

  UINT64 toUint64 () const {
    return low;
  }

  UINT128 operator^ (const UINT128 &v) const {
    return UINT128 (high ^ v.high, low ^ v.low);
  }

  UINT128 operator& (const UINT128 &v) const {
    return UINT128 (high & v.high, low & v.low);
  }

  UINT64 operator& (const UINT64 &v) const {
    return low & v;
  }

  UINT128 operator| (const UINT128 &v) const {
    return UINT128 (high | v.high, low | v.low);
  }

  UINT128 operator| (const UINT64 &v) const {
    return UINT128 (high, low | v);
  }

  UINT128 operator>> (int bits) const {
    if (bits < 64) {
      return UINT128 (high >> bits, (high << (64 - bits)) | (low >> bits));
    } else {
      return UINT128 (0, high >> (bits - 64));
    }
  }

  UINT128 operator<< (int bits) const {
    if (bits < 64) {
      return UINT128 ((high << bits) | (low >> (64 - bits)), low << bits);
    } else {
      return UINT128 (low << (bits - 64), 0);
    }
  }

  UINT64 operator/ (UINT64 divisor) const {
    UINT64 quotient;
    asm (
          "DIV %[divisor]"
          : "=a" (quotient)
          : "d" (high), "a" (low), [divisor] "r" (divisor)
          );
    return quotient;
  }

  UINT128 operator% (const UINT128 &divisor) const {
    if (divisor.high == 0) {
      return UINT128 (0, (*this) % (divisor.low));
    }
    UINT128 remainder = *this;
    while (remainder >= divisor) {
      remainder = remainder - divisor;
    }
    return remainder;
  }

  UINT64 operator% (UINT64 divisor) const {
    UINT64 remainder;
    asm (
          "DIV %[divisor]"
          : "=d" (remainder)
          : "d" (high), "a" (low), [divisor] "r" (divisor)
          );
    return remainder;
  }

  UINT128 operator+ (UINT64 addee) const {
    UINT64 msp, lsp;
    asm (
          "ADD %[a], %%rbx\n\t"
          "ADC $0, %%rcx"
          : "=c" (msp), "=b" (lsp)
          : "c" (high), "b" (low), [a] "r" (addee)
          );
    return UINT128 (msp, lsp);
  }

  UINT128 operator+ (UINT128 addee) const {
    UINT64 msp, lsp;
    asm (
          "ADD %[l], %%rbx\n\t"
          "ADC %[h], %%rcx"
          : "=c" (msp), "=b" (lsp)
          : "c" (high), "b" (low), [h] "r" (addee.high), [l] "r" (addee.low)
          );
    return UINT128 (msp, lsp);
  }

  UINT128 operator- (UINT64 minusee) const {
    UINT64 msp, lsp;
    asm (
          "SUB %[a], %%rbx\n\t"
          "SBB $0, %%rcx"
          : "=c" (msp), "=b" (lsp)
          : "c" (high), "b" (low), [a] "r" (minusee)
          );
    return UINT128 (msp, lsp);
  }

  UINT128 operator- (UINT128 minusee) const {
    UINT64 msp, lsp;
    asm (
          "SUB %[l], %%rbx\n\t"
          "SBB %[h], %%rcx"
          : "=c" (msp), "=b" (lsp)
          : "c" (high), "b" (low), [h] "r" (minusee.high), [l] "r" (minusee.low)
          );
    return UINT128 (msp, lsp);
  }

  UINT128 operator- () const {
    return UINT128 (0, 0) - (*this);
  }

  friend inline UINT128 arithmeticShiftToRight (const UINT128 &v, int bits) {
    return UINT128 (INT64 (v.high) >> bits, (v.high << (64 - bits)) | (v.low >> bits));
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
  return b + a;
}

static inline UINT128 operator- (UINT64 a, UINT128 b) {
  return UINT128 (0, a) - b;
}

static inline UINT128 logicalShiftToLeft (const UINT128 v, const int bits) {
  return v << bits;
}

static inline UINT128 logicalShiftToLeft (const UINT64 v, const int bits) {
  return UINT128 (v >> (64 - bits), v << bits);
}

static inline UINT64 arithmeticShiftToRight (const UINT64 &v, int bits) {
  return (INT64 (v) >> bits);
}

#endif	/* tc-uint128.h */

