#include "posn.h"

#include <UnitTest++/UnitTest++.h>

TEST(DistanceInt)
{
    posn<int> p{2, 4}, q{6, 1};
    CHECK_EQUAL(5, distance(p, q));
}

TEST(DistanceDouble)
{
    posn<double> p{2, 4}, q{6, 1};
    CHECK_EQUAL(5, distance(p, q));
}
