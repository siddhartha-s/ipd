#include "functions.h"
#include <UnitTest++/UnitTest++.h>

TEST(Freezing)
{
    CHECK_EQUAL(0, f2c(32));
}

TEST(Boiling)
{
    CHECK_EQUAL(100, f2c(212));
}

TEST(Same)
{
    CHECK_EQUAL(-40, f2c(-40));
}

TEST(FreezingC2F)
{
    CHECK_EQUAL(32, c2f(0));
}

TEST(BoilingC2F)
{
    CHECK_EQUAL(212, c2f(100));
}

TEST(SameC2F)
{
    CHECK_EQUAL(-40, c2f(-40));
}

TEST(FiveByTen)
{
    CHECK_EQUAL(50, rectangle_area(5, 10));
}
