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

#include <doctest/doctest.h>

#include <core.h>

#include <limits>
#include <algorithm>

TEST_SUITE_BEGIN("core tests");

TEST_CASE("alignedImageBuffer allocation") {
    size_t bpl;
    const size_t width = GENERATE(1, 10, 255, 999, 1000);
    const size_t height = GENERATE(1, 10, 255, 999, 1000);
    const Format fmt = GENERATE(GRAY, RGB);

    
    SUBCASE("address alignment") {
        const size_t dataAlign = 16;
        const size_t addrAlign = GENERATE(1, 2, 4, 8, 16);
        
        auto ptr = alignedImageBufferAlloc(width, height, fmt, dataAlign, addrAlign, &bpl);
        REQUIRE(ptr);

        CHECK_EQ((uint64_t)ptr % addrAlign, 0);

        imageBufferFree(ptr);
    }

    SUBCASE("data alignment") {
        const size_t dataAlign = GENERATE(1, 2, 4, 8, 16);
        const size_t addrAlign = 1;
        
        auto ptr = alignedImageBufferAlloc(width, height, fmt, dataAlign, addrAlign, &bpl);
        REQUIRE(ptr);

        CHECK(bpl >= width);
        CHECK(bpl - width * fmt < dataAlign); // padding < dataAlign
        CHECK_EQ(bpl % dataAlign, 0);

        imageBufferFree(ptr);
    }

    SUBCASE("buffer is zeroed") {
        auto ptr = alignedImageBufferAlloc(width, height, fmt, 4, 4, &bpl);
        REQUIRE(ptr);
        
        CHECK(std::all_of(ptr, ptr + bpl * height,
            [](uint8_t b) { return b == 0; }));

        imageBufferFree(ptr);
    }

}

TEST_CASE("alignedImageBuffer invalid args") {
    const Format fmt = GENERATE(GRAY, RGB);
    size_t bpl;

    SUBCASE("zeroed args") {
        CHECK_EQ(alignedImageBufferAlloc(0, 10, fmt, 4, 4, &bpl), nullptr);
        CHECK_EQ(alignedImageBufferAlloc(10, 0, fmt, 4, 4, &bpl), nullptr);
        CHECK_EQ(alignedImageBufferAlloc(10, 10, (Format)0, 4, 4, &bpl), nullptr);
        CHECK_EQ(alignedImageBufferAlloc(10, 10, fmt, 0, 4, &bpl), nullptr);
        CHECK_EQ(alignedImageBufferAlloc(10, 10, fmt, 4, 0, &bpl), nullptr);
        CHECK_EQ(alignedImageBufferAlloc(10, 10, fmt, 4, 4, nullptr), nullptr);
    }

    SUBCASE("overflow") {
        constexpr auto max = std::numeric_limits<size_t>::max();
        CHECK_EQ(alignedImageBufferAlloc(max, 10, fmt, 4, 4, &bpl), nullptr);
        CHECK_EQ(alignedImageBufferAlloc(10, max, fmt, 4, 4, &bpl), nullptr);
        CHECK_EQ(alignedImageBufferAlloc(10, 10, Format(max), 4, 4, &bpl), nullptr);
        CHECK_EQ(alignedImageBufferAlloc(10, 10, fmt, max, 4, &bpl), nullptr);
        CHECK_EQ(alignedImageBufferAlloc(10, 10, fmt, 4, max, &bpl), nullptr);
    }

    SUBCASE("invalid address alignment") {
        const size_t align = GENERATE(3, 5, 7, 99);        
        CHECK_EQ(alignedImageBufferAlloc(16, 16, fmt, 4, 7, &bpl), nullptr);
    }

    SUBCASE("invalid data alignment") {
        CHECK_EQ(alignedImageBufferAlloc(16, 16, fmt, 1, 2, &bpl), nullptr);
        CHECK_EQ(alignedImageBufferAlloc(16, 16, fmt, 2, 4, &bpl), nullptr);
        CHECK_EQ(alignedImageBufferAlloc(16, 16, fmt, 8, 16, &bpl), nullptr);
        CHECK_EQ(alignedImageBufferAlloc(16, 16, fmt, 2, 7, &bpl), nullptr);
        CHECK_EQ(alignedImageBufferAlloc(16, 16, fmt, 17, 7, &bpl), nullptr);
        CHECK_EQ(alignedImageBufferAlloc(16, 16, fmt, 5, 7, &bpl), nullptr);
    }
}

TEST_CASE("imageBufferAlloc allocation") {
    const Format fmt = GENERATE(GRAY, RGB);
    const size_t width = 10;
    const size_t height = 10;
    size_t bpl;

    auto ptr = imageBufferAlloc(width, height, fmt, &bpl);
    REQUIRE(ptr);

    SUBCASE ("address alignment") {
        CHECK((uint64_t)ptr % 4 == 0);
    }

    SUBCASE("data alignment") {
        CHECK(bpl >= width);
        CHECK(bpl - width * fmt < 4); // padding < dataAlign
        CHECK(bpl % 4 == 0);
    }

    SUBCASE("buffer is zeroed") {
        CHECK(std::all_of(ptr, ptr + bpl * height,
            [](uint8_t b) { return b == 0; }));
    }

    imageBufferFree(ptr);
}

TEST_CASE("imageBufferAlloc invalid args") {
    size_t bpl;
    const Format fmt = GENERATE(GRAY, RGB);

    SUBCASE("zeroed args") {
        CHECK_EQ(imageBufferAlloc(0, 10, fmt, &bpl), nullptr);
        CHECK_EQ(imageBufferAlloc(10, 0, fmt, &bpl), nullptr);
        CHECK_EQ(imageBufferAlloc(10, 10, (Format)0, &bpl), nullptr);
        CHECK_EQ(imageBufferAlloc(10, 10, fmt, nullptr), nullptr);
    }

    SUBCASE("overflow") {
        constexpr auto max = std::numeric_limits<size_t>::max();
        CHECK_EQ(imageBufferAlloc(max, 10, fmt, &bpl), nullptr);
        CHECK_EQ(imageBufferAlloc(10, max, fmt, &bpl), nullptr);
        CHECK_EQ(imageBufferAlloc(10, max, Format(max), &bpl), nullptr);
    }
}

TEST_SUITE_END();
