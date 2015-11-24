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

    raster be{WIDTH, HEIGHT, color::WHITE};

    for (int x = 0; x < be.width(); ++x) {
        for (int y = 0; y < be.height(); ++y) {
            auto dist = sqrt(sqr(x - WIDTH/2) + sqr(y - HEIGHT/2));
            auto band = static_cast<int>(dist / THICKNESS) % 2;

            if (band == 1) {
                be[{x, y}] = fcolor{1, 0, 0};
            }
        }
    }

    be.write_out("bulls_eye.ppm");
}

TEST(Gradient)
{
    auto constexpr WIDTH = 400, HEIGHT = 400;

    auto constexpr WAVE_Y         = HEIGHT / 4.0;
    auto constexpr WAVE_MAGNITUDE = HEIGHT / 8.0;
    auto constexpr WAVELENGTH     = 20.0f / WIDTH;

    fcolor darkest{0, 0, 0.3f};
    fcolor dark{0, 0.1f, 0.4f};
    fcolor light{0, 0.5f, 0.8f};

    auto wave_line = [](double x) {
        return WAVE_Y + WAVE_MAGNITUDE * sin(WAVELENGTH * x);
    };

    raster be{WIDTH, HEIGHT};

    for (int x = 0; x < be.width(); ++x) {
        for (int y = 0; y < be.height(); ++y) {
            if (y > wave_line(x)) {
                be[{x,y}] = dark.interpolate(1.0 * x / WIDTH, light);
            } else {
                be[{x,y}] = darkest;
            }
        }
    }

    be.write_out("wave.ppm");
}

}  // namespace raster

int
main(int, const char* [])
{
    return UnitTest::RunAllTests();
}
