#include "linked_structures/linked_list.hpp"
#include <UnitTest++/UnitTest++.h>

#include <string>

namespace linked_list
{

using std::string;

namespace unique
{

TEST(CreateEmpty)
{
    list<string> lst {nullptr};
}

TEST(CreateNonEmpty)
{
    list<string> lst = cons(string{"hello"}, nullptr);
}

struct TwoLists {
    list<string> lst1 =
        cons(string{"zero"},
             cons(string{"one"},
                  cons(string{"two"}, cons(string{"three"}, nullptr))));

    list<string> lst2 =
        cons(string{"a"}, cons(string{"b"}, cons(string{"c"}, nullptr)));
};

TEST_FIXTURE(TwoLists, Length)
{
    CHECK_EQUAL(4u, length<string>(lst1));
    CHECK_EQUAL(3u, length<string>(lst2));
}

TEST_FIXTURE(TwoLists, NthTail)
{
    CHECK(nullptr != nth_tail<string>(3, lst1));
    CHECK(nullptr == nth_tail<string>(4, lst1));
}

TEST_FIXTURE(TwoLists, Nth)
{
    CHECK_EQUAL(string{"a"}, nth<string>(0, lst2));
    CHECK_EQUAL(string{"b"}, nth<string>(1, lst2));
    CHECK_EQUAL(string{"c"}, nth<string>(2, lst2));
}

TEST_FIXTURE(TwoLists, Append)
{
    list<string> lst = append<string>(lst1, move(lst2));

    CHECK_EQUAL(string{"zero"}, lst->first());
    CHECK_EQUAL(string{"one"}, lst->rest()->first());
    CHECK_EQUAL(string{"two"}, nth<string>(2, lst));
    CHECK_EQUAL(string{"three"}, nth<string>(3, lst));
    CHECK_EQUAL(string{"a"}, nth<string>(4, lst));
    CHECK_EQUAL(string{"b"}, nth<string>(5, lst));
    CHECK_EQUAL(string{"c"}, nth<string>(6, lst));
}

TEST_FIXTURE(TwoLists, Concat)
{
    concat<string>(lst1, move(lst2));
    CHECK_EQUAL(string{"zero"}, lst1->first());
    CHECK_EQUAL(string{"one"}, lst1->rest()->first());
    CHECK_EQUAL(string{"two"}, nth<string>(2, lst1));
    CHECK_EQUAL(string{"three"}, nth<string>(3, lst1));
    CHECK_EQUAL(string{"a"}, nth<string>(4, lst1));
    CHECK_EQUAL(string{"b"}, nth<string>(5, lst1));
    CHECK_EQUAL(string{"c"}, nth<string>(6, lst1));
}

}  // namespace unique

namespace shared
{

TEST(CreateEmpty)
{
    list<string> lst {nullptr};
}

TEST(CreateNonEmpty)
{
    list<string> lst = cons(string{"hello"}, nullptr);
}

struct TwoLists {
    list<string> lst1 =
        cons(string{"zero"},
             cons(string{"one"},
                  cons(string{"two"}, cons(string{"three"}, nullptr))));

    list<string> lst2 =
        cons(string{"a"}, cons(string{"b"}, cons(string{"c"}, nullptr)));
};

TEST_FIXTURE(TwoLists, Length)
{
    CHECK_EQUAL(4u, length<string>(lst1));
    CHECK_EQUAL(3u, length<string>(lst2));
}

TEST_FIXTURE(TwoLists, NthTail)
{
    CHECK(nullptr != nth_tail<string>(3, lst1));
    CHECK(nullptr == nth_tail<string>(4, lst1));
}

TEST_FIXTURE(TwoLists, Nth)
{
    CHECK_EQUAL(string{"a"}, nth<string>(0, lst2));
    CHECK_EQUAL(string{"b"}, nth<string>(1, lst2));
    CHECK_EQUAL(string{"c"}, nth<string>(2, lst2));
}

TEST_FIXTURE(TwoLists, Append)
{
    list<string> lst = append<string>(lst1, move(lst2));

    CHECK_EQUAL(string{"zero"}, lst->first());
    CHECK_EQUAL(string{"one"}, lst->rest()->first());
    CHECK_EQUAL(string{"two"}, nth<string>(2, lst));
    CHECK_EQUAL(string{"three"}, nth<string>(3, lst));
    CHECK_EQUAL(string{"a"}, nth<string>(4, lst));
    CHECK_EQUAL(string{"b"}, nth<string>(5, lst));
    CHECK_EQUAL(string{"c"}, nth<string>(6, lst));
}

TEST_FIXTURE(TwoLists, Concat)
{
    concat<string>(lst1, lst2);
    CHECK_EQUAL(string{"zero"}, lst1->first());
    CHECK_EQUAL(string{"one"}, lst1->rest()->first());
    CHECK_EQUAL(string{"two"}, nth<string>(2, lst1));
    CHECK_EQUAL(string{"three"}, nth<string>(3, lst1));
    CHECK_EQUAL(string{"a"}, nth<string>(4, lst1));
    CHECK_EQUAL(string{"b"}, nth<string>(5, lst1));
    CHECK_EQUAL(string{"c"}, nth<string>(6, lst1));

    CHECK_EQUAL(string{"a"}, nth<string>(0, lst2));
    CHECK_EQUAL(string{"b"}, nth<string>(1, lst2));
    CHECK_EQUAL(string{"c"}, nth<string>(2, lst2));
}

}  // namespace shared

}  // namespace list

int
main(int, const char* [])
{
    return UnitTest::RunAllTests();
}
