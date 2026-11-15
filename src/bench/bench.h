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

#pragma once

#include <core.h>

#include <vector>
#include <ratio>
#include <chrono>
#include <string>
#include <format>
#include <functional>

namespace bench {

    using thrdc_t = uint32_t;
    using threads_array = std::vector<thrdc_t>;

    struct worker_entry {
        std::string_view name;
        detect_edges_worker worker;
    };
     
    using workers_array = std::vector<worker_entry>;

    using ms_t = std::chrono::duration<double, std::milli>;

    struct csv {
        inline void operator()(std::string_view worker, std::string_view inputLabel, uint32_t width, uint32_t height, thrdc_t tc, ms_t time) {
            text += std::format("{};{} {}x{};{};{:.3f};{:%q}\n", worker, inputLabel, width, height, tc, time.count(), time);
        }

        std::string text = "version;input size;threads;time;time unit\n";
    };

    void bench(std::string_view path, std::string_view label, uint32_t runs, const threads_array& threads, const bench::workers_array& workers, bench::csv& csv);

    ms_t measureTime(std::function<void()> f);

    class progress {
    public:
        progress(uint32_t max_val, const std::string& text = {}, char fill = '#');
        ~progress();

        void notify();
        void notify(uint32_t increment);
        void operator++();
        void operator++(int);

        uint32_t val;
        uint32_t max_val;
        std::string text;
        char fill = '#';
    };

}