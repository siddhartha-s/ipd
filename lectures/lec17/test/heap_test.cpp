#include "../../lec17/src/Binomial_heap.h"
#include <UnitTest++/UnitTest++.h>

#include <random>
#include <iostream>
using namespace ipd;

TEST(New_is_empty)
{
    Binomial_heap<int> h;
    CHECK(h.empty());
    CHECK_EQUAL(0, h.size());
}

TEST(Add_increases_size)
{
    Binomial_heap<int> h;

    h.add(5);
    CHECK(!h.empty());
    CHECK_EQUAL(1, h.size());

    h.add(6);
    CHECK(!h.empty());
    CHECK_EQUAL(2, h.size());
}

TEST(Add_changes_min)
{
    Binomial_heap<int> h;

    h.add(5);
    CHECK_EQUAL(5, h.get_min());

    h.add(6);
    CHECK_EQUAL(5, h.get_min());

    h.add(3);
    CHECK_EQUAL(3, h.get_min());

    h.add(1);
    CHECK_EQUAL(1, h.get_min());

    h.add(2);
    CHECK_EQUAL(1, h.get_min());
}

TEST(Remove_changes_min)
{
    Binomial_heap<int> h;

    h.add(5);
    h.add(6);
    h.add(7);
    h.add(8);
    h.add(9);
    h.add(10);

    CHECK_EQUAL(5, h.get_min());
    h.remove_min();
    CHECK_EQUAL(6, h.get_min());
    h.remove_min();
    CHECK_EQUAL(7, h.get_min());
    h.remove_min();
    CHECK_EQUAL(8, h.get_min());
    h.remove_min();
    CHECK_EQUAL(9, h.get_min());
    h.remove_min();
    CHECK_EQUAL(10, h.get_min());
    h.remove_min();
    CHECK(h.empty());
}

TEST(Many_insertions)
{
    Binomial_heap<size_t> h;

    for (size_t i = 1000; i >= 1; --i) {
        h.add(i);
    }

    for (size_t i = 1; i <= 1000; ++i) {
        CHECK_EQUAL(i, h.get_min());
        h.remove_min();
    }
}

TEST(Merge)
{
    Binomial_heap<size_t> h1, h2;

    for (size_t i = 0; i < 1000; ++i) {
        if (i % 2 == 0)
            h2.add(i);
        else
            h1.add(i);
    }

    h1.merge(h2);

    CHECK(h2.empty());
    CHECK_EQUAL(1000, h1.size());

    for (size_t i = 0; i < 1000; ++i) {
        CHECK_EQUAL(i, h1.get_min());
        h1.remove_min();
    }
}

TEST(Repeated)
{
    Binomial_heap<int> h;

    h.add(1);
    h.add(1);
    h.add(1);

    CHECK_EQUAL(3, h.size());

    CHECK_EQUAL(1, h.get_min());
    h.remove_min();
    CHECK_EQUAL(1, h.get_min());
    h.remove_min();
    CHECK_EQUAL(1, h.get_min());
    h.remove_min();

    CHECK(h.empty());
}

TEST(Random)
{
    std::mt19937_64 rng;
    rng.seed(std::random_device{}());
    std::uniform_int_distribution<size_t> dist;

    for (int trials=0; trials < 100; trials++) {
        Binomial_heap<size_t> h;
        size_t                elements = (dist(rng) % 20) + 1;
        for (int              i        = 0; i < elements; i++) {
            h.add(i);
        }
        size_t                prev     = h.get_min();
        h.remove_min();
        while (!h.empty()) {
            CHECK(prev <= h.get_min());
            h.remove_min();
        }
    }
}
