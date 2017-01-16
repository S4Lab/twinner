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

#ifndef PIN_WRAPPER_H
#define PIN_WRAPPER_H

#ifdef TARGET_WINDOWS
#define REAL_TARGET_IS_WINDOWS
#endif

#if defined(TARGET_IA32E) && defined(TARGET_LINUX)
#define TARGET_IS_64BITS_LINUX
#elif defined(TARGET_IA32) && defined(TARGET_WINDOWS)
#define TARGET_IS_32BITS_WINDOWS7_SP1
#endif

#ifdef __GNUG__

#define COMPILER_IS_COMPATIBLE
#include "pin.H"
#undef TARGET_WINDOWS
#undef TARGET_LINUX
#define TARGET_LINUX

#else

#include "pin.H"

#endif


#endif /* pin-wrapper.h */
