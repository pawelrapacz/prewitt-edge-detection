#include "bench.h"

#include <cimpl.h>
#include <asmimpl.h>

#include <fstream>

int main() {
    const bench::threads_array threads = {1, 2, 4, 8, 16, 32, 64};
    const bench::workers_array workers = {
        // {"ASM x64 Scalar implementation", asmDetectEdgesWorker},
        {"ASM x64 SIMD implementation", asmDetectEdgesSIMDWorker},
        {"C implementation", cDetectEdgesWorker},
    };

    bench::csv csv;
    uint32_t runs = 10;

    bench::bench("./img/small.jpg", "small", runs, threads, workers, csv);
    bench::bench("./img/medium.jpg", "medium", runs, threads, workers, csv);
    bench::bench("./img/large.jpg", "large", runs, threads, workers, csv);

    std::ofstream file("bench.csv");
    if (file.is_open())
        file << csv.text;

    return 0;
}