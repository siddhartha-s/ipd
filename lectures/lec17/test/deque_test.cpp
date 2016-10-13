#include "deque.h"

#include <UnitTest++/UnitTest++.h>

using namespace ipd;

TEST(New_is_empty)
{
    deque<int> dq;
    CHECK(dq.empty());
}

TEST(New_has_size_0)
{
    deque<int> dq;
    CHECK_EQUAL(0, dq.size());
}

TEST(Push_front_changes_size)
{
    deque<int> dq;
    dq.push_front(5);
    CHECK_EQUAL(1, dq.size());
    CHECK(!dq.empty());
    dq.push_front(6);
    CHECK_EQUAL(2, dq.size());
    CHECK(!dq.empty());
}

TEST(Push_back_changes_size)
{
    deque<int> dq;
    dq.push_back(5);
    CHECK_EQUAL(1, dq.size());
    CHECK(!dq.empty());
    dq.push_back(6);
    CHECK_EQUAL(2, dq.size());
    CHECK(!dq.empty());
}

TEST(Push_front_changes_front)
{
    deque<int> dq;
    dq.push_front(5);
    CHECK_EQUAL(5, dq.front());
    dq.push_front(6);
    CHECK_EQUAL(6, dq.front());
}

TEST(Push_back_changes_back)
{
    deque<int> dq;
    dq.push_back(5);
    CHECK_EQUAL(5, dq.back());
    dq.push_back(6);
    CHECK_EQUAL(6, dq.back());
}

TEST(Pop_front_removes_front)
{
    deque<int> dq;
    dq.push_back(5);
    dq.push_back(6);
    dq.push_back(7);
    CHECK_EQUAL(5, dq.front());
    dq.pop_front();
    CHECK_EQUAL(6, dq.front());
    dq.pop_front();
    CHECK_EQUAL(7, dq.front());
    dq.pop_front();
    CHECK(dq.empty());
}

TEST(Pop_back_removes_back)
{
    deque<int> dq;
    dq.push_front(5);
    dq.push_front(6);
    dq.push_front(7);
    CHECK_EQUAL(5, dq.back());
    dq.pop_back();
    CHECK_EQUAL(6, dq.back());
    dq.pop_back();
    CHECK_EQUAL(7, dq.back());
    dq.pop_back();
    CHECK(dq.empty());
}

TEST(Can_push_after_pop)
{
    deque<int> dq;
    dq.push_back(5);
    dq.push_back(6);
    dq.push_back(7);
    dq.pop_front();
    dq.push_back(8);
    CHECK_EQUAL(6, dq.front());
    CHECK_EQUAL(8, dq.back());
    dq.pop_front();
    CHECK_EQUAL(7, dq.front());
    CHECK_EQUAL(8, dq.back());
    dq.pop_front();
    CHECK_EQUAL(8, dq.front());
    CHECK_EQUAL(8, dq.back());
    dq.pop_front();
    CHECK(dq.empty());
    dq.push_back(9);
    dq.push_back(10);
    CHECK_EQUAL(9, dq.front());
    CHECK_EQUAL(10, dq.back());
}

TEST(Copy)
{
    deque<int> dq1;
    dq1.push_back(5);
    dq1.push_back(6);

    deque<int> dq2(dq1);
    CHECK_EQUAL(5, dq2.front());
    CHECK_EQUAL(6, dq2.back());

    dq2.push_back(7);
    CHECK_EQUAL(5, dq2.front());
    CHECK_EQUAL(7, dq2.back());
    CHECK_EQUAL(5, dq1.front());
    CHECK_EQUAL(6, dq1.back());
}

TEST(Assign)
{
    deque<int> dq1;
    dq1.push_back(5);
    dq1.push_back(6);

    deque<int> dq2;
    dq2.push_back(10);

    dq2 = dq1;
    CHECK_EQUAL(5, dq2.front());
    CHECK_EQUAL(6, dq2.back());

    dq2.push_back(7);
    CHECK_EQUAL(5, dq2.front());
    CHECK_EQUAL(7, dq2.back());
    CHECK_EQUAL(5, dq1.front());
    CHECK_EQUAL(6, dq1.back());
}
