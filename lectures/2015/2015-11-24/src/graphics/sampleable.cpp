#include "sampleable.hpp"

#include "gsl/gsl.h"

#include <utility>

namespace graphics
{

using raster::fcolor;

using ptr   = sampleable::ptr;
using coord = sampleable::coord;
using color = sampleable::color;

void sample(const sampleable& scene, raster::raster& target)
{
    for (int y = 0; y < target.height(); ++y) {
        auto row = target[y];
        for (int x = 0; x < target.width(); ++x) {
            row[x] = scene.color_at({x + 0.5, y + 0.5});
        }
    }
}

static const double INFTY = std::numeric_limits<double>::infinity();
static const raster::bounding_box<double>
EVERYTHING{-INFTY, INFTY, INFTY, -INFTY};

class canvas_impl : public sampleable
{
public:
    canvas_impl(color color)
        : color_{color}
    { }

private:
    color color_;

    virtual const bbox& get_bounding_box_() const override
    {
        return EVERYTHING;
    }

    virtual color color_at_(coord) const override { return color_; }
};

ptr canvas(color color)
{
    return std::make_shared<canvas_impl>(color);
}

class rectangle_impl : public bounded_sampleable
{
public:
    rectangle_impl(coord p1, coord p2, color color)
        : bounded_sampleable{p1, p2}
        , color_{color}
    { }

private:
    color color_;

    virtual color color_at_(coord p) const override
    {
        if (get_bounding_box().contains(p)) {
            return color_;
        } else {
            return color::TRANSPARENT;
        }
    }
};

ptr rectangle(coord p1, coord p2, color color)
{
    return std::make_shared<rectangle_impl>(p1, p2, color);
}

} // namespace graphics
