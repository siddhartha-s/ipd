#include "Picture.h"

Picture::Picture(const bbox& box)
        : bbox_(box)
{ }

const Picture::bbox& Picture::get_bbox() const
{
    return bbox_;
}

Picture::color Picture::color_at(posn point) const
{
    if (contains(point))
        return color::black;
    else
        return color::transparent;
}

Picture::~Picture() { }

