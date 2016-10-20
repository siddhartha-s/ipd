#pragma once

#include "Picture.h"

// Abstract class for shapes that adapt/decorate another shape. This stores
// the shape-to-be-decorated and passes through function calls to it by
// default. Derive and override to change the behavior.
class Picture_decorator : public Picture
{
public:
    // Decorates the given shape, using its bounding box as the decorator's
    // bounding box.
    Picture_decorator(drawing_ptr);

    // Decorates the given shape, using the given bounding box.
    Picture_decorator(drawing_ptr, bbox);

    bool contains(posn) const override;
    color color_at(posn) const override;

private:
    const drawing_ptr base_;
};
