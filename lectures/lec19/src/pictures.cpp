#include "pictures.h"
#include "Picture_decorator.h"

// A limitless expanse of color with infinite bounding box, suitable for use
// as a background for other shapes.
class Background : public Picture
{
public:
    // Constructs a background of the given color, white by default.
    Background(const color& fill)
            : Picture{bbox::everything()}, color_{fill} {}

    bool contains(posn) const override;
    color color_at(posn) const override;

private:
    color color_;
};

bool Background::contains(posn) const
{
    return true;
}

Picture::color Background::color_at(posn) const
{
    return color_;
}

picture_ptr background(const Picture::color& fill)
{
    return std::make_shared<Background>(fill);
}

// A circle specified by its center and radius.
class Circle : public Picture
{
public:
    // Constructs a circle with the given center position and radius.
    Circle(posn center, double radius)
            : Picture{bbox_of_circle(center, radius)}
            , center_{center}
            , radius_{radius}
    { }

    bool contains(posn) const override;

private:
    posn center_;
    double radius_;

    // Computes the bounding box of a circle, given its center and radius.
    static bbox bbox_of_circle(posn center, double radius)
    {
        return bbox(center.y - radius, center.x + radius,
                    center.y + radius, center.x - radius);
    }
};

bool Circle::contains(posn point) const
{
    return distance(center_, point) <= radius_;
}

picture_ptr circle(Picture::posn center, double radius)
{
    return std::make_shared<Circle>(center, radius);
}


class Difference : public Picture
{
public:
    Difference(picture_ptr base, picture_ptr mask)
            : Picture{base->get_bbox()}
            , base_{base}
            , mask_{mask}
    { }

    bool contains(posn) const override;
    color color_at(posn) const override;

private:
    picture_ptr base_;
    picture_ptr mask_;
};

bool Difference::contains(posn point) const
{
    return (!mask_->contains(point)) && base_->contains(point);
}

Picture::color Difference::color_at(posn point) const
{
    if (! mask_->contains(point))
        return base_->color_at(point);
    else
        return color::transparent;
}

picture_ptr difference(picture_ptr base, picture_ptr mask)
{
    return std::make_shared<Difference>(base, mask);
}

// Adapts another shape to change its color.
class Fill : public Picture
{
public:
    Fill(picture_ptr base, const color& color)
            : Picture(base->get_bbox())
            , base_{base}
            , color_{color}
    { }

    bool contains(posn) const override;
    color color_at(posn) const override;

private:
    const picture_ptr base_;
    const color color_;
};

bool Fill::contains(posn point) const
{
    return base_->contains(point);
}

Picture::color Fill::color_at(posn point) const
{
    if (base_->contains(point))
        return color_;
    else
        return color::transparent;
}

picture_ptr fill(picture_ptr base, const Picture::color& color)
{
    return std::make_shared<Fill>(base, color);
}

class Intersection : public Picture
{
public:
    Intersection(picture_ptr base, picture_ptr mask)
            : Picture{base->get_bbox() * mask->get_bbox()}
            , base_{base}
            , mask_{mask}
    { }

    bool contains(posn) const override;
    color color_at(posn) const override;

private:
    picture_ptr base_;
    picture_ptr mask_;
};

bool Intersection::contains(posn point) const
{
    return mask_->contains(point) && base_->contains(point);
}

Picture::color Intersection::color_at(posn point) const
{
    if (mask_->contains(point))
        return base_->color_at(point);
    else
        return color::transparent;
}

picture_ptr intersection(picture_ptr base, picture_ptr mask)
{
    return std::make_shared<Intersection>(base, mask);
}

// The empty drawing
class Nothing : public Picture
{
public:
    Nothing()
            : Picture(bbox::nothing())
    { }

    bool contains(posn) const override;
};

bool Nothing::contains(posn) const
{
    return false;
}

picture_ptr nothing()
{
    return std::make_shared<Nothing>();
}

// Adapts a shape to alter its opacity/transparency.
class Opacity : public Picture_decorator
{
public:
    Opacity(picture_ptr base, graphics::sample opacity)
            : Picture_decorator{base}
            , opacity_{opacity}
    { }

    color color_at(posn) const override;

private:
    graphics::sample opacity_;
};


Picture::color Opacity::color_at(posn point) const
{
    auto base_color = Picture_decorator::color_at(point);
    return color{base_color.red(),
                 base_color.green(),
                 base_color.blue(),
                 opacity_ * base_color.alpha()};
}

picture_ptr opacity(picture_ptr base, graphics::sample opacity)
{
    return std::make_shared<Opacity>(base, opacity);
}

// Composes two shapes by overlaying one over the other.
class Overlay : public Picture
{
public:
    // Places `over` over `under`.
    Overlay(picture_ptr over, picture_ptr under)
            : Picture{{&*over, &*under}}
            , over_{over}
            , under_{under}
    { }

    bool contains(posn) const override;
    color color_at(posn) const override;

private:
    const picture_ptr over_;
    const picture_ptr under_;
};

