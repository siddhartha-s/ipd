#include "Sbox_hash.h"
#include <UnitTest++/UnitTest++.h>

const Sbox_hash hash1, hash2;
const std::string message1 = "Hello, world";
const std::string message2 = "Hello, world!";

TEST(Deterministic)
{
    CHECK_EQUAL(hash1(message1), hash1(message1));
    CHECK_EQUAL(hash1(message2), hash1(message2));
    CHECK_EQUAL(hash2(message1), hash2(message1));
    CHECK_EQUAL(hash2(message2), hash2(message2));
}

// Could fail, but very unlikely...
TEST(DifferentStrings)
{
    CHECK(hash1(message1) != hash1(message2));
    CHECK(hash2(message1) != hash2(message2));
}

// Could fail, but very unlikely...
TEST(DifferentFunctions)
{
    CHECK(hash1(message1) != hash2(message1));
    CHECK(hash1(message2) != hash2(message2));
}
