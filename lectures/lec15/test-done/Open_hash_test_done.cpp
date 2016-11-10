#include "UnitTest++/UnitTest++.h"
#include "../src-done/Open_hash_done.h"

TEST(HASH1)
{
    Open_hash<int> vh;
    vh.add("abc", 1);
    const auto & vhc=vh;
    CHECK_EQUAL(1, vhc.lookup("abc"));
}


TEST(HASH2)
{
    Open_hash<int> vh(10);
    vh.add("abc", 1);
    vh.lookup("abc")=2;
    CHECK_EQUAL(2, vh.lookup("abc"));
}


TEST(HASH3)
{
    Open_hash<int> vh(2);
    vh.add("abc", 1);
    vh.add("def", 2);
    CHECK_EQUAL(1, vh.lookup("abc"));
    CHECK_EQUAL(2, vh.lookup("def"));
}


TEST(HASH4)
{
    Open_hash<int> vh(1);
    vh.add("abc", 1);
    vh.add("abc", 2);
    CHECK_EQUAL(2, vh.lookup("abc"));
}

TEST(Member)
{
    Open_hash<int> vh(100);
    CHECK(!vh.member("a1"));
    CHECK(!vh.member("a2"));
    vh.add("a1", 1);
    CHECK(vh.member("a1"));
    CHECK(!vh.member("a2"));
    vh.add("a2", 2);
    CHECK(vh.member("a1"));
    CHECK(vh.member("a2"));
}

TEST(HASH5)
{
    Open_hash<int> vh(2);
    vh.add("abc", 1);
    vh.add("def", 2);
    vh.add("ghi", 3);
    CHECK_EQUAL(1, vh.lookup("abc"));
    CHECK_EQUAL(2, vh.lookup("def"));
    CHECK_EQUAL(3, vh.lookup("ghi"));
}

TEST(GrowFromEmpty)
{
    Open_hash<int> vh(0);
    CHECK_EQUAL(0, vh.size());
    CHECK_EQUAL(0, vh.table_size());
    vh.add("abc", 1);
    CHECK_EQUAL(1, vh.size());
    CHECK_EQUAL(1, vh.lookup("abc"));
    CHECK(!vh.member("def"));
    vh.add("def", 2);
    CHECK_EQUAL(2, vh.size());
    CHECK_EQUAL(1, vh.lookup("abc"));
    CHECK_EQUAL(2, vh.lookup("def"));
}

TEST(LookupThrows)
{
    Open_hash<int> vh;

    CHECK_THROW(vh.lookup("abc"), Not_found);
    vh.add("abc", 1);
    CHECK_EQUAL(1, vh.lookup("abc"));
    CHECK_THROW(vh.lookup("def"), Not_found);
}