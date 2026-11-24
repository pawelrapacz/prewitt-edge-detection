/*
 * Project: Prewitt edge detection
 * 
 * The Prewitt algorithm detects edges in images by calculating
 * intensity changes in horizontal and vertical directions using
 * two convolution kernels.
 * 
 * 2026/2027 sem. 5
 * Author: Paweł Rapacz
 * 
 * Version: 1.0
 * 
 * Version history:
 * 1.0 - Initial version of the application.
 *      Implemented Prewitt edge detection in C and x86-64 ASM.
 *      Created GUI app and benchmark with core functionality.
 */

#pragma once

// This file checks that the layout of detect_edges_params still matches
// the offsets used by the assembly implementations (Windows and Linux).
// The offsets themselves are defined in detect_edges_params_offsets.inc.

// If any assertion below fails, update the assembly implementations to
// match the new layout of detect_edges_params before changing the checks.

#include <core.h>

#include <assert.h>

#define ASMIMPL_CHECKED_STRUCT detect_edges_params

// check the layout
#define ASMIMPL_CHECK_OFFSET(field, offset) \
    static_assert(offsetof(ASMIMPL_CHECKED_STRUCT, field) == offset, \
    "Layout of detect_edges_params has changed! Update asmimpl includes.")

ASMIMPL_CHECK_OFFSET(input,              0);
ASMIMPL_CHECK_OFFSET(output,             8);
ASMIMPL_CHECK_OFFSET(inBytesPerLine,    16);
ASMIMPL_CHECK_OFFSET(outBytesPerLine,   24);
ASMIMPL_CHECK_OFFSET(amount,            32);
ASMIMPL_CHECK_OFFSET(width,             40);
ASMIMPL_CHECK_OFFSET(height,            48);
ASMIMPL_CHECK_OFFSET(fmt,               56);
ASMIMPL_CHECK_OFFSET(start,             64);
ASMIMPL_CHECK_OFFSET(end,               72);

// check the size
static_assert(sizeof(ASMIMPL_CHECKED_STRUCT) == 80,
"Size of detect_edges_params has changed! Update asmimpl includes.");    


#undef ASMIMPL_CHECK_OFFSET
#undef ASMIMPL_CHECKED_STRUCT
