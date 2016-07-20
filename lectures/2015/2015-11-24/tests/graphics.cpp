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

}  // namespace graphics

int
main(int, const char* [])
{
    return UnitTest::RunAllTests();
}
