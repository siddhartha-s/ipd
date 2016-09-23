#include "My_vec.h"

#include <UnitTest++/UnitTest++.h>

TEST(Empty)
{
    My_vec<int> v;
    CHECK(v.empty());
}

TEST(Size0)
{
    My_vec<int> v;
    CHECK_EQUAL(0, v.size());
}

TEST(Push)
{
    My_vec<int> v;

    v.push_back(0);
    v.push_back(1);
    v.push_back(2);
}

TEST(PushEmptySize)
{
    My_vec<int> v;

    v.push_back(0);
    v.push_back(1);
    v.push_back(2);
    CHECK(!v.empty());
    CHECK_EQUAL(3, v.size());
}

TEST(PushIndex)
{
    My_vec<int> v;

    v.push_back(0);
    v.push_back(1);
    v.push_back(2);
    CHECK_EQUAL(0, v[0]);
    CHECK_EQUAL(1, v[1]);
    CHECK_EQUAL(2, v[2]);
}

TEST(Init10Size)
{
    My_vec<int> v(10);
    CHECK_EQUAL(10, v.size());
}

TEST(Init10SetGet)
{
    My_vec<int> v(10);
    v[3] = 7;
    CHECK_EQUAL(7, v[3]);
}

TEST(InitValueGetSetGet)
{
    My_vec<int> v(10, 15);
    CHECK_EQUAL(10, v.size());
    CHECK_EQUAL(15, v[4]);
    v[4] = 9;
    CHECK_EQUAL(9, v[4]);
}

TEST(InitPush)
{
    My_vec<int> v(3);
    v.push_back(3);
    v.push_back(4);
    CHECK_EQUAL(3, v[3]);
    CHECK_EQUAL(4, v[4]);
}

TEST(Copy)
{
    My_vec<int> v;
    v.push_back(0);
    v.push_back(1);
    v.push_back(2);
    My_vec<int> w(v);
    CHECK_EQUAL(3, w.size());
    CHECK_EQUAL(0, w[0]);
    CHECK_EQUAL(1, w[1]);
    CHECK_EQUAL(2, w[2]);
}

TEST(Assign)
{
    My_vec<int> v(2);
    My_vec<int> w(3);

    v[0] = 3;
    v[1] = 4;

    w = v;

    CHECK_EQUAL(2, w.size());
    CHECK_EQUAL(3, w[0]);
    CHECK_EQUAL(4, w[1]);
}

TEST(AssignRealloc)
{
    My_vec<int> v(4);
    My_vec<int> w(3);

    v[0] = 3;
    v[1] = 4;
    v[2] = 5;
    v[3] = 6;

    w = v;

    CHECK_EQUAL(4, w.size());
    CHECK_EQUAL(3, w[0]);
    CHECK_EQUAL(4, w[1]);
    CHECK_EQUAL(5, w[2]);
    CHECK_EQUAL(6, w[3]);
}
