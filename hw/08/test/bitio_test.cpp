#include "UnitTest++/UnitTest++.h"
#include "bit_io.h"

using namespace ipd;

TEST(BISTREAM1) {
    bistream *b = new bistringstream({0});
    bool bit;

    b->read(bit);
    CHECK_EQUAL(false, bit);
}

TEST(BISTREAM2) {
    bistream *b = new bistringstream({255});
    bool bit;

    b->read(bit);
    CHECK_EQUAL(true, bit);
}

TEST(BISTREAM3) {
    bistream *b = new bistringstream({128});
    bool bit;

    b->read(bit);
    CHECK_EQUAL(true, bit);
}

TEST(BISTREAM4) {
    bistream *b = new bistringstream({1 << 6 | 1 << 5});
    bool bit;

    b->read(bit);
    CHECK_EQUAL(false, bit);
    b->read(bit);
    CHECK_EQUAL(true, bit);
    b->read(bit);
    CHECK_EQUAL(true, bit);
}


TEST(BISTREAM5) {
    bistream *b = new bistringstream({255, 1 << 6 | 1 << 5});
    bool bit;

    for (int i = 0; i < 8; i++) {
        b->read(bit);
    }

    b->read(bit);
    CHECK_EQUAL(false, bit);
    b->read(bit);
    CHECK_EQUAL(true, bit);
    b->read(bit);
    CHECK_EQUAL(true, bit);
}


TEST(BISTREAM6) {
    bistream *b = new bistringstream({255, 255});
    bool bit;

    for (int i = 0; i < 16; i++) {
        b->read(bit);
        CHECK_EQUAL(true, bit);
    }
    CHECK_EQUAL(b->eof(), true);
}


TEST(BOSTREAM1) {
    bostringstream *b = new bostringstream();
    CHECK_EQUAL(0, b->bits_written);
    b->write(false);
    CHECK_EQUAL(1, b->bits_written);
    CHECK_EQUAL(0, b->get_data()[0]);
}

TEST(BOSTREAM2) {
    bostringstream *b = new bostringstream();
    b->write(true);
    CHECK_EQUAL(128, b->get_data()[0]);
}

TEST(BOSTREAM3) {
    bostringstream *b = new bostringstream();
    b->write_bits('a', 8);
    CHECK_EQUAL(8, b->bits_written);
    CHECK_EQUAL('a', b->get_data()[0]);
}
