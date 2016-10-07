#include "Background.h"

Background::Background(color fill)
        : Drawing{bbox::everything()}, color_{fill}
{ }

bool Background::contains(posn) const
{
    return true;
}

Drawing::color Background::color_at(posn) const
{
    return color_;
}


drawing_ptr background(Drawing::color fill)
{
    return std::make_shared<Background>(fill);
}
