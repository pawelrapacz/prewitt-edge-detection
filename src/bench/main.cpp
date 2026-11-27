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
#include <stdexcept>

// Usage:
// bench [input dir] [output]
// input dir - directory containing image files to bench (bmp, png, jpg)
// output - file to save the results
int main(int argc, char** argv) {
    const bench::threads_array threads = {1, 2, 4, 8, 16, 32, 64};
    const bench::workers_array workers = {
        {"ASM x64 Scalar implementation", asmDetectEdgesWorker},
        {"ASM x64 SIMD implementation", asmDetectEdgesSIMDWorker},
        {"C implementation", cDetectEdgesWorker},
    };

    try {
        bench::csv csv;
        uint32_t runs = 10;

        const auto [in, out] = bench::parse(argc, argv);

        for (const auto& img : in) {
            bench::bench(img.string(), img.stem().string(), runs, threads, workers, csv);
        }

        std::ofstream file(out);
        if (file.is_open())
            file << csv.text;
        else
            throw std::runtime_error("Failed to create file: " + out.string());

    } catch (std::exception &e) {
        std::println("An error occured what(): {}", e.what());
        return 1;
    }

    return 0;
}