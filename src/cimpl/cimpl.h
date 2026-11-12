#pragma once

#include <stdint.h>

#include <core.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t* cGrayScale(uint32_t width, uint32_t height, const rgb_t* input);

uint8_t* cDetectEdges(uint32_t width, uint32_t height, const uint8_t* input, size_t bytesPerLine, double amount, size_t* outBytesPerLine);

int cDetectEdgesWorker(detect_edges_params* params);

#ifdef __cplusplus
}
#endif