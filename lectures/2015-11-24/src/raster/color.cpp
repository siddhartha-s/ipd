#include "color.hpp"
#include "geometry.hpp"

#include <algorithm>

namespace raster
{

using byte                     = color::byte;
static byte constexpr BYTE_MAX = color::BYTE_MAX;

fcolor::fcolor(fsample red, fsample green, fsample blue, fsample alpha) noexcept
    : red_{red}, green_{green}, blue_{blue}, alpha_{alpha}
{
    if (alpha == 0) red_ = green_ = blue_ = fsample::ZERO;
}

fcolor::fcolor(double red, double green, double blue, double alpha) noexcept
    : fcolor{fsample{red}, fsample{green}, fsample{blue}, fsample{alpha}}
{ }

static fsample to_fsample(byte b) noexcept
{
    return fsample{b / static_cast<double>(BYTE_MAX)};
}

fcolor::fcolor(color c) noexcept
    : fcolor{to_fsample(c.red()),
             to_fsample(c.green()),
             to_fsample(c.blue()),
             to_fsample(c.alpha())}
{ }

fcolor const fcolor::TRANSPARENT{0, 0, 0, 0};
fcolor const fcolor::WHITE{1, 1, 1};
fcolor const fcolor::BLACK{0, 0, 0};

bool opaque(const fcolor& c) noexcept
{
    return c.alpha() == 1;
}

bool transparent(const fcolor& c) noexcept
{
    return c.alpha() == 0;
}

fcolor grayscale(const fcolor& c) noexcept
{
    double lum = (c.red() + c.green() + c.blue()) / 3;
    return {lum, lum, lum, c.alpha()};
}

fcolor overlay(const fcolor& fg, const fcolor& bg) noexcept
{
    if (fg.alpha() == 1 || bg.alpha() == 0) return fg;
    if (fg.alpha() == 0) return bg;

    return interpolate(bg, fg.alpha(), fcolor{fg.red(), fg.green(), fg.blue()});
}

fcolor interpolate(const fcolor& a, double weight, const fcolor& b)
    noexcept
{
    auto pre_red =
        interpolate(a.alpha() * a.red(), weight, b.alpha() * b.red());
    auto pre_green =
        interpolate(a.alpha() * a.green(), weight, b.alpha() * b.green());
    auto pre_blue =
        interpolate(a.alpha() * a.blue(), weight, b.alpha() * b.blue());
    auto new_alpha = interpolate(a.alpha(), weight, b.alpha());

    return {pre_red / new_alpha, pre_green / new_alpha, pre_blue / new_alpha,
            new_alpha};
}

static size_t constexpr
    RED_SHIFT   = 16,
    GREEN_SHIFT = 8,
    BLUE_SHIFT  = 0,
    ALPHA_SHIFT = 24;

color::color(byte red, byte green, byte blue, byte alpha) noexcept
    : value{static_cast<uint32_t>((red   << RED_SHIFT)
                                | (green << GREEN_SHIFT)
                                | (blue  << BLUE_SHIFT)
                                | (alpha << ALPHA_SHIFT))}
{
    if (alpha == 0) value = 0;
}

color::color(uint32_t argb) noexcept : value{argb}
{}

static byte from_fsample(fsample fs) noexcept
{
    return static_cast<byte>(BYTE_MAX * fs.value());
}

color::color(const fcolor& fc) noexcept
    : color{from_fsample(fc.red()),
            from_fsample(fc.green()),
            from_fsample(fc.blue()),
            from_fsample(fc.alpha())}
{}

color::color() noexcept : color{0, 0, 0} {}

color const color::TRANSPARENT{0, 0, 0, 0};
color const color::WHITE{BYTE_MAX, BYTE_MAX, BYTE_MAX};
color const color::BLACK{0, 0, 0};

byte color::red()   const noexcept
{
    return static_cast<byte>((value >> RED_SHIFT) & 0xFF);
}

byte color::green() const noexcept
{
    return static_cast<byte>((value >> GREEN_SHIFT) & 0xFF);
}

byte color::blue()  const noexcept
{
    return static_cast<byte>((value >> BLUE_SHIFT) & 0xFF);
}

byte color::alpha() const noexcept
{
    return static_cast<byte>((value >> ALPHA_SHIFT) & 0xFF);
}

} // namespace raster
