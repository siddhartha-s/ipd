#pragma once

#include <algorithm>
#include <cmath>
#include <exception>

namespace raster
{

// forward declaration
template <typename T>
class bounding_box;

template <typename T>
struct posn
{
    T x, y;

    bounding_box<T> get_bounding_box() const noexcept;
};

template <typename T>
posn<T> operator+(const posn<T>& p1, const posn<T>& p2)
    noexcept(noexcept(p1.x + p2.x))
{
    return { p1.x + p2.x, p1.y + p2.y };
}

template <typename T>
posn<T> operator-(const posn<T>& p1, const posn<T>& p2)
    noexcept(noexcept(p1.x - p2.x))
{
    return { p1.x - p2.x, p1.y - p2.y };
}

template <typename T>
posn<T> operator*(const T& c, const posn<T>& p)
    noexcept(noexcept(c * p.x))
{
    return { c * p.x, c * p.y };
}

template <typename T>
T interpolate(const T& t1, double weight, const T& t2)
{
    return static_cast<T>((1 - weight) * static_cast<double>(t1) +
                          weight * static_cast<double>(t2));
}

template <typename T>
posn<T> interpolate(const posn<T>& p1, double weight, const posn<T>& p2)
{
    return {interpolate(p1.x, weight, p2.x), interpolate(p1.y, weight, p2.y)};
}

template <typename T>
double magnitude(const posn<T>& p)
{
    auto x = static_cast<double>(p.x);
    auto y = static_cast<double>(p.y);
    return sqrt(x * x + y * y);
}

template <typename T>
double distance(const posn<T>& p1, const posn<T>& p2)
{
    return magnitude(p1 - p2);
}

template <typename T>
class bounding_box
{
public:
    class invalid_exception : public std::exception
    {
        virtual const char* what() const throw() override
        {
            return "Invalid bounding box (type does not have infinity)";
        }
    };

    bounding_box(const T& top, const T& right, const T& bottom,
                 const T& left) noexcept
    {
        if (top > bottom || left > right) {
            if (std::numeric_limits<T>::has_infinity) {
                top_    = left_   = std::numeric_limits<T>::infinity();
                bottom_ = right_  = -top_;
            } else {
                throw invalid_exception{};
            }
        } else {
            top_    = top;
            right_  = right;
            bottom_ = bottom;
            left_   = left;
        }
    }

    template <typename Boundable>
    bounding_box(const Boundable& b)
        : bounding_box{b.get_bounding_box()}
    { }

    template <typename Boundable1, typename Boundable2, typename... Boundables>
    bounding_box(const Boundable1& b1, const Boundable2& b2,
                 const Boundables&... rest)
        : bounding_box{b1.get_bounding_box() + b2.get_bounding_box(), rest...}
    { }

    const T& top()    const noexcept { return top_; }
    const T& right()  const noexcept { return right_; }
    const T& bottom() const noexcept { return bottom_; }
    const T& left()   const noexcept { return left_; }

    T height() const noexcept { return bottom() - top(); }
    T width() const noexcept { return right() - left(); }

    bool contains(const posn<T>& p) const noexcept
    {
        return p.x >= left() && p.x < right() && p.y >= top() && p.y < bottom();
    }

    inline const bounding_box& get_bounding_box() const
    {
        return *this;
    }

private:
    T top_, right_, bottom_, left_;
};

// Union
template <typename T>
bounding_box<T>
operator+(const bounding_box<T>& bb1, const bounding_box<T>& bb2)
{
    return {std::min(bb1.top(), bb2.top()),
            std::max(bb1.right(), bb2.right()),
            std::max(bb1.bottom(), bb2.bottom()),
            std::min(bb1.left(), bb2.left())};
}

// Intersection
template <typename T>
bounding_box<T>
operator*(const bounding_box<T>& bb1, const bounding_box<T>& bb2)
{
    return {std::max(bb1.top(), bb2.top()),
            std::min(bb1.right(), bb2.right()),
            std::min(bb1.bottom(), bb2.bottom()),
            std::max(bb1.left(), bb2.left())};
}

template <typename T>
class boundable
{
public:
    using bbox_t = bounding_box<T>;

    inline const bbox_t& get_bounding_box() const
    {
        return get_bounding_box_();
    }

protected:
    ~boundable() {};

private:
    virtual const bbox_t& get_bounding_box_() const = 0;
};

template <typename T>
bounding_box<T> posn<T>::get_bounding_box() const noexcept
{
    return { y, x, y, x };
}

} // namespace raster
