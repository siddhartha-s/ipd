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

class circle_impl : public bounded_sampleable
{
public:
    circle_impl(coord center, double radius, color color)
        : bounded_sampleable{center.y - radius, center.x + radius,
                             center.y + radius, center.x - radius}
        , center_{center}
        , radius_{radius}
        , color_{color}
    {}

    virtual color color_at_(coord p) const override
    {
        if (distance(p, center_) <= radius_) {
            return color_;
        } else {
            return color::TRANSPARENT;
        }
    }

private:
    coord center_;
    double radius_;
    color color_;
};

ptr circle(coord center, double radius, color color)
{
    return std::make_shared<circle_impl>(center, radius, color);
}

class overlay_impl : public bounded_sampleable
{
public:
    overlay_impl(ptr fg, ptr bg)
        : bounded_sampleable{fg->get_bounding_box(), bg->get_bounding_box()}
        , fg_{fg}
        , bg_{bg}
    {}

    virtual color color_at_(coord p) const override
    {
        bool in_fg = fg_->get_bounding_box().contains(p);
        bool in_bg = bg_->get_bounding_box().contains(p);

        if (in_fg && in_bg) {
            return overlay(fg_->color_at(p), bg_->color_at(p));
        } else if (in_fg) {
            return fg_->color_at(p);
        } else if (in_bg) {
            return bg_->color_at(p);
        } else {
            return color::TRANSPARENT;
        }
    }

private:
    ptr fg_, bg_;
};

ptr overlay(ptr foreground, ptr background)
{
    return std::make_shared<overlay_impl>(foreground, background);
}

class scale_impl : public bounded_sampleable
{
public:
    scale_impl(double factor, ptr base)
        : bounded_sampleable{scale(factor, base->get_bounding_box())}
        , factor_{factor}
        , base_{base}
    {}

private:
    double factor_;
    ptr base_;

    virtual color color_at_(coord p) const override
    {
        return base_->color_at(p / factor_);
    }

    static bbox scale(double factor, const bbox& original)
    {
        return {factor * original.top(), factor * original.right(),
                factor * original.bottom(), factor * original.left()};
    }
};

ptr scale(double factor, ptr base)
{
    return std::make_shared<scale_impl>(factor, base);
}

} // namespace graphics
