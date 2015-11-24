#include "sampleable.hpp"

#include "gsl/gsl.h"

namespace graphics
{

using raster::fcolor;

void sample(const sampleable& scene, raster::raster& target)
{
    for (int x = 0; x < target.width(); ++x) {
        for (int y = 0; y < target.height(); ++y) {
            target[{x, y}] = scene.color_at({x + 0.5, y + 0.5});
        }
    }
}

rectangle::rectangle(coord p1, coord p2, color c)
    : box_{p1, p2}, color_{c}
{ }

fcolor rectangle::color_at(coord p) const
{
    if (box_.within(p)) {
        return color_;
    } else {
        return color::TRANSPARENT;
    }
}

rectangle::bbox rectangle::get_bounding_box() const
{
    return box_;
}

circle::circle(coord center, double radius, fcolor color)
    : center_{center}, radius_{radius}, color_{color}
{
    Expects(radius > 0);
}

fcolor circle::color_at(coord p) const
{
    if (distance(p, center_) <= radius_) {
        return color_;
    } else {
        return fcolor::TRANSPARENT;
    }
}

rectangle::bbox circle::get_bounding_box() const
{
    return {{center_.x - radius_, center_.y - radius_},
            {center_.x + radius_, center_.y + radius_}};
}

} // namespace graphics
