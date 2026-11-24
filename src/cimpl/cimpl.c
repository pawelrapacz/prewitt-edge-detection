/*
 * Project: Prewitt edge detection
 * 
 * The Prewitt algorithm detects edges in images by calculating
 * intensity changes in horizontal and vertical directions using
 * two convolution kernels.
 * 
 * 2026/2027 sem. 5
 * Author: Paweł Rapacz
 * 
 * Version: 1.0
 * 
 * Version history:
 * 1.0 - Initial version of the application.
 *      Implemented Prewitt edge detection in C and x86-64 ASM.
 *      Created GUI app and benchmark with core functionality.
 */

#include "cimpl.h"

#include <core.h>

#include <stddef.h>
#include <stdint.h>
#include <math.h>


uint8_t* cDetectEdges(size_t width, size_t height, Format fmt, const uint8_t* input, size_t bytesPerLine, double amount, size_t* outBytesPerLine) {
    return detectEdges(width, height, fmt, input, bytesPerLine, amount, outBytesPerLine, cDetectEdgesWorker);
}


int cDetectEdgesWorker(detect_edges_params* p) {
    if (p->input == NULL)
        return CORE_WORKER_FAIL;

    if (p->output == NULL)
        return CORE_WORKER_FAIL;

    size_t size = p->width * p->height;
    if (size == 0)
        return CORE_WORKER_FAIL;
    
    float amount = p->amount;
    size_t shift = p->inBytesPerLine;
    size_t ch = p->fmt; // channels

    for (size_t j = p->start; j < p->end; j++) {
        for (size_t i = ch; i < (p->width - 1) * ch; i++) {
            size_t pos = p->inBytesPerLine * j + i;
            size_t upperRow = pos - shift;
            size_t lowerRow = pos + shift;

            uint8_t tl = p->input[upperRow - ch];
            uint8_t tc = p->input[upperRow];
            uint8_t tr = p->input[upperRow + ch];

            uint8_t cl = p->input[pos - ch];
            uint8_t cr = p->input[pos + ch];

            uint8_t bl = p->input[lowerRow - ch];
            uint8_t bc = p->input[lowerRow];
            uint8_t br = p->input[lowerRow + ch];

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

    return CORE_WORKER_SUCCESS;
}