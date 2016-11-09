#include "UnitTest++/UnitTest++.h"
#include "../src-done/Vec_open_hash_done.h"

TEST(HASH1)
{
    Vec_open_hash<int> vh;
    vh.add("abc", 1);
    const auto & vhc=vh;
    CHECK_EQUAL(1, vhc.lookup("abc"));
}


TEST(HASH2)
{
    Vec_open_hash<int> vh(10);
    vh.add("abc", 1);
    vh.lookup("abc")=2;
    CHECK_EQUAL(2, vh.lookup("abc"));
}


TEST(HASH3)
{
    Vec_open_hash<int> vh(2);
    vh.add("abc", 1);
    vh.add("def", 2);
    CHECK_EQUAL(1, vh.lookup("abc"));
    CHECK_EQUAL(2, vh.lookup("def"));
}


TEST(HASH4)
{
    Vec_open_hash<int> vh(1);
    vh.add("abc", 1);
    vh.add("abc", 2);
    CHECK_EQUAL(2, vh.lookup("abc"));
}

TEST(Member)
{
    Vec_open_hash<int> vh(100);
    CHECK(!vh.member("a1"));
    CHECK(!vh.member("a2"));
    vh.add("a1", 1);
    CHECK(vh.member("a1"));
    CHECK(!vh.member("a2"));
    vh.add("a2", 2);
    CHECK(vh.member("a1"));
    CHECK(vh.member("a2"));
}
