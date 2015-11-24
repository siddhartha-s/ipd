#include "fsample.hpp"

#include <algorithm>

namespace raster
{

fsample const fsample::ZERO {0.0f};
fsample const fsample::ONE  {1.0f};

fsample::fsample(float value) noexcept
    : value_(std::min<float>(1.0, std::max<float>(0.0, value)))
{}

fsample fsample::interpolate(double weight, fsample other) const noexcept
{
    return fsample{
        static_cast<float>(value() * weight + other.value() * (1 - weight))};
}

fsample fsample::operator*(fsample other) const noexcept
{
    fsample result;
    result.value_ = value() * other.value(); // can't overflow
    return result;
}

fsample& fsample::operator*=(fsample other) noexcept
{
    value_ *= other.value(); // cannot overflow
    return *this;
}

} // namespace raster
