#pragma once

#include <stddef.h>
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
    size_t inBytesPerLine;
    size_t outBytesPerLine;
    double amount;
    size_t width, height;
    size_t start, end;
} detect_edges_params;

typedef int (*detect_edges_worker)(detect_edges_params*);

uint8_t* imageBufferAlloc(size_t width, size_t height, size_t* bytesPerLine);

uint8_t* alignedImageBufferAlloc(size_t width, size_t height, size_t dataAlign, size_t addrAlign, size_t* bytesPerLine);

uint8_t* detectEdges(size_t width, size_t height, const uint8_t* input, size_t bytesPerLine, double amount, size_t* outBytesPerLine, detect_edges_worker worker);

uint8_t* detectEdgesThreads(size_t width, size_t height, const uint8_t* input, size_t bytesPerLine, double amount, size_t* outBytesPerLine, detect_edges_worker worker, uint32_t threads);

#ifdef __cplusplus
}
#endif
