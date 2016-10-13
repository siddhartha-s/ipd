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

