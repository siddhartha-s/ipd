#pragma once

#include "fsample.hpp"

#include <cstdint>
#include <cstddef>

namespace raster
{

class fcolor
{
public:
    fcolor(fsample red, fsample green, fsample blue, fsample alpha) noexcept;
    fcolor(fsample red, fsample green, fsample blue) noexcept;
    fcolor(float red, float green, float blue, float alpha) noexcept;
    fcolor(float red, float green, float blue) noexcept;
    fcolor(struct color) noexcept;

    static fcolor const TRANSPARENT;
    static fcolor const WHITE;
    static fcolor const BLACK;

    fcolor grayscale() const noexcept;
    fcolor overlay(const fcolor& background) const noexcept;
    fcolor interpolate(double weight, const fcolor& background) const noexcept;

    fsample red()   const noexcept { return red_;   }
    fsample green() const noexcept { return green_; }
    fsample blue()  const noexcept { return blue_;  }
    fsample alpha() const noexcept { return alpha_; }

private:
    fsample red_, green_, blue_, alpha_;
};

struct color
{
    uint32_t value;

    using byte = unsigned char;

    color(byte red, byte green, byte blue, byte alpha) noexcept;
    color(byte red, byte green, byte blue) noexcept;
    color(const fcolor&) noexcept;

    // constructs transparent by default
    color() noexcept;

    explicit color(uint32_t argb) noexcept;

    static color const TRANSPARENT;
    static color const WHITE;
    static color const BLACK;

    static size_t constexpr RED_SHIFT   = 16;
    static size_t constexpr GREEN_SHIFT = 8;
    static size_t constexpr BLUE_SHIFT  = 0;
    static size_t constexpr ALPHA_SHIFT = 24;

    byte red()   const noexcept;
    byte green() const noexcept;
    byte blue()  const noexcept;
    byte alpha() const noexcept;
};

} // namespace raster
