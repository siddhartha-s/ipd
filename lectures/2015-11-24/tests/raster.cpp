#include "raster/raster.hpp"
#include "raster/color.hpp"

#include <UnitTest++/UnitTest++.h>
#include <cmath>

namespace raster
{

TEST(RedSquare)
{
    raster red_square{400, 400, fcolor{1, 0, 0}};

    red_square.write_out("red_square.ppm");
}

TEST(PurpleSquare)
{
    raster red_square{400, 400, fcolor{.6f, 0, .4f}};

    red_square.write_out("purple_square.ppm");
}

TEST(BullsEye)
{
    int constexpr WIDTH = 400, HEIGHT = 400, THICKNESS = 20;
    auto sqr = [](double x) { return x * x; };

    raster bs{400, 400, color::WHITE};

    for (int x = 0; x < bs.width(); ++x) {
        for (int y = 0; y < bs.height(); ++y) {
            auto dist = sqrt(sqr(x - WIDTH/2) + sqr(y - HEIGHT/2));
            auto band = static_cast<int>(dist / THICKNESS) % 2;

            if (band == 1) {
                bs[{x, y}] = fcolor{1, 0, 0};
            }
        }
    }

    bs.write_out("bulls_eye.ppm");
}

}  // namespace raster

int
main(int, const char* [])
{
    return UnitTest::RunAllTests();
}
