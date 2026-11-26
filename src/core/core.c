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

#include "core.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <threads.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
    #include <malloc.h>
#endif

uint8_t* imageBufferAlloc(size_t width, size_t height, Format fmt, size_t* bpl) {
    return alignedImageBufferAlloc(width, height, fmt, 4, 4, bpl);
}

uint8_t* alignedImageBufferAlloc(size_t width, size_t height, Format fmt, size_t dataAlign, size_t addrAlign, size_t* bpl) {
    if (width == 0 || height == 0 || dataAlign == 0 || addrAlign == 0 || bpl == NULL)
        return NULL;

    if (dataAlign % addrAlign != 0)
        return NULL;

    if (fmt != GRAY && fmt != RGB)
        return NULL;

    if (width > (SIZE_MAX - dataAlign + 1) / fmt)
        return NULL;

    const size_t _bpl = (width * fmt + dataAlign - 1) / dataAlign * dataAlign;

    if (_bpl > (SIZE_MAX - addrAlign + 1) / height)
        return NULL;

   const size_t size = _bpl * height;
    
#ifdef _WIN32
    void* ptr = _aligned_malloc(size, addrAlign);
#else
    void* ptr = aligned_alloc(addrAlign, size);
#endif

    if (ptr != NULL) {
        *bpl = _bpl;
        memset(ptr, 0, size);
    }

    return ptr;
}

void imageBufferFree(void* ptr) {
#ifdef _WIN32
    _aligned_free(ptr);
#else
    free(ptr);
#endif
}


typedef struct {
    detect_edges_worker worker_func;
    detect_edges_params* worker_params;
} worker_params;


// need a wrrapper to ensure type safety
static inline int workerWrapper(void* p) {
    worker_params* params = p;
    return params->worker_func(params->worker_params);
}

uint8_t* detectEdges(size_t width, size_t height, Format fmt, const uint8_t* input, size_t bytesPerLine, double amount, size_t* outBytesPerLine, detect_edges_worker worker) {
    if (input == NULL) return NULL;
    if (width * height == 0) return NULL;

    size_t obpl;
    uint8_t* output = imageBufferAlloc(width, height, fmt, &obpl);

    if (output == NULL) return NULL;

    detect_edges_params params = {
        input,
        output,
        bytesPerLine,
        obpl,
        amount,
        width,
        height,
        fmt,
        1,
        height - 1
    };

    const int res = worker(&params);

    if (res != CORE_WORKER_SUCCESS) {
        imageBufferFree(output);
        return NULL;
    }

    if (outBytesPerLine != NULL)
        *outBytesPerLine = obpl;

    return output;
}

uint8_t* detectEdgesThreads(size_t width, size_t height, Format fmt, const uint8_t* input, size_t bytesPerLine, double amount, size_t* outBytesPerLine, detect_edges_worker worker, uint32_t threads) {
    if (threads == 0) return NULL;
    if (input == NULL) return NULL;
    if (width * height == 0) return NULL;

    size_t obpl;
    uint8_t* output = imageBufferAlloc(width, height, fmt, &obpl);
    detect_edges_params* params = malloc(threads * sizeof(detect_edges_params));
    worker_params* wparams = malloc(threads * sizeof(worker_params));
    thrd_t* thrd = malloc(threads * sizeof(thrd_t));

    if (!(output && params && wparams && thrd)) {
        free(wparams);
        imageBufferFree(output);
        free(params);
        free(thrd);
        return NULL;
    }

    uint32_t created = 0;
    for (uint32_t i = 0; i < threads; i++) {
        params[i].input = input;
        params[i].output = output;
        params[i].inBytesPerLine = bytesPerLine;
        params[i].outBytesPerLine = obpl;
        params[i].width = width;
        params[i].height = height;
        params[i].amount = amount;
        params[i].fmt = fmt;
        params[i].start = height * i / threads;
        params[i].end = height * (i + 1) / threads;

        if (i == 0) params[i].start++;
        if (i == threads - 1) params[i].end--;

        wparams[i].worker_func = worker;
        wparams[i].worker_params = &params[i];

        const int status = thrd_create(&thrd[i], workerWrapper, &wparams[i]);
        if (status != thrd_success)
            break;
        else
            created++;
    }

    bool fail = false;
    for (uint32_t i = 0; i < created; i++) {
        int res;
        const int status = thrd_join(thrd[i], &res);
        
        if (status != thrd_success || res != CORE_WORKER_SUCCESS)
            fail = true;
    }

    free(wparams);
    free(params);
    free(thrd);

    if (created != threads || fail) {
        imageBufferFree(output);
        return NULL;
    }

    if (outBytesPerLine != NULL)
        *outBytesPerLine = obpl;
    
    return output;
}