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

#include "bench.h"

#include <cimpl.h>
#include <asmimpl.h>

#include <print>
#include <fstream>
#include <filesystem>
#include <stdexcept>

int main(int argc, char** argv) {
    const bench::threads_array threads = {1, 2, 4, 8, 16, 32, 64};
    const bench::workers_array workers = {
        {"ASM x64 Scalar implementation", asmDetectEdgesWorker},
        {"ASM x64 SIMD implementation", asmDetectEdgesSIMDWorker},
        {"C implementation", cDetectEdgesWorker},
    };

    bench::csv csv;
    uint32_t runs = 10;

    try {
        bench::bench("./img/small.jpg", "small", runs, threads, workers, csv);
        bench::bench("./img/medium.jpg", "medium", runs, threads, workers, csv);
        bench::bench("./img/large.jpg", "large", runs, threads, workers, csv);

        std::filesystem::path path = "bench.csv";
        if (argc > 1)
            path = argv[1];

        std::ofstream file(path);
        if (file.is_open())
            file << csv.text;
        else
            throw std::runtime_error("Failed to create file: " + path.string());

    } catch (std::exception &e) {
        std::println("An error occured what(): {}", e.what());
        return 1;
    }

    return 0;
}