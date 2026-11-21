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

#ifdef _WIN32
    #ifdef ASMIMPL_STATIC
        #define ASMIMPL_API
    #elif defined(ASMIMPL_BUILDING)
        #define ASMIMPL_API __declspec(dllexport)
    #else
        #define ASMIMPL_API __declspec(dllimport)
    #endif
#else
    #define ASMIMPL_API __attribute__((visibility("default")))
#endif

#include <core.h>

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

ASMIMPL_API uint8_t* asmDetectEdges(size_t width, size_t height, const uint8_t* input, size_t bytesPerLine, double amount, size_t* outBytesPerLine);

ASMIMPL_API int asmDetectEdgesWorker(detect_edges_params* params);

ASMIMPL_API int asmDetectEdgesSIMDWorker(detect_edges_params* params);

#ifdef __cplusplus
}
#endif