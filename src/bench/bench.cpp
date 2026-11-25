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

#include <core.h>

#include <mutex>
#include <string>
#include <string_view>
#include <cstdio>
#include <format>
#include <print>

#include <QImageReader>
#include <QImage>

bench::ms_t bench::measureTime(std::function<void()> f) {
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end = std::chrono::high_resolution_clock::now();
    return {end - start};
}

void bench::bench(std::string_view path, std::string_view label, uint32_t runs, const threads_array& threads, const bench::workers_array& workers, bench::csv& csv) {
    static std::once_flag once;
    std::call_once(once, QImageReader::setAllocationLimit, 1024); // 1024 MB

    QImage image(path.data());
    image.convertTo(QImage::Format_RGB888);

    if (image.isNull() || image.format() != QImage::Format_RGB888)
        throw std::runtime_error(std::format("Image \"{}\" invalid or not loaded properly: {}", label, path));
    
    const auto width = image.width();
    const auto height = image.height();
    const auto bpl = image.bytesPerLine();
    const auto input = image.constBits();
   
    runs++; // first run is skipped
    progress prog(threads.size() * workers.size() * runs, std::format("bench: {:10}", label));

    for (auto thrd : threads) {
        for (auto& w : workers) {
            for (uint32_t i = 0; i < runs; i++) {
                prog++;

                uint8_t* output;
                auto time = bench::measureTime([&] {
                    output = detectEdgesThreads(width, height, RGB, input, bpl, 1., nullptr, w.worker, thrd);
                });

                if (output == nullptr)
                    throw std::runtime_error("processing failed");

                imageBufferFree(output);

                if (i > 0)
                    csv(w.name, label, width, height, thrd, time);
            }
        }
    }
}

bench::progress::progress(uint32_t max_val, std::string_view text, char fill, uint32_t barlen)
    : val(), max_val(max_val), text(text), bar(barlen, ' '), fill(fill) {
    std::print(stdout, "\x1b[?25l");
}

bench::progress::~progress() {
    std::println("\x1b[?25h");
}

void bench::progress::notify() {
    const auto progress = val * 100 / max_val;
    const auto pos = val * bar.size() / max_val;
    bar[pos] = fill;

    if (text.empty())
        std::print("\r[{}] {:>3}%", bar, progress);
    else
        std::print("\r{} [{}] {:>3}%", text, bar, progress);

    std::fflush(stdout);
}

void bench::progress::notify(uint32_t increment) {
    val += increment;
    if (val > max_val)
        val = max_val;
    notify();
}

void bench::progress::operator++() {
    notify(1);
}

void bench::progress::operator++(int) {
    operator++();
}