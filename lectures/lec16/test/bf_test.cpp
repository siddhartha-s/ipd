#include "Bloom_filter.h"
#include <UnitTest++/UnitTest++.h>

TEST(Empty)
{
    Bloom_filter filt(100, 3);
    CHECK(!filt.check("Hello, world"));
}

TEST(InsertCheck)
{
    Bloom_filter filt(100, 3);

    filt.insert("Hello, world");
    filt.insert("Hello, world.");

    CHECK(filt.check("Hello, world"));
    CHECK(filt.check("Hello, world."));
    CHECK(! filt.check("hello, world."));
}