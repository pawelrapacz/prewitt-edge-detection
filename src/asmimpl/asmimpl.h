#pragma once

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

struct rgb_t {
    uint8_t r, g, b;
};

typedef struct rgb_t rgb_t;

uint8_t* asmGrayScale(uint32_t width, uint32_t height, const rgb_t* input);

#ifdef __cplusplus
}
#endif