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

#include "prewitt_impl.h"

#include <doctest/doctest.h>

#include <core.h>

#include <cstdint>
#include <filesystem>
#include <functional>
#include <algorithm>

#include <QImage>

namespace fs = std::filesystem;
constexpr auto EXT = ".png";

QImage loadImage(const fs::path& p) {
    QImage image = QImage(p.string().c_str()).convertedTo(QImage::Format_RGB888);
    REQUIRE_FALSE(image.isNull());
    return image;
}

// checks the given prewitt function against reference results
void checkWithReference(std::function<PrewittResult(const QImage&, size_t*)> prewitt) {
    fs::path inputdir = TESTS_REFERENCE_INPUT_DIR;
    fs::path expectdir = TESTS_REFERENCE_EXPECT_DIR;

    REQUIRE(fs::is_directory(inputdir));
    REQUIRE(fs::is_directory(expectdir));

    auto fileCount = std::ranges::count_if(fs::directory_iterator(expectdir),
        [](const auto& entry) { return entry.is_regular_file(); });
    REQUIRE(fileCount > 0);

    for (const auto& entry : fs::directory_iterator(expectdir)) {
        if (!entry.is_regular_file() ||
            entry.path().extension() != EXT) {
            continue;
        }

        SUBCASE(entry.path().filename().string()) {
            const fs::path inputp = inputdir / entry.path().filename();
            REQUIRE(fs::exists(inputp));

            const QImage img = loadImage(inputp);
            const QImage expected = loadImage(entry.path());

            size_t bpl;
            auto output = prewitt(img, &bpl);
            REQUIRE(output);

            const QImage result(
                output,
                img.width(),
                img.height(),
                bpl,
                QImage::Format_RGB888,
                imageBufferFree,
                output
            );

            REQUIRE_FALSE(result.isNull());
            CHECK(result == expected);
        }
    }
}


TEST_SUITE_BEGIN("Prewitt reference tests");

TEST_CASE_TEMPLATE("Reference test ", MyImpl, PREWITT_IMPLEMENTATIONS) {
    checkWithReference([] (const QImage& img, size_t* bpl) {
        return detectEdges(
            img.width(),
            img.height(),
            RGB,
            img.constBits(),
            img.bytesPerLine(),
            TESTS_AMOUNT,
            bpl,
            MyImpl::get()
        );
    });
}

TEST_CASE_TEMPLATE("Reference multithreaded test", MyImpl, PREWITT_IMPLEMENTATIONS) {
    uint32_t threads = GENERATE(1, 2, 3, 4, 7, 8, 16, 21, 32, 55, 64);

    checkWithReference([=] (const QImage& img, size_t* bpl) {
        return detectEdgesThreads(
            img.width(),
            img.height(),
            RGB,
            img.constBits(),
            img.bytesPerLine(),
            TESTS_AMOUNT,
            bpl,
            MyImpl::get(),
            threads
        );
    });
}

TEST_SUITE_END();
