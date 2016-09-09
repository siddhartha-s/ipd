#include "vectors.h"
#include <UnitTest++/UnitTest++.h>

TEST(Sum0)
{
    std::vector<double> v;
    CHECK_EQUAL(0, sum(v));
}

TEST(Sum10)
{
    std::vector<double> v{1, 2, 3, 4};
    CHECK_EQUAL(10, sum(v));
}

TEST(Mean)
{
    std::vector<double> v{5, 10, 18};
    CHECK_EQUAL(11, mean(v));
}

TEST(Renorm0)
{
    std::vector<double> v{2, 8};
    renorm0(v);

    CHECK_EQUAL(2, v[0]);
    CHECK_EQUAL(8, v[1]);
}

TEST(Renorm1Oops)
{
    std::vector<double> v{2, 8};
    renorm1(v);

    CHECK_EQUAL(2, v[0]);
    CHECK_EQUAL(8, v[1]);
}

TEST(Renorm1)
{
    std::vector<double> v{2, 8};
    v = renorm1(v);

    CHECK_EQUAL(-3, v[0]);
    CHECK_EQUAL(3, v[1]);
}

TEST(Renorm2)
{
    std::vector<double> v{2, 8};
    renorm2(v);

    CHECK_EQUAL(-3, v[0]);
    CHECK_EQUAL(3, v[1]);
}
