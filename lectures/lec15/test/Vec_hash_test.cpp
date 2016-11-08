#include <Various_hashes.h>
#include "UnitTest++/UnitTest++.h"
#include "Vec_hash.h"

TEST(HASH1)
{
    Vec_hash<int> vh;
    vh.add("abc", 1);
    const auto & vhc=vh;
    CHECK_EQUAL(1, vhc.lookup("abc"));
}


TEST(HASH2)
{
    Vec_hash<int> vh(10);
    vh.add("abc", 1);
    vh.lookup("abc")=2;
    CHECK_EQUAL(2, vh.lookup("abc"));
}


TEST(HASH3)
{
    Vec_hash<int> vh(1);
    vh.add("abc", 1);
    vh.add("def", 2);
    CHECK_EQUAL(1, vh.lookup("abc"));
    CHECK_EQUAL(2, vh.lookup("def"));
}


TEST(HASH4)
{
    Vec_hash<int> vh(1);
    vh.add("abc", 1);
    vh.add("abc", 2);
    CHECK_EQUAL(2, vh.lookup("abc"));
}

TEST(ID_HASH)
{
    Identity_hash<int> ih;
    std::string s(8, 0);
    for(int i=0; i<8; i++) s[i]=255;
    CHECK_EQUAL((size_t)-1,ih.hash(s));
    for(int i=0; i<8; i++) s[i]=i;
    CHECK_EQUAL(506097522914230528,ih.hash(s));
}

TEST(Member)
{
    Vec_hash<int> vh(100);
    CHECK(!vh.member("a1"));
    CHECK(!vh.member("a2"));
    vh.add("a1", 1);
    CHECK(vh.member("a1"));
    CHECK(!vh.member("a2"));
    vh.add("a2", 2);
    CHECK(vh.member("a1"));
    CHECK(vh.member("a2"));
}
