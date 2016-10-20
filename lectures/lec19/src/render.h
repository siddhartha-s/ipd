#pragma once

#include "graphics.h"
#include "Picture.h"

// Renders a shape on a raster by asking the shape what color each pixel
// should be.
void simple_render(const Picture&, graphics::raster&);

// Like simple_render, but smooths by sampling multiple (antialias^2) times
// for each pixel and averaging.
void render(const Picture&, graphics::raster&, size_t antialias = 3);

