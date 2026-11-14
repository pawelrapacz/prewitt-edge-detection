#include "cimpl.h"

#include <core.h>

#include <stdlib.h>
#include <stdint.h>
#include <math.h>


uint8_t* cGrayScale(uint32_t width, uint32_t height, const rgb_t* input) {
    if (input == nullptr)
        return nullptr;

    uint8_t* result = malloc(width * height);

    if (result == nullptr)
        return nullptr;
    
    for (uint32_t i = 0; i < width * height; i++) {
        rgb_t pix = input[i];
        result[i] = (uint8_t)(0.299f * pix.r + 0.587f * pix.g + 0.114f * pix.b);
    }

    return result;
}

uint8_t* cDetectEdges(uint32_t width, uint32_t height, const uint8_t* input, size_t bytesPerLine, double amount, size_t* outBytesPerLine) {
    if (input == nullptr) return nullptr;
    if (width * height == 0) return nullptr;

    uint32_t size = width * height;
    if (size == 0)
        return nullptr;

    uint8_t* output = imageBufferAlloc(width, height, outBytesPerLine);

    if (output == nullptr) return nullptr;
    
    detect_edges_params params = {
        input,
        output,
        bytesPerLine,
        *outBytesPerLine,
        amount,
        width,
        height,
        0,
        height
    };

    int res = cDetectEdgesWorker(&params);

    if (res != 0) {
        free(output);
        return nullptr;
    }

    return output;
}


int cDetectEdgesWorker(detect_edges_params* p) {
    if (p->input == nullptr)
        return -1;

    if (p->output == nullptr)
        return -1;

    size_t size = p->width * p->height;
    if (size == 0)
        return -1;

    size_t shift = p->inBytesPerLine;
    for (size_t j = p->start; j < p->end; j++) {
        for (size_t i = 1; i < p->width - 1; i++) {

            size_t pos = p->inBytesPerLine * j + i;
            size_t upperRow = pos - shift;
            size_t lowerRow = pos + shift;
            int64_t gy = p->input[upperRow - 1] + p->input[upperRow] + p->input[upperRow + 1]
                        - p->input[lowerRow - 1] - p->input[lowerRow] - p->input[lowerRow + 1];

            size_t leftCol = pos - 1;
            size_t rightCol = pos + 1;
            int64_t gx = p->input[leftCol - shift] + p->input[leftCol] + p->input[leftCol + shift]
                        - p->input[rightCol - shift] - p->input[rightCol] - p->input[rightCol + shift];

            int64_t magnitude = hypot(gx, gy) * p->amount;
            size_t pidx = p->outBytesPerLine * j + i;

            if (magnitude < 255)
                p->output[pidx] = magnitude;
            else
                p->output[pidx] = 255;
        }
    }

    return 0;
}