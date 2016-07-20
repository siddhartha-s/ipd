#pragma once

#include "color.hpp"
#include "geometry.hpp"

#include <vector>

namespace raster
{

class raster
{
public:
    using coord = posn<int>;

    raster(int width, int height);
    raster(int width, int height, color fill);

    // forward declaration
    class const_row_ref;
    class row_ref;

    const_row_ref operator[](int) const;
    row_ref operator[](int);

    color& operator[](coord);
    const color& operator[](coord) const;

    int width() const noexcept { return width_; }
    int height() const noexcept { return height_; }

    bool in_bounds(coord) const noexcept;

    void write_out(const char* filename) const;

    class const_row_ref
    {
    public:
        const color& operator[](int) const;

    private:
        friend class raster;
        const_row_ref(const raster*, int y);
        const raster* const raster_;
        const int y_;
    };

    class row_ref : const_row_ref
    {
    public:
        color& operator[](int);

    private:
        friend class raster;
        row_ref(raster*, int y);
    };

private:
    int width_, height_;
    std::vector<color> pixels_;
};

} // namespace raster
