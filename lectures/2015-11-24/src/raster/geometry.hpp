#pragma once

#include <algorithm>
#include <cmath>

namespace raster
{

template <typename T>
struct point
{
    T x, y;
};

template <typename T>
point<T> operator-(const point<T>& p1, const point<T>& p2)
{
    return { p1.x - p2.x, p1.y - p2.y };
}

template <typename T>
point<T> operator+(const point<T>& p1, const point<T>& p2)
{
    return { p1.x + p2.x, p1.y + p2.y };
}

template <typename T>
T interpolate(const T& t1, double weight, const T& t2)
{
    return static_cast<T>(weight * static_cast<double>(t1) +
                          (1 - weight) * static_cast<double>(t2));
}

template <typename T>
point<T> interpolate(const point<T>& p1, double weight, const point<T>& p2)
{
    return {interpolate(p1.x, weight, p2.x), interpolate(p1.y, weight, p2.y)};
}

template <typename T>
double distance(const point<T>& p1, const point<T>& p2)
{
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

template <typename T>
class bounding_box
{
public:
    bounding_box(const point<T>& p1, const point<T>& p2) noexcept
    {
        upper_left_  = { std::min(p1.x, p2.x), std::min(p1.y, p2.y) };
        lower_right_ = { std::max(p1.x, p2.x), std::max(p1.y, p2.y) };
    }

    bounding_box(const T& top, const T& right, const T& bottom,
                 const T& left) noexcept
        : bounding_box{{left, top}, {right, bottom}}
    {}

    const T& top() const noexcept { return upper_left_.y; }
    const T& bottom() const noexcept { return lower_right_.y; }
    const T& left() const noexcept { return upper_left_.x; }
    const T& right() const noexcept { return lower_right_.x; }

    T height() const noexcept { return bottom() - top(); }
    T width() const noexcept { return right() - left(); }

    bool within(const point<T>& p) const noexcept
    {
        return p.x >= left() && p.x < right() && p.y >= top() && p.y < bottom();
    }

private:
    point<T> upper_left_, lower_right_;
};

template <typename T>
class boundable
{
public:
    virtual bounding_box<T> get_bounding_box() const = 0;
    virtual ~boundable() {};
};

} // namespace raster
