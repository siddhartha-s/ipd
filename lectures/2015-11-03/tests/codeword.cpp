#include "codeword/codeword.hpp"
#include <UnitTest++/UnitTest++.h>

namespace codeword
{

TEST(Create)
{
    codeword cw;
    (void) cw;
}

TEST(DefaultInit)
{
    codeword cw;
    CHECK_EQUAL(0, cw.length);
    CHECK_EQUAL(0, cw.bits);
}

TEST(InitList)
{
    codeword cw{4, 9};
    CHECK_EQUAL(4, cw.length);
    CHECK_EQUAL(9, cw.bits);
}

TEST(Equals)
{
    codeword cw1{4, 5};
    codeword cw2{4, 5};
    CHECK_EQUAL(true, cw1 == cw2);
}

TEST(Extend0Leading1)
{
    auto cw = extend(codeword{4, 9}, false);
    CHECK_EQUAL(5, cw.length);
    CHECK_EQUAL(18, cw.bits);
}

TEST(Extend1Leading1)
{
    auto cw = extend(codeword{4, 9}, true);
    CHECK_EQUAL(5, cw.length);
    CHECK_EQUAL(19, cw.bits);
}

TEST(Extend0Leading0)
{
    auto cw = extend(codeword{5, 9}, false);
    CHECK_EQUAL(6, cw.length);
    CHECK_EQUAL(18, cw.bits);
}

TEST(Extend1Leading0)
{
    auto cw = extend(codeword{5, 9}, true);
    CHECK_EQUAL(6, cw.length);
    CHECK_EQUAL(19, cw.bits);
}

TEST(ExtendEmpty0)
{
    auto cw = extend(codeword{}, false);
    CHECK_EQUAL(1, cw.length);
    CHECK_EQUAL(0, cw.bits);
}

TEST(ExtendEmpty1)
{
    auto cw = extend(codeword{}, true);
    CHECK_EQUAL(1, cw.length);
    CHECK_EQUAL(1, cw.bits);
}

TEST(FormatEmpty)
{
    CHECK_EQUAL("", format(codeword{}));
}

TEST(FormatLeading1)
{
    CHECK_EQUAL("10000011", format(codeword{8, 131}));
}

TEST(FormatLeading0)
{
    CHECK_EQUAL("01000010", format(codeword{8, 66}));
}

} // namespace codeword

int
main(int, const char*[])
{
    return UnitTest::RunAllTests();
}
