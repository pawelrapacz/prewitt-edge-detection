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

// this file provides convinience types and macros for testing different implementations

#include <core.h>
#include <cimpl.h>
#include <asmimpl.h>
#include <functional>

template<detect_edges_worker Worker>
struct Impl { static detect_edges_worker get() { return Worker; }; };

using PrewittResult = decltype(std::function{detectEdges})::result_type;

#define PREWITT_IMPLEMENTATIONS \
    Impl<cDetectEdgesWorker>, \
    Impl<asmDetectEdgesWorker>, \
    Impl<asmDetectEdgesSIMDWorker>


template<size_t Height, size_t Width>
struct TestImage {
    static_assert(Width > 0 && Height > 0);
    using type = uint8_t;

    constexpr TestImage(const type (&tab)[Height][Width]) noexcept {
        for (size_t i{}; i < Height; i++)
            for (size_t j{}; j < Width; j++)
                _data[i * Width + j] = static_cast<type>(tab[i][j]);
    }

    constexpr TestImage(type fill) noexcept {
        for (auto& pix : _data)
                pix = fill;
    }

    constexpr auto width() const  noexcept {
        return Width;
    }

    constexpr auto bpl() const  noexcept {
        return Width;
    }
    
    constexpr auto height() const noexcept {
        return Height;
    }

    constexpr const type* data() const noexcept {
        return _data;
    }

    constexpr type* data() noexcept {
        return _data;
    }

    constexpr bool equals(const type* in, size_t bpl) const noexcept {
        for (size_t i{}; i < Height; i++)
            for (size_t j{}; j < Width; j++)
                if (_data[i * Width + j] != in[i * bpl + j])
                    return false;
        return true;
    }

private:
    type _data[Width * Height];
};

template<size_t Height, size_t Width>
struct TestImageCase {
    TestImage<Height, Width> input, expect;
    const char* info = nullptr;
};
