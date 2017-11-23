#include "RP-ring-buffer.h"
#include <UnitTest++/UnitTest++.h>

TEST(EMPTY)
{
    RingBuffer<size_t> rp;
    CHECK_EQUAL(true, rp.empty());
    rp.insert(0);
    CHECK_EQUAL(false, rp.empty());
    rp.remove();
    CHECK_EQUAL(true, rp.empty());
}

TEST(INSERT)
{
    RingBuffer<size_t> rp;
    rp.insert(0);
    CHECK_EQUAL(0, rp.remove());
    rp.insert(1);
    rp.insert(2);
    rp.insert(3);
    CHECK_EQUAL(3, rp.remove());
    CHECK_EQUAL(2, rp.remove());
    CHECK_EQUAL(1, rp.remove());
}

TEST(ROTATE)
{
    RingBuffer<size_t> rp;
    rp.rotate();
    CHECK_EQUAL(true, rp.empty());

    rp.insert(1);
    rp.insert(2);
    rp.insert(3);
    rp.rotate();
    CHECK_EQUAL(2, rp.remove());
    CHECK_EQUAL(1, rp.remove());
    CHECK_EQUAL(3, rp.remove());


    rp.insert(1);
    rp.insert(2);
    rp.insert(3);
    rp.rotate();
    rp.rotate();
    CHECK_EQUAL(1, rp.remove());
    CHECK_EQUAL(3, rp.remove());
    CHECK_EQUAL(2, rp.remove());


    rp.insert(1);
    rp.insert(2);
    rp.insert(3);
    rp.rotate();
    rp.rotate();
    rp.rotate();
    CHECK_EQUAL(3, rp.remove());
    CHECK_EQUAL(2, rp.remove());
    CHECK_EQUAL(1, rp.remove());
}
