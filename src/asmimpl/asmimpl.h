#pragma once

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

uint8_t* asmDetectEdges(uint32_t width, uint32_t height, const uint8_t* input);

#ifdef __cplusplus
}
#endif