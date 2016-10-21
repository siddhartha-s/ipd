#pragma once

// This file contains factory functions for constructing a variety of pictures.

#include "Picture.h"

#include <initializer_list>
#include <vector>

// A field of the given color filling all space.
picture_ptr background(const Picture::color& fill = Picture::color::white);

// A circle with the given center and radius.
picture_ptr circle(Picture::posn center, double radius);

// A polygon with the given vertices.
picture_ptr polygon(const std::vector<Picture::posn>& vertices);

// A regular polygon with the given center and number of sides, where
// `radius` is the distance from the center to each vertex.
picture_ptr regular_polygon(Picture::posn center, double radius, size_t sides);

// A rectangle with the given coordinates.
picture_ptr rectangle(double top, double right, double bottom, double left);

// A rectangle with the given opposite corners.
picture_ptr rectangle(Picture::posn v1, Picture::posn v2);

// Superimposes one picture over another.
picture_ptr overlay(picture_ptr over, picture_ptr under);

// Superimposes a sequence of pictures.
picture_ptr overlay(std::initializer_list<picture_ptr>);

// Set difference for pictures, removes points that are within the mask.
picture_ptr difference(picture_ptr base, picture_ptr mask);

// Set intersection for pictures, retains points that are within the mask.
picture_ptr intersection(picture_ptr base, picture_ptr mask);

// Fills the given picture with the given color.:w
picture_ptr fill(picture_ptr, const Picture::color&);

// Adjusts the transparency/opacity of a picture.
picture_ptr opacity(picture_ptr, graphics::sample);

// The empty picture.
picture_ptr nothing();

// Transforms a picture via an affine transformation.
picture_ptr transform(picture_ptr, const graphics::affinity&);
