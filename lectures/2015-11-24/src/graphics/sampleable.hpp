#pragma once

#include "raster/color.hpp"
#include "raster/geometry.hpp"
#include "raster/raster.hpp"

#include <memory>

namespace graphics
{

class sampleable : public raster::boundable<double>
{
public:
    using coord = raster::point<double>;
    using bbox  = raster::bounding_box<double>;
    using color = raster::fcolor;
    using ptr   = std::shared_ptr<sampleable>;

    virtual color color_at(coord) const = 0;
};

class rectangle : public sampleable
{
public:
    rectangle(coord, coord, color);

    virtual color color_at(coord) const override;
    virtual bbox get_bounding_box() const override;

private:
    bbox box_;
    color color_;
};

class circle : public sampleable
{
public:
    circle(coord, double radius, color);

    virtual color color_at(coord) const override;
    virtual bbox get_bounding_box() const override;

private:
    coord center_;
    double radius_;
    color color_;
};

void
sample(const sampleable& scene, raster::raster& target);

} // namespace graphics
