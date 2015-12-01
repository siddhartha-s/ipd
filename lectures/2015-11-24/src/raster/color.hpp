#pragma once

#include "fsample.hpp"

#include <cstdint>
#include <cstddef>

namespace raster
{

class fcolor
{
public:
    fcolor(fsample red, fsample green, fsample blue,
           fsample alpha = fsample::ONE) noexcept;
    fcolor(double red, double green, double blue, double alpha = 1.0) noexcept;
    fcolor(struct color) noexcept;

    static fcolor const TRANSPARENT;
    static fcolor const WHITE;
    static fcolor const BLACK;

    fsample red()   const noexcept { return red_;   }
    fsample green() const noexcept { return green_; }
    fsample blue()  const noexcept { return blue_;  }
    fsample alpha() const noexcept { return alpha_; }

private:
    fsample red_, green_, blue_, alpha_;
};

fcolor grayscale(const fcolor&) noexcept;
fcolor overlay(const fcolor& foreground, const fcolor& background) noexcept;
fcolor interpolate(const fcolor& a, double weight, const fcolor& b) noexcept;

class color_blender
{
public:
    color_blender() noexcept;

    color_blender& add(double weight, const fcolor&) noexcept;
    operator fcolor() const noexcept;

private:
    double red_    = 0;
    double green_  = 0;
    double blue_   = 0;
    double alpha_  = 0;
    double weight_ = 0;
};

color_blender& operator<<(color_blender&, const fcolor&) noexcept;

struct color
{
    uint32_t value;

    using byte = unsigned char;

    static byte constexpr BYTE_MAX = 255;

    color(byte red, byte green, byte blue, byte alpha = BYTE_MAX) noexcept;
    color(const fcolor&) noexcept;

    // constructs transparent by default
    color() noexcept;

    explicit color(uint32_t argb) noexcept;

    static color const TRANSPARENT;
    static color const WHITE;
    static color const BLACK;

    byte red()   const noexcept;
    byte green() const noexcept;
    byte blue()  const noexcept;
    byte alpha() const noexcept;
};

} // namespace raster
