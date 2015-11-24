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
    return pixels_[width() * p.x + p.y];
}

const color& raster::operator[](coord p) const
{
    Expects(in_bounds(p));
    return pixels_[width() * p.x + p.y];
}

bool raster::in_bounds(coord p) const noexcept
{
    return p.x >= 0 && p.y >= 0 && p.x < static_cast<long>(width()) &&
           p.y < static_cast<long>(height());
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

} // namespace raster
