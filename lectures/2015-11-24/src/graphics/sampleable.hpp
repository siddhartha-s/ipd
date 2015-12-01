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
    using coord = raster::posn<double>;
    using color = raster::fcolor;
    using ptr   = std::shared_ptr<const sampleable>;

    inline color color_at(coord p) const
    { return color_at_(p); }

protected:
    virtual color color_at_(coord) const = 0;
};

class bounded_sampleable : public sampleable
{
public:
    template <typename... Params>
    bounded_sampleable(Params... params)
        : bbox_{std::forward<Params>(params)...}
    { }

private:
    bbox_t bbox_;

    virtual const bbox_t& get_bounding_box_() const noexcept override
    { return bbox_; }
};

void sample(const sampleable& scene, raster::raster& target);

sampleable::ptr canvas(sampleable::color);

sampleable::ptr rectangle(sampleable::coord, sampleable::coord,
                          sampleable::color);

sampleable::ptr circle(sampleable::coord, double, sampleable::color);

sampleable::ptr overlay(sampleable::ptr foreground, sampleable::ptr background);

} // namespace graphics
