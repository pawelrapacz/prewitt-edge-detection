#include "core.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <threads.h>


uint8_t* imageBufferAlloc(size_t width, size_t height, size_t* bytesPerLine) {
    return alignedImageBufferAlloc(width, height, 4, 4, bytesPerLine);
}

uint8_t* alignedImageBufferAlloc(size_t width, size_t height, size_t dataAlign, size_t addrAlign, size_t* bytesPerLine) {
    if (dataAlign == 0 || addrAlign == 0)
        return nullptr;

    size_t _bytesPerLine = (width + dataAlign - 1) / dataAlign * dataAlign;
    size_t size = (_bytesPerLine * width + addrAlign - 1) / addrAlign * addrAlign;

    if (bytesPerLine)
        *bytesPerLine = _bytesPerLine;
         
    return aligned_alloc(addrAlign, size);
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


uint8_t* detectEdgesThreads(size_t width, size_t height, const uint8_t* input, size_t bytesPerLine, double amount, size_t* outBytesPerLine, detect_edges_worker worker, uint32_t threads) {
    if (threads == 0) return nullptr;
    if (outBytesPerLine == nullptr) return nullptr;
    if (input == nullptr) return nullptr;
    if (width * height == 0) return nullptr;

    uint8_t* output = imageBufferAlloc(width, height, outBytesPerLine);
    detect_edges_params* params = malloc(threads * sizeof(detect_edges_params));
    worker_params* wparams = malloc(threads * sizeof(worker_params));
    thrd_t* thrd = malloc(threads * sizeof(thrd_t));

    if (!(output && params && wparams && thrd)) {
        free(wparams);
        free(output);
        free(params);
        free(thrd);
        return nullptr;
    }


    uint32_t created = 0;
    for (uint32_t i = 0; i < threads; i++) {
        params[i].input = input;
        params[i].output = output;
        params[i].inBytesPerLine = bytesPerLine;
        params[i].outBytesPerLine = *outBytesPerLine;
        params[i].width = width;
        params[i].height = height;
        params[i].amount = amount;
        params[i].start = height * i / threads;
        params[i].end = height * (i + 1) / threads;

        if (i == 0) params[i].start++;
        if (i == threads - 1) params[i].end--;

        wparams[i].worker_func = worker;
        wparams[i].worker_params = &params[i];

        int status = thrd_create(&thrd[i], workerWrapper, &wparams[i]);
        if (status != thrd_success)
            break;
        else
            created++;
    }

    for (uint32_t i = 0; i < created; i++) {
        thrd_join(thrd[i], nullptr);
    }

    free(wparams);
    free(params);
    free(thrd);

    if (created != threads)
        return nullptr;
    else
        return output;
}