#include "posn.h"
#include <UnitTest++/UnitTest++.h>

#include <string>

using namespace ipd;

TEST(PosnOfInt)
{
    posn<int> p{3, 4};
}

TEST(PosnOfDouble)
{
    posn<double> p{3.0, 4.0};
}

TEST(PosnOfPosnOfString)
{
    posn<posn<std::string>> p{{"a", "b"}, {"c", "d"}};
}

TEST(DistanceInt)
{
    posn<int> p{0, 0};
    posn<int> q{3, 4};
    CHECK_EQUAL(5.0, distance(p, q));
}

TEST(DistanceDouble)
{
    posn<double> p{0, 0};
    posn<double> q{3, 4};
    CHECK_EQUAL(5.0, distance(p, q));
}

TEST(DistanceString)
{
    posn<std::string> p{"a", "b"};
    posn<std::string> q{"c", "d"};
    // doesn't type check:
    distance(p, q);
}
