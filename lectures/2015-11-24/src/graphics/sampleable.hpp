#pragma once

#include "raster/color.hpp"
#include "raster/geometry.hpp"
#include "raster/raster.hpp"

namespace graphics
{

class sampleable : public raster::boundable<double>
{
public:
    using coord = raster::point<double>;

    virtual raster::fcolor color_at(coord) const = 0;
};

void
sample(const sampleable& scene, raster::raster& target);

} // namespace graphics
