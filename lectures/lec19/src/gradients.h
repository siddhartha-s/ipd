#pragma once

#include "Picture_decorator.h"

#include <cmath>
#include <memory>

// A map from [0, 1] -> [0, 1] (the unit interval) specifying how quickly the
// gradient changes and in what direction.
struct Modulator
{
    using sample = graphics::sample;
    virtual sample modulate(sample) const = 0;
};

using modulator_ptr = std::shared_ptr<Modulator>;

modulator_ptr linear_modulator();
modulator_ptr sinusoidal_modulator(double frequency = 1, double phase = 0);

// A map from [0, 1] x [0, 1] -> [0, 1], specifying how to collapse the
// 2-D coordinates to a 1-D gradient.
struct Projector
{
    using sample = graphics::sample;
    virtual sample project(sample x, sample y) const = 0;
};

using projector_ptr = std::shared_ptr<Projector>;

projector_ptr horizontal_projector();
projector_ptr vertical_projector();
projector_ptr circular_projector();

picture_ptr gradient(picture_ptr,
                     Picture::color start, Picture::color end,
                     projector_ptr = horizontal_projector(),
                     modulator_ptr = linear_modulator());
