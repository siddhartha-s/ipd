#include "posn.h"
#include <UnitTest++/UnitTest++.h>

TEST(ThreeFourFive)
{
    posn p{1, 2};
    posn q{4, 6};

    CHECK_EQUAL(5, distance(p, q));
}

TEST(FiveTwelveThirteen)
{
    posn p{-6, -8};
    posn q{-11, 4};

    CHECK_EQUAL(13, distance(p, q));

    q.x = -1;
    CHECK_EQUAL(13, distance(p, q));
}
