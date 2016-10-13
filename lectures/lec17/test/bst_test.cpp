#include "Bst.h"
#include <UnitTest++/UnitTest++.h>

using namespace ipd;

TEST(New_is_empty_and_size_0)
{
    Bst<std::string> t;
    CHECK(t.empty());
    CHECK_EQUAL(0, t.size());
}

TEST(Insert_increases_size)
{
    Bst<std::string> t;

    t.insert("hello");
    CHECK(!t.empty());
    CHECK_EQUAL(1, t.size());

    t.insert("world");
    CHECK_EQUAL(2, t.size());
}

TEST(Contains_after_insert)
{
    Bst<std::string> t;

    t.insert("hello");
    t.insert("world");

    CHECK(t.contains("hello"));
    CHECK(t.contains("world"));
    CHECK(!t.contains("other"));
}

TEST(More_inserts)
{
    Bst<int> t{4, 2, 6, 1, 3, 5, 7};

    CHECK(!t.contains(0));
    CHECK(!t.contains(8));
    for (int i = 1; i <= 7; ++i)
        CHECK(t.contains(i));
}

TEST(Insert_then_delete)
{
    Bst<int> t{4, 2, 6, 1, 3, 5, 7};

    CHECK_EQUAL(7, t.size());
    t.remove(4);
    CHECK_EQUAL(6, t.size());
    t.remove(4);
    CHECK_EQUAL(6, t.size());

    CHECK(!t.contains(0));
    CHECK(t.contains(1));
    CHECK(t.contains(2));
    CHECK(t.contains(3));
    CHECK(!t.contains(4));
    CHECK(t.contains(5));
    CHECK(t.contains(6));
    CHECK(t.contains(7));
    CHECK(!t.contains(8));
}

TEST(Insert_then_delete_all)
{
    Bst<int> t{4, 2, 6, 1, 3, 5, 7};
    for (int i = 1; i <= 7; ++i)
        t.remove(i);
    CHECK(t.empty());
}