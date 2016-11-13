#include "Bst.h"
#include <UnitTest++/UnitTest++.h>
#include <random>

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

TEST(Invariant)
{
    Bst<size_t> b;
    CHECK_EQUAL(true,b.bst_invariant_holds());
    b.insert(0);
    CHECK_EQUAL(true,b.bst_invariant_holds());
    b.insert(2);
    CHECK_EQUAL(true,b.bst_invariant_holds());
    b.insert(1);
    CHECK_EQUAL(true,b.bst_invariant_holds());
}

void random_test_remove_something(std::uniform_int_distribution<size_t>& dist,
                                  std::mt19937_64& rng,
                                  std::vector<size_t>& to_remove,
                                  Bst<size_t>& b)
{
    size_t number_before = b.size();
    size_t index = dist(rng) % to_remove.size();
    b.remove(to_remove[index]);
    CHECK(b.bst_invariant_holds());
    CHECK_EQUAL(number_before-1,b.size());
    to_remove.erase(to_remove.begin() + index);
}

TEST(Random)
{
    std::mt19937_64 rng;
    rng.seed(std::random_device{}());
    std::uniform_int_distribution<size_t> dist;

    for (int trials = 0; trials < 100; trials++) {
        Bst<size_t>         b;
        std::vector<size_t> to_remove;
        size_t              elements = (dist(rng) % 20) + 1;
        for (int            i        = 0; i < elements; i++) {
            size_t to_insert = dist(rng) % 10;
            size_t size_before = b.size();
            b.insert(to_insert);
            CHECK(b.bst_invariant_holds());
            CHECK(b.contains(to_insert));

            bool        already_inserted = false;
            for (size_t ele : to_remove) {
                if (ele == to_insert) already_inserted = true;
            }
            CHECK_EQUAL(size_before+(already_inserted?0:1),b.size());

            if (!already_inserted) {
                to_remove.push_back(to_insert);
                if (dist(rng) % 3)
                    random_test_remove_something(dist, rng, to_remove, b);
            }
        }
        while (!to_remove.empty()) {
            random_test_remove_something(dist, rng, to_remove, b);
        }
    }
}

