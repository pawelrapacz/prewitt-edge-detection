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

#include "prewitt_impl.h"
#include <core.h>


template<size_t Height, size_t Width>
void checkGray(const TestImage<Height, Width>& in, const TestImage<Height, Width>& expect, detect_edges_worker worker) {
    size_t bpl;
    auto ptr = detectEdges(
        in.width(),
        in.height(),
        GRAY,
        in.data(),
        in.bpl(),
        TESTS_AMOUNT,
        &bpl,
        worker
    );
    REQUIRE(ptr);
    CHECK(expect.equals(ptr, bpl));
    imageBufferFree(ptr);
}

template<size_t Height, size_t Width>
    requires (Width % RGB == 0)
void checkRGB(const TestImage<Height, Width>& in, const TestImage<Height, Width>& expect, detect_edges_worker worker) {
    size_t bpl;
    auto ptr = detectEdges(
        in.width() / 3,
        in.height(),
        RGB,
        in.data(),
        in.bpl(),
        TESTS_AMOUNT,
        &bpl,
        worker
    );
    REQUIRE(ptr);
    CHECK(expect.equals(ptr, bpl));
    imageBufferFree(ptr);
}


TEST_SUITE_BEGIN("Known value tests");

TEST_CASE_TEMPLATE("gray 1x1", MyImpl, PREWITT_IMPLEMENTATIONS) {
    const uint8_t fill = GENERATE(0, 1, 255);
    const TestImage<1,1> img = fill;
    constexpr TestImage<1, 1> zero = 0;
    checkGray(img, zero, MyImpl::get());
}

TEST_CASE_TEMPLATE("gray 3x3 pattern", MyImpl, PREWITT_IMPLEMENTATIONS) {
    constexpr TestImage<3, 3> zero = 0;
    const uint8_t fill = GENERATE(0, 1, 10, 15, 255);
    const auto testImageCase = GENERATE(
        TestImageCase<3,3>{
            {fill}, zero, "monocolor"
        },
        TestImageCase<3,3>{
            {{{fill, 0, fill},
              {fill, 0, fill},
              {fill, 0, fill}}},
            zero, "vertical sieds"
        },
        TestImageCase<3,3>{
            {{{0, fill, 0},
              {0, fill, 0},
              {0, fill, 0}}},
            zero, "vertical sieds"
        },
        TestImageCase<3,3>{
            {{{fill, fill, fill},
              {   0,    0,    0},
              {fill, fill, fill}}},
            zero, "horizontal sieds"
        },
        TestImageCase<3,3>{
            {{{   0,    0,    0},
              {fill, fill, fill},
              {   0,    0,    0}}},
            zero, "horizontal center"
        },
        TestImageCase<3,3>{
            {{{fill,    0,    0},
              {   0, fill,    0},
              {   0,    0, fill}}},
            zero, "diagonal left"
        },
        TestImageCase<3,3>{
            {{{   0,    0, fill},
              {   0, fill,    0},
              {fill,    0,    0}}},
            zero, "diagonal right"
        }
    );
    INFO(testImageCase.info);
    checkGray(testImageCase.input, testImageCase.expect, MyImpl::get());
}