bool Overlay::contains(posn point) const
{
    return over_->contains(point) || under_->contains(point);
}

graphics::color Overlay::color_at(posn point) const
{
    bool in_over  = over_->get_bbox().contains(point);
    bool in_under = under_->get_bbox().contains(point);

    if (in_over) {
        if (in_under)
            return graphics::overlay(over_->color_at(point),
                                     under_->color_at(point));
        else
            return over_->color_at(point);
    } else {
        if (in_under)
            return under_->color_at(point);
        else
            return graphics::color::transparent;
    }
}

picture_ptr overlay(picture_ptr over, picture_ptr under)
{
    return std::make_shared<Overlay>(over, under);
}

picture_ptr overlay(std::initializer_list<picture_ptr> layers)
{
    picture_ptr result = nothing();

    for (auto layer : layers)
        result = overlay(result, layer);

    return result;
}

// A polygon, defined as a sequence of vertex positions.
class Polygon : public Picture
{
public:
    // Constructs a polygon from a sequence of vertices (e.g., a vector).
    Polygon(const std::vector<posn>& sequence)
            : Picture{bbox(sequence)}
            , vertices_{sequence}
    { }

    bool contains(posn p) const override;

protected:
    // Provides derived classes access to the sequence of vertices.
    const std::vector<posn>& get_vertices() const;

private:
    std::vector<posn> vertices_;

    static bool has_crossing(Picture::posn previous,
                      Picture::posn p,
                      Picture::posn current)
    {
        if (current.y < previous.y)
            std::swap(current, previous);

        if (previous.y <= p.y && p.y < current.y) {
            double y = p.y - previous.y;
            double x = p.x - previous.x;

            double dy = current.y - previous.y;
            double dx = current.x - previous.x;

            return y * dx > dy * x;
        } else {
            return false;
        }
    }
};

const std::vector<Picture::posn>& Polygon::get_vertices() const
{
    return vertices_;
}

bool Polygon::contains(posn p) const
{
    if (vertices_.size() == 0) return false;

    size_t crossings = 0;

    posn previous = vertices_.back();

    for (posn current : vertices_) {
        if (has_crossing(previous, p, current))
            ++crossings;

        previous = current;
    }

    return crossings % 2 == 1;
}

picture_ptr polygon(const std::vector<Picture::posn>& sequence)
{
    return std::make_shared<Polygon>(sequence);
}

picture_ptr polygon(std::initializer_list<Picture::posn> vertices)
{
    return std::make_shared<Polygon>(vertices);
}

picture_ptr regular_polygon(Polygon::posn center, double radius, size_t sides)
{
    std::vector<Picture::posn> vertices;

    for (size_t i = 0; i < sides; ++i) {
        double angle = M_PI / 2 + (2 * M_PI * i) / sides;
        double x = center.x + radius * cos(angle);
        double y = center.y - radius * sin(angle);
        vertices.push_back({x, y});
    }

    return polygon(vertices);
}

// A rectangle with sides orthogonal to the axes. This is a special case of
// polygon that we can handle especially efficiently.
class Rectangle : public Polygon
{
public:
    // Constructs a rectangle with the given top, right, bottom, and left
    // coordinates.
    Rectangle(double top, double right, double bottom, double left)
            : Polygon{{{left, top}, {right, top},
                       {right, bottom}, {left, bottom}}}
    { }

    // Constructs a rectangle with the given positions as opposing vertices.
    Rectangle(posn p, posn q)
            : Polygon{{{p.x, p.y}, {p.x, q.y}, {q.x, q.y}, {q.x, p.y}}}
    { }

    bool contains(posn) const override;
};

bool Rectangle::contains(posn point) const
{
    return get_bbox().contains(point);
}

picture_ptr rectangle(double top, double right, double bottom, double left)
{
    return std::make_shared<Rectangle>(top, right, bottom, left);
}

picture_ptr rectangle(Picture::posn v1, Picture::posn v2)
{
    return std::make_shared<Rectangle>(v1, v2);
}

// Applies an affine transformation (e.g., rotation, scaling, translation,
// shear) to a shape.
class Transform : public Picture_decorator
{
public:
    // Applies the given transformation to the base shape.
    Transform(picture_ptr base, const graphics::affinity& transform)
            : Picture_decorator{base, transform(bbox{&*base})}
            , inv_{transform.inverse()}
    { }

    bool contains(posn) const;
    color color_at(posn) const;

private:
    // Stores the inverse of the desired transformation. To transform a shape, we
    //  1) transform the bounding box, and
    //  2) apply the *inverse transformation* to positions before passing them to
    //     the shape.
    graphics::affinity inv_;
};

bool Transform::contains(posn point) const {
    return Picture_decorator::contains(inv_(point));
}

Picture::color Transform::color_at(posn point) const {
    return Picture_decorator::color_at(inv_(point));
}

picture_ptr transform(picture_ptr base, const graphics::affinity& transform)
{
    return std::make_shared<Transform>(base, transform);
}
