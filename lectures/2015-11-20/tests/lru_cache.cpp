#include "lru_cache/lru_cache.hpp"
#include <UnitTest++/UnitTest++.h>

#include <string>

namespace lru
{

  TEST(Create)
  {
    lru_cache<int> c;
  }

  TEST(Small)
  {
    lru_cache<int> c{2};
    c.put(1,1);
    c.put(2,2);
    CHECK(c.hasKey(1));
    CHECK(c.hasKey(2));
    c.put(3,3);
    CHECK(!c.hasKey(1));
    CHECK(c.hasKey(2));
    CHECK(c.hasKey(3));
    CHECK_EQUAL(2,c.get(2));
    c.put(4,4);
    CHECK(!c.hasKey(3));
    CHECK_EQUAL(2,c.get(2));
    CHECK_EQUAL(4,c.get(4));
  }

  TEST(Medium)
  {
    lru_cache<int> c{10};
    for(int i = 0; i < 20; i++)
      c.put(i,i);
    for(int i = 0; i < 20; i++) {
      if (i < 10)
	CHECK(!c.hasKey(i));
      else
	CHECK(c.hasKey(i));
    }
    CHECK_EQUAL(10, c.get(10));
    c.put(21,21);
    CHECK(!c.hasKey(11));
  }

  TEST(Larger)
  {
    lru_cache<int> c{20};
    for(int i = 0; i <= 200; i++)
      c.put(i,i);
    for(int i = 200; i > 180; i--)
      c.get(i);
    for(int i = 200; i > 180; i--) {
      c.put(201 - i, 201 - i);
      CHECK(!c.hasKey(i));
    }
    for(int i = 1; i < 20; i ++) {
      CHECK(c.hasKey(i));
      CHECK_EQUAL(i, c.get(i));
    }
  }

}  // namespace lru

int
main(int, const char* [])
{
    return UnitTest::RunAllTests();
}
