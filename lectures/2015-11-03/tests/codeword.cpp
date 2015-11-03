#include <UnitTest++/UnitTest++.h>

TEST(FailSpectacularly)
{
    CHECK(false);
}

int
main(int, const char*[])
{
    return UnitTest::RunAllTests();
}
