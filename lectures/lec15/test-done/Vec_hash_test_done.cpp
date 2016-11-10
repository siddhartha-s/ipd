#include "../src-done/Various_hashes_done.h"
#include "UnitTest++/UnitTest++.h"
#include "../src-done/Vec_hash_done.h"

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

TEST(EIGHT_BYTES_HASH)
{
    Eight_bytes<int> eh;
    CHECK_EQUAL(0, eh.hash(""));
    CHECK_EQUAL(eh.hash(""), eh.hash("1"));
    CHECK_EQUAL(eh.hash("abcdefgh"),eh.hash("abcdefghi"));
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
