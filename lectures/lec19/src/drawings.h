#pragma once

#include "Drawing.h"

drawing_ptr background(const Drawing::color& fill = Drawing::color::white);

drawing_ptr circle(Drawing::posn center, double radius);

drawing_ptr difference(drawing_ptr base, drawing_ptr mask);

drawing_ptr fill(drawing_ptr, const Drawing::color&);
