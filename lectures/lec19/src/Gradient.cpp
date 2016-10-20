#include "Gradient.h"

// Adapts a shape to fill it with a color gradient.
class Gradient : public Drawing_decorator
{
public:
    Gradient(drawing_ptr, color start, color end,
             const Projector& = Horizontal{},
             const Modulator& = Linear{});

    using sample = graphics::sample;

    color color_at(posn) const override;

protected:
    virtual color color_at(sample, sample) const;
    virtual color color_at(sample) const;

    color           start_;
    color           end_;
    const Modulator& modulator_;
    const Projector& projector_;
};

Gradient::Gradient(drawing_ptr base, color start, color end,
                   const Projector& projector,
                   const Modulator& modulator)
        : Drawing_decorator{base}
        , start_{start}
        , end_{end}
        , projector_{projector}
        , modulator_{modulator}
{ }

// Interpolates a color between start_ and end_, using the modulator
// to alter the weight.
Drawing::color Gradient::color_at(sample weight) const
{
    return interpolate(start_, modulator_.modulate(weight), end_);
}

// Computes the color at an abstract (unit square) position by
// projecting it to 1-D and then delegating to color_at(sample).
Drawing::color Gradient::color_at(sample x, sample y) const
{
    return color_at(projector_.project(x, y));
}

graphics::color Gradient::color_at(posn point) const
{
    if (contains(point)) {
        // Maps the bounding box to the unit square
        sample x = (point.x - get_bbox().left()) / get_bbox().width();
        sample y = (point.y - get_bbox().top()) / get_bbox().height();

        return color_at(x, y);
    } else {
        return color::transparent;
    }
}

drawing_ptr gradient(drawing_ptr base, Drawing::color start, Drawing::color end,
                     const Projector& projector, const Modulator& modulator)
{
    return std::make_shared<Gradient>(base, start, end, projector, modulator);
}
