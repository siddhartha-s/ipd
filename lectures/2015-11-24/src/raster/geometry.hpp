#pragma once

#include <algorithm>

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
class bounding_box
{
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