TEST_CASE_TEMPLATE("gray 3x3 random", MyImpl, PREWITT_IMPLEMENTATIONS) {
    constexpr TestImage<3, 3> zero = 0;
    const auto testImageCase = GENERATE(
        TestImageCase<3,3>{
            {{{2, 4, 0},
              {4, 7, 6},
              {5, 6, 1}}},
            {{{0, 0, 0},
              {0, 7, 0},
              {0, 0, 0}}},
            "1"
        },
        TestImageCase<3, 3>{
            {{{1, 0, 4},
              {3, 3, 2},
              {1, 8, 1}}},
            {{{0, 0, 0},
              {0, 5, 0},
              {0, 0, 0}}},
            "2"
        },
        TestImageCase<3, 3>{
            {{{9, 6, 0},
              {0, 1, 3},
              {3, 8, 9}}},
            {{{0, 0, 0},
              {0, 5, 0},
              {0, 0, 0}}},
            "3"
        },
        TestImageCase<3, 3>{
            {{{0, 8, 3},
              {8, 6, 3},
              {7, 9, 4}}},
            {{{0, 0, 0},
              {0, 10, 0},
              {0, 0, 0}}},
            "4"
        },
        TestImageCase<3, 3>{
            {{{0, 2, 6},
              {5, 4, 2},
              {3, 5, 1}}},
            {{{0, 0, 0},
              {0, 1, 0},
              {0, 0, 0}}},
            "5"
        }
    );
    INFO(testImageCase.info);
    checkGray(testImageCase.input, testImageCase.expect, MyImpl::get());
}

TEST_CASE_TEMPLATE("rgb shoud be zero", MyImpl, PREWITT_IMPLEMENTATIONS) {
    SUBCASE("3x1") {
        const auto img = GENERATE(
            TestImage<3, 3>(0),
            TestImage<3, 3>(1),
            TestImage<3, 3>(50),
            TestImage<3, 3>(255),
            TestImage
            {{{2, 4, 0},
            {4, 7, 6},
            {5, 6, 1}}},
            TestImage
            {{{1, 0, 4},
            {3, 3, 2},
            {1, 8, 1}}},
            TestImage
            {{{9, 6, 0},
            {0, 1, 3},
            {3, 8, 9}}}
        );
        checkRGB(img, TestImage<3, 3>(0), MyImpl::get());
    }

    SUBCASE("3x2") {
        const auto img = GENERATE(
            TestImage<3, 6>(0),
            TestImage<3, 6>(1),
            TestImage<3, 6>(50),
            TestImage<3, 6>(255),
            TestImage
            {{{1, 2, 0, 9, 7, 2},
              {9, 2, 3, 0, 4, 5},
              {7, 8, 9, 5, 7, 8}}},
            TestImage
            {{{8, 3, 0, 1, 0, 1},
              {5, 7, 3, 3, 0, 3},
              {4, 4, 3, 2, 4, 9}}}
        );
        checkRGB(img, TestImage<3, 6>(0), MyImpl::get());
    }

    SUBCASE("3x3") {
        const auto img = GENERATE(
            TestImage<3, 9>(0),
            TestImage<3, 9>(1),
            TestImage<3, 9>(50),
            TestImage<3, 9>(255)
        );
        checkRGB(img, TestImage<3, 9>(0), MyImpl::get());
    }
}

TEST_CASE_TEMPLATE("rgb 3x3 random", MyImpl, PREWITT_IMPLEMENTATIONS) {
    const auto testImageCase = GENERATE(
        TestImageCase<3, 9>{
            {{{1, 2, 0, 9, 7, 2, 2, 6, 4},
              {9, 2, 3, 0, 4, 5, 6, 8, 1},
              {7, 8, 9, 5, 7, 8, 3, 6, 3}}},
            {{{0, 0, 0, 0, 0, 0, 0, 0, 0},
              {0, 0, 0, 6, 10, 14, 0, 0, 0},
              {0, 0, 0, 0, 0, 0, 0, 0, 0}}}
        },
        TestImageCase<3, 9>{
            {{{1, 5, 0, 9, 4, 2, 0, 1, 5},
              {9, 2, 5, 7, 4, 5, 6, 0, 1},
              {3, 7, 5, 5, 6, 0, 7, 8, 0}}},
            {{{0, 0, 0, 0, 0, 0, 0, 0, 0},
              {0, 0, 0, 5, 12, 4, 0, 0, 0},
              {0, 0, 0, 0, 0, 0, 0, 0, 0}}}
        }
    );
    checkRGB(testImageCase.input, testImageCase.expect, MyImpl::get());
}

TEST_SUITE_END();
