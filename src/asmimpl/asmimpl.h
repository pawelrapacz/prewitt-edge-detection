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

#include <core.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t* asmDetectEdges(size_t width, size_t height, const uint8_t* input, size_t bytesPerLine, double amount, size_t* outBytesPerLine);

int asmDetectEdgesWorker(detect_edges_params* params);

int asmDetectEdgesSIMDWorker(detect_edges_params* params);

#ifdef __cplusplus
}
#endif