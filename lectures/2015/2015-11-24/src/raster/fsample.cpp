#include "fsample.hpp"

#include <algorithm>

namespace raster
{

fsample const fsample::ZERO {0.0};
fsample const fsample::ONE  {1.0};

fsample::fsample(double value) noexcept
    : value_(std::min(1.0, std::max(0.0, value)))
{}

fsample interpolate(fsample a, double weight, fsample b) noexcept
{
    return fsample{(1 - weight) * a + weight * b};
}

// fsample fsample::operator*(fsample other) const noexcept
// {
//     fsample result;
//     result.value_ = value() * other.value(); // can't overflow
//     return result;
// }

// fsample& fsample::operator*=(fsample other) noexcept
// {
//     value_ *= other.value(); // cannot overflow
//     return *this;
// }

} // namespace raster
