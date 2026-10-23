#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct rgb_t {
    uint8_t r, g, b;
};

typedef struct rgb_t rgb_t;

uint8_t* cimplGrayScale(uint32_t width, uint32_t height, const rgb_t* input);

uint8_t* cimplDetectEdges(uint32_t width, uint32_t height, const uint8_t* input);

#ifdef __cplusplus
}
#endif