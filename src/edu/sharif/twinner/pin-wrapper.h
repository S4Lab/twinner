//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2015  Behnam Momeni
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
