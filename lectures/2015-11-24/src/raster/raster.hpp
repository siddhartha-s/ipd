#pragma once

#include "color.hpp"
#include "geometry.hpp"

#include <vector>

namespace raster
{

class raster
{
public:
    using coord = point<int>;

    raster(int width, int height);
    raster(int width, int height, color fill);

    color& operator[](coord);
    const color& operator[](coord) const;

    int width() const noexcept { return width_; }
    int height() const noexcept { return height_; }

    bool in_bounds(coord) const noexcept;

    void write_out(const char* filename) const;

private:
    int width_, height_;
    std::vector<color> pixels_;
};

} // namespace raster
