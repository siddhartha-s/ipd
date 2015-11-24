#include "sampleable.hpp"

namespace graphics
{

void sample(const sampleable& scene, raster::raster& target)
{
    for (int x = 0; x < target.width(); ++x) {
        for (int y = 0; y < target.height(); ++y) {
            double xd = x, yd = y;
            target[{x, y}] = scene.color_at({xd, yd});
        }
    }
}

} // namespace graphics
