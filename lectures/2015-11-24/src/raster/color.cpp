#include "color.hpp"

#include <algorithm>

namespace raster
{

fcolor::fcolor(fsample red, fsample green, fsample blue, fsample alpha) noexcept
    : red_{red}, green_{green}, blue_{blue}, alpha_{alpha}
{
    if (alpha == 0) red_ = green_ = blue_ = fsample{0};
}

fcolor::fcolor(fsample red, fsample green, fsample blue) noexcept
    : fcolor{red, green, blue, fsample::ONE}
{}

fcolor::fcolor(float red, float green, float blue, float alpha) noexcept
    : fcolor{fsample{red}, fsample{green}, fsample{blue}, fsample{alpha}}
{ }

fcolor::fcolor(float red, float green, float blue) noexcept
    : fcolor{fsample{red}, fsample{green}, fsample{blue}}
{ }

static fsample to_fsample(color::byte b) noexcept
{
    return fsample{b / 255.0f};
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

fcolor fcolor::grayscale() const noexcept
{
    float lum = (red().value() + green().value() + blue().value()) / 3;
    return {lum, lum, lum, alpha()};
}

fcolor fcolor::overlay(const fcolor& background) const noexcept
{
    return fcolor{red(), green(), blue(), fsample::ONE}
             .interpolate(1 - alpha(), background);
}

fcolor fcolor::interpolate(double weight, const fcolor& bg)
    const noexcept
{
    auto pre_red = (alpha() * red()).interpolate(weight, bg.alpha() * bg.red());
    auto pre_green =
        (alpha() * green()).interpolate(weight, bg.alpha() * bg.green());
    auto pre_blue =
        (alpha() * blue()).interpolate(weight, bg.alpha() * bg.blue());
    auto new_alpha = alpha().interpolate(weight, bg.alpha());

    return fcolor{pre_red / new_alpha, pre_green / new_alpha,
                  pre_blue / new_alpha, new_alpha};
}

color::color(byte red, byte green, byte blue, byte alpha) noexcept
    : value{static_cast<uint32_t>(red   << RED_SHIFT)
          | static_cast<uint32_t>(green << GREEN_SHIFT)
          | static_cast<uint32_t>(blue  << BLUE_SHIFT)
          | static_cast<uint32_t>(alpha << ALPHA_SHIFT)}
{
    if (alpha == 0) value = 0;
}

color::color(byte red, byte green, byte blue) noexcept
    : color{red, green, blue, 255}
{}

color::color(uint32_t argb) noexcept : value{argb}
{}

static color::byte from_fsample(fsample fs) noexcept
{
    return static_cast<color::byte>(255 * fs.value());
}

color::color(const fcolor& fc) noexcept
    : color{from_fsample(fc.red()),
            from_fsample(fc.green()),
            from_fsample(fc.blue()),
            from_fsample(fc.alpha())}
{}

color::color() noexcept : color{0, 0, 0} {}

color const color::TRANSPARENT{0, 0, 0, 0};
color const color::WHITE{255, 255, 255};
color const color::BLACK{0, 0, 0};

color::byte color::red()   const noexcept
{
    return static_cast<byte>((value >> RED_SHIFT) & 0xFF);
}

color::byte color::green() const noexcept
{
    return static_cast<byte>((value >> GREEN_SHIFT) & 0xFF);
}

color::byte color::blue()  const noexcept
{
    return static_cast<byte>((value >> BLUE_SHIFT) & 0xFF);
}

color::byte color::alpha() const noexcept
{
    return static_cast<byte>((value >> ALPHA_SHIFT) & 0xFF);
}

} // namespace raster
