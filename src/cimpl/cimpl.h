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
    #ifdef CIMPL_STATIC
        #define CIMPL_API
    #elif defined(CIMPL_BUILDING)
        #define CIMPL_API __declspec(dllexport)
    #else
        #define CIMPL_API __declspec(dllimport)
    #endif
#else
    #define CIMPL_API __attribute__((visibility("default")))
#endif

#include <core.h>

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

CIMPL_API uint8_t* cDetectEdges(size_t width, size_t height, const uint8_t* input, size_t bytesPerLine, double amount, size_t* outBytesPerLine);

CIMPL_API int cDetectEdgesWorker(detect_edges_params* params);

#ifdef __cplusplus
}
#endif