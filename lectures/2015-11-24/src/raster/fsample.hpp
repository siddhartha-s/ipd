#pragma once

namespace raster
{

// A floating-point sample in the range 0.0 to 1.0.
class fsample
{
public:
    static fsample const ZERO;
    static fsample const ONE;

    // Constructs a sample, clipping the given value into the correct
    // range.
    explicit fsample(double) noexcept;

    // Constructs the 0.0 sample
    fsample() noexcept : fsample{0.0} {};

    // The double value of a sample.
    double value() const noexcept { return value_; }
    operator double() const noexcept { return value_; }

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

    // fsample operator*(fsample other) const noexcept;
    // fsample& operator*=(fsample) noexcept;

private:
    double value_;
    // INVARIANT: 0.0 ≤ value_ ≤ 1.0
};

fsample interpolate(fsample a, double weight, fsample b) noexcept;

} // namespace raster
