#include "drawings.h"
#include "Drawing_decorator.h"

// A limitless expanse of color with infinite bounding box, suitable for use
// as a background for other shapes.
class Background : public Drawing
{
public:
    // Constructs a background of the given color, white by default.
    Background(const color& fill);

    bool contains(posn) const override;
    color color_at(posn) const override;

private:
    color color_;
};

Background::Background(const color& fill)
        : Drawing{bbox::everything()}, color_{fill}
{ }

bool Background::contains(posn) const
{
    return true;
}

Drawing::color Background::color_at(posn) const
{
    return color_;
}

drawing_ptr background(const Drawing::color& fill)
{
    return std::make_shared<Background>(fill);
}

// A circle specified by its center and radius.
class Circle : public Drawing
{
public:
    // Constructs a circle with the given center position and radius.
    Circle(posn center, double radius);

    bool contains(posn) const override;

private:
    posn center_;
    double radius_;
};

// Computes the bounding box of a circle, given its center and radius.
static Drawing::bbox bbox_of_circle(Drawing::posn center, double radius)
{
    return Drawing::bbox(center.y - radius, center.x + radius,
                         center.y + radius, center.x - radius);
}

Circle::Circle(posn center, double radius)
        : Drawing{bbox_of_circle(center, radius)}
        , center_{center}
        , radius_{radius}
{ }

bool Circle::contains(posn point) const
{
    return distance(center_, point) <= radius_;
}

drawing_ptr circle(Drawing::posn center, double radius)
{
    return std::make_shared<Circle>(center, radius);
}

class Difference : public Drawing
{
public:
    Difference(drawing_ptr base, drawing_ptr mask);
    bool contains(posn) const override;
    color color_at(posn) const override;

private:
    drawing_ptr base_;
    drawing_ptr mask_;
};

Difference::Difference(drawing_ptr base, drawing_ptr mask)
        : Drawing{base->get_bbox() * mask->get_bbox()}
        , base_{base}
        , mask_{mask}
{ }

bool Difference::contains(posn point) const
{
    return (!mask_->contains(point)) && base_->contains(point);
}

Drawing::color Difference::color_at(posn point) const
{
    if (! mask_->contains(point))
        return base_->color_at(point);
    else
        return color::transparent;
}

drawing_ptr difference(drawing_ptr base, drawing_ptr mask)
{
    return std::make_shared<Difference>(base, mask);
}

// Adapts another shape to change its color.
class Fill : public Drawing
{
public:
    Fill(drawing_ptr, const color&);
    bool contains(posn) const override;
    color color_at(posn) const override;

private:
    const drawing_ptr base_;
    const color color_;
};

Fill::Fill(drawing_ptr base, const color& color)
        : Drawing(base->get_bbox())
        , base_{base}
        , color_{color}
{ }

bool Fill::contains(posn point) const
{
    return base_->contains(point);
}

Drawing::color Fill::color_at(posn point) const
{
    if (base_->contains(point))
        return color_;
    else
        return color::transparent;
}

drawing_ptr fill(drawing_ptr base, const Drawing::color& color)
{
    return std::make_shared<Fill>(base, color);
}
