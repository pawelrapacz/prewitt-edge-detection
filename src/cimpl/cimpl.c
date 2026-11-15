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

uint8_t* cDetectEdges(size_t width, size_t height, const uint8_t* input, size_t bytesPerLine, double amount, size_t* outBytesPerLine) {
    return detectEdges(width, height, input, bytesPerLine, amount, outBytesPerLine, cDetectEdgesWorker);
}


int cDetectEdgesWorker(detect_edges_params* p) {
    if (p->input == nullptr)
        return -1;

    if (p->output == nullptr)
        return -1;

    size_t size = p->width * p->height;
    if (size == 0)
        return -1;
    
    float amount = p->amount;
    size_t shift = p->inBytesPerLine;

    for (size_t j = p->start; j < p->end; j++) {
        for (size_t i = 1; i < p->width - 1; i++) {

            size_t pos = p->inBytesPerLine * j + i;
            size_t upperRow = pos - shift;
            size_t lowerRow = pos + shift;

            uint8_t tl = p->input[upperRow - 1];
            uint8_t tc = p->input[upperRow];
            uint8_t tr = p->input[upperRow + 1];

            uint8_t cl = p->input[pos - 1];
            uint8_t cr = p->input[pos + 1];

            uint8_t bl = p->input[lowerRow - 1];
            uint8_t bc = p->input[lowerRow];
            uint8_t br = p->input[lowerRow + 1];

            int32_t gy = tl + tc + tr - bl - bc - br;
            int32_t gx = tl + cl + bl - tr - cr - br;


            int32_t magnitude = sqrt(gx * gx + gy * gy) * amount;
            size_t pidx = p->outBytesPerLine * j + i;

            if (magnitude < 255)
                p->output[pidx] = magnitude;
            else
                p->output[pidx] = 255;
        }
    }

    return 0;
}