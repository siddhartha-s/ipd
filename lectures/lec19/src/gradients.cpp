#include "gradients.h"

// Causes the gradient to change uniformly throughout its range.
struct Linear : Modulator
{
    sample modulate(sample in) const override { return in; }
};

modulator_ptr linear_modulator()
{
    return std::make_shared<Linear>();
}

// Causes the gradient to oscillate like a sine wave.
struct Sinusoidal : Modulator
{
    double frequency;   // The number of full waves in the gradient
    double phase;       // Where in the wave to start, 1 being the whole wave

    Sinusoidal(double frequency, double phase)
            : frequency{frequency}
            , phase{phase}
    { }

    sample modulate(sample in) const override
    {
        return 0.5 + 0.5 * sin(2 * M_PI * (frequency * in.value() + phase));
    }
};

modulator_ptr sinusoidal_modulator(double frequency, double phase)
{
    return std::make_shared<Sinusoidal>(frequency, phase);
}

// Makes the gradient change from left to right.
struct Horizontal : Projector
{
    sample project(sample x, sample) const override
    { return x; }
};

projector_ptr horizontal_projector()
{
    return std::make_shared<Horizontal>();
}

// Makes the gradient change from top to bottom.
struct Vertical : Projector
{
    sample project(sample, sample y) const override
    { return y; }
};

projector_ptr vertical_projector()
{
    return std::make_shared<Vertical>();
}

// Makes the gradient change from inside to out in concentric same-color
// circles.
struct Circular : Projector
{
    sample project(sample x, sample y) const override
    {
        auto dx = x.value() - 0.5;
        auto dy = y.value() - 0.5;
        return sqrt(2 * (dx*dx + dy*dy));
    }
};

projector_ptr circular_projector()
{
    return std::make_shared<Circular>();
}

// Adapts a shape to fill it with a color gradient.
class Gradient : public Drawing_decorator
{
public:
    Gradient(drawing_ptr, color start, color end, projector_ptr, modulator_ptr);

    using sample = graphics::sample;

    color color_at(posn) const override;

protected:
    virtual color color_at(sample, sample) const;
    virtual color color_at(sample) const;

    color           start_;
    color           end_;
    modulator_ptr   modulator_;
    projector_ptr   projector_;
};

Gradient::Gradient(drawing_ptr base, color start, color end,
                   projector_ptr projector,
                   modulator_ptr modulator)
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
    return interpolate(start_, modulator_->modulate(weight), end_);
}

// Computes the color at an abstract (unit square) position by
// projecting it to 1-D and then delegating to color_at(sample).
Drawing::color Gradient::color_at(sample x, sample y) const
{
    return color_at(projector_->project(x, y));
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
                     projector_ptr projector, modulator_ptr modulator)
{
    return std::make_shared<Gradient>(base, start, end, projector, modulator);
}

