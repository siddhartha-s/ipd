#include "Picture_decorator.h"

Picture_decorator::Picture_decorator(drawing_ptr base)
        : Picture_decorator{base, {&*base}}
{ }

Picture_decorator::Picture_decorator(drawing_ptr base, bbox box)
        : Picture{box}
        , base_{base}
{ }

bool Picture_decorator::contains(posn point) const
{
    return base_->contains(point);
}

graphics::color Picture_decorator::color_at(posn point) const
{
    return base_->color_at(point);
}
