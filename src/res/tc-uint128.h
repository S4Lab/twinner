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

struct UINT128 {
  UINT64 low;
  UINT64 high;

  constexpr UINT128 (const UINT64 _high, const UINT64 _low)
  : high (_high), low (_low) {
  }

  constexpr UINT128 (const UINT32 a, const UINT32 b, const UINT32 c, const UINT32 d)
  : high ((UINT64 (a) << 32) | b), low ((UINT64 (c) << 32) | d) {
  }

  constexpr UINT128 (const UINT64 *v)
  : low (v[0]), high (v[1]) {
  }

  constexpr UINT64 toUint64 () const {
    return low;
  }

  constexpr explicit operator UINT64 () const {
    return toUint64 ();
  }
};

bool operator== (const UINT128 u, const UINT128 v) {
  return u.high == v.high && u.low == v.low;
}

bool operator== (const UINT128 u, const UINT64 v) {
  return u.high == 0 && u.low == v;
}

bool operator!= (const UINT128 u, const UINT128 v) {
  return !(u == v);
}

bool operator!= (const UINT128 u, const UINT64 v) {
  return !(u == v);
}

bool operator< (const UINT128 u, const UINT128 v) {
  return u.high < v.high || (u.high == v.high && u.low < v.low);
}

bool operator< (const UINT128 u, const UINT64 v) {
  return u.high == 0 && u.low < v;
}

bool operator<= (const UINT128 u, const UINT128 v) {
  return u.high < v.high || (u.high == v.high && u.low <= v.low);
}

bool operator<= (const UINT128 u, const UINT64 v) {
  return u.high == 0 && u.low <= v;
}

bool operator> (const UINT128 u, const UINT128 v) {
  return !(u <= v);
}

bool operator> (const UINT128 u, const UINT64 v) {
  return !(u <= v);
}

bool operator>= (const UINT128 u, const UINT128 v) {
  return !(u < v);
}

bool operator>= (const UINT128 u, const UINT64 v) {
  return !(u < v);
}

UINT128 operator^ (const UINT128 u, const UINT128 v) {
  return UINT128 (u.high ^ v.high, u.low ^ v.low);
}

UINT128 operator& (const UINT128 u, const UINT128 v) {
  return UINT128 (u.high & v.high, u.low & v.low);
}

UINT64 operator& (const UINT128 u, const UINT64 v) {
  return u.low & v;
}

UINT128 operator| (const UINT128 u, const UINT128 v) {
  return UINT128 (u.high | v.high, u.low | v.low);
}

UINT128 operator| (const UINT128 u, const UINT64 v) {
  return UINT128 (u.high, u.low | v);
}

UINT128 operator| (const UINT64 v, const UINT128 u) {
  return UINT128 (u.high, u.low | v);
}

UINT128 operator>> (const UINT128 u, const int bits) {
  if (bits < 64) {
    return UINT128 (u.high >> bits, (u.high << (64 - bits)) | (u.low >> bits));
  } else {
    return UINT128 (0, u.high >> (bits - 64));
  }
}

UINT128 operator<< (const UINT128 u, const int bits) {
  if (bits < 64) {
    return UINT128 ((u.high << bits) | (u.low >> (64 - bits)), u.low << bits);
  } else {
    return UINT128 (u.low << (bits - 64), 0);
  }
}

UINT128 operator+ (const UINT128 u, const UINT64 addee) {
  UINT64 msp, lsp;
  asm (
        "ADD %[a], %%rbx\n\t"
        "ADC $0, %%rcx"
        : "=c" (msp), "=b" (lsp)
        : "c" (u.high), "b" (u.low), [a] "r" (addee)
        );
  return UINT128 (msp, lsp);
}

UINT128 operator+ (const UINT128 u, const UINT128 addee) {
  UINT64 msp, lsp;
  asm (
        "ADD %[l], %%rbx\n\t"
        "ADC %[h], %%rcx"
        : "=c" (msp), "=b" (lsp)
        : "c" (u.high), "b" (u.low), [h] "r" (addee.high), [l] "r" (addee.low)
        );
  return UINT128 (msp, lsp);
}

UINT128 operator- (const UINT128 u, const UINT64 minusee) {
  UINT64 msp, lsp;
  asm (
        "SUB %[a], %%rbx\n\t"
        "SBB $0, %%rcx"
        : "=c" (msp), "=b" (lsp)
        : "c" (u.high), "b" (u.low), [a] "r" (minusee)
        );
  return UINT128 (msp, lsp);
}

UINT128 operator- (const UINT128 u, const UINT128 minusee) {
  UINT64 msp, lsp;
  asm (
        "SUB %[l], %%rbx\n\t"
        "SBB %[h], %%rcx"
        : "=c" (msp), "=b" (lsp)
        : "c" (u.high), "b" (u.low), [h] "r" (minusee.high), [l] "r" (minusee.low)
        );
  return UINT128 (msp, lsp);
}

UINT128 operator- (const UINT128 u) {
  return UINT128 (0, 0) - u;
}

UINT64 operator/ (const UINT128 u, const UINT64 divisor) {
  UINT64 quotient;
  asm (
        "DIV %[divisor]"
        : "=a" (quotient)
        : "d" (u.high), "a" (u.low), [divisor] "r" (divisor)
        );
  return quotient;
}

UINT64 operator% (const UINT128 u, const UINT64 divisor) {
  UINT64 remainder;
  asm (
        "DIV %[divisor]"
        : "=d" (remainder)
        : "d" (u.high), "a" (u.low), [divisor] "r" (divisor)
        );
  return remainder;
}

UINT64 operator% (const UINT64 u, const UINT128 divisor) {
  if (divisor.high == 0) {
    return u % (divisor.low);
  } else { // u < divisor
    return u;
  }
}

UINT128 operator% (const UINT128 u, const UINT128 divisor) {
  if (divisor.high == 0) {
    return UINT128 (0, u % (divisor.low));
  }
  UINT128 remainder = u;
  while (remainder >= divisor) {
    remainder = remainder - divisor;
  }
  return remainder;
}

UINT128 arithmeticShiftToRight (const UINT128 v, const int bits) {
  return UINT128 (INT64 (v.high) >> bits, (v.high << (64 - bits)) | (v.low >> bits));
}

bool operator== (const UINT64 a, const UINT128 b) {
  return b == a;
}

bool operator!= (const UINT64 a, const UINT128 b) {
  return b != a;
}

bool operator< (const UINT64 a, const UINT128 b) {
  return b > a;
}

bool operator<= (const UINT64 a, const UINT128 b) {
  return b >= a;
}

bool operator> (const UINT64 a, const UINT128 b) {
  return b < a;
}

bool operator>= (const UINT64 a, const UINT128 b) {
  return b <= a;
}

UINT128 operator+ (const UINT64 a, const UINT128 b) {
  return b + a;
}

UINT128 operator- (const UINT64 a, const UINT128 b) {
  return UINT128 (0, a) - b;
}

UINT128 logicalShiftToLeft (const UINT128 v, const int bits) {
  return v << bits;
}

UINT128 logicalShiftToLeft (const UINT64 v, const int bits) {
  return UINT128 (v >> (64 - bits), v << bits);
}

UINT64 arithmeticShiftToRight (const UINT64 v, const int bits) {
  return (INT64 (v) >> bits);
}

#endif	/* tc-uint128.h */

