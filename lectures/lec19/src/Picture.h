#pragma once

#include <graphics.h>

#include <memory>

// Abstract base class for renderable pictures. Derive from the class to
// create different kinds of pictures.
class Picture
{
public:
    using color = graphics::color;
    using posn  = graphics::posn<double>;
    using bbox  = graphics::bbox<double>;

    // Constructs a picture with the given bounding box.
    Picture(const bbox&);

    // Gets the bounding box of the picture.
    const bbox& get_bbox() const;

    // Returns whether the given position lies within the picture. Derived
    // classes must override this to specify their form.
    virtual bool contains(posn) const = 0;

    // Returns the color of the picture at the given position. By default,
    // this returns black for posns contained within the picture and
    // transparent otherwise.
    virtual color color_at(posn) const;

    // Classes with virtual functions need a virtual destructor.
    virtual ~Picture();

private:
    bbox bbox_;
};

using picture_ptr = std::shared_ptr<Picture>;
