#pragma once

#include "posn.hpp"

namespace raster
{

class affine_transformation
{
    constexpr affine_transformation(double a, double b, double dx,
                                    double c, double d, double dy) noexcept;

    static constexpr affine_transformation translation(double dx, double dy) noexcept;

    constexpr affine_transformation inverse() const noexcept;

    constexpr posn<double>
    operator()(posn<double>) const noexcept;

    constexpr bounding_box<double>
    operator()(const bounding_box<double>&) const noexcept;

    constexpr affine_transformation
    operator()(const affine_transformation&) const noexcept;

    template <typename T>
    T operator()(T t) const noexcept(noexcept(t.transform(*this)))
    {
        return t.transform(*this);
    }

    template <typename T>
    constexpr inline T apply(const T& obj) const noexcept
    {
        return operator()(obj);
    }

private:
    double a_, b_, dx_,
           c_, d_, dy_;
        //  0   0   1
};

constexpr affine_transformation::affine_transformation(double a, double b, double dx,
                                                       double c, double d, double dy)
    noexcept
        : a_{a}, b_{b}, dx_{dx}, c_{c}, d_{d}, dy_{dy}
{ }

constexpr affine_transformation
affine_transformation::translation(double dx, double dy) noexcept
{
    return affine_transformation{ 1.0, 0.0, dx, 0.0, 1.0, dy };
}

constexpr affine_transformation affine_transformation::inverse() const noexcept
{
    auto det = a_ * d_ - b_ * c_;

    auto a =  d_ / det;
    auto b = -b_ / det;
    auto c = -c_ / det;
    auto d =  a_ / det;

    return affine_transformation{a, b, -(a * dx_ + b * dy_),
                                 c, d, -(c * dx_ + d * dy_)};
}

constexpr posn<double>
affine_transformation::operator()(posn<double> point) const noexcept
{
    return {a_ * point.x + b_ * point.y + dx_, c_ * point.x + d_ * point.y + dy_};
}

constexpr bounding_box<double>
affine_transformation::operator()(const bounding_box<double>& bb) const noexcept
{
    return bounding_box<double>{apply(bb.top_left()),
                                apply(bb.top_right()),
                                apply(bb.bottom_left()),
                                apply(bb.bottom_right())};
}

constexpr affine_transformation
affine_transformation::operator()(const affine_transformation& t) const noexcept
{
    return affine_transformation{a_ * t.a_  + b_ * t.c_,
                                 a_ * t.b_  + b_ * t.d_,
                                 a_ * t.dx_ + b_ * t.dy_ + dx_,
                                 c_ * t.a_  + d_ * t.c_,
                                 c_ * t.b_  + d_ * t.d_,
                                 c_ * t.dx_ + d_ * t.dy_ + dy_};
}

} // namespace raster
