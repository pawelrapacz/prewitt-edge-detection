#pragma once

#include <core.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t* asmDetectEdges(size_t width, size_t height, const uint8_t* input, size_t bytesPerLine, double amount, size_t* outBytesPerLine);

int asmDetectEdgesWorker(detect_edges_params* params);

#ifdef __cplusplus
}
#endif