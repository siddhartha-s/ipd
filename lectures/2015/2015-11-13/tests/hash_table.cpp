#include "hash_table/hash_table.hpp"
#include <UnitTest++/UnitTest++.h>
#include <cstdlib>
#include <iostream>
#include <string>

namespace hash_table
{

  TEST(TestTest)
  {
    CHECK(true);
  }

  TEST(BASIC)
  {
    hash_table<std::string> ht(2);
    ht.put(1,"one");
    ht.put(2,"two");
    ht.put(3,"three");
    CHECK(ht.get(1) == "one");
    CHECK(ht.get(2) == "two");
    CHECK(ht.get(3) == "three");
    CHECK(ht.hasKey(3));
    ht.remove(3);
    CHECK(!ht.hasKey(3));
  }

  TEST(BIGGER)
  {
    hash_table<int> ht(2);
    for(int i = 0; i < 1000; i++) {
      ht.put(i,i);
    }
    for(int i = 0; i < 1000; i++) {
      CHECK_EQUAL(i, ht.get(i));
    }
    for(int i = 0; i < 1000; i++) {
      if (i % 2 == 0) {
	ht.remove(i);
      }
    }
     for(int i = 0; i < 1000; i++) {
       if (i % 2 == 0) {
	 CHECK(!ht.hasKey(i));
       } else {
	 CHECK_EQUAL(i, ht.get(i));
       }
    }
  }

} // namespace hash_table

int
main(int, const char*[])
{
    return UnitTest::RunAllTests();
}
