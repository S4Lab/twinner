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

#ifndef TC_UINT128_H
#define TC_UINT128_H

struct UINT128 {
  const UINT64 low;
  const UINT64 high;

  constexpr UINT128 (const UINT128 &u)
  : high (u.high), low (u.low) {
  }

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

UINT64 operator& (const UINT64 v, const UINT128 u) {
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
#ifdef TARGET_IA32E
  UINT64 msp, lsp;
  asm (
        "ADD %[a], %%rbx\n\t"
        "ADC $0, %%rcx"
        : "=c" (msp), "=b" (lsp)
        : "c" (u.high), "b" (u.low), [a] "r" (addee)
        );
  return UINT128 (msp, lsp);
#else
  UINT32 msph = (u.high >> 32), mspl = u.high & 0xFFFFFFFF;
  UINT32 lsph = (u.low >> 32), lspl = u.low & 0xFFFFFFFF;
  UINT32 ah = (addee >> 32), al = addee & 0xFFFFFFFF;
  asm (
        "ADD %[low],  %%eax\n\t"
        "ADC %[high], %%ebx\n\t"
        "ADC $0,      %%ecx\n\t"
        "ADC $0,      %%edx"
        : "=d" (msph), "=c" (mspl), "=b" (lsph), "=a" (lspl)
        : "d" (msph), "c" (mspl), "b" (lsph), "a" (lspl),
        [high] "r" (ah), [low] "r" (al)
        );
  return UINT128 (msph, mspl, lsph, lspl);
#endif
}

UINT128 operator+ (const UINT128 u, const UINT128 addee) {
#ifdef TARGET_IA32E
  UINT64 msp, lsp;
  asm (
        "ADD %[l], %%rbx\n\t"
        "ADC %[h], %%rcx"
        : "=c" (msp), "=b" (lsp)
        : "c" (u.high), "b" (u.low), [h] "r" (addee.high), [l] "r" (addee.low)
        );
  return UINT128 (msp, lsp);
#else
  UINT32 msph = (u.high >> 32), mspl = u.high & 0xFFFFFFFF;
  UINT32 lsph = (u.low >> 32), lspl = u.low & 0xFFFFFFFF;
  UINT32 ahh = (addee.high >> 32), ahl = addee.high & 0xFFFFFFFF;
  UINT32 alh = (addee.low >> 32), all = addee.low & 0xFFFFFFFF;
  asm (
        "ADD %[ll], %%eax\n\t"
        "ADC %[lh], %%ebx\n\t"
        "ADC $0,    %%ecx\n\t"
        "ADC $0,    %%edx"
        : "=d" (msph), "=c" (mspl), "=b" (lsph), "=a" (lspl)
        : "d" (msph), "c" (mspl), "b" (lsph), "a" (lspl),
        [lh] "r" (alh), [ll] "r" (all)
        );
  asm (
        "ADD %[hl], %%ecx\n\t"
        "ADC %[hh], %%edx"
        : "=d" (msph), "=c" (mspl)
        : "d" (msph), "c" (mspl),
        [hh] "r" (ahh), [hl] "r" (ahl)
        );
  return UINT128 (msph, mspl, lsph, lspl);
#endif
}

UINT128 operator- (const UINT128 u, const UINT64 minusee) {
#ifdef TARGET_IA32E
  UINT64 msp, lsp;
  asm (
        "SUB %[a], %%rbx\n\t"
        "SBB $0, %%rcx"
        : "=c" (msp), "=b" (lsp)
        : "c" (u.high), "b" (u.low), [a] "r" (minusee)
        );
  return UINT128 (msp, lsp);
#else
  UINT32 msph = (u.high >> 32), mspl = u.high & 0xFFFFFFFF;
  UINT32 lsph = (u.low >> 32), lspl = u.low & 0xFFFFFFFF;
  UINT32 mh = (minusee >> 32), ml = minusee & 0xFFFFFFFF;
  asm (
        "SUB %[low],  %%eax\n\t"
        "SBB %[high], %%ebx\n\t"
        "SBB $0,      %%ecx\n\t"
        "SBB $0,      %%edx"
        : "=d" (msph), "=c" (mspl), "=b" (lsph), "=a" (lspl)
        : "d" (msph), "c" (mspl), "b" (lsph), "a" (lspl),
        [high] "r" (mh), [low] "r" (ml)
        );
  return UINT128 (msph, mspl, lsph, lspl);
#endif
}

UINT128 operator- (const UINT128 u, const UINT128 minusee) {
#ifdef TARGET_IA32E
  UINT64 msp, lsp;
  asm (
        "SUB %[l], %%rbx\n\t"
        "SBB %[h], %%rcx"
        : "=c" (msp), "=b" (lsp)
        : "c" (u.high), "b" (u.low), [h] "r" (minusee.high), [l] "r" (minusee.low)
        );
  return UINT128 (msp, lsp);
#else
  UINT32 msph = (u.high >> 32), mspl = u.high & 0xFFFFFFFF;
  UINT32 lsph = (u.low >> 32), lspl = u.low & 0xFFFFFFFF;
  UINT32 mhh = (minusee.high >> 32), mhl = minusee.high & 0xFFFFFFFF;
  UINT32 mlh = (minusee.low >> 32), mll = minusee.low & 0xFFFFFFFF;
  asm (
        "SUB %[ll], %%eax\n\t"
        "SBB %[lh], %%ebx\n\t"
        "SBB $0,    %%ecx\n\t"
        "SBB $0,    %%edx"
        : "=d" (msph), "=c" (mspl), "=b" (lsph), "=a" (lspl)
        : "d" (msph), "c" (mspl), "b" (lsph), "a" (lspl),
        [lh] "r" (mlh), [ll] "r" (mll)
        );
  asm (
        "SUB %[hl], %%ecx\n\t"
        "SBB %[hh], %%edx"
        : "=d" (msph), "=c" (mspl)
        : "d" (msph), "c" (mspl),
        [hh] "r" (mhh), [hl] "r" (mhl)
        );
  return UINT128 (msph, mspl, lsph, lspl);
#endif
}

UINT128 operator- (const UINT128 u) {
  return UINT128 (0, 0) - u;
}

UINT64 operator/ (const UINT128 u, const UINT64 divisor) {
  UINT64 quotient;
#ifdef TARGET_IA32E
  asm (
        "DIV %[divisor]"
        : "=a" (quotient)
        : "d" (u.high), "a" (u.low), [divisor] "r" (divisor)
        );
#else
  if (u.high == 0) {
    quotient = u.low / divisor;
  } else {
    UINT64 ah = u.high % divisor, al = u.low;
    UINT64 bh = 0, bl = divisor;
    quotient = 0;
    UINT64 bit = 1;
    const UINT128 ahalf (ah >> 1, (al >> 1) || (ah << 63));
    while (ahalf.high > bh || (ahalf.high == bh && ahalf.low >= bl)) {
      bh = (bh << 1) | (bl >> 63);
      bl = bl << 1;
      bit = bit << 1;
    }
    while (bit > 0) {
      if (ah > bh || (ah == bh && al >= bl)) {
        UINT128 remainder (UINT128 (ah, al) - UINT128 (bh, bl));
        ah = remainder.high;
        al = remainder.low;
        quotient |= bit;
      }
      bh = bh >> 1;
      bl = (bl >> 1) | (bh << 63);
      bit = bit >> 1;
    }
  }
#endif
  return quotient;
}

UINT64 operator% (const UINT128 u, const UINT64 divisor) {
#ifdef TARGET_IA32E
  UINT64 remainder;
  asm (
        "DIV %[divisor]"
        : "=d" (remainder)
        : "d" (u.high), "a" (u.low), [divisor] "r" (divisor)
        );
  return remainder;
#else
  UINT64 ah = u.high % divisor, al = u.low % divisor;
  UINT64 bh = 0, bl = divisor;
  const UINT128 ahalf (ah >> 1, (al >> 1) || (ah << 63));
  while (ahalf.high > bh || (ahalf.high == bh && ahalf.low >= bl)) {
    bh = (bh << 1) | (bl >> 63);
    bl = bl << 1;
  }
  while (ah > 0 || al >= divisor) {
    if (ah > bh || (ah == bh && al >= bl)) {
      UINT128 remainder (UINT128 (ah, al) - UINT128 (bh, bl));
      ah = remainder.high;
      al = remainder.low;
    }
    bh = bh >> 1;
    bl = (bl >> 1) | (bh << 63);
  }
  return al;
#endif
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
  } else if (u < divisor) {
    return u;
  } else {
    return (u - divisor) % divisor;
  }
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

UINT128 logicalShiftToRight (const UINT128 v, const int bits) {
  return v >> bits;
}

UINT64 logicalShiftToRight (const UINT64 v, const int bits) {
  return v >> bits;
}

UINT128 logicalShiftToLeft (const UINT128 v, const int bits) {
  return v << bits;
}

UINT128 logicalShiftToLeft (const UINT64 v, const int bits) {
  return UINT128 (0, v) << bits;
}

UINT64 arithmeticShiftToRight (const UINT64 v, const int bits) {
  return (INT64 (v) >> bits);
}

#endif /* tc-uint128.h */

