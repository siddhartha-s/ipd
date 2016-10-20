#pragma once

#include "Picture.h"

#include <initializer_list>
#include <vector>

drawing_ptr background(const Picture::color& fill = Picture::color::white);

drawing_ptr circle(Picture::posn center, double radius);

drawing_ptr polygon(std::initializer_list<Picture::posn> vertices);
drawing_ptr polygon(const std::vector<Picture::posn>& vertices);

// Makes a regular polygon with the given center and number of sides, where
// `radius` is the distance from the center to each vertex.
drawing_ptr regular_polygon(Picture::posn center, double radius, size_t sides);

drawing_ptr rectangle(double top, double right, double bottom, double left);
drawing_ptr rectangle(Picture::posn v1, Picture::posn v2);

drawing_ptr overlay(drawing_ptr over, drawing_ptr under);
drawing_ptr overlay(std::initializer_list<drawing_ptr>);

drawing_ptr difference(drawing_ptr base, drawing_ptr mask);

drawing_ptr intersection(drawing_ptr base, drawing_ptr mask);

drawing_ptr fill(drawing_ptr, const Picture::color&);

drawing_ptr opacity(drawing_ptr, graphics::sample);

drawing_ptr nothing();

drawing_ptr transform(drawing_ptr, const graphics::affinity&);
