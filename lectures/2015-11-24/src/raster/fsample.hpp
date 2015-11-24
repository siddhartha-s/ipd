#pragma once

namespace raster
{

// An floating-point sample in the range 0.0 to 1.0.
class fsample
{
public:
    static fsample const ZERO;
    static fsample const ONE;

    // Constructs a sample, clipping the given value into the correct
    // range.
    explicit fsample(float) noexcept;

    // Constructs the 0.0 sample
    fsample() noexcept : fsample{0.0} {};

    // The float value of a sample.
    float value() const noexcept { return value_; }
    operator float() const noexcept { return value_; }

    fsample interpolate(double weight, fsample other) const noexcept;

    bool operator==(fsample other) const noexcept
    { return value() == other.value(); }

    bool operator!=(fsample other) const noexcept
    { return value() != other.value(); }

    bool operator<(fsample other) const noexcept
    { return value() < other.value(); }

    bool operator<=(fsample other) const noexcept
    { return value() <= other.value(); }

    bool operator>(fsample other) const noexcept
    { return value() > other.value(); }

    bool operator>=(fsample other) const noexcept
    { return value() >= other.value(); }

    fsample operator*(fsample other) const noexcept;
    fsample& operator*=(fsample) noexcept;

private:
    float value_;
};

} // namespace raster
