#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void cimplDetectEdges(uint8_t** tab, uint32_t width, uint32_t height, uint32_t threads);

#ifdef __cplusplus
}
#endif