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

class Intersection : public Drawing
{
public:
    Intersection(drawing_ptr base, drawing_ptr mask);
    bool contains(posn) const override;
    color color_at(posn) const override;

private:
    drawing_ptr base_;
    drawing_ptr mask_;
};

Intersection::Intersection(drawing_ptr base, drawing_ptr mask)
        : Drawing{base->get_bbox() * mask->get_bbox()}
        , base_{base}
        , mask_{mask}
{ }

bool Intersection::contains(posn point) const
{
    return mask_->contains(point) && base_->contains(point);
}

Drawing::color Intersection::color_at(posn point) const
{
    if (mask_->contains(point))
        return base_->color_at(point);
    else
        return color::transparent;
}

drawing_ptr intersection(drawing_ptr base, drawing_ptr mask)
{
    return std::make_shared<Intersection>(base, mask);
}

// The empty drawing
class Nothing : public Drawing
{
public:
    Nothing();
    bool contains(posn) const override;
};

Nothing::Nothing()
        : Drawing(bbox::nothing())
{ }

bool Nothing::contains(posn) const
{
    return false;
}

drawing_ptr nothing()
{
    return std::make_shared<Nothing>();
}

// Adapts a shape to alter its opacity/transparency.
class Opacity : public Drawing_decorator
{
public:
    Opacity(drawing_ptr, graphics::sample);
    color color_at(posn) const override;

private:
    graphics::sample opacity_;
};

Opacity::Opacity(drawing_ptr base, graphics::sample opacity)
        : Drawing_decorator{base}
        , opacity_{opacity}
{ }

Drawing::color Opacity::color_at(posn point) const
{
    auto base_color = Drawing_decorator::color_at(point);
    return color{base_color.red(),
                 base_color.green(),
                 base_color.blue(),
                 opacity_ * base_color.alpha()};
}

drawing_ptr opacity(drawing_ptr base, graphics::sample opacity)
{
    return std::make_shared<Opacity>(base, opacity);
}

// Composes two shapes by overlaying one over the other.
class Overlay : public Drawing
{
public:
    // Places `over` over `under`.
    Overlay(drawing_ptr over, drawing_ptr under);

    bool contains(posn) const override;
    color color_at(posn) const override;

private:
    const drawing_ptr over_;
    const drawing_ptr under_;
};

Overlay::Overlay(drawing_ptr over, drawing_ptr under)
        : Drawing{{&*over, &*under}}
        , over_{over}
        , under_{under}
{ }

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

drawing_ptr overlay(drawing_ptr over, drawing_ptr under)
{
    return std::make_shared<Overlay>(over, under);
}

drawing_ptr overlay(std::initializer_list<drawing_ptr> layers)
{
    drawing_ptr result = nothing();

    for (auto layer : layers)
        result = overlay(result, layer);

    return result;
}

// A polygon, defined as a sequence of vertex positions.
class Polygon : public Drawing
{
public:
    // Constructs a polygon from a sequence of vertices (e.g., a vector).
    Polygon(const std::vector<posn>&);

    bool contains(posn p) const override;

protected:
    // Provides derived classes access to the sequence of vertices.
    const std::vector<posn>& get_vertices() const;

private:
    std::vector<posn> vertices_;
};

Polygon::Polygon(const std::vector<posn>& sequence)
        : Drawing{bbox(sequence)}
        , vertices_{sequence}
{ }

const std::vector<Drawing::posn>& Polygon::get_vertices() const
{
    return vertices_;
}

bool has_crossing(Drawing::posn previous,
                  Drawing::posn p,
                  Drawing::posn current)
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

drawing_ptr polygon(const std::vector<Drawing::posn>& sequence)
{
    return std::make_shared<Polygon>(sequence);
}

drawing_ptr polygon(std::initializer_list<Drawing::posn> vertices)
{
    return std::make_shared<Polygon>(vertices);
}

drawing_ptr regular_polygon(Polygon::posn center, double radius, size_t sides)
{
    std::vector<Drawing::posn> vertices;

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

drawing_ptr rectangle(double top, double right, double bottom, double left)
{
    return std::make_shared<Rectangle>(top, right, bottom, left);
}

drawing_ptr rectangle(Drawing::posn v1, Drawing::posn v2)
{
    return std::make_shared<Rectangle>(v1, v2);
}

// Applies an affine transformation (e.g., rotation, scaling, translation,
// shear) to a shape.
class Transform : public Drawing_decorator
{
public:
    // Applies the given transformation to the base shape.
    Transform(drawing_ptr, const graphics::affinity& transform);

    bool contains(posn) const;
    color color_at(posn) const;

private:
    // Stores the inverse of the desired transformation. See implementation
    // for explanation.
    graphics::affinity inv_;
};

// To transform a shape, we
//  1) transform the bounding box, and
//  2) apply the *inverse transformation* to positions before passing them to
//     the shape.

Transform::Transform(drawing_ptr base, const graphics::affinity& transform)
        : Drawing_decorator{base, transform(bbox{&*base})}
        , inv_{transform.inverse()}
{ }

bool Transform::contains(posn point) const {
    return Drawing_decorator::contains(inv_(point));
}

Drawing::color Transform::color_at(posn point) const {
    return Drawing_decorator::color_at(inv_(point));
}

drawing_ptr transform(drawing_ptr base, const graphics::affinity& transform)
{
    return std::make_shared<Transform>(base, transform);
}
