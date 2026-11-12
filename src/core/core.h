#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t r, g, b;
} rgb_t;

typedef struct {
    const uint8_t* input;
    uint8_t* output;
    double amount;
    uint32_t width, height;
    uint32_t start, end;
} detect_edges_params;

typedef int (*detect_edges_worker)(detect_edges_params*);

uint8_t* DetectEdgesThreads(uint32_t width, uint32_t height, const uint8_t* input, double amount, detect_edges_worker worker, uint32_t threads);

#ifdef __cplusplus
}
#endif
