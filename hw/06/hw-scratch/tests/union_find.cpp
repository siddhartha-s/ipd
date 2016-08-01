#include "union_find/union_find.hpp"
#include <UnitTest++/UnitTest++.h>

namespace union_find
{

  TEST(Create)
  {
    int_sets sets;
  }

  TEST(MAKE)
  {
    int_sets sets{3};
    CHECK_EQUAL(1, sets.find(1));
    CHECK_EQUAL(2, sets.find(2));
  }

  TEST(UNION)
  {
    int_sets sets{3};
    CHECK_EQUAL(1, sets.find(1));
    CHECK_EQUAL(2, sets.find(2));
    sets.do_union(1,2);
    CHECK_EQUAL(sets.find(1), sets.find(2));
  }

  TEST(UNIONS)
  {
    int_sets sets{5};

    CHECK_EQUAL(1, sets.find(1));

    CHECK_EQUAL(2, sets.find(2));
    sets.do_union(1,2);
    CHECK_EQUAL(sets.find(1), sets.find(2));

    CHECK_EQUAL(3, sets.find(3));

    CHECK_EQUAL(4, sets.find(4));
    sets.do_union(3,4);
    CHECK_EQUAL(sets.find(3), sets.find(4));
    sets.do_union(1,4);
    for (int i = 1; i <= 4; i++) {
      for (int j = 1; j <= 4; j++) {
	CHECK_EQUAL(sets.find(i), sets.find(j));
      }
    }
  }

}  // namespace union_find

int
main(int, const char* [])
{
    return UnitTest::RunAllTests();
}
