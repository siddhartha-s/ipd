#include "raster.hpp"

#include "gsl/gsl.h"
#include <fstream>

namespace raster
{

raster::raster(int width, int height)
    : raster{width, height, color::BLACK}
{ }

raster::raster(int width, int height, color fill)
    : width_{width}
    , height_{height}
    , pixels_(width * height, fill)
{
    Expects(width > 0 && height > 0);
}

color& raster::operator[](coord p)
{
    Expects(in_bounds(p));
    return pixels_[width() * p.y + p.x];
}

const color& raster::operator[](coord p) const
{
    Expects(in_bounds(p));
    return pixels_[width() * p.y + p.x];
}

bool raster::in_bounds(coord p) const noexcept
{
    return 0 <= p.x && p.x < width() && 0 <= p.y && p.y < height();
}

void raster::write_out(const char* filename) const
{
    std::ofstream of{filename};
    of << "P6\n";       // magic number
    of << width() << ' ' << height() << " 255\n";

    for (const auto& pixel : pixels_) {
        of << pixel.red() << pixel.green() << pixel.blue();
    }
}

raster::const_row_ref::const_row_ref(const raster* raster, int y)
    : raster_{raster}, y_{y}
{ }

raster::row_ref::row_ref(raster* raster, int y)
    : const_row_ref{raster, y}
{ }

raster::const_row_ref raster::operator[](int y) const {
    return raster::const_row_ref{this, y};
}

raster::row_ref raster::operator[](int y) {
    return raster::row_ref{this, y};
}

const color& raster::const_row_ref::operator[](int x) const
{
    return (*raster_)[{x, y_}];
}

color& raster::row_ref::operator[](int x)
{
    return const_cast<raster&>(*raster_)[{x, y_}];
}

} // namespace raster
