#include "graphics/sampleable.hpp"
#include "raster/raster.hpp"
#include "raster/color.hpp"
#include "raster/geometry.hpp"

#include <UnitTest++/UnitTest++.h>
#include <cmath>

namespace graphics
{

using raster::fcolor;

auto constexpr WIDTH = 600, HEIGHT = 400;

static void render(sampleable::ptr img, const char* file)
{
    raster::raster output{WIDTH, HEIGHT};
    sample(*img, output);
    output.write_out(file);
}

TEST(SimpleRectangle)
{
    auto rect = rectangle({WIDTH / 3, HEIGHT / 3},
                          {2 * WIDTH / 3, 2 * HEIGHT / 3},
                          fcolor{1, 0, 0});
    render(rect, "rectangle.ppm");
}

TEST(SimpleCircle)
{
    auto circ = circle({WIDTH/2, HEIGHT/2}, 150, fcolor{0, .6, 0});
    render(circ, "circle.ppm");
}

TEST(SimpleOverlay)
{
    auto background = canvas({.5, .5, .5});
    auto circ1 = circle({200, 100}, 150, fcolor{0, 1, 0, 0.4});
    auto circ2 = circle({400, 100}, 150, fcolor{1, 0, 0, 0.4});
    auto circ3 = circle({300, 300}, 150, fcolor{0, 0, 1, 0.4});

    auto scene = overlay(circ1, overlay(circ2, overlay(circ3, background)));

    render(scene, "overlay.ppm");
}

TEST(Affinity)
{
    auto rect = rectangle({WIDTH / 3, HEIGHT / 3},
                          {2 * WIDTH / 3, 2 * HEIGHT / 3},
                          fcolor{0.6, 0, 0});
    auto trans = raster::affinity::rotation(.7);

    render(transform(trans, rect), "affinity.ppm");
}

}  // namespace graphics

int
main(int, const char* [])
{
    return UnitTest::RunAllTests();
}
