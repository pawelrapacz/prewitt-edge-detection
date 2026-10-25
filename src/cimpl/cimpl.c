#include "cimpl.h"

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

uint8_t* cDetectEdges(uint32_t width, uint32_t height, const uint8_t* input) {
    if (input == nullptr)
        return nullptr;

    uint32_t size = width * height;
    if (size == 0)
        return nullptr;

    uint8_t* result = malloc(size);

    if (result == nullptr)
        return nullptr;
    
    for (uint32_t j = 1; j < height - 1; j++) {
        for (uint32_t i = 1; i < width - 1; i++) {

            uint32_t upperRow = width * (j - 1) + i;
            uint32_t lowerRow = width * (j + 1) + i;
            double gy = input[upperRow - 1] + input[upperRow] + input[upperRow + 1]
                        - input[lowerRow - 1] - input[lowerRow] - input[lowerRow + 1];

            uint32_t shift = width;
            uint32_t leftCol = width * j + i - 1;
            uint32_t rightCol = width * j + i + 1;
            double gx = input[leftCol - shift] + input[leftCol] + input[leftCol + shift]
                        - input[rightCol - shift] - input[rightCol] - input[rightCol + shift];

            double magnitude = hypot(gx, gy);
            uint32_t pidx = width * j + i;

            if (magnitude < 255.0)
                result[pidx] = magnitude;
            else
                result[pidx] = 255;
        }
    }

    return result;
}