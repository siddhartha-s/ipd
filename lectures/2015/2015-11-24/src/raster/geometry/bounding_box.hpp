#pragma once

#include "posn.hpp"

#include <algorithm>
#include <exception>

namespace raster
{

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

    constexpr bounding_box(const T& top, const T& right, const T& bottom,
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
    constexpr bounding_box(const Boundable& b)
        : bounding_box{b.get_bounding_box()}
    { }

    template <typename Boundable1, typename Boundable2, typename... Boundables>
    constexpr bounding_box(const Boundable1& b1, const Boundable2& b2,
                           const Boundables&... rest)
        : bounding_box{b1.get_bounding_box() + b2.get_bounding_box(), rest...}
    { }

    constexpr const T& top()    const noexcept { return top_; }
    constexpr const T& right()  const noexcept { return right_; }
    constexpr const T& bottom() const noexcept { return bottom_; }
    constexpr const T& left()   const noexcept { return left_; }

    constexpr posn<T> top_left()     const noexcept { return {left(), top()}; }
    constexpr posn<T> top_right()    const noexcept { return {right(), top()}; }
    constexpr posn<T> bottom_left()  const noexcept { return {left(), bottom()}; }
    constexpr posn<T> bottom_right() const noexcept { return {right(), bottom()}; }

    constexpr T height() const noexcept { return bottom() - top(); }
    constexpr T width()  const noexcept { return right() - left(); }

    constexpr bool contains(const posn<T>& p) const noexcept
    {
        return p.x >= left() && p.x < right() && p.y >= top() && p.y < bottom();
    }

    constexpr inline const bounding_box& get_bounding_box() const
    {
        return *this;
    }

private:
    T top_, right_, bottom_, left_;
};

// Union
template <typename T>
constexpr bounding_box<T>
operator+(const bounding_box<T>& bb1, const bounding_box<T>& bb2)
{
    return {std::min(bb1.top(), bb2.top()),
            std::max(bb1.right(), bb2.right()),
            std::max(bb1.bottom(), bb2.bottom()),
            std::min(bb1.left(), bb2.left())};
}

// Intersection
template <typename T>
constexpr bounding_box<T>
operator*(const bounding_box<T>& bb1, const bounding_box<T>& bb2)
{
    return {std::max(bb1.top(), bb2.top()),
            std::min(bb1.right(), bb2.right()),
            std::min(bb1.bottom(), bb2.bottom()),
            std::max(bb1.left(), bb2.left())};
}

} // namespace raster
