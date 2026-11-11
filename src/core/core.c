#include "core.h"

#include <stdint.h>
#include <stdlib.h>
#include <threads.h>


typedef struct {
    detect_edges_worker worker_func;
    detect_edges_params* worker_params;
} worker_params;


// need a wrrapper to ensure type safety
static inline int worker_wrapper(void* p) {
    worker_params* params = p;
    return params->worker_func(params->worker_params);
}


uint8_t* DetectEdgesThreads(uint32_t width, uint32_t height, const uint8_t* input, double amount, detect_edges_worker worker, uint32_t threads) {
    if (threads == 0) return nullptr;
    if (input == nullptr) return nullptr;
    if (width * height == 0) return nullptr;

    uint8_t* output = malloc(width * height);
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
        params[i].width = width;
        params[i].height = height;
        params[i].amount = amount;
        params[i].start = height * i / threads;
        params[i].end = height * (i + 1) / threads;

        wparams[i].worker_func = worker;
        wparams[i].worker_params = &params[i];

        int status = thrd_create(&thrd[i], worker_wrapper, &wparams[i]);
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