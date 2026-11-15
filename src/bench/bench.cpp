#include "bench.h"

#include <mutex>
#include <string>
#include <string_view>
#include <cstdio>
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
    image.convertTo(QImage::Format_Grayscale8);

    if (image.isNull() || image.format() != QImage::Format_Grayscale8)
        throw std::runtime_error("invalid image");
    
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
                    output = detectEdgesThreads(width, height, input, bpl, 1., nullptr, w.worker, thrd);
                });
                std::free(output);

                if (i > 0)
                    csv(w.name, label, width, height, thrd, time);
            }
        }
    }
}

bench::progress::progress(uint32_t max_val, const std::string& text, char fill)
    : val(), max_val(max_val), text(text), fill(fill){
    std::print(stdout, "\x1b[?25l");
}

bench::progress::~progress() {
    // val = max_val;
    // notify();
    std::println("\x1b[?25h");
}

void bench::progress::notify() {
    auto p = val * 100 / max_val;
    std::string bar(val * 40 / max_val, fill);

    if (text.empty())
        std::print("\r[{:<40}] {:>3}%", bar, p);
    else
        std::print("\r{} [{:<40}] {:>3}%", text, bar, p);

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